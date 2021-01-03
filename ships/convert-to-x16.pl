use strict;
use YAML;
use Data::Dumper;
$Data::Dumper::Indent = 1;
$Data::Dumper::Terse = 1;

=pod
********************************************************************************

   The goal is to encode a ship by its characteristics and component list.

   Components are identified by a unique 8-bit TYPE(:3)+DEVICE(:5) identifier.

   Most of the components are distinctly recognizable by the 'category' field,
   with the exception of 'fittings'.

********************************************************************************
=cut

my $pattern = shift @ARGV;

$/ = undef;
my %catalog;
my @list;
my $ships = 0;
# my %deviceMap;
my $componentIDs = getComponentMap();

foreach my $file (<$pattern*.yml>)
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
	av		=> $tl,
	owner		=> $owner,
	tl		=> $tl,
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
         $x16header{ 'av' } = $1;
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

      if ( $mount =~ /T1|T2|T3|T4|M|B|Surf|LBay|Bay|Ant|Ext|B1|B2/ )
      {
         $totalTons = 0;        # size means nothing
         $count     = 0;        # count means nothing
         $rating    = $mod + 5; # rating means everything
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
   $x16header{ 'fuel'      } = $x16data{ 32 }->{'r'}; 
   $x16header{ 'cargo'     } = $x16data{ 33 }->{'r'};
   $x16header{ 'low'       } = $x16data{ 34 }->{'r'};
   $x16header{ 'stateroom' } = $x16data{ 35 }->{'r'};
   $x16header{ 'hop'       } = $x16data{ 99 }->{'r'} if $x16data{ 70 };
        
   my $layers = int( $x16header{'av'} / $tl );

   $x16data{ 0  }->{ 'r' } = ord $x16header{ 'cfg' };     # hull cfg
   $x16data{ 1  }->{ 'r' } = ord $x16header{ 'landers' }; # landers
   $x16data{ 2  }->{ 'r' } = $layers;      # armor
   $x16data{ 36 }->{ 'r' } = $bridge;      # bridge rating
   $ships++;

   printShipRecord(\%x16header, \%x16data);
}

#
#  One file to rule them all
#
open my $out, '>', 'ALL-SHIPS';
print $out pack 'xx';
close $out;

`cat *.SHIP >> ALL-SHIPS`;

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

   my $owner  = mapOwner( $hdr{owner} );
   my $qsp    = $hdr{qsp};
   my $class  = $hdr{classname};
   my $brief  = $hdr{brief};
   
   my $siz    = $hdr{siz};
#   my $hull   = uc $hdr{hull};
   my $cfg    = uc $hdr{cfg};
   my $landers = $hdr{landers};
   my $cargo  = $hdr{cargo};
   my $fuel   = $hdr{fuel};
   my $mcr    = $hdr{mcr};

   my $cargop = int($cargo/$siz);
   my $fuelp  = int($fuel/$siz);
   my $mcrp   = int($mcr/$siz);  # GENIUS!!

   my $tl     = $hdr{tl};

   #
   #  Figure out the number of layers of armor
   #
   my $layers = int( $hdr{av} / $tl );
   #$layers = chr( $layers + 65 ); # 0 = "A"

#   print "Cargo($cargop) Fuel($fuelp) MCr($mcrp)\n";

   my $buffer = '';

   # comfort and demand each range from 0 to 11.
   # so, let's shove comfort into the upper nybble.
   my $comfortDemand = $hdr{demand} + $hdr{comfort} * 16;

   $buffer .= pack 'A18', 	uc $class;		
   $buffer .= pack 'A2',   	uc $hdr{mission};
   $buffer .= pack 'CC',    	$siz, $tl;
   $buffer .= pack 'C', 	$comfortDemand;
   $buffer .= pack 'C',         $mcrp;
   $buffer .= pack 'A',    	uc $owner;		

   foreach my $id (sort { $a <=> $b } keys %dat)
   {
      my $obj  = $dat{$id};
      my $type = $id >> 5;
      my $q    = $obj->{ 'q' };
      my $r    = $obj->{ 'r' };

      $r /= $siz if $id == 32  # fuel
                 or $id == 33; # cargo

      my $desc = $obj->{'_thing'};
      $buffer .= pack 'CCCC', $id, $q, $r, 1;
   }

   $buffer = pack('A3C', 'ACS', length($buffer)) . $buffer;

   my $outshipname = uc "$owner-$qsp-$class.SHIP";
   open OUT, '>', $outshipname;
   print OUT $buffer;
   close OUT;

   print STDERR sprintf("Ship: %-36s\n", $outshipname); # [MCr %3d -> %3d -> %3d]\n", $outshipname, $hdr{mcr}, $mcrp, $siz * $mcrp);
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
