use strict;
use YAML;
use Data::Dumper;
$Data::Dumper::Indent = 1;
$Data::Dumper::Terse = 1;

=pod
********************************************************************************

   The goal is to encode a ship by its characteristics mainly.
   Mounted components (e.g. sensors, defenses) are NOT included.

********************************************************************************
=cut

my $pattern = shift @ARGV;

$/ = undef;
my %catalog;
my @list;
my $ships = 0;
# my %deviceMap;
my $componentIDs = getComponentMap();

my @shipList = ();
open my $CSV, '>', 'x16-trader-allships.csv';

   print $CSV join ', ',
        'class',
        'mission',
        'siz',
        'cfg',
        'm',
        'j',
        'hop',
        'collector',
        'antimatter',
        'landers',
        'cargo',
        'fuel',
        'bridge',
        'computer',
        'layers',
        'lifeSupport',
        'purifiers',
        'scoops',
        'comfort',
        'demand',
        'mcr',
        'tl',
        'passengers',
        'low',
        'owner';

   print $CSV "\n";


foreach my $file (sort <$pattern*.yml>)
{
   my $yaml = YAML::LoadFile( $file );

   my %x16header  = ();
   my %x16data    = ();

   my $hdr      = $yaml->{ header };
   my $owner    = $hdr->{ owner };
   my $name     = $hdr->{ shipname };
   my $qsp      = $hdr->{ qsp };
   my $tl       = $hdr->{ tl };
   my $mcr      = $hdr->{ totalMCr };
   my $crew     = $hdr->{ crew };
   my $pass     = $hdr->{ passengers };
   my $brief    = $hdr->{ brief };
   my $bridge   = 0;
      $bridge   = 3 if $hdr->{ spaciousness } > 1.5;
      $bridge   = 1 if $hdr->{ spaciousness } < 1;
   my $demand   = $hdr->{ 'demand' } + 5;     
   my $comfort  = $hdr->{ 'crewComfort' } + 5;
   my $tons     = $hdr->{ 'tons' };
   my $cargo    = $hdr->{ 'totalCargoSpace' };

   my ($ignoremission, $hull, $config, $maneuver, $jump) = $qsp =~ /(\w\w?)-(.)\d?(.)(.)(.)/;

   my $mission = $hdr->{ 'mission' };
   
   #
   # siz = $tons / 100;
   #
   my $siz = int($tons/100);

   #if ($tons > 2400)
   #{
   #   print STDERR "IGNORING $name.  > 2400 tons.\n";
   #   next;
   #}

   %x16header = ( 
   brief           => uc $brief,
	qsp		=> $qsp,
	classname 	=> uc substr($name, 0, 23),
	mission   	=> uc $mission,
	hull		=> uc $hull,
	siz  	 	=> $siz,
	cfg		=> uc $config,
	mnv		=> $maneuver,
	jmp		=> $jump,
	hop		=> 0,
	landers		=> 'S',
	mcr		=> $mcr,
	comfort		=> $comfort,
	demand		=> $demand,
	owner		=> $owner,
	tl		=> $tl,
	cargo		=> $cargo,
	passengers	=> $pass,
   );

   my @comp = @{$yaml->{ components }};
   foreach my $component (@comp)
   {
      my $label     = $component->{ 'label' };
      my $rating    = $component->{ 'rating' };
      my $mod       = $component->{ 'mod'    };
      my $totalTons = $component->{ 'totalTons' };
      my $count     = $component->{ 'howMany'   };

      my $quality = 32 
                  + $component->{ 'q' }
                  + $component->{ 'r' }
                  + $component->{ 'e' }
                  + $component->{ 'b' }
                  + $component->{ 's' }
                  + $component->{ 'eff' };

      ##########################################################
      #
      #   We convert and organize based on type and category.
      #
      ##########################################################
      my $type  = $component->{ 'type' };
      my $thing = $component->{ 'category' }; 
      my $mount = '';

      if ( $thing =~ /AV=(\d+)/ )
      {
         $x16header{ 'av' } += $1;
         #print "$name: found AV=$1\n";
         next;
      }

      $thing =~ s/'//g;

      $thing = $1 if $thing =~ /(Model.\d)/;
      $thing = "Model/$1" if $thing =~ /Computer M(\d)/;

      if ( $thing =~ /Model.(\d)/ )
      {
         $rating    = $1;
         $count     = 0;
         $totalTons = 0;
         $thing  = "Computer";
         $x16header{ 'computer' } = $rating;
      }

      $thing =~ s/(Vd|Or|Fo|DS|SR|LR|G|L|D) //;
      ($mount, $thing) = ($1, $3) if $thing =~ /(T1|T2|T3|T4|M|B|Surf|Bay|LBay|Ant|Ext|B1|B2)(de)? (.*)/;

      my $emplacement = 0;
      if ( $mount =~ /T1|T2|T3|T4|M|B|Surf|LBay|Bay|Ant|Ext|B1|B2/ )
      {
         $totalTons   = 0;        # size means nothing
         $count       = 0;        # count means nothing
         $rating      = $mod + 5; # rating means everything
         $emplacement = 0 if $mount eq 'Surf' || $mount eq 'B';
         $emplacement = 1 if $mount eq 'Ant';
         $emplacement = 2 if $mount eq 'Ext';
         $emplacement = 1 if $mount eq 'T1';
         $emplacement = 2 if $mount eq 'T2';
         $emplacement = 3 if $mount eq 'T3';
         $emplacement = 4 if $mount eq 'T4';
         $emplacement = 5 if $mount eq 'B1';
         $emplacement = 6 if $mount eq 'B2';
         $emplacement = 7 if $mount eq 'Bay';
         $emplacement = 8 if $mount eq 'LBay';
         $emplacement = 9 if $mount =~ /^M/; # main
      }

      next if $thing =~ /jump field/;
      next if $thing =~ /Space/;
      next if $thing eq 'Empty';
      next if $thing eq 'fittings'; # we're gonna ignore submersible hulls
      next if $thing eq 'wings'   ; # we're gonna ignore wings and fins

      $thing = 'Maneuver Drive' if $thing =~ /Gravitic Drive/;
      $thing = 'Jump Drive'     if $thing =~ /NAFAL/;
      $thing = 'Particle Accelerator' if $thing =~ /Particle Accelerator/;
      $thing = 'Life Support' if $thing =~ /Life Support/;

      if ($thing =~ /Fuel/)
      {
         $thing = 'Fuel Scoops' if $thing =~ /Scoops|Intakes|Bins|Transfer/;
         $thing = 'Fuel' if $thing =~ /Jump|Powerplant|Hop/;
      }
      elsif ($thing eq 'landers')
      {
         $thing = 'S' if $label =~ /Skid/;
         $thing = 'L' if $label =~ /Legs/i;
         $thing = 'W' if $label =~ /Wheel/i;
         $thing = '-' if $label =~ /No /i;
         $x16header{ 'landers' } = $thing;
         next;
      }
      elsif ( $thing eq '' || $thing eq 'Hull' ) # huh, what is this thing?
      {
         $thing = "IGNORED THING[$label]";
         $thing = 'Barracks'     if $label =~ /Barracks/;
         $thing = 'Stateroom'    if $label =~ /Stateroom|Suite/;
         $thing = 'Life Support' if $label =~ /Life Support/;
         $thing = 'Low Berth'    if $label eq 'Low Berth';
         $thing = 'Stasis Berth' if $label eq 'Stasis Berth';
         $thing = 'Vault'        if $label =~ /Vault/;
         $thing = 'Capture Tank' if $label =~ /(Capture|Animal) Tank/;
      }

      $type = 'Ops' if $thing eq 'Life Support';
      $type = 'Payload' if $type eq 'Passenger';
      $type = 'Engineering' if $type eq 'Drives';
      $type = 'Ops' if $thing eq 'Barracks';

      next if $thing =~ /IGNORED THING/;
      next if $type eq 'Crew';

      $totalTons = 0 if $thing =~ /Stateroom|Berth/;  # force to not use vol as rating

      my $object = {
		q    	 => $quality,
		r        => $rating || $totalTons || $count,
                emplacement => $emplacement,
                _thing   => $thing,
      };

      if ( $label =~ /Standard controls/i )
      {
         $bridge = 2;
      }

#       $deviceMap{ $type }->{ $thing }++;  # for statistical purposes

      #
      #  Don't record it unless it's got a component ID entry, please.
      #
      next unless defined $componentIDs->{ $thing };

      my $devID = $componentIDs->{ $thing };

      if ( $x16data{ $devID } ) # one's already there!
      {
         $x16data{ $devID }->{ 'r' } += $object->{ 'r' };
      }
      else
      {
         $x16data{ $devID } = $object;
      }
   }

   #
   #  Now that we have the component data, we can roll up some data
   #  into the header.  It's more accurate in some instances than the
   #  actual header data.
   #
   $x16header{ 'av' } |= $tl;
   $x16header{ 'fuel'      } = $x16data{ 32 }->{'r'}; 
#   $x16header{ 'cargo'     } = $x16data{ 33 }->{'r'};
   $x16header{ 'low'       } = $x16data{ 34 }->{'r'};
   $x16header{ 'stateroom' } = $x16data{ 35 }->{'r'};
   $x16header{ 'bridge'    } = $x16data{ 36 }->{'r'};
   $x16header{ 'computer'  } = $x16data{ 37 }->{'r'};
   $x16header{ 'life support' } = $x16data{ 38 }->{'r'};
   $x16header{ 'fuel purifier' } = $x16data{ 96 }->{'r'} if $x16data{ 96 };
   $x16header{ 'fuel scoops' } = $x16data{ 97 }->{'r'} if $x16data{ 97 };
   $x16header{ 'collector' } = $x16data{ 98 }->{'r'} if $x16data{ 98 };
   $x16header{ 'hop'       } = $x16data{ 99 }->{'r'} if $x16data{ 99 };
   $x16header{ 'antimatter' } = $x16data{ 100 }->{'r'} if $x16data{ 100 };

   my $layers = int( $x16header{'av'} / $tl );

   $x16data{ 0  }->{ 'r' } = ord $x16header{ 'cfg' };     # hull cfg
   $x16data{ 1  }->{ 'r' } = ord $x16header{ 'landers' }; # landers
   $x16data{ 2  }->{ 'r' } = $layers;      # armor
   $x16data{ 36 }->{ 'r' } = $bridge;      # bridge rating
   $ships++;

   push @shipList, printShipRecord( \%x16header, \%x16data);
}

close $CSV;

#
#  One file to rule them all
#
my $outfile = '64B-ALL-SHIPS.BIN';

open my $out, '>', $outfile;
print $out pack 'xx';
close $out;

`cat 64B*.SHIP >> $outfile`;

open my $out, '>>', $outfile;
print $out pack 'x16'; # end of data
print $out uc<<EOHEADER;
acs ship file  
a18 class      
x   null       
a2  mission    
x   null       
cac3siz,cfg,mjh
cc  collect,am 
a   landers    
cc  cgop,fuelp 
ccc bri,cpu,ar 
ccc ls,ref,scp 
c   comf/deman 
c   mcrp       
c   tl         
x4  4 nulls    
a18 owner      
x   null       
EOHEADER

close $out;

sub printShipRecord # ( %x16header, %x16data )
{
   my $hdrref = shift;
   my $datref = shift;
   my %hdr = %$hdrref;
   my %dat = %$datref;

=pod
   printf "%-23s %2s : %3d %s %d %d %d : %s %2d %2d %3d %2d %2d\n",
	$hdr{classname}, 
	$hdr{mission} 	,
	$hdr{siz} ,
	$hdr{cfg} ,
	$hdr{mnv} ,
	$hdr{jmp} ,
	$hdr{hop} ,
	$hdr{landers} ,
	$hdr{comfort} ,
	$hdr{demand} ,
	$hdr{av} ,
	$hdr{cargo} ,
	$hdr{fuel};
=cut

   # -----------------------------------------------------
   #
   #   Sort out the primary and secondary characteristics
   #
   # -----------------------------------------------------
   my $owner  = $hdr{owner}; # mapOwner( $hdr{owner} );
   #$owner = 'I' if $owner eq '?';

   my $qsp    = $hdr{qsp};
   my $class  = $hdr{classname};
   my $brief  = $hdr{brief};
   
   my $siz    = $hdr{siz};
   my $cfg    = uc $hdr{cfg}; # a number
   my $m      = uc $hdr{mnv}; # a number
   my $j      = uc $hdr{jmp}; # a number
   my $h      = uc $hdr{hop}; # a number

   my $collector   = uc $hdr{collector};
   my $antimatter  = uc $hdr{antimatter};

   my $landers = uc $hdr{landers};
   my $cargo   = uc $hdr{cargo};
   my $fuel    = uc $hdr{fuel};
   my $tl      = uc $hdr{tl};
   my $mcr     = uc $hdr{mcr};

   my $bridge   = uc $hdr{bridge} || 0;
   my $computer = uc $hdr{computer} || 0;

   my $lifeSupport = uc $hdr{'life support'} || 0;
   my $purifiers   = uc $hdr{'fuel purifier'} || 0;
   my $scoops      = uc $hdr{'fuel scoops'} || 0;

   # Storing these as a radio of the hull size
   # means we strike a balance between resolution
   # and memory efficiency.

   my $cargop = int($cargo/$siz);
   my $fuelp  = int($fuel/$siz);
   my $mcrp   = int($mcr/$siz); 

   #
   #  Figure out the number of layers of armor
   #
   my $layers = int( $hdr{av} / $tl );
   $layers = 255 if $layers > 255; # just in case

   # comfort and demand each range from 0 to 11.
   # so, let's shove comfort into the upper nybble.
   my $comfortDemand = $hdr{demand} + $hdr{comfort} * 16;


   my $passengers = $hdr{passengers};
   my $low	  = $hdr{low};

   printf "%18s pax: $passengers  low: $low\n", $class, $passengers, $low;

   # -----------------------------------------------------
   #
   #   Build the output byte buffer.
   #
   # -----------------------------------------------------

   my $buffer = '';

   # QSP (27 bytes)
   $buffer .= pack 'A18', 	uc $class;
   $buffer .= pack 'x';
   $buffer .= pack 'A2',   	uc $hdr{mission}; 
   $buffer .= pack 'x';
   $buffer .= pack 'CACCC',     $siz, $cfg, $m, $j, $h;

   # Secondary features of all ships (12 bytes)
   $buffer .= pack 'CC',        $collector, $antimatter;
   $buffer .= pack 'A',         substr($landers,0,1);
   $buffer .= pack 'CC',        $cargop, $fuelp;
   $buffer .= pack 'CCC',       $bridge, $computer, $layers;
   $buffer .= pack 'CCC',       $lifeSupport, $purifiers, $scoops;
   $buffer .= pack 'C', 	$comfortDemand;    
   $buffer .= pack 'C',         $mcrp;             
   $buffer .= pack 'C',         $tl;
   $buffer .= pack 'CC',        $passengers, $low;

   $buffer .= pack 'x2'; 
   $buffer .= pack 'A18', sprintf("%-18s",substr(uc $owner, 0, 18));
   $buffer .= pack 'x';

   print $CSV join ', ', 	
	$class, 
	$hdr{mission}, 
	$siz, 
	$cfg, 	
	$m, 	
	$j, 
	$h, 
	$collector, 
	$antimatter, 
	$landers,
	$cargop * $siz,
	$fuelp  * $siz,
	$bridge,
	$computer,
	$layers,
	$lifeSupport,
	$purifiers,
	$scoops,
	$comfortDemand && 15,
	$comfortDemand << 4,
	$mcrp * $siz,
	$tl,
	$passengers,
	$low,
	$owner;	

   print $CSV "\n";

=pod

   *************  IGNORE SENSORS  ************

   # -----------------------------------------------------
   #
   #   Group sensors into four bytes.
   #   Store the emplacement.  Let TL dictate the type.
   #
   # -----------------------------------------------------
   my $opsSensors    = 0;
   my $miningSensors = 0;
   my $surveySensors = 0;
   my $milSensors    = 0;

   foreach my $id (sort { $a <=> $b } keys %dat)
   {
      my $obj  = $dat{$id};
      my $emplacement = $obj->{ 'emplacement' };

      $opsSensors    = $emplacement if $id == 39 && $emplacement > $opsSensors;
      $miningSensors = $emplacement if $id == 64 && $emplacement > $miningSensors;
      $surveySensors = $emplacement if $id == 65 && $emplacement > $surveySensors;
      $milSensors    = $emplacement if $id == 66 && $emplacement > $milSensors;
   }

   $buffer .= pack 'CCCC', $opsSensors, $miningSensors, $surveySensors, $milSensors;
=cut

   # -----------------------------------------------------
   #   Ignore weapons and defenses for now.
   #   
   #   Add some null padding.
   # -----------------------------------------------------
#   $buffer .= pack 'xxxxx';

   my $mission = $hdr{'mission'};
   my $cost    = sprintf( "%04d", int($mcr) );

   my $outshipname = uc "64B-$mission-$cost-$qsp-$class.SHIP";
   open OUT, '>', $outshipname;
   print OUT $buffer;
   close OUT;
}


sub mapOwner
{
   my $owner = shift;
   my $map =
   {
	'Aslan'			=> 'A',
	'Baraccai Technum'	=> 'B',
	'Bilstein'		=> 'C',
	'Droyne'		=> 'D',
	'Kursae'		=> 'E',
	'Hiver'			=> 'H',
	'Imperial'		=> 'I',
	'Kkree'			=> 'K',
	'Valtra'		=> 'L',
	'Al Morai'		=> 'M',
	'Oberlindes'		=> 'O',
	'Republic of Regina'	=> 'P',
	'Darrian'		=> 'R',
	'Sword Worlds'		=> 'S',
	'Tukera'		=> 'T',
	'Humbolt'		=> 'U',
	'Vargr'			=> 'V',
	'Chamaxi'		=> 'X',
	'Zhodani'		=> 'Z',
   };
   my $abbr = $map->{ $owner };
   $abbr = '?' unless $abbr;
   return $abbr;
}



sub getComponentMap
{
   return {
# Config  000xxxxx (00-31)
   Config               => 0,
   Landers              => 1,
   Armor		=> 2,
   ArmorLayers          => 2,
   'Maneuver Drive'     => 3,
   'Jump Drive'         => 4,
   PowerPlant           => 5,

# Ops     001xxxxx (32-63)
   Fuel                 => 32,
   Cargo		=> 33,
   'Low Berth'          => 34,
   Stateroom            => 35,
   Bridge               => 36,
   Computer 		=> 37,
   'Life Support'       => 38,
   'Ops Sensors'        => 39, 		# ops sensors...

   Barracks             => 63,

# Sensor  010xxxxx (64-95)
   Communicator         => 39,          # map to ops!
   HoloVisor            => 39,		# map to ops!
   Visor                => 39,	        # map to ops!
   Scope                => 39,		# map to ops!
   EMS                  => 39,		# map to ops!
   'Scanner'		=> 39,		# map to ops!

   'Mining Sensors'     => 64, 		# mining sensors...
   Densitometer         => 64,		# 
   Proximeter           => 64,		# 
   'Deep Radar'         => 64,		# 
   'Mass Sensor'        => 64,		# 

   'Survey Sensors'	=> 65,		# survey sensors...
   'Grav Sensor'        => 65,		# 
   'Life Detector'      => 65,		# 
   'Field Sensor'       => 65,		#

   'Military Sensors'   => 66,		# military sensors...
   CommPlus             => 66,		# 
   'Neutrino Detector'  => 66,		# 
   'Activity Sensor'    => 66,		# 

# Drive   011xxxxx (96-127)
   'Fuel Purifiers'     => 96,
   'Fuel Scoops'        => 97,
   Collector            => 98,
   'Hop Drive'          => 99,
   'Antimatter Plant'   => 100,

# Payload 100xxxxx (128-159)
   'Capture Tank'       => 128,
   Vault                => 129,
   Magazine		=> 130,
   'Stasis Berth'       => 131,

# Weapon  101xxxxx (160-191)
   'Hybrid L-S-M'       => 160,
   Missile              => 161,
   'Beam Laser'         => 162,
   'Pulse Laser'        => 163,
   'Mining Laser'       => 164,
   'Particle Accelerator' => 165,
   'Meson Gun'          => 165,		# map to PA
   'Plasma Gun'         => 162,		# map to "Beam Laser"
   'Fusion Gun'         => 163,		# map to "Pulse Laser"
   'Jump Damper'        => 169,
   'KK Missile'         => 161,		# map to "Missile"
#   CommCaster           => 
#   DataCaster           => 
   Ortillery            => 161,		# map to "Missile"
   'Salvo Rack'         => 161,		# map to "Missile"
   'Tractor/Pressor'    => 175,	

# Defense 110xxxxx (192-223)
   Sandcaster           => 192,
   Jammer               => 193,
   'Stealth Mask'       => 193,		# map to Jammer
   'Grav Scrambler'     => 198, 	# map to Damper
   'Mag Scrambler'      => 198, 	# map to Damper
   'Black Globe'        => 197,
   'Nuclear Damper'     => 198,
   'Meson Screen'       => 198,		# map to Damper

# Exotic  111xxxxx (224-255)
   Disintegrator        => 224,
   'Jump Inducer'       => 225,
   'Stasis Gun'         => 226,
   'AM Missile'         => 227,
   'Proton Screen'      => 228,
   'White Globe'	=> 229,
   'Stasis Globe'       => 230,
   }
}

sub byPrice
{
   my ($ahdrref) = $a->[0];
   my ($bhdrref) = $b->[0];

   return $ahdrref->{'mcr'} <=> $bhdrref->{'mcr'};
}

