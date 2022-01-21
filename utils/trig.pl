

print "    Value  >   Sin  sign\n",
      "    -----  >   ---  ----\n";

open my $fp, '>', 'TRIG';
print $fp pack 'xx';      # initial 2 bytes

my $sinTable    = '';
my $arcsinTable = '';

# 256 + 64 + 1 is needed to handle sin and cos
for(my $value=0; $value<256 + 64 + 1; ++$value)
{
   my $theta    = 1.40625 * $value;         # degrees
   my $radians  = $theta / 57.295779513;
   my $sin      = int(100 * sin($radians));
   #my $sign     = 0;

   #$sign = 1 if $sin < 0;
   #$sin = abs($sin);

   printf("     %3d      %3d    \n",
	$value,
        $sin
	);

   my $outbyte = $sin;        # abs
   $outbyte |= 128 if $sign;  # flip bit 8 if < 0

   $sinTable .= pack 'C', $outbyte;
}

print $fp $sinTable;

close $fp;
