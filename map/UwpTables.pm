package UwpTables;

# ----------------------------------------------------------------------
#
#  Belts GGs and Zone 
#
# ----------------------------------------------------------------------
my %bgz = 
(
   '   ' => 0,   '  A' => 1,  '  R' => 2,
   ' 1 ' => 4,   ' 1A' => 5,  ' 1R' => 6,
   '1  ' => 8,   '1 A' => 9,  '1 R' => 10,
   '11 ' => 12,  '11A' => 13, '11R' => 14
);

sub encodeBGandZone # not worth the trouble!!
{
   my $b = shift;
   my $gg = shift;
   my $zone = shift;

   $b  = ' ' if $b = '0';
   $gg = ' ' if $gg = '0';
   $b  = '1' if $b ne ' ';
   $gg = '1' if $gg ne ' ';
   $zone = ' ' unless $zone =~ /[AR]/;

   my $code = $b . $g . $zone;
   return $bgz{ $code };
}

# ----------------------------------------------------------------------
#
#  Starport + Base Codes index
#
#  In case you ever want to compress Starport + Base Codes
#
# ----------------------------------------------------------------------
my %spb =
(
   'A'     => 0, 'AN'    => 1, 'AS'    => 2, 'ANS'   => 3,
   'AD'    => 4, 'ADS'   => 5, 'AW'    => 6, 'ANW'   => 7,
   'B'     => 8, 'BN'    => 9, 'BS'    => 10, 'BNS'  => 11,
   'BD'    => 12, 'BDS'  => 13, 'BW'   => 14, 'BNW'  => 15,
   'C'     => 16, 'CS'   => 17, 'D'    => 18, 'DS'   => 19,
   'E'     => 20, 'X'    => 21
);

sub encodeStarportAndBases
{
   my $starport = shift;
   my $bases    = shift;
   my $str = $starport . $bases;
   $str =~ s/[- ]//g;
   return $spb{ $str };
}

# ----------------------------------------------------------------------
#
#  Trade System index
#
#  WHY THIS IS IMPORTANT:
#
#  This catalog is a mapping between ALL GENERATABLE COMBINATIONS
#  of trade codes WHICH ARE USED BY THE TRADE SYSTEM.
#
#  This therefore has value BEYOND simply cataloging trade remarks
#  that apply to a world.
#
#  SAMPLE SIZE:
#
#  This set of 54 unique trade code combinations was validated with
#  multiple random UWP sets from 200,000 up to 100,000,000 entries.
#
# ----------------------------------------------------------------------
my @codes = (
   ' ',
   'Ag',
   'Ag Ni',
   'Ag Ni Ri',
   'Ag Ri',
   'As Ba Va',
   'As Hi In Na Va',
   'As Lo Va',
   'As Na Ni Va',
   'As Na Va',
   'As Ni Va',
   'As Va',
   'Ba',
   'Ba De Po',
   'Ba Fl',
   'Ba Po',
   'Ba Va',
   'De Hi In Na Po',
   'De Hi In Po',
   'De Hi Na Po',
   'De Hi Po',
   'De Lo Po',
   'De Na Ni Po',
   'De Na Po',
   'De Ni Po',
   'De Po',
   'Fl',
   'Fl Hi In',
   'Fl Lo',
   'Fl Ni',
   'Hi',
   'Hi In',
   'Hi In Na',
   'Hi In Na Po',
   'Hi In Na Va',
   'Hi In Po',
   'Hi Na Po',
   'Hi Po',
   'Lo',
   'Lo Po',
   'Lo Va',
   'Na',
   'Na Ni',
   'Na Ni Po',
   'Na Ni Va',
   'Na Po',
   'Na Va',
   'Ni',
   'Ni Po',
   'Ni Ri',
   'Ni Va',
   'Po',
   'Ri',
   'Va'
);
my $i = 0;
my %codes = ();
for (@codes)
{
  $codes{ $_ } = sprintf "%-2s", $i;
  $i++;
}

sub getCodes { @codes }

sub getTCIndexForCodeString
{
   my $codeString = shift;
   return $codes{ $codeString };
}

1;

