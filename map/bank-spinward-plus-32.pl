use UText;
use UwpTables;
use StarCodec;

# -------------------------------------------------------------------
#
#  Generates one bankable binary file centered on the Marches.
#
#  32 byte records will fill a bank with 4 columns of 64 rows.
#  This includes empty hexes.
#
# -------------------------------------------------------------------

my $uwpCount  = 0;
my $byteCount = 0;
my @buffer    = ();
my $tl = 0;
my %hex = map{ $_ => $tl++ } split '', '0123456789ABCDEFGHJKLMNPQRSTUVWXYZ';
my @letter = ('A'..'P');

my $sectorName; # = uc $1 if $sector =~ /^(.*?)\.\w+$/;

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
   }

   # ---------------------------------------------
   #
   #  Sector Y, X
   #
   # ---------------------------------------------
#   ($sx,$sy) = ($1,$2) if /^# (-?\d+),(-?\d+)\s*/;
#
#   if (/^# Subsector (.): (.*?)\s*$/)
#   {
#      my $letter = $1;
#      $subsectorsByLetter{$letter} = uc $2;
#   }

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
   #   Split up SAHPGL
   #
   # ---------------------------------------------
   # my ($siz, $atm, $hyd, $pop, $gov, $law) = split '', $uwp;

   # ---------------------------------------------
   #
   #   Gently manage starport + base codes
   #
   # ---------------------------------------------
   $ba =~ s/[JK]/N/;
   $ba =~ s/M/S/;
   $ba =~ s/[- ]//g;
   $ba = '' unless $ba =~ /[A-Z]/;	# empty

   my $base = '';
   $base = 'N' if $ba =~ /N/;
   $base = 'S' if $ba =~ /S/;
   $base = 'A' if $ba =~ /NS/;
   $base = 'W' if $ba =~ /W/;
   $base = 'B' if $ba =~ /NW/;
   $base = 'D' if $ba =~ /D/;

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
   for my $code (qw/An Cp He Ic Mr Re Rs Sa/)
   {
      $tc1 << 1;
      $tc1++ if $tc =~ /$code\b/;
   }

   # ---------------------------------------------
   #
   #  Importance Code
   #
   # ---------------------------------------------
   # my $ix = 0; # print "NEED IMPORTANCE\n";

   # ---------------------------------------------
   #
   #  Nobility Code
   #
   # ---------------------------------------------
   # $nobility =~ s/^.*(.)\s*/$1/; # save the last non-space character, if it exists
   # $nobility = ' ' unless $nobility =~ /[A-Z]/i;

   # ---------------------------------------------
   #
   #  Encode Belts + GGs
   #
   # ---------------------------------------------
   my $bgg = 0;
   $bgg = 'B' if  $b > 0;
   $bgg = 'G' if $gg > 0;
   $bgg = '2' if  $b > 0 && $gg > 0;

   # ---------------------------------------------
   #
   #  Manage Zone
   #
   # ---------------------------------------------
   $z = ' ' unless $z =~ /[AR]/;

   # ---------------------------------------------
   # 
   #  Manage star data
   #
   # ---------------------------------------------
#   $st =~ s/\s*$//;
#
#   my @halfstar = StarCodec::encodeStarData( $st );
#   my $stp = pack 'CC', @halfstar;
    
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
#   my $aHexCol = (129 + $sx) * 32 + $col;
#   my $aHexRow = (126 + $sy) * 40 + $row;

   my $viewcol = (($col-1) % 32) + 1;
   my $viewrow = (($row-1) % 40) + 1;
   my $location = sprintf("%02d%02d", $viewcol, $viewrow); # this is for ease of printing

   # --------------------------------------
   #
   #  UText-encode text elements
   #
   # --------------------------------------
#   my $str = sprintf "%1s%-15s", 
#	$al,
#        $name;
#
#   # input: 16 chars
#   # output: call it 12 chars
#   my @utext = UText::convert( $str );
#
#   while (@utext > 12)
#   {
#      chop $str;
#      @utext = UText::convert( $str );
#   }

   # --------------------------------------
   #
   #  Build binary data
   #
   # --------------------------------------
   my $rec;

   $rec  = pack 'CC', $col,$row; 	# $00 - $01 
   $rec .= pack 'A4', $location;    	# $02 - $05
   $rec .= ' ';				# $06
   $rec .= pack 'AA', $sp, $base;	# $07 - $08
   $rec .= ' ';				# $09
   $rec .= pack 'A15', $name;           # $0a - $18
   $rec .= ' ';				# $19
   $rec .= pack 'AAA', $bgg, $al, $z;   # $1a - $1c
   $rec .= pack 'C', $hex{$tl};  	# $1d
   $rec .= pack 'C', $tcSan;      	# $1e
   $rec .= pack 'C', $tc1;        	# $1f

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

#   printf STDERR "$count:  $aHexCol [$minx,$maxx] $aHexRow [$miny,$maxy]\n";
}

print "writing $count entries\n";

open OUT, '>', 'SPINWARD.SPACE.BIN';
print OUT pack 'xx'; # 2 byte header to ignore

my $hexnums = "0123456789ABCDEFGHJKLMNPQRSTUVWXYZ";

#my $columnCount = 0;


print "************ OVERRIDING minx(1), miny(1), and maxy(64)!\n";

$minx = 1;
$miny = 1;
$maxy = 64;

for my $x ($minx .. $maxx)
{
#   if ($columnCount % 6 == 0) # bank boundary; print header
#   {
#      print OUT pack 'C', $x;
#      print OUT pack 'x';
#      print OUT pack 'A16', 'SPINWARD MARCHES';
#      print OUT pack 'x';
#      print OUT pack 'A*', uc 'this data is a flat matrix of hex data for traveller-trader. data'; # 65
#      print OUT pack 'A*', uc ' begins at $a200 of each bank, and consists of six columns of 40 '; # 65
#      print OUT pack 'A*', uc 'records. each record is 32 bytes long.';  # 38
#      print OUT pack 'x287';
#      print OUT pack 'CCC', $year, $mon+1, $mday;  # timestamp
#      print OUT pack 'x';
#      print OUT pack 'A34', $hexnums;              # ehex!  why not?
#   }
#   ++$columnCount;
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
         print OUT pack 'A4', $loc;
         print OUT '                       ';
         print OUT pack 'xxx';
      }
   }
}
close OUT;

