use strict;
use ComponentMap;

my ($componentMapRef, $componentArrayRef) = ComponentMap::getComponentMap();
my @componentMap = @$componentArrayRef;

open my $fp, '>', 'COMPONENTS.BIN';
for my $i (0..255)
{
   print $fp pack 'A16', uc $componentMap[$i];
}
close $fp;

