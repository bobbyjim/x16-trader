
open my $out, '>', 'BIGTESTFILE';

my $size = 100_000_000;

print "Creating a file of size $size\n";

for(1..100_000_000)
{
   print $out pack 'c', int(rand(256));
}
close $out;

print "Done\n";
