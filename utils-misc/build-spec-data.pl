use strict;
use autodie;

my %normalizeComment = (
    'A'  => 'A',
    'NA' => 'B',
    'I'  => 'I',
    'NI' => 'J',
    'P'  => 'P',
    'R'  => 'R'
);

print "build speculative trade database\n";

open my $out, '>', 'B3-TRADE.BIN';

print $out pack 'xx';

foreach (<DATA>)
{
    my ($hdr, $purchase, $resale) = split /:/;
    my ($label, $base, $quantity) = split /,/, $hdr;
    my ($dice, $multiplier)       = $quantity =~ /(\d)Dx?(\d+)?/;
    my @purchase                  = split /,/, $purchase;
    my @resale                    = split /,/, $resale;

    $multiplier = 1 unless $multiplier;

    foreach (@purchase, @resale)
    {
        my ($code, $value) = $_ =~ /^\s*(.*)?([-+].*?)\s*$/;
        $code = $normalizeComment{ $code };
        $_ = [$code, $value];
    }

    $base /= 100;  # this makes it fit unto an unsigned int.

    # dice is 3 bits (1 to 7)
    # multiplier is 4 bits (1 to 10)
#    my $quantity = $multiplier + $dice << 4;

    printf "%15s %5u %dD x%-2d \n",
        $label,
        $base,
        $dice,
        $multiplier;

    $label =~ s/\s*$//;
    my $z = length($label);

    # This is a tidy 32 byte record.
    print $out pack 'Z15', uc $label;
    print $out pack 'x';
    print $out pack 'v',   $base;      # in hcr (hundreds of credits)
    print $out pack 'C',   $dice;
    print $out pack 'C',   $multiplier;

    print $out pack 'Ac',  $purchase[0]->[0], $purchase[0]->[1];
    print $out pack 'Ac',  $purchase[1]->[0], $purchase[1]->[1];
    print $out pack 'Ac',  $purchase[2]->[0], $purchase[2]->[1];

    print $out pack 'Ac',  $resale[0]->[0], $resale[0]->[1];
    print $out pack 'Ac',  $resale[1]->[0], $resale[1]->[1];
    print $out pack 'Ac',  $resale[2]->[0], $resale[2]->[1];
}

close $out;


# Label, Base Price, Quantity, Purchase DMs (Ag, Na, Ri, Po, In, Ni), Resale DMs (Ag, Na, Ri, Po, In, Ni)
__DATA__
Textiles,           3000,   3Dx5    : A-7, NA-5, NI-3 : A-6, NA+1, R+3
Polymers,           7000,   4Dx5    : I-2, R-3, P+2   : I-2, R+3
Liquor,             10000,  1Dx5    : A-4             : A-3, I+1, R+2
Wood,               1000,   2Dx10   : A-6             : A-6, I+1, R+2
Crystals,           20000,  1D      : NA-3, I+4       : NA-3, I+3, R+3
Radioactives,     1000000,  1D      : I+7, NI-3, R+5  : I+6, NI-3, R-4
Steel,              500,    4Dx10   : I-2, R-1, P+1   : I-2, R-1, P+3
Copper,             2000,   2Dx10   : I-3, R-2, P+1   : I-3, R-1
Aluminum,           1000,   5Dx10   : I-3, R-2, P+1   : I-3, NI+4, R-1
Tin,                9000,   3Dx10   : I-3, R-2, P+1   : I-3, R-1
Silver,             70000,  1Dx5    : I+5, R-1, P+2   : I+5, R-1
Alloys,           200000,   1D      : I-3, NI+5, R-2  : I-3, NI+4, R-1
Petrochemicals,     10000,  1D      : NA-4, I+1, NI-5 : NA-4,I+3, NI-5
Grain,              300,    7Dx6    : A-2, NA+1, I+2  : A-2
Meat,               1500,   4Dx5    : A-2, NA+2, I+3  : A-2, I+2, P+1
Spices,             6000,   1Dx5    : A-2, NA+3, I+2  : A-2, R+2, P+3
Fruit,              1000,   2Dx5    : A-3, NA+1, I+2  : A-2, I+3, P+2
Pharmaceuticals,   100000,  1D      : NA-3, I+4, P+3  : NA-3, I+5, R+4
Gems,             1000000,  1D      : I+4, NI-8, P-3  : I+4, NI-2, R+8
Firearms,           30000,  2D      : I-3, R-2, P+3   : I-2, R-1, P+3
Ammunition,         30000,  2D      : I-3, R-2, P+3   : I-2, R-1, P+3
Blades,             10000,  2D      : I-3, R-2, P+3   : I-2, R-1, P+3
Tools,              10000,  2D      : I-3, R-2, P+3   : I-2, R-1, P+3
Body Armor,         50000,  2D      : I-1, R-3, P+3   : I-2, R+1, P+4
Aircraft,          100000,  1Dx10   : I-4, R-3        : NI+2, P+1
Air/Raft,          120000,  1Dx5    : I-3, R-2        : NI+2, P+1
Computers,        2000000,  1Dx5    : I-2, R-2        : NI+2,P+1,A-3
ATVs,                3000,  1Dx10   : I-2, R-2        : NI+2,P+1,A+1
AFVs,                7000,  1Dx10   : I-5, R-2, P+4   : NA-2, A+2, R+1
Farm Machinery,     30000,  1Dx5    : I-5, R-2        : A+5, NA-8, P+1
Electronics,       100000,  1Dx5    : I-4, R-3        : NI+2, P+1
Mechanical,         70000,  1Dx5    : I-5, R-3        : NI+3, A+2
Cybernetic,        250000,  1Dx5    : I-4, R-1        : NI+4, A+1, NA+2
Computer Parts,    150000,  1Dx5    : I-5, R-3        : NI+3, A+1, NA+2
Machine Tools,     750000,  1Dx5    : I-5, R-4        : NI+3, A+1, NA+2
Vacc Suits,        400000,  1Dx5    : NA-5, I-3, R-1  : NA-1, NI+2, P+1
