use UText;
use UwpTables;
use StarCodec;

# -------------------------------------------------------------------
#
#  Generates one bankable binary file centered on the Marches.
#
#  64 byte records will fill a bank with 128 records.
#  This includes empty hexes.
#  Yes, empty hexes are important.
#
# -------------------------------------------------------------------

my $uwpCount  = 0;
my $byteCount = 0;
my @buffer    = ();
my $tl = 0;
my %hex = map{ $_ => $tl++ } split '', '0123456789ABCDEFGHJKLMNPQRSTUVWXYZ';
my @letter = ('A'..'P');

my $sectorName; # = uc $1 if $sector =~ /^(.*?)\.\w+$/;
my $sectorAbbr;

open IN, '<', 'plus-spin.sec' or die "ERROR: plus-spin.sec nonexistent\n";
my @data = <IN>;
close IN;

my ($sx,$sy);
my ($minx, $maxx, $miny, $maxy) = ( 5000, -1, 5000, -1 );
my $mapref;
my %subsectorsByLetter;
my $letter;
my $count = 0;

my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime(time);
++$mon;

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
      $sectorAbbr = substr($sectorName,0,4);
   }

   ### SPECIAL JIGGERY HERE: extract the sector abbreviation
   if (/^# Abbrev: (\w.*)/)
   {
      $sectorAbbr = substr(sprintf("%-4s", uc $1),0,4);
   }

   s/\{.+\}//g;
   s/\(...[+-].\)//g;
   s/\[.+\]//g;

   my ($col, $row, $name, $sp, $uwp, $tl, $tc, $nobility, $ba, $z, $p, $b, $gg, $w, $al, $st) = /^(\d\d)(\d\d) (.+)\s* (.)(......-)(.) (.+)\s*    (.....) (..) (.) (.)(.)(.) (..) (..)\S+ (.*)$/;

   next unless $name;
   # print "$col,$row\n";

   # ---------------------------------------------
   #
   #   Truncate @name at 15 chars
   #
   # ---------------------------------------------
   $name = substr(uc $name, 0, 15); 

   # ---------------------------------------------
   #
   #   Split up SAHPGL,
   #   And mush it into 3 bytes.
   #
   # ---------------------------------------------
   my ($siz, $atm, $hyd, $pop, $gov, $law) = split '', $uwp;

   my $sizatm = ($hex{$siz} & 15) + (($hex{$atm} & 15) * 16);
   my $hydpop = ($hex{$hyd} & 15) + (($hex{$pop} & 15) * 16);
   my $govlaw = ($hex{$gov} & 15) + (($hex{$law} & 15) * 16);

   # ---------------------------------------------
   #
   #   Gently manage base codes
   #
   # ---------------------------------------------
   $ba =~ s/[JK]/N/;
   $ba =~ s/M/S/;
   $ba =~ s/[- ]//g;
   $ba = '' unless $ba =~ /[A-Z]/;	# empty

   $ba =~ s/NS/A/;
   $ba =~ s/NW/B/;

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

   #printf "%-16s TCSAN $tcSan\n", $name;

   # ---------------------------------------------
   #
   #  Gather up stray trade remarks
   #
   # ---------------------------------------------
   my $tc1 = 0;
   for my $code (reverse qw/An Cp He Ic Mr Re Rs Sa/)
   {
      $tc1 <<= 1;
      $tc1++ if $tc =~ /$code\b/;
   }

   my $tc2 = 0; # this one is two bytes.
   for my $code (reverse qw/Ag As Ba De Fl Hi In Lo Na Ni Po Ri Va Wa/)
   {
      $tc2 <<= 1;
      $tc2++ if $tc =~ /$code\b/;
   }

   #
   #  Let's see if we can classify the world very broadly.
   #
   my $tc3 = 0;
   $tc3 = 1 if $tc =~ /(Ag|Ga)\b/;
   $tc3 = 2 if $tc =~ /Ic\b/;
   $tc3 = 3 if $tc =~ /Fl\b/;
   $tc3 = 4 if $tc =~ /(Va|De)\b/;
   $tc3 = 5 if $hex{$atm} > 9;
   $tc3 = 6 if $tc =~ /(He|In)\b/;
   $tc3 = 7 if $tc =~ /(He|In)\b/ && rand() > 0.5;
   $tc3 = 8 if $tc =~ /(Na|Ni)\b/;
   $tc3 = 9 if $tc =~ /(Wa|Oc)\b/;
   $tc3 = 10 if $tc =~ /(Va|De)\b/ && rand() > 0.5;
   $tc3 = 11 if $tc =~ /As\b/;

   # ---------------------------------------------
   #
   #  Manage Belts + GGs and Zone ($b, $g, $z)
   #
   # ---------------------------------------------
   $z = ' ' if $z eq '-';

   my $bggz = 0;
   $bggz  = 0 if  $z eq ' ';  # 
   $bggz  = 1 if  $z eq 'A';  # zone = bits 0 & 1
   $bggz  = 2 if  $z eq 'R';  # 
   # there is no '3'

   $bggz += 4 if  $b > 0;
   $bggz += 8 if $gg > 0;

   # add in digital bases' Scout and Naval presence bits
   $bggz += 16  if $ba =~  /S|A|W|B/;
   $bggz += 32  if $ba =~  /N|A|D|B/;

   # ---------------------------------------------
   #
   #  Prepare Stellar Data
   #
   #  example 1:
   #  1.  G3 IV M0 V M3 V
   #  2.  G34M05M35
   #  3.  G,34,M,5,M,35 = 6 bytes
   # ---------------------------------------------
   $st =~ s/BD/M0 D/g;
   $st =~ s/\s//g;

   $st =~ s/VI|D/6/g;
   $st =~ s/IV/4/g;
   $st =~ s/V/5/g;

   $st =~ s/III/3/g;
   $st =~ s/II/2/g;
   $st =~ s/I/1/g;

   my @st1 = split '', $st;

   my @st2 = (
      $st1[0],                # byte 1
      $st1[1] * 8 + $st1[2],  # byte 2  (3:5) little-endian
      $st1[3],                # byte 3
      $st1[4] * 8 + $st1[5],  # byte 4  (3:5) little-endian
      $st1[6],                # byte 5
      $st1[7] * 8 + $st1[8]   # byte 6  (3:5) little-endian
   );

   my $stp = pack 'ACACAC', @st2;

   # --------------------------------------
   #
   #  Prepare Allegiance code
   #
   # --------------------------------------
   $al = uc $al;

   # --------------------------------------
   #
   #  Prepare absolute hex (two 15 bit numbers)
   #
   # --------------------------------------
   my $viewcol = (($col-1) % 32) + 1;
   my $viewrow = (($row-1) % 40) + 1;
   my $location = sprintf("%02d%02d", $viewcol, $viewrow); # this is for ease of printing

   # --------------------------------------
   #
   #  Build binary data
   #
   # --------------------------------------
   my $rec;

   $name =~ s/\s*$//;  # remove that space padding.  we'll null pad.

   $rec  = pack 'CC', $col,$row; 	# 00 - 01   0-255 x 0-255
   $rec .= pack 'A4', $sectorAbbr;  # 02 - 05
   $rec .= ' ';				         # 06
   $rec .= pack 'A4', $location;    # 07 - 0a   e.g. 0101-3240
   $rec .= pack 'x';		      		# 0b
   $rec .= pack 'Z15', $name;       # 0c - 1a   15 bytes
   $rec .= pack 'x';			   	   # 1b
   $rec .= pack 'A',   $sp;         # 1c
   $rec .= pack 'A6',  $uwp;        # 1d - 22           
   $rec .= pack 'AA',  '-', $tl;    # 23 - 24
   $rec .= pack 'x';				      # 25
   $rec .= pack 'A', $z;            # 26
   $rec .= pack 'A', $al;           # 27
   $rec .= pack 'A', $ba;           # 28
   $rec .= pack 'x';                # 29
   $rec .= pack 'C', $bggz;         # 2a  
   $rec .= pack 'C', $tcSan;      	# 2b  # probably don't need this
   $rec .= pack 'C', $tc1;        	# 2c  
   $rec .= pack 'C', $sizatm;       # 2d  # probably don't need this
   $rec .= pack 'C', $hydpop;       # 2e  # probably don't need this
   $rec .= pack 'C', $govlaw;       # 2f  # probably don't need this
   $rec .= pack 'C', $hex{$tl};  	# 30  # probably don't need this
   $rec .= pack 'v', $tc2;          # 31 - 32
   $rec .= pack 'C', $tc3;          # 33  # probably don't need this
   $rec .= pack 'x6';               # 34 - 39  spare
   $rec .= $stp;                    # 3a - 3f


   $mapref->[ $col ]->[ $row ] = $rec;
   $minx = $col if $col < $minx;
   $maxx = $col if $maxx < $col;
   $miny = $row if $row < $miny;
   $maxy = $row if $maxy < $row;

#   $mapref->[ $aHexCol ]->[ $aHexRow ] = $rec;

#   $minx = $aHexCol if $aHexCol < $minx;
#   $maxx = $aHexCol if $maxx    < $aHexCol;

#   $miny = $aHexRow if $aHexRow < $miny;
#   $maxy = $aHexRow if $maxy    < $aHexRow;

   ++$count;
}

print "writing $count entries\n";

open OUT, '>', 'BD-MAP64.BIN';
print OUT pack 'xx'; # 2 byte header to ignore

my $hexnums = "0123456789ABCDEFGHJKLMNPQRSTUVWXYZ";

#my $columnCount = 0;


print "************ OVERRIDING minx(0), miny(0), and maxy(63)!\n";

$minx = 0;    #  ADJUSTED to allow (0,0)
$miny = 0;    #  ADJUSTED to allow (0,0)
$maxy = 63;   #  ADJUSTED to allow (0,0)

for my $x ($minx .. $maxx)
{
   for my $y ($miny .. $maxy)
   {
      if ($mapref->[$x]->[$y])
      {
         print OUT $mapref->[$x]->[$y]; 
      }
      else  # otherwise, print empty hex
      {
         my $loc = sprintf("%02d%02d", (($x-1)%32)+1, (($y-1)%40)+1 );
         print OUT pack 'CC', $x, $y;
         print OUT pack 'x4';
         print OUT pack 'A', ' ';
         print OUT pack 'A4', $loc;
         print OUT pack 'A', ' ';
         print OUT pack 'A8', '        ';
         print OUT pack 'A8', '        ';
         print OUT pack 'A8', '        ';
         print OUT pack 'A8', '        ';
         print OUT pack 'A8', '        ';
         print OUT pack 'A8', '        ';
         print OUT pack 'A4', '    ';
      }
   }
}

print OUT pack 'x16'; # separator

print OUT uc<<EOHEADER;
64 byte starmap
cc col,row     
a4 sector      
' '            
a4 hex         
x              
z15 name       
x              
a  starport    
a6 uwp         
a  -           
a  tl          
x              
zone:2 b:1 gg:1  
a  alleg [A-Z]
a  bases:NSAWBD
x              
x              
C  tcSan       
C  tc1         
C  siz+atm     
C  hyd+pop     
C  gov+law     
C  tl          
v  tc2         
C  tc3 (0-11)  
x6             
C  s1 class    
C  s1 div+siz  
C  s2 class    
C  s2 div+siz  
C  s3 class    
C  s3 div+siz  
EOHEADER

close OUT;

