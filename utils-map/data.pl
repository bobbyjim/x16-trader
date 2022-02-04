
my @in = <STDIN>;

foreach (@in)
{
   next unless /(\d\d\d\d) (\w.*?)\s*([ABCDEX]......-.)/;
   print "$1 $3 $2\n";
}
