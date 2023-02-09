#
#  The challenge: compress .ACS files into binary records, and
#  produce a C structure to match it.
#
use YAML;
use strict;
use autodie;
use warnings;

my $shipDAOName = 'ShipDAO';
my $shipStructName = 'Ship';

my %fields;     # registry
my %componentIndex; # by name
my @fields;     # ordered

my @tl = (0..9, 'A'..'H', 'J'..'N', 'P'..'Z');
my %cfg = ('C' => 0, 'B' => 1, 'P' => 2, 'U' => 3, 'S' => 4, 'A' => 5, 'L' => 6);
my %owner = (
    Aslan               => 'A',
    'Baraccai Technum'  => 'B',
    Droyne              => 'D',
    Scouts              => 'E', # imperial scouts, labs, surveys, etc
    Humbolt             => 'H',
    Imperial            => 'I', # imperial commercial
    'Ling Standard'     => 'I',
    'Judges Guild'      => 'J',
    Delta               => 'L',
    'Al Morai'          => 'M',
    Navy                => 'N', # imperial cruisers, escorts, etc
    Oberlindes          => 'I',
    'Paranoia Press'    => 'P',
    Republic            => 'R',
    'Republic of Regina' => 'R',
    'Sword Worlds'      => 'S',
    Tukera              => 'T',
    Universal           => 'U',
    Vargr               => 'V',
    Exotic              => 'X',
    DayStar             => 'Y',
    Zhodani             => 'Z',
);

my %emplacements =  # 3 bits
(
    'T1'    => 0,
    'T2'    => 32,
    'T3'    => 64,
    'T4'    => 64,   # downgrade the T4
    'B1'    => 96,
    'B2'    => 128,
    'Ba'    => 160,
    'L'     => 196,
    'M'     => 224,
);

my %supportedWeapons =  # 5 bits
(
    'Empty'                 => 1,
    'Mining Laser'          => 2,
    'Pulse Laser'           => 3,
    'Beam Laser'            => 4,
    'Plasma Gun'            => 5,
    'Fusion Gun'            => 6,
    'Salvo Rack'            => 7,
    'Missile'               => 8,
    'KK Missile'            => 9,
    'AM Missile'            => 10,
    'Jump Damper'           => 11,
    'Tractor/Pressor'       => 12,
    'Inducer'               => 13,
    'Disruptor'             => 14,
    'Stasis'                => 15,
    'Sandcaster'            => 16,
    'Hybrid L-S-M'          => 17,
    'Particle Accelerator'  => 18, 
    'Meson Gun'             => 19,
);

my %weaponAbbrev =  # 5 bits
(
    'Mining Laser'          => 'ml',
    'Pulse Laser'           => 'pl',
    'Beam Laser'            => 'l',
    'Plasma Gun'            => 'pg',
    'Fusion Gun'            => 'fg',
    'Salvo Rack'            => 'sr',
    'Missile'               => 'm',
    'KK Missile'            => 'kk',
    'AM Missile'            => 'am',
    'Jump Damper'           => 'jd',
    'Tractor/Pressor'       => 'tp',
    'Inducer'               => 'in',
    'Disruptor'             => 'di',
    'Stasis'                => 'st',
    'Sandcaster'            => 's',
    'Hybrid L-S-M'          => 'hy',
    'Particle Accelerator'  => 'pa', 
    'Meson Gun'             => 'mg',
);

my @out;

sub build_component_structure
{
    my $unit = shift;                #  5 bits
    my $emplacement = shift || 0;    #  3 bits

    return $unit + $emplacement * 32;
}

open my $out, '>', '32B-ALL-SHIPS.BIN';
print $out pack 'xx';

my @acs = sort <*.acs>;
print STDERR "There are ", scalar @acs, " ships.\n";

open my $outfoxed, '>', 'BD-SHIPS.BIN';
print $outfoxed pack 'xx';

################################################################################
#
#   Print Mission Codes and Owners
#
################################################################################
my @missions = getMissionCodes();
my @owners   = getOwners();
my @cfgs     = getHullCfgs();
my @emplacements = getEmplacementNames();
my @weapons  = getWeaponNames();
my @problems = getProblems();

print $outfoxed pack 'Z12', uc $_ foreach @missions;
print $outfoxed '----';
print $outfoxed pack 'Z12', uc $_ foreach @owners;
print $outfoxed '--------';
print $outfoxed pack 'Z16', uc $_ foreach @cfgs;
print $outfoxed pack 'Z8',  uc $_ foreach @emplacements;
print $outfoxed pack 'Z12', uc $_ foreach @weapons;
print $outfoxed pack 'Z32', uc $_ foreach @problems;

################################################################################
#
#   Print Ship Designs
#
################################################################################
print $outfoxed pack 'Z64', uc "26 byte header, 22 byte component array";
print $outfoxed pack 'C', scalar @acs;
print $outfoxed pack 'x15';

my $index = 0;
foreach my $acsfile (@acs)
{
    my $yaml = YAML::LoadFile($acsfile);

    my $qsp     = $yaml->{ 'QSP' };

    if ($qsp =~ /-\w\w\w\w\w/)
    {
        print "SKIPPED (too large): $acsfile\n";
        next;
    }

    my ($mission, $hull, $cfg, $m, $j) = $qsp =~ /^(.).?.?-(.)(.)(.)(.)/;
    my $tl      = $yaml->{ 'TL' };
    my $name    = $yaml->{ 'Name' };
    my $tons    = $yaml->{ 'Tons' };
    my $mcr     = $yaml->{ 'MCr' };
    my $mcrp    = int(100 * $mcr / $tons);
    $mcrp = 127 if $mcrp > 127;

    my $ownerName   = $yaml->{ 'Owner' } || 'Imperial';
    $ownerName = 'Republic' if $ownerName =~ /Republic of Reg/;
    $ownerName = 'Paranoia Press' if $ownerName =~ /Paranoia Press/;

    my $owner = $owner{ $ownerName } || 'U'; 
    $owner = ord($owner) - 64; # 5 bits

    my $comfort = $yaml->{ 'Crew comfort' } || -1;
    my $demand  = $yaml->{ 'Passenger demand' } || -1;
    my $cargo   = $yaml->{ 'Cargo' } || 0;
    my $cargop  = int(100 * $cargo / $tons);
    my $fuelp   = $yaml->{ 'FuelPercentage' };

    my @components = @{$yaml->{ 'Components' }};
    my $components = join "\n", @components;
    my ($cpu)      = $components =~ / Model.(\d)/;
    my $collectors = grep(/Collectors/, @components);
    my $landers = grep(/Landers/, @components);
    my $scoops  = grep(/Scoops/,  @components);
    my $intakes = grep(/Intakes/, @components);
    my $bins    = grep(/Bins/,    @components);
    my $spaceSensors = grep(/^SS1 /, @components);
    my $worldSensors = grep(/^WS1 /, @components);
    my ($lbi)   = grep(/  Low Berth/, @components);
    my ($lb) = $lbi =~ /^PO0   - (..) \d{5}/ if $lbi;
    $lb |= 0;
    my @sri     = grep(/^PO0 .* Stateroom/, @components);
    my $sr = 0;
    foreach my $line (@sri) 
    {
       my ($num) = $line =~ /^PO0   - (..) \d{5}/;
       $sr += $num;
    }
    my ($bridge) = grep(/[Bb]ridge|Controls/, @components);
    $bridge     = 0 if $bridge =~ /Cramped/;
    $bridge     = 1 if $bridge =~ /Standard/;
    $bridge     = 2 if $bridge =~ /Spacious/;
    my ($avi)   = grep(/AV=[^0]/, @components);
    my ($av) = $avi =~ /AV=(\d+)\./ if $avi;

    my @weaponlist = grep(/WW1/, @components);
    my @weapons = (0) x 8;
    my @weaponSummary;
    foreach my $weapon (@weaponlist)
    {
        # column 32+ has the emplacement and then type.
        my ($empl) = $1 if $weapon =~ /(T1|T2|T3|T4|B1|B2|Bay|LBay)/;
        next unless $empl;

        my ($wpn)  = $1 if $weapon =~ /$empl (.*)$/;
        next unless $wpn;
        next if $wpn =~ /Empty/;
        next unless $supportedWeapons{ $wpn };
        
        $empl = 'Ba' if $empl eq 'Bay';
        $empl = 'L'  if $empl eq 'LBay';
        $empl = 'M'  if $empl eq 'Main';

        my $value = $supportedWeapons{ $wpn } + $emplacements{ $empl };

        #print STDERR sprintf "$value [%s, %s]\n", $empl, $wpn;

        unshift @weapons, $value;

        push @weaponSummary, $empl . uc $weaponAbbrev{ $wpn };
    }
    @weapons = @weapons[0..7]; # take the first 8
#    print STDERR "weapons: @weapons\n";

    $av = $tl if $av < $tl;
    $av *= 2 if $tl >= 13;   # charged
    $av = 127 if $av > 127;  # max

    $comfort += 3;
    $demand  += 3;

    $comfort = 0 if $comfort < 0;
    $demand  = 0 if $demand  < 0;

    $comfort = 7 if $comfort > 7;
    $demand  = 7 if $demand  > 7;

    $sr = 63 if $sr > 63;
    $lb = 63 if $lb > 63;

    #
    #  There are lots of other neat things we could look for, including:
    #  - clinic
    #  - surgery
    #  - counsellor
    #  - barracks
    #  - hop drive
    #  - small craft
    #

    push @out, sprintf "%s %s-%s%s%s%s %-15s %-3s  %3s   %3s  %2d:%-2d  %4d  %s\n",
        chr($owner+64),                # one letter
#        $ownerName,                    # full string
        #$owner,
        $mission, $hull, $cfg, $m, $j,  # QSP
        $name,
        $av,
#        $cpu,
#        $spaceSensors,
#        $worldSensors,
#        $scoops,
        int($cargop * $tons / 100),
        int($fuelp  * $tons / 100),
#        $av,
        $sr, $lb,
        int($mcrp * $tons / 100),
        join ' ', @weaponSummary;

    my $cfgtl = $cfg{$cfg} + ($tl << 3);
    my $dmco  = $comfort   + ($demand << 3);
    ############################################################################
    #
    #  Write as a packed header, plus an array of components.
    #  Simpler to write = simpler to manage.
    #
    ############################################################################
    print $outfoxed pack 'C', $index;                                         #
    print $outfoxed pack 'Z16', uc $name;                                     #  Header
    print $outfoxed pack 'AAC', chr($owner+64), $mission, int($tons/100);     #
    print $outfoxed pack 'CCC', $cfgtl, $av, $mcrp;                           #
    print $outfoxed pack 'CCC', $sr, $lb, $dmco;                              #

    # Component array
    print $outfoxed pack 'C',     build_component_structure($m);
    print $outfoxed pack 'C',     build_component_structure($j);
    print $outfoxed pack 'C',     build_component_structure($bridge);
    print $outfoxed pack 'C',     build_component_structure($cpu);
    print $outfoxed pack 'C',     build_component_structure($cargop);
    print $outfoxed pack 'C',     build_component_structure($fuelp);
    print $outfoxed pack 'C',     build_component_structure($spaceSensors);
    print $outfoxed pack 'C',     build_component_structure($worldSensors);
    print $outfoxed pack 'C',     build_component_structure($scoops);
    print $outfoxed pack 'C',     build_component_structure($collectors);
    print $outfoxed pack 'x';     # stealth
    print $outfoxed pack 'x';     # damper
    print $outfoxed pack 'x';     # screen
    print $outfoxed pack 'x';     # globe
    print $outfoxed pack 'C8',    @weapons;   # "hardpoints"

    ############################################################################
    #
    #  Write as packed bitfields.
    #
    ############################################################################
    $cpu = 7 if $cpu > 7;

    $m = 7 if $m > 7;
    $j = 7 if $j > 7;

    print $out pack 'C', $index;
    print $out pack 'A15x', uc $name;

    # byte 17
    $mission = ord($mission) - 65; # 5 bits
    $cfg     = $cfg{$cfg} << 5;    # 3 bits
    print $out pack 'C', $mission + $cfg;                # byte 17

    # byte 18
    $hull    = ord($hull)    - 65; # 5 bits
    $m <<= 5;                      # 3 bits
    print $out pack 'C', $hull + $m;                     # byte 18

    # byte 19
    # $tl is next                  # 5 bits
    $j <<= 5;                      # 3 bits
    print $out pack 'C', $tl + $j;                       # byte 19
    print $out pack 'C', $cargop  + ($collectors << 7);  # byte 20
    print $out pack 'C', $mcrp    + ($landers << 7);     # byte 21
    print $out pack 'C', $fuelp   + ($scoops << 7);      # byte 22
    print $out pack 'C', $av      + ($intakes << 7);     # byte 23
    print $out pack 'C', $sr      + ($bridge << 6);      # byte 24
    print $out pack 'C', $lb      + ($bins << 6);        # byte 25
    print $out pack 'C', $comfort + ($demand <<3);       # byte 26
    print $out pack 'C', $cpu;                           # byte 27
    print $out pack 'C', $spaceSensors +  ($worldSensors << 4); # byte 28
    print $out pack 'C', ($owner + 64);                  # byte 29
    print $out pack 'x';                                 # byte 30
    print $out pack 'x';                                 # byte 31

    ++$index;
}
close $out;
close $outfoxed;

$index = 0;
for (sort @out)
{
    unless ($index % 10)
    {
       print "\n";
       print "A QSP    Name            AV  Cargo Fuel  SR:LB   MCr  Weapons\n";
       print "- ------ --------------- --- ----- ----  -----  ----  ------------------------------------\n";
    }

    print $_;
    ++$index;
}


print "\n\n  ***** NOT GENERATING THE C FILES.  SUE ME.  ***** \n\n";
exit(0);


sub getMissionCodes
{
return (
'free trader',
'beagle',
'cruiser',
'defender',
'escort',
'freighter',
'frigate',
'special',
'special',
'seeker',
'safari ship',
'lab ship',
'liner',
'survey ship',
'special',
'corsair',
'special',
'merchant',
'scout',
'transport',
'packet',
'destroyer',
'barge',
'express',
'yacht',
);
}

sub getOwners
{
   return (
    'Aslan',
    'BT',
    'C/Unknown',
    'Droyne',
    'Scouts',
    'F/Unknown',
    'G/Unknown',
    'Humbolt',
    'Imperial',
    'JG',
    'K/Unknown',
    'Delta',
    'Al Morai',
    'Navy',
    'O/Unknown',
    'Narapoia',
    'Q/Unknown',
    'Republic',
    'Gram',
    'Tukera',
    'Universal',
    'Vargr',
    'W/Unknown',
    'Exotic',
    'DayStar',
    'Zhodani',
   );
}

sub getHullCfgs {
   return (
       'cluster',
       'braced',
       'planetoid',
       'unstreamlined',
       'streamlined',
       'airframe',
       'lift body');
}

sub getEmplacementNames {
    return qw/t1 t2 t3 b1 b2 bay lbay main/;
}
sub getWeaponNames {
    return (
    "-",
    "-",
    "mining las",       
    "pulse las"  ,       
    "beam las"    ,      
    "plasma gun"     ,     
    "fusion gun"      ,    
    "salvo rack"       ,   
    "missile"           ,  
    "k-missile"         , 
    "a-missile"          ,
    "damper"         ,
    "tractor"     ,
    "inducer"             ,
    "disruptor"           ,
    "stasis"              ,
    "sandcaster"          ,
    "hybrid lsm"        ,
    "pa",
    "meson gun"    

    );
}

sub getProblems{
    return (
    "the helm is not responding",
    "the jump drives are sparking",
    "a control console is on fire",
    "the computer has caught fire",
    "the cargo bay is ruptured",
    "we're leaking hydrogen fuel",
    "an antenna dish just melted",
    "a surface sensor just cracked",
    "fuel scoops are down",
    "the collector has a hole in it",
    "the stealth coating melted",
    "the damper is offline",
    "the mescon screen is down",
    "the globe generator went dark",
    "battery one is not responding",
    "battery two is not responding",
    "battery three is not responding",
    "battery four is not responding",
    "battery five is not responding",
    "battery six is not responding",
    "battery seven is not responding",
    "battery eight is not responding"    
    );
}