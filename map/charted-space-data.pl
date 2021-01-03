use UText;
use UwpTables;
use StarCodec;

# -------------------------------------------------------------------
#
#  Packs UWP data into very tight DATA statements.
#
#  * One string per DATA line.
#  * Base 128 encoding where reasonable.
#  * UWP varies in length.
#
#  BYTE CONTENTS
#  ---- -------------------------------------------------------------
#    0  Hex Column    (:7)  
#    1  Hex Row       (:7)  
#    2  Starport+Base (:5), Zone    (:2)
#    3  TL            (:5), An Cp   (:2)
#    4  TC Index      (:6), Sa      (:1)
#    5  Allegiance    (:5), Belt,GG (:2)
#    6+ Name
#
# -------------------------------------------------------------------
my $uwpCount  = 0;
my $byteCount = 0;
my @buffer    = ();
my $tl = 0;
my %tl = map{ $_ => $tl++ } split '', '0123456789ABCDEFGHJKLMNPQRSTUVWXYZ';
my @letter = ('A'..'P');

my $sectorName; # = uc $1 if $sector =~ /^(.*?)\.\w+$/;

open IN, '<', 'charted-space.txt' or die "ERROR: charted-space.txt nonexistent\n";
my @data = <IN>;
close IN;

my ($sx,$sy);
my ($minx, $maxx, $miny, $maxy) = ( 5000, -1, 5000, -1 );
my $mapref;
my %subsectorsByLetter;
my $letter;
my $count = 0;


my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime(time);
my $timestamp = pack "CCC", $year, $mon+1, $mday;


my $basicLine = 700;
my @PETSCII_BASE_128 = ( 0x30..0x5f, 0x90..0xbf, 0x60..0x7f );


foreach(@data)
{
   # ---------------------------------------------
   #
   #  Sector Name
   #
   # ---------------------------------------------
   unless ($sectorName)
   {
      $sectorName = sprintf("%-23s", uc $1) if /^# Name: (\w.*)/;
   }

   # ---------------------------------------------
   #
   #  Sector Y, X
   #
   # ---------------------------------------------
   ($sx,$sy) = ($1,$2) if /^# (-?\d+),(-?\d+)\s*/;


   #########################################################
   #
   #  Limit sectors to Spin, Deneb, and Corridor.
   #
   #########################################################
   next if $sx < -4 || $sx > -2 || $sy < -1 || $sy > -1;

   s/\{.+\}//g;
   s/\(...[+-].\)//g;
   s/\[.+\]//g;

   my ($col, $row, $name, $sp, $uwp, $tl, $tc, $nobility, $ba, $z, $p, $b, $gg, $w, $al, $st) = /^(\d\d)(\d\d) (.+)\s* (.)(......-)(.) (.+)\s*    (.....) (..) (.) (.)(.)(.) (..) (..)\S+ (.*)$/;

   next unless $name;

   # ---------------------------------------------
   #
   #   Truncate @name 
   #
   # ---------------------------------------------
   $name = 'TRANSIT'  if $name =~ /Fleet Transit/;
   $name = 'CALIB PT' if $name =~ /Calibration/;
   $name = substr(uc $name, 0, 15); 
   $name =~ s/\s*$//;

   # ---------------------------------------------
   #
   #   Split up SAHPGL
   #
   # ---------------------------------------------
   # my ($siz, $atm, $hyd, $pop, $gov, $law) = split '', $uwp;


   # ---------------------------------------------
   #
   #   Gently manage starport + base codes
   #
   # ---------------------------------------------
   # what we allow
   my $hasN = 1 if $ba =~ /[JKN]/;
   my $hasS = 1 if $ba =~ /[MS]/;
   my $hasD = 1 if $ba =~ /D/;
   my $hasW = 1 if $ba =~ /W/;

   # now reconstruct
   $ba = '';
   $ba = 'N' if $hasN;
   $ba = 'D' if $hasD;
   $ba .= 'S' if $hasS;
   $ba .= 'W' if $hasW;

   # now map
   my $spb = $spb{ $sp . $ba };

   # ---------------------------------------------
   #
   #  Compute TRADE SYSTEM index
   #
   # ---------------------------------------------
   $tc =~ s/\s+$//;
   my $tcSan = "";
   for my $code (qw/Ag As Ba De Fl Hi In Lo Na Ni Po Ri Va/)
   {  
      $tcSan .= "$code " if $tc =~ /$code\b/;
   }
   $tcSan =~ s/\s+$//;

   $tcSan = UwpTables::getTCIndexForCodeString( $tcSan );
   $tcSan = 0 unless $tcSan =~ /\w/;

   # ---------------------------------------------
   #
   #  Gather up stray trade remarks
   #
   # ---------------------------------------------
   my $tc1 = 0;
   $tc1 += 32  if $code =~ /An/;
   $tc1 += 64 if $code =~ /Cp/;

   my $tc2 = 0;
   $tc2 += 64  if $code =~ /Sa/;

   # ---------------------------------------------
   #
   #  Encode Belts + GGs
   #
   # ---------------------------------------------
   my $bgg = 0;
   $bgg += 32  if  $b > 0;
   $bgg += 64 if $gg > 0;

   # ---------------------------------------------
   #
   #  Manage Zone
   #
   # ---------------------------------------------
   my $z1 = 0;
   $z1 = 32  if $z =~ /A/;
   $z1 = 64 if $z =~ /R/;

   # --------------------------------------
   #
   #  Prepare Allegiance code
   #
   # --------------------------------------
   $al = (ord(uc $al)-65) & 31; # reduce to 5 bits

   # --------------------------------------
   #
   #  Prepare absolute hex (two 7 bit numbers)
   #
   # --------------------------------------
   my $aHexCol = (4+$sx) * 32 + $col;
   my $aHexRow = (1+$sy) * 40 + $row;

   # --------------------------------------
   #
   #  Build DATA line.
   #
   #  BYTE CONTENTS
   #  ---- --------------------------------
   #    0  Hex Column    (:7)  
   #    1  Hex Row       (:7)  
   #    2  Starport+Base (:5), Zone    (:2)
   #    3  TL            (:5), An Cp   (:2)
   #    4  TC Index      (:5), Rs Sa   (:2)
   #    5  Allegiance    (:5), Belt,GG (:2)
   #    6+ Name *
   #
   #    * The last letter is manipulated 
   #      to be a delimiter thus:
   #      L = (ord(L)+32).
   #
   # --------------------------------------

#   next unless $name =~ /567-908/;   test

   unless ($count % 4)
   {
      print $basicLine, " DATA \"";
   }

   ++$count;
   print base128($aHexCol);
   print base128($aHexRow);
   print base128($spb + $z1);
   print base128($tl  + $tc1);
   print base128($tcSan + $tc2);
   print base128($al+ $bgg);

   # doctor name
   my $last = chop $name;
   print $name;
   $last = ord($last) + 32; # guard
   print base128($last);

   unless ($count % 4)
   {
      print "\"\n";
      ++$basicLine;
   }
}
print "\"\n" if $count % 4;

print STDERR "\n\n--> $count worlds processed.\n";


sub base128
{
   my $num = shift;
   return sprintf("\\X%02X", $PETSCII_BASE_128[ $num ]);
}

