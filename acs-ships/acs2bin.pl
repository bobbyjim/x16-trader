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
    Humbolt             => 'H',
    Imperial            => 'I',
    'Ling Standard'     => 'I',
    'Judges Guild'      => 'J',
    Delta               => 'L',
    'Al Morai'          => 'M',
    'Oberlindes'        => 'I',
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


sub registerBitfield
{
    my $field = shift;
    my $size  = shift;
    my $isComponent = shift;
    my $componentNum = shift;

    return if $fields{ $field }; # registered
    ++$fields{ $field };
    if ($isComponent)
    {
        $componentIndex{ $field } = $componentNum; # for struct record
    }
    push @fields, sprintf "   unsigned %10s: $size;\n", $field;
}

sub registerString
{
    my $field = shift;
    my $size  = shift;

    return if $fields{ $field }; # registered
    ++$fields{ $field };
    push @fields, "   char $field\[$size];\n";
}

#
#  If you want to use this for a Component, then pass in 'Component*' as the type.
#  BUT.... I think the setter should never be a "Component".  I could be wrong...
#
sub buildSignaturesForField
{
    my ($objectType, $fieldName, $type) = @_;
    $type = 'char*' if $type eq 'char';
    my $getSignature = sprintf "%-10s %s_get%s($objectType* obj)",             $type, lcfirst $objectType, ucfirst $fieldName;

    $type = 'unsigned' if $type =~ /Component/; # A sneaking suspicion.

    my $setSignature = sprintf "%-10s %s_set%s($objectType* obj, $type value)", 'void', lcfirst $objectType, ucfirst $fieldName;
    return ($getSignature, $setSignature);
}

#
#  This is used for accessors for regular fields (strings, bitfields).
#
sub buildBodiesForField
{
    my ($fieldName, $type) = @_;

    my $getter = qq/{ return obj->$fieldName; }/;
    my $setter = qq/\n{\n   obj->$fieldName = value;\n}/;
       $setter = qq/\n{\n   strcpy(obj->$fieldName, value);\n}/ if $type =~ /char/;

    return ($getter, $setter);
}

#
#  Use this method for $shipStructName accessors.  It will assign
#  data to the component array when applicable.  Otherwise, it will
#  default to the standard bitfields (usually for "metadata").
#  
#  The component index is stored in $componentIndex{ $fieldName };
#
sub buildBodiesForComponentField
{
    my ($fieldName, $type) = @_;

    return buildBodiesForField($fieldName, $type) unless defined $componentIndex{ $fieldName };

    my $index = $componentIndex{ $fieldName };
    my $getter = qq/{ return &(obj->component[$index]); }\n/;
    my $setter = qq/\n{\n   obj->component[$index].value = value;\n}/;

    return ($getter, $setter);
}


open my $out, '>', '32B-ALL-SHIPS.BIN';
print $out pack 'xx';

open my $outfoxed, '>', 'SHIPS.BIN';
print $outfoxed pack 'xx';
print $outfoxed sprintf "%-64s", uc "26 byte header, 22 byte component array";

my $index = 0;
foreach my $acsfile (sort <*.acs>)
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

    unless($av)
    {
        $av = $tl;
        $av *= 2 if $tl >= 13; # charged
    }
    $av = 127 if $av > 127;

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

    push @out, sprintf "%s %s-%s%s%s%s %-15s      +%s    %2s  %2s:%-2s   %3s   %3s   %2s:%-2s   %3s %s\n",
        chr($owner+64), $mission, $hull, $cfg, $m, $j, 
        $name,
        $bridge,
        $cpu,
        $spaceSensors,
        $worldSensors,
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
    print $outfoxed pack 'A15x', uc $name;                                    #  Header
    print $outfoxed pack 'AAC', chr($owner+64), $mission, int($tons/100);     #
    print $outfoxed pack 'CCC', $cfgtl, $av, $mcrp;                           #
    print $outfoxed pack 'CCC', $sr, $lb, $dmco;                              #

    # Component array
    print $outfoxed pack 'CCCC',     $m, $j, $bridge, $cpu;
    print $outfoxed pack 'CC',       $cargop, $fuelp;
    print $outfoxed pack 'CCCC',     $spaceSensors, $worldSensors, $scoops, $collectors;
    print $outfoxed pack 'xxxx';     # stealth, damper, screen, globe
    print $outfoxed pack 'C8', @weapons;   # "hardpoints"

    ############################################################################
    #
    #  Write as packed bitfields with generated accessor C code.
    #  It's not as great as it sounds, but it is clever.
    #
    ############################################################################
    $cpu = 7 if $cpu > 7;

    $m = 7 if $m > 7;
    $j = 7 if $j > 7;

    print $out pack 'C', $index;
    registerBitfield("index", 8);

    print $out pack 'A15x', uc $name;
    registerString("name", 16);

    # byte 17
    $mission = ord($mission) - 65; # 5 bits
    $cfg     = $cfg{$cfg} << 5;    # 3 bits
    print $out pack 'C', $mission + $cfg;
    registerBitfield("mission", 5);
    registerBitfield("cfg", 3, 'component', 1);

    # byte 18
    $hull    = ord($hull)    - 65; # 5 bits
    $m <<= 5;                      # 3 bits
    print $out pack 'C', $hull + $m;
    registerBitfield("hull", 5);
    registerBitfield("mdrv", 3, 'component', 3);

    # byte 19
    # $tl is next                  # 5 bits
    $j <<= 5;                      # 3 bits
    print $out pack 'C', $tl + $j;
    registerBitfield("tl", 5);
    registerBitfield("jdrv", 3, 'component', 4);

    print $out pack 'C', $cargop  + ($collectors << 7);  # byte 20
    registerBitfield("cargop", 7);
    registerBitfield("collectors", 1, 'component', 16 );
    print $out pack 'C', $mcrp    + ($landers << 7);     # byte 21
    registerBitfield("mcrp", 7);
    registerBitfield("landers", 1);
    print $out pack 'C', $fuelp   + ($scoops << 7);      # byte 22
    registerBitfield("fuelp", 7, 'component', 5);
    registerBitfield("scoops", 1, 'component', 6 );
    print $out pack 'C', $av      + ($intakes << 7);     # byte 23
    registerBitfield("av", 7);
    registerBitfield("intakes", 1 );
    print $out pack 'C', $sr      + ($bridge << 6);      # byte 24
    registerBitfield("staterooms", 6);
    registerBitfield("bridge", 2, 'component', 0);
    print $out pack 'C', $lb      + ($bins << 6);        # byte 25
    registerBitfield("low", 6);
    registerBitfield("bins", 1 );
    registerBitfield("jammer", 1, 'component', 17);
    print $out pack 'C', $comfort + ($demand <<3);       # byte 26
    registerBitfield("comfort", 3);
    registerBitfield("demand", 3);
    registerBitfield("globe",  2, 'component', 20);
    print $out pack 'C', $cpu;                           # byte 27
    registerBitfield("cpu", 3, 'component', 2);
    registerBitfield("damper", 1, 'component', 18);
    registerBitfield("screen", 1, 'component', 19);
    registerBitfield("scrambler", 1);
    registerBitfield("spare1", 2);
    print $out pack 'C', $spaceSensors +  ($worldSensors << 4); # byte 28
    registerBitfield("combatSensors", 4, 'component', 21);
    registerBitfield("surveySensors", 4, 'component', 22);

    print $out pack 'C', ($owner + 64);                  # byte 29
    registerBitfield('owner',  5);
    registerBitfield('spare2', 3);

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
       print "A QSP    Name              Bridge  cpu  SS:WS  Cargo  Fuel  SR:LB   MCr Weapons\n";
       print "- ------ ----------------- ------  ---  -----  -----  ----  -- --  ---- ------------\n";
    }

    print $_;
    ++$index;
}


#####################################################################
#
#    Build the Header File
#
#####################################################################
open my $hout, '>', 'ship-struct.h';
print $hout <<EO_H_TOP;
#ifndef _SHIP_STRUCT_H_
#define _SHIP_STRUCT_H_

// auto-generated with perl

#define     START_ADDRESS   0xa000

#define     BRIDGE      0
#define     CFG         1
#define     CPU         2
#define     MDRV        3
#define     JDRV        4
#define     FUELP       5
#define     SCOOPS      6
#define     HARDPOINT_1 7
#define     HARDPOINT_2 8
#define     HARDPOINT_3 9
#define     HARDPOINT_4 10
#define     HARDPOINT_5 11
#define     HARDPOINT_6 12
#define     HARDPOINT_7 13
#define     HARDPOINT_8 14
#define     HARDPOINT_9 15
#define     COLLECTOR   16
#define     JAMMER      17
#define     DAMPER      18
#define     SCREEN      19
#define     GLOBE       20
#define     TRACKING    21
#define     SURVEY      22

typedef struct {
    unsigned value    : 8;
    unsigned something: 5;
    unsigned damage   : 3;
} Component;

//////////////////////////////////////////////////
//
//   $shipStructName
//
//////////////////////////////////////////////////
EO_H_TOP

my $components = "   Component component[28];\n";
print $hout "typedef struct \{\n", @fields, $components, "\} $shipStructName;\n\n";

print $hout <<EO_H_MIDDLE;

$shipStructName* build$shipStructName(int num, $shipStructName* ship);
void debugShip($shipStructName* ship);

//////////////////////////////////////////////////
//
//   $shipStructName accessors
//
//////////////////////////////////////////////////
EO_H_MIDDLE

foreach my $field (@fields)
{
    my ($type, $fieldName) = $field =~ /^\s+(\w+)\s+(\w+)/;
    $type = 'Component*' if defined $componentIndex{ $fieldName };
    my ($getsig, $setsig) = buildSignaturesForField( $shipStructName, $fieldName, $type);
    print $hout "$getsig;\n";
    print $hout "$setsig;\n";
}

print $hout "\n#endif\n";
close $hout;

#####################################################################
#
#    Build the C File
#
#####################################################################
open my $cout, '>', 'ship-struct.c';
print $cout <<EO_DAO_TOP;
// auto-generated with perl
#include <string.h>
#include <stdio.h>
#include "ship-struct.h"

//////////////////////////////////////////////////
//
//  $shipDAOName
//
//  I moved this out of the header and into the C file
//  in order to hide it from the rest of the system.
//
//////////////////////////////////////////////////
EO_DAO_TOP
print $cout "typedef struct \{\n", @fields, "\} $shipDAOName;\n\n";

my $BEGIN = '{';
my $END   = '}';

foreach my $field (@fields)
{
    my ($type, $fieldName) = $field =~ /^\s+(\w+)\s+(\w+)/;
    my ($getsig) = buildSignaturesForField( $shipDAOName, $fieldName, $type);
    my ($getbod) = buildBodiesForField( $fieldName, $type);
    print $cout "$getsig $getbod\n";
}

print $cout <<EO_ACCESSORS;
//////////////////////////////////////////////////
//
//   $shipStructName Accessors
//
//////////////////////////////////////////////////
EO_ACCESSORS

foreach my $field (@fields)
{
    my ($type, $fieldName) = $field =~ /^\s+(\w+)\s+(\w+)/;
    $type = 'Component*' if defined $componentIndex{ $fieldName };

    my ($getsig, $setsig) = buildSignaturesForField( $shipStructName, $fieldName, $type);
    my ($getbod, $setbod) = buildBodiesForComponentField( $fieldName, $type);
    print $cout "$getsig $getbod\n";
    print $cout "$setsig $setbod\n";
}

print $cout <<EO_ACCESSORS;
//////////////////////////////////////////////////
//
//   $shipStructName Builder
//
//////////////////////////////////////////////////
$shipStructName* build$shipStructName(int num, $shipStructName* ship)
$BEGIN
   $shipDAOName* data = ($shipDAOName*)(START_ADDRESS + (num<<5));

EO_ACCESSORS

foreach my $field (@fields)
{
   my ($type, $name) = $field =~ /^\s+(\w+)\s+(\w+)/;
   my $signature = lcfirst $shipStructName . "_set" . ucfirst $name;
   my $DAOsignature = lcfirst $shipDAOName . "_get" . ucfirst $name . "(data)";
   print $cout "   $signature( ship, $DAOsignature );\n";  # data->$name );\n"
}

print $cout "   return ship;\n";
print $cout "$END\n";

print $cout <<EODEBUG;

//////////////////////////////////////////////////
//
//   debuggers
//
//////////////////////////////////////////////////
void ${shipDAOName}_debug( $shipDAOName* data )
$BEGIN
EODEBUG

foreach my $field (@fields)
{
   my ($type, $name) = $field =~ /^\s+(\w+)\s+(\w+)/;
   my $DAOsignature = lcfirst $shipDAOName . "_get" . ucfirst $name . "(data)";
   print $cout qq/   printf("   %10s %u\\n", "$name", $DAOsignature );\n/ if $type ne 'char*';
   print $cout qq/   printf("   %10s %s\\n", "$name", $DAOsignature );\n/ if $type eq 'char*';
}

print $cout "$END\n\n";
print $cout "void ${shipStructName}_debug( $shipStructName* ship )\n";
print $cout "$BEGIN\n";

foreach my $field (@fields)
{
   my ($type, $name) = $field =~ /^\s+(\w+)\s+(\w+)/;
   my $signature = lcfirst $shipStructName . "_get" . ucfirst $name . "(ship)";
   print $cout qq/   printf("   %10s %d\\n", "$name", $signature );\n/ if $type ne 'char*';
   print $cout qq/   printf("   %10s %s\\n", "$name", $signature );\n/ if $type eq 'char*';
}

print $cout "$END\n\n";

close $cout;

