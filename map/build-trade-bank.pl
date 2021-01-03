use strict;
use UwpTables;
use Data::Dumper;
$Data::Dumper::Indent = 1;
$Data::Dumper::Terse = 1;

my $buf;
my %demandMatrix = (
	'Ag' =>	[ qw/Ag As De    Hi In Ri Va/ ],  # Ag
	'As' => [ qw/   As          In Ri Va/ ],  # As
	'Ba' => [ qw/               In      / ],  # Ba
	'De' => [ qw/      De               / ],  # De
	'Fl' => [ qw/         Fl    In      / ],  # Fl
	'Hi' => [ qw/            Hi         / ],  # Hi
	'In' => [ qw/Ag As De Fl Hi In Ri Va/ ],  # In
	'Na' => [ qw/   As De             Va/ ],  # Na
	'Po' => [ qw/Ag          Hi In Ri   / ],  # Po
	'Ri' => [ qw/Ag    De    Hi In Ri   / ],  # Ri
	'Va' => [ qw/   As          In    Va/ ],  # Va
);

my @table = UwpTables::getCodes();
my $tradeMatrix = {};
my $sourceIndex = "00";

foreach my $source (@table)
{
   # Index the sales matrix based on the source world trade codes.
   foreach my $sourceTradeCode (split /\s/, $source)
   {
      my $demandEnhancerCodeRef = $demandMatrix{$sourceTradeCode};
      next unless $demandEnhancerCodeRef;

      my @demandEnhancerCodes = @$demandEnhancerCodeRef;

      # Check each market world for the demand enhancers.
      my $marketIndex = "00";
      foreach my $market (@table)
      {
         my $price = 0;
         foreach my $demandCode (@demandEnhancerCodes)
         {
            if ($market =~ /$demandCode/) # found one
            { 
               ++$price;
            }
         }
         $price = -$price if $sourceTradeCode eq 'Po';
         $price += 5;
         $tradeMatrix->{"$sourceIndex.$marketIndex"} = $price;
         ++$marketIndex;
      }
   }
   ++$sourceIndex;
}

# Now create a binary file and pad to 64 entries 
# (i.e. this becomes a 64x64 matrix filling 4K exactly)
open my $fp, '>', 'MARKET.BIN';
for my $source (0..63)
{
   for my $market (0..63)
   {
      my $value = $tradeMatrix->{"$source.$market"};
      $value = 5 unless defined $tradeMatrix->{"$source.$market"};

      print $fp pack 'C', $value; # $source, $market, $value, '|'; 
      # write 3 bytes just for testing purposes!
   }
}
close $fp;

=pod
Ag sells: Textiles,Wood,Liquor,Fruit,Grain,Meat,Spices
Ag buys : Machinery,Parts,Cybernetics,Tools
Ri sells: Tools,Polymers,Parts,Armor,Machinery,Vehicles,Weapons,Alloys,Metals
Ri buys : Gems,Pharmaceuticals,Spices,Crystals,Polymers,Wood,Liquor
Ni sells: Gems,Petrochem,Radioactives,Textiles
Ni buys : Metals,Alloys,Cybernetics,Tools,Parts,Vehicles,Computers
In sells: Parts,Tools,Vehicles,Cybernetics,Weapons,Alloys,Metals,Polymers
In buys : Radioactives,Metals,Gems,Liquor,Wood
Po sells: Vehicles,Weapons,Pharmaceuticals,Polymers,Metals,Steel
Po buys : Steel,Armor,Spices,Weapons,Fruit,Vehicles
=cut

sub abbreviateTradeCodes
{
   foreach (UwpTables::getCodes())
   {
      s/Ag/A/;
      s/As/C/;
      s/Ba/B/;
      s/De/D/;
      s/Fl/F/;
      s/Hi/H/;
      s/In/I/;
      s/Lo/L/;
      s/Na/M/;
      s/Ni/J/;
      s/Po/K/;
      s/Ri/E/;
      s/Va/G/;
      s/\s//g;

      my $tc = sprintf("%-5s", $_);
      print "[$tc]\n";
   }
}

