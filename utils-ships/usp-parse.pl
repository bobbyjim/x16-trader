#
#  The High Guard USP is almost completely opaque to me.
#  So: a script that can read one.
#
use strict;

foreach my $usp (<DATA>)
{
    my ($name, $mission, $specs, $defense, $offense, $fighters) = split /[-\s]+/, $usp;
    my ($tonnage, $cfg, $jmp, $man, $pwr, $cpu, $crew)   = split '', $specs;
    my ($ar, $sand, $screen, $damp, $globe, $repulsor)   = split '', $defense;
    my ($laser, $plasma, $pa, $meson, $missile)          = split '', $offense;

    print<<EODATABLOCK;

        mission: $mission       ($name)
   tonnage code: $tonnage    
            cfg: $cfg           ar: $ar      laser: $laser
            jmp: $jmp         sand: $sand     plasma: $plasma
            man: $man       screen: $screen         pa: $pa
            pwr: $pwr         damp: $damp      meson: $meson
       computer: $cpu        globe: $globe    missile: $missile
           crew: $crew     repulsor: $repulsor    fighter: $fighters (squadrons)

EODATABLOCK
}


__DATA__
Tigress   BB-V5368J4-F97909-967T9-30
Nolikian  BR-L106QJ3-B39905-299N9-0
Plankwell BB-S4458J4-A73909-967T9-0
Kokirrak  BB-S436AJ4-C78909-697T9-0
Atlantic  CR-Q4459J3-A66900-909N9-0
Lightning CF-P4525F3-596500-99N09-8
Ghalalk   CA-P4459J3-585900-99H09-0
Arakoine  CS-P7349J3-049909-909N9-10
Gionetti  CL-M2559J3-060909-960J7-0
