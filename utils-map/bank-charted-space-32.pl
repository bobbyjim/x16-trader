use UText;
use UwpTables;
use StarCodec;

# -------------------------------------------------------------------
#
#  Generates one bankable binary file per sector.
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

   if (/^# Subsector (.): (.*?)\s*$/)
   {
      my $letter = $1;
      $subsectorsByLetter{$letter} = uc $2;
   }

   s/\{.+\}//g;
   s/\(...[+-].\)//g;
   s/\[.+\]//g;

   my ($col, $row, $name, $sp, $uwp, $tl, $tc, $nobility, $ba, $z, $p, $b, $gg, $w, $al, $st) = /^(\d\d)(\d\d) (.+)\s* (.)(......-)(.) (.+)\s*    (.....) (..) (.) (.)(.)(.) (..) (..)\S+ (.*)$/;

   next unless $name;

   # ---------------------------------------------
   #
   #   Truncate @name at 15 chars
   #
   # ---------------------------------------------
   $name = substr(uc $name, 0, 16); 

   # ---------------------------------------------
   #
   #   Split up SAHPGL
   #
   # ---------------------------------------------
   my ($siz, $atm, $hyd, $pop, $gov, $law) = split '', $uwp;

   # encode HYD + POP
   my $hypop = $hyd << 4 + $pop;

   # encode GOV + LAW
   my $golaw = $gov << 4 + $law;

   # ---------------------------------------------
   #
   #   Gently manage starport + base codes
   #
   # ---------------------------------------------
   $ba =~ s/[JK]/N/;
   $ba =~ s/M/S/;

   $ba =~ s/NS/A/;			# A
   $ba =~ s/NW/B/;			# B
   $ba = 'C' if $ba =~ /C/;		# C
   $ba = 'D' if $ba =~ /D/;		# D
   
   $ba =~ s/[- ]//g;
   $ba = ' ' unless $ba =~ /[A-Z]/;	# empty

   #print "bases [$ba]\n" if length($ba) > 1;
   

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
   my $ix = 0; # print "NEED IMPORTANCE\n";

   # ---------------------------------------------
   #
   #  Nobility Code
   #
   # ---------------------------------------------
   $nobility =~ s/^.*(.)\s*/$1/; # save the last non-space character, if it exists
   $nobility = ' ' unless $nobility =~ /[A-Z]/i;

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
   $st =~ s/\s*$//;

   my @halfstar = StarCodec::encodeStarData( $st );
   my $stp = pack 'CC', @halfstar;
    
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
   my $aHexCol = (129 + $sx) * 32 + $col;
   my $aHexRow = (126 + $sy) * 40 + $row;

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

   $rec  = pack 'vv',$aHexCol,$aHexRow; # $00 - $03

   $rec .= pack 'A16', $name;           # 16b

   $rec .= pack 'A', $sp;               # 6b
   $rec .= pack 'C', $siz;       	# 
   $rec .= pack 'C', $atm;       	# 
   $rec .= pack 'C', $hypop;       	# 
   $rec .= pack 'C', $golaw;       	# 
   $rec .= pack 'C', $tl{$tl};  	# 

   $rec .= pack 'A', $ba;               # 6b 
   $rec .= pack 'C', $tcSan;      	# 
   $rec .= pack 'C', $tc1;        	# 
   $rec .= pack 'A', $z;                # 
   $rec .= pack 'A', $bgg;              # 
   $rec .= pack 'A', $al;               # 

   $mapref->[ $aHexCol ]->[ $aHexRow ] = $rec;

   $minx = $aHexCol if $aHexCol < $minx;
   $maxx = $aHexCol if $maxx    < $aHexCol;

   $miny = $aHexRow if $aHexRow < $miny;
   $maxy = $aHexRow if $maxy    < $aHexRow;

   ++$count;

   #printf STDERR "$count:  $aHexCol [$minx,$maxx] $aHexRow [$miny,$maxy]\n";
}

print "writing $count entries\n";

open OUT, '>', 'charted-space.bin';
print OUT pack 'xx'; # 2 byte header to ignore
for my $x ($minx .. $maxx)
{
   for my $y ($miny .. $maxy)
   {
      if ($mapref->[$x]->[$y])
      {
         print OUT $mapref->[$x]->[$y]; 
      }
      #else  # otherwise, print empty hex
      #{
      #   print OUT pack( 'vvx28', $x, $y );
      #}
   }
}
close OUT;

