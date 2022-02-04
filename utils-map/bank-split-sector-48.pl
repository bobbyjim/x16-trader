use UText;
use UwpTables;
use StarCodec;

# -------------------------------------------------------------------
#
#  Generates binary subsectors suitable for loading into banked RAM.
#
#  This creates 48-byte records per UWP.
#
# -------------------------------------------------------------------

my $uwpCount  = 0;
my $byteCount = 0;
my @buffer    = ();
my $tl = 0;
my %hex = map{ $_ => $tl++ } split '', '0123456789ABCDEFGHJKLMNPQRSTUVWXYZ';
my @letter = ('A'..'P');

my $sector = shift or die "SYNOPSIS: $0 SEC-file\n";
my $sectorName; # = uc $1 if $sector =~ /^(.*?)\.\w+$/;

open IN, '<', $sector;
my @data = <IN>;
close IN;

my ($sx,$sy);
my $mapref;
my %subsectorsByLetter;
my $letter;


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
   $name = substr(uc $name, 0, 15); 

   # ---------------------------------------------
   #
   #   Handle base codes
   #
   # ---------------------------------------------
   $ba =~ s/-/ /;
   my ($ba1, $ba2) = split '', $ba;

   # ---------------------------------------------
   #
   #   Manage SAHPGL
   #
   # ---------------------------------------------
   my ($siz, $atm, $hyd, $pop, $gov, $law) = split '', $uwp;

   my $SA = $hex{$siz} + $hex{$atm} << 4;
   my $HP = $hex{$hyd} + $hex{$pop} << 4;
   my $GL = $hex{$gov} + $hex{$law} << 4;

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
#   $nobility = ord($nobility);

   # ---------------------------------------------
   #
   #    Zone    cleaned up
   #    Belt + GG: ' ', 'B', 'G', or '2'
   #
   # ---------------------------------------------
   $z = ' ' if $z eq '-';

   my $bgg = ' ';
   $bgg = 'B' if $b > 0;
   $bgg = 'G' if $gg > 0;
   $bgg = '2' if $b > 0 && $gg > 0;

=pod
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
      $st1[0],                 # byte 1
      $st1[1] * 10 + $st1[2],  # byte 2
      $st1[3],                 # byte 3
      $st1[4] * 10 + $st1[5],  # byte 4
      $st1[6],                 # byte 5
      $st1[7] * 10 + $st1[8]   # byte 6
   );

   my $stp = pack 'aCaCaC', @st2;
=cut

   $st =~ s/\s*$//;

   my @halfstar = StarCodec::encodeStarData( $st );
   my $stp = pack 'CC', @halfstar;
   #print "$st => @halfstar\n";
    
   # --------------------------------------
   #
   #  Prepare Allegiance code
   #
   # --------------------------------------
   $al = uc $al;

   # --------------------------------------
   #
   #  Prepare *subsector* col and row
   #
   # --------------------------------------
   my $sc = (($col-1) % 8)   + 1;
   my $sr = (($row-1) % 10)  + 1;
 
   my $location = sprintf("%d%02d", $sc, $sr); # this is for ease of printing

   # -------------------------------------
   #
   #  Calculate subsector address
   #
   # -------------------------------------
   my $subx = 516 + $sx * 4 + int(($col-1)/8);
   my $suby = 504 + $sy * 4 + int(($row-1)/10);

   # --------------------------------------
   #
   #  Prepare absolute hex (two 15 bit numbers)
   #
   # --------------------------------------
   my $absoluteHexColumn = (129 + $sx) * 32 + $col;
   my $absoluteHexRow    = (126 + $sy) * 40 + $row;

   #printf "Absolute coords: %04x, %04x\n", $absoluteHexColumn, $absoluteHexRow;

   # --------------------------------------
   #
   #  UText-encode text elements
   #
   # --------------------------------------
#   my $str = sprintf "%-15s%1s%1s%1s%1s%1s%2s", 
#	$name, 
#	$sp, 
#	$ba1, 
#	$ba2, 
#	$nobility, 
#	$z, 
#	$al;
#
#   # input: 22 chars
#   # output: 16 chars
#   my @utext = UText::convert( $str );

   # now strip trailing whitespace from $name
   $name =~ s/\s*$//;
   $name .= '.';

   # --------------------------------------
   #
   #  Build binary data
   #
   # --------------------------------------
   my $rec = pack 'CC',$sc,$sr; 	# 00-01  
   $rec .= pack 'vv',$absoluteHexColumn,$absoluteHexRow; # 02-05

   #
   #   These characters are formatted for convenience 
   #
   $rec .= pack "A3", $location;        # 06-08
   $rec .= ' ';                         # 09    and a space
   $rec .= pack "A", $sp; 		# 0a
   $rec .= pack "AA", $ba1, $ba2;	# 0b-0c
   $rec .= ' ';                         # 0d    and a space
   $rec .= pack "A15", $name;           # 0e-1c (15 b)
   $rec .= ' ';                         # 1d    and a space
   $rec .= pack "A", $bgg;          	# 1e
   $rec .= pack "A", $al;       	# 1f
   $rec .= pack "A", $z;                # 20
   $rec .= pack "A", $nobility;		# 21

   $rec .= pack "C", $SA;       	# 22
   $rec .= pack "C", $HP;       	# 23
   $rec .= pack "C", $GL;       	# 24
   $rec .= pack "C", $hex{$tl};  	# 25
   $rec .= pack "C", $tcSan;      	# 26
   $rec .= pack "C", $tc1;        	# 27
   $rec .= pack "C", $ix;        	# 28
   $rec .= pack "C", $w;         	# 29

   $rec .= $stp; 			# 2a-2b
   $rec .= $timestamp;                  # 2c-2e unused (y/m/d)
   $rec .= pack 'A', '-'; 		# 2f unused 

#   my $ss = chr( 65 + int(($col-1)/8) + 4 * int(($row-1)/10) );
   my $letter = $letter[ ($subx % 4) + 4 * ($suby % 4) ];

   $mapref->{ "ss$subx.$suby" }->{ 'x' } = $subx;
   $mapref->{ "ss$subx.$suby" }->{ 'y' } = $suby;
   $mapref->{ "ss$subx.$suby" }->{ 'map' }->[ $sc ]->[ $sr ] = $rec;
   $mapref->{ "ss$subx.$suby" }->{ 'letter' } = $letter;
   $mapref->{ "ss$subx.$suby" }->{ 'name' } = $subsectorsByLetter{$letter};
}

my %hexOffset = (
	'A' => [ 0, 0 ],
	'B' => [ 8, 0 ],
	'C' => [ 16, 0 ],
	'D' => [ 24, 0 ],
	'E' => [ 0, 10 ],
	'F' => [ 8, 10 ],
	'G' => [ 16, 10 ],
	'H' => [ 24, 10 ],
	'I' => [ 0, 20 ],
	'J' => [ 8, 20 ],
	'K' => [ 16, 20 ],
	'L' => [ 24, 20 ],
	'M' => [ 0, 30 ],
	'N' => [ 8, 30 ],
	'O' => [ 16, 30 ],
	'P' => [ 24, 30 ],
);

my $secx0 = 516 + $sx * 4;
my $secy0 = 504 + $sy * 4;

# create subsector files
foreach my $name (sort keys %$mapref)
{
   # --------------------------------------------
   # build header
   # --------------------------------------------
   my $letter = $mapref->{ $name }->{ 'letter' };
   my $ssname = sprintf( "%-23s", $mapref->{ $name }->{ 'name' } );
   my $hexnums = "0123456789ABCDEFGHJKLMNPQRSTUVWXYZ";
   my $x = $mapref->{ $name }->{'x'}; # subsector x
   my $y = $mapref->{ $name }->{'y'}; # subsector y

   my $header = pack("vv", $x, $y);           #  4 bytes, little-endian      $00-03
   $header .= substr( $sectorName, 0, 23 );   # 23 bytes                     $04-1a
   $header .= pack('x');                      #  1 null byte                 $1b
   $header .= pack 'A', $letter;              #  1 byte	                     $1c
   $header .= pack 'A23', $ssname;            # 23 bytes                     $1d-33
   $header .= pack('x');                      #  1 null byte                 $34
   $header .= pack('x11');                    # 11 null bytes                $35-3f
   $header .= pack('AC', 'X', 16);            #  2 bytes of junk             $40-41
   $header .= pack('x62');		      # 62 null bytes                $42-7f
   $header .= pack('AC', 'X', 16);            #  2 bytes of junk             $80-41
   $header .= pack('x62');		      # 62 null bytes                $82-bf
   $header .= pack('A34', $hexnums);          # 34 bytes: ehex!  why not?    $c0-e2

   my $pad = 256 - length($header);

   $header .= pack("x$pad");                  # pad block with 0x0

   # --------------------------------------------
   # create subsector file
   # --------------------------------------------
   print "Creating \U$name.bin\n";
   open OUT, '>', uc "$name.bin";
   print OUT pack('xx'); # remember, this isn't actually LOADED.
   print OUT $header;
   for my $col (1..8)
   {
      for my $row (1..10)
      {
         my $rec = $mapref->{$name}->{'map'}->[$col]->[$row];
         print OUT $rec if $rec;
         next if $rec;
    
         my $scol = $col + $hexOffset{ $letter }->[0];
         my $srow = $row + $hexOffset{ $letter }->[1];
         my $absoluteHexColumn = (129 + $sx) * 32 + $scol;
         my $absoluteHexRow    = (126 + $sy) * 40 + $srow;
         print OUT pack("CCvvx42", $col, $row, $absoluteHexColumn, $absoluteHexRow);
      }
   }
   close OUT;
}

