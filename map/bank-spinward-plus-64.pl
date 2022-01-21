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

   my $sizatm = ($hex{$siz} & 15) + ($hex{$atm} & 15) << 4;
   my $hydpop = ($hex{$hyd} & 15) + ($hex{$pop} & 15) << 4;
   my $govlaw = ($hex{$gov} & 15) + ($hex{$law} & 15) << 4;

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
   for my $code (qw/An Cp He Ic Mr Re Rs Sa/)
   {
      $tc1 << 1;
      $tc1++ if $tc =~ /$code\b/;
   }

   # ---------------------------------------------
   #
   #  Manage Belts + GGs and Zone ($b, $g, $z)
   #
   # ---------------------------------------------
   $z = ' ' if $z eq '-';

   my $bgg = ' ';
   $bgg = 'B' if $b > 0;
   $bgg = 'G' if $gg > 0;
   $bgg = '2' if $b > 0 && $gg > 0;

   # ---------------------------------------------
   #
   #  Prepare Stellar Data
   #
   #  e.g.
   #  1.  G3 IV M0 V M3 V
   #  2.  G34M05M35
   #  3.  G,34,M,5,M,35 = 6 bytes
   # ---------------------------------------------
   $st =~ s/\s//g;

   $st =~ s/VI/6/g;
   $st =~ s/IV/4/g;
   $st =~ s/V/5/g;

   $st =~ s/III/3/g;
   $st =~ s/II/2/g;
   $st =~ s/I/1/g;

   my @st1 = split '', $st;

   my @st2 = (
      $st1[0],                # byte 1
      $st1[1] * 8 + $st1[2],  # byte 2  (5:3)
      $st1[3],                # byte 3
      $st1[4] * 8 + $st1[5],  # byte 4  (5:3)
      $st1[6],                # byte 5
      $st1[7] * 8 + $st1[8]   # byte 6  (5:3)
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

   $rec  = pack 'CC', $col,$row; 	# 00 - 01   0-255 x 0-255
   $rec .= pack 'A4', $sectorAbbr;      # 02 - 05
   $rec .= ' ';				# 06
   $rec .= pack 'A4', $location;    	# 07 - 0a   e.g. 0101-3240
   $rec .= ' ';				# 0b
   $rec .= pack 'A15', $name;           # 0c - 1a   16 bytes
   $rec .= ' ';				# 1b
   $rec .= pack 'A',   $sp;             # 1c
   $rec .= pack 'A6',  $uwp;            # 1d - 22           
   $rec .= pack 'AA',  '-', $tl;        # 23 - 24
   $rec .= ' ';				# 25
   $rec .= pack 'A', $z;                # 26
   $rec .= pack 'A', $al;               # 27
   $rec .= pack 'x';                    # 28
   $rec .= pack 'A', $ba;               # 29
   $rec .= pack 'A', $bgg;              # 2a
   $rec .= pack 'C', $tcSan;      	# 2b
   $rec .= pack 'C', $tc1;        	# 2c
   $rec .= pack 'C', $sizatm;           # 2d
   $rec .= pack 'C', $hydpop;           # 2e
   $rec .= pack 'C', $govlaw;           # 2f
   $rec .= pack 'C', $hex{$tl};  	# 30
   $rec .= pack 'x9';                   # 31 - 39  trade goods quantities?
   $rec .= $stp;                        # 3a - 3f


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

open OUT, '>', 'SPINWARD-MAP-64B.BIN';
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
' '            
a15 name       
' '            
a  starport    
a6 uwp         
a  -           
a  tl          
' '            
a  zone [ AR]  
a  allegiance  
x              
a  bases:NSAWBD
a  bgg [ BG2]  
C  tcSan       
C  tc1         
C  siz+atm     
C  hyd+pop     
C  gov+law     
C  tl          
x9             
C  s1 class    
C  s1 div+siz  
C  s2 class    
C  s2 div+siz  
C  s3 class    
C  s3 div+siz  
EOHEADER

close OUT;

