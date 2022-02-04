

# give me subsector letter and I'll dump it only

my $sector    = shift || die "SYNOPSIS: $0 <sector> <subsector letter> [col shift]\n";
my $subsector = shift || die "SYNOPSIS: $0 <sector> <subsector letter> [col shift]\n";
my $colshift  = shift || 0;

my %ssmap = (
   'a' => [  1,  8,    1, 10 ],
   'b' => [  9,  16,   1, 10 ],
   'c' => [  17, 24,   1, 10 ],
   'd' => [  25, 32,   1, 10 ],
   'e' => [  1,  8,   11, 20 ],
   'f' => [  9, 16,   11, 20 ],
   'g' => [  17, 24,  11, 20 ],
   'h' => [  25, 32,  11, 20 ],
   'i' => [  1, 8,    21, 30 ],
   'j' => [  9, 16,   21, 30 ],
   'k' => [  17, 24,  21, 30 ],
   'l' => [  25, 32,  21, 30 ],
   'm' => [  1, 8,    31, 40 ],
   'n' => [  9, 16,   31, 40 ],
   'o' => [  17, 24,  31, 40 ],
   'p' => [  25, 32,  31, 40 ]
);

my $map = $ssmap{ $subsector };
my ($colmin,$colmax,$rowmin,$rowmax) = @$map;

open my $in, '<', $sector;
foreach (<$in>)
{
   next unless /^(\d\d)(\d\d) /;
   $col = $1;
   $row = $2;

   next unless $col >= $colmin && $col <= $colmax && $row >= $rowmin && $row <= $rowmax;

   if ($colshift)
   {
      $col += sprintf("%02d", $colshift);
      s/^\d\d/$col/;
   }

   print;
}

