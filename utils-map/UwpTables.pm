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

#
#  Simplified Star Multiplex
#
#  A  = O B A
#  1  = Ia Ib II III IV
#  2  = II III IV
#  3  = II III
#  6  = V VI
#
my %simpleStar = 
(
   'A 1' => 0,  'A V' => 1,  'A D' => 2,
   'F 2' => 3,  'F V' => 4,  'F D' => 5,
   'G 2' => 6,  'G 6' => 7,  'G D' => 8,
   'K 3' => 9,  'K 6' => 10, 'K D' => 11,
   'M 3' => 12, 'M 6' => 13, 'M D' => 14
);

# ----------------------------------------------------------------------
#
#  Starport + Base Codes + Zone index
#
#  In case you ever want to compress these into 6 bits.
#
# ----------------------------------------------------------------------
my %spbz =
(
   'A'     => 0,  'AN'   => 1,  'AS'    => 2, 'ANS'   => 3,
   'AD'    => 4,                'AW'    => 5, 'ANW'   => 6,
   'B'     => 7,  'BN'   => 8,  'BS'    => 9, 'BNS'   => 10,
   'BD'    => 11,               'BW'    => 12, 'BNW'  => 13,
   'C'     => 14, 'CS'   => 15, 'D'     => 16, 'DS'   => 17,
   'E'     => 18, 'X'    => 19,

   # Amber zone
   'aA'     => 20, 'aAN'  => 21, 'aAS'  => 22, 'aANS'  => 23,
   'aAD'    => 24,               'aAW'  => 25, 'aANW'  => 26,
   'aB'     => 27, 'aBN'  => 28, 'aBS'  => 29, 'aBNS'  => 30,
   'aBD'    => 31,               'aBW'  => 32, 'aBNW'  => 33,
   'aC'     => 34, 'aCS'  => 35, 'aD'   => 36, 'aDS'   => 37,
   'aE'     => 38, 'aX'   => 39,

   # Red zone
   'rA'     => 40, 'rAN'  => 41, 'rAS'  => 42, 'rANS'  => 43,
   'rAD'    => 44,               'rAW'  => 45, 'rANW'  => 46,
   'rB'     => 47, 'rBN'  => 48, 'rBS'  => 49, 'rBNS'  => 50,
   'rBD'    => 51,               'rBW'  => 52, 'rBNW'  => 53,
   'rC'     => 54, 'rCS'  => 55, 'rD'   => 56, 'rDS'   => 57,
   'rE'     => 58, 'rX'   => 59,
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
#  This set of unique trade code combinations was validated with
#  multiple random UWP sets from 200,000 up to 100,000,000 entries.
#
# ----------------------------------------------------------------------
=pod
my @codes = (
   ''              ,  # 6095.59 , 3000
   'Ag'            ,  # 2401.44 , 2000
   'Ag Ni'         ,  # 8394.71 , 3000
   'Ag Ni Ri'      ,  # 1603.24 , 4000
   'Ag Ri'         ,  # 1284.05 , 3000
   'As Ba Va'      ,  # 25.93   , 4000
   'As Hi In Na Va',  # 211.30  , 2000
   'As Lo Va'      ,  # 691.80  , 4000
   'As Na Ni Va'   ,  # 389.65  , 4000
   'As Na Va'      ,  # 553.04  , 3000
   'As Ni Va'      ,  # 847.67  , 4000
   'As Va'         ,  # 50.30   , 3000
   'Ba'            ,  # 583.69  , 4000
   'Ba De'         ,  # 1.34    , 5000
   'Ba De Po'      ,  # 64.65   , 4000
   'Ba Fl'         ,  # 78.84   , 5000
   'Ba Po'         ,  # 121.92  , 3000
   'Ba Va'         ,  # 75.79   , 5000
   'De'            ,  # 2.77    , 4000
   'De Hi'         ,  # 11.42   , 3000
   'De Hi In Na Po',  # 204.03  , 1000
   'De Hi In Po'   ,  # 99.54   , 1000
   'De Hi Na Po'   ,  # 155.37  , 2000
   'De Hi Po'      ,  # 46.15   , 2000
   'De Lo'         ,  # 38.47   , 5000
   'De Lo Po'      ,  # 1676.90 , 4000
   'De Na Ni Po'   ,  # 670.95  , 4000
   'De Na Po'      ,  # 956.92  , 3000
   'De Ni'         ,  # 47.72   , 5000
   'De Ni Po'      ,  # 2317.17 , 4000
   'De Ni Ri'      ,  # 21.68   , 6000
   'De Po'         ,  # 506.69  , 3000
   'De Ri'         ,  # 30.51   , 5000
   'Fl'            ,  # 1821.34 , 4000
   'Fl Hi In'      ,  # 627.58  , 2000
   'Fl Lo'         ,  # 2088.10 , 5000
   'Fl Ni'         ,  # 3730.14 , 5000
   'Hi'            ,  # 2416.74 , 2000
   'Hi In'         ,  # 1765.40 , 1000
   'Hi In Na'      ,  # 437.27  , 1000
   'Hi In Na Po'   ,  # 232.75  , 1000
   'Hi In Na Va'   ,  # 607.77  , 2000
   'Hi In Po'      ,  # 256.73  , 1000
   'Hi Na Po'      ,  # 275.75  , 2000
   'Hi Po'         ,  # 203.61  , 2000
   'Lo'            ,  # 15265.79, 4000
   'Lo Po'         ,  # 3218.21 , 3000
   'Lo Va'         ,  # 2029.97 , 5000
   'Na'            ,  # 1155.68 , 3000
   'Na Ni'         ,  # 807.47  , 4000
   'Na Ni Po'      ,  # 948.94  , 3000
   'Na Ni Va'      ,  # 1136.57 , 5000
   'Na Po'         ,  # 1350.80 , 2000
   'Na Va'         ,  # 1615.87 , 4000
   'Ni'            ,  # 15485.21, 4000
   'Ni Po'         ,  # 4796.86 , 3000
   'Ni Ri'         ,  # 989.51  , 5000
   'Ni Va'         ,  # 2486.40 , 5000
   'Po'            ,  # 1461.13 , 2000
   'Ri'            ,  # 2379.02 , 4000
   'Va'            ,  # 148.15  , 4000
);
=cut
my %coderef = (
   ''              =>  [ 6095.59 , 3000, 'Red Tape' ],
   'Ag'            =>  [ 2401.44 , 2000, 'Flowers' ],
   'Ag Ni'         =>  [ 8394.71 , 3000, 'Textiles' ],
   'Ag Ni Ri'      =>  [ 1603.24 , 4000, 'Local Fauna' ],
   'Ag Ri'         =>  [ 1284.05 , 3000, 'Exotic Foods' ],
   'As Ba Va'      =>  [ 25.93   , 4000, 'Fine Dusts' ],
   'As Hi In Na Va'=>  [ 211.30  , 1000, 'Refined Alloys' ],
   'As Lo Va'      =>  [ 691.80  , 4000, 'Unprocessed Ore' ],
   'As Na Ni Va'   =>  [ 389.65  , 4000, 'Processed Ore' ],
   'As Na Va'      =>  [ 553.04  , 3000, 'Processed Metal' ],
   'As Ni Va'      =>  [ 847.67  , 4000, 'Nonferrites' ],
   'As Va'         =>  [ 50.30   , 3000, 'Sifted Regolith' ],
   'Ba'            =>  [ 583.69  , 4000, 'Archaeologicals' ],
   'Ba De'         =>  [ 1.34    , 5000, 'Nectars' ],
   'Ba De Po'      =>  [ 64.65   , 4000, 'Minerals' ],
   'Ba Fl'         =>  [ 78.84   , 5000, 'Palliatives' ],
   'Ba Po'         =>  [ 121.92  , 3000, 'Art' ],
   'Ba Va'         =>  [ 75.79   , 5000, 'Vacc Suits' ],
   'De'            =>  [ 2.77    , 4000, 'Pheromones' ],
   'De Hi'         =>  [ 11.42   , 3000, 'Polymers' ],
   'De Hi In Na Po'=>  [ 204.03  , 1000, 'Computers' ],
   'De Hi In Po'   =>  [ 99.54   , 1000, 'Biologics' ],
   'De Hi Na Po'   =>  [ 155.37  , 2000, 'Silica Dusts' ],
   'De Hi Po'      =>  [ 46.15   , 2000, 'Body Parts' ],
   'De Lo'         =>  [ 38.47   , 5000, 'Sifted Minerals' ],
   'De Lo Po'      =>  [ 1676.90 , 4000, 'Adhesives' ],
   'De Na Ni Po'   =>  [ 670.95  , 4000, 'Raw Silicates' ],
   'De Na Po'      =>  [ 956.92  , 3000, 'Salts' ],
   'De Ni'         =>  [ 47.72   , 5000, 'Gypsum' ],
   'De Ni Po'      =>  [ 2317.17 , 4000, 'Rare Earths' ],
   'De Ni Ri'      =>  [ 21.68   , 6000, 'Spices' ],
   'De Po'         =>  [ 506.69  , 3000, 'Metalloids' ],
   'De Ri'         =>  [ 30.51   , 5000, 'Hallucinogens' ],
   'Fl'            =>  [ 1821.34 , 4000, 'Acids' ],
   'Fl Hi In'      =>  [ 627.58  , 2000, 'Chelates' ],
   'Fl Lo'         =>  [ 2088.10 , 5000, 'Chlorides' ],
   'Fl Ni'         =>  [ 3730.14 , 5000, 'Organifluorines' ],
   'Hi'            =>  [ 2416.74 , 2000, 'Entertainments' ],
   'Hi In'         =>  [ 1765.40 , 1000, 'Durable Goods' ],
   'Hi In Na'      =>  [ 437.27  , 1000, 'Machine Parts' ],
   'Hi In Na Po'   =>  [ 232.75  , 0,    'Pharmaceuticals' ],
   'Hi In Na Va'   =>  [ 607.77  , 2000, 'Nostrums' ],
   'Hi In Po'      =>  [ 256.73  , 0,    'Gravitics' ],
   'Hi Na Po'      =>  [ 275.75  , 1000, 'Copper' ],
   'Hi Po'         =>  [ 203.61  , 1000, 'Alloys' ],
   'Lo'            =>  [ 15265.79, 4000, 'Bulk Fibers' ],
   'Lo Po'         =>  [ 3218.21 , 3000, 'Silanes' ],
   'Lo Va'         =>  [ 2029.97 , 5000, 'Insulants' ],
   'Na'            =>  [ 1155.68 , 3000, 'Isotopes' ],
   'Na Ni'         =>  [ 807.47  , 4000, 'Petrochemicals' ],
   'Na Ni Po'      =>  [ 948.94  , 3000, 'Abrasives' ],
   'Na Ni Va'      =>  [ 1136.57 , 5000, 'Vacc Gems' ],
   'Na Po'         =>  [ 1350.80 , 2000, 'Pigments' ],
   'Na Va'         =>  [ 1615.87 , 4000, 'Crystals' ],
   'Ni'            =>  [ 15485.21, 4000, 'Radioactives' ],
   'Ni Po'         =>  [ 4796.86 , 3000, 'Gems' ],
   'Ni Ri'         =>  [ 989.51  , 5000, 'Tisanes' ],
   'Ni Va'         =>  [ 2486.40 , 5000, 'Catalysts' ],
   'Po'            =>  [ 1461.13 , 2000, 'Wafers' ],
   'Ri'            =>  [ 2379.02 , 4000, 'Machine Tools' ],
   'Va'            =>  [ 148.15  , 4000, 'Sludges' ],
);
my $i = 0;
my %codes = ();
for (sort keys %coderef)
{
  $codes{ $_ } = sprintf "%-2s", $i;
  $i++;
}

sub getCodes { sort keys %coderef }
sub getCoderef { %coderef }

sub getTCIndexForCodeString
{
   my $codeString = shift;
   return $codes{ $codeString };
}

1;

