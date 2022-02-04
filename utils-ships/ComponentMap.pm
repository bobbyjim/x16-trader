package ComponentMap;
use strict;
use autodie;

sub mapMission
{
   my $missionCode = shift;

   my $map = 
   {
	A => 'Free Trader',
	A2 => 'Far Trader',
	AF => 'Variant Far Trader',
	AG => 'Gunned Trader',
	AL => 'Exploratory Trader',

	C => 'Cruiser',
	CB => 'Battle Cruiser',
	CF => 'Frontier Cruiser',
	CM => 'Mercenary Cruiser',
        CP => 'Patrol Cruiser',

	D => 'Defender',
	DL => 'Wilderness Defender',

	E => 'Escort',
	EB => 'Corvette',
	EC => 'Close Escort',
	EF => 'Fast Escort',
	EG => 'Gunned Escort',
	EM => 'Mercenary Escort',
	EN => 'Naval Escort',
	EP => 'Patrol Corvette',

	F => 'Freighter',
	F2 => 'Specialized Freighter',
	FB => 'Bulk Freighter',
	FF => 'Frontier Freighter',
	FK => 'Subsidized Freighter',

	G => 'Frigate',
	GM => 'Mercenary Frigate',
	GP => 'Patrol Frigate',

	J => 'Prospector',
	K => 'Touring',
	KJ => 'Survey Expedition',
	KS => 'Safari',
	L => 'Lab',
	LC => 'Communications Lab',

	M => 'Liner',
	MK => 'Subsidized Liner',
 	M2 => 'Long Liner',
 	ML => 'Long Liner',

	N => 'Surveyor',
	NP => 'Medical',
	NR => 'Search and Rescue',
	
	P => 'Corsair',
	PQ => 'Privateer',
	PN => 'Marauder',
	PL => 'Picket',
	PP => 'Patrol',
	PR => 'Search and Rescue',

	R => 'Merchant',
	R2 => 'Merchant Cruiser',
	RC => 'Merchant Cruiser',
	RK => 'Subsidized Merchant',
	RL => 'Exploratory Merchant',
	RQ => 'Raider',
	RB => 'Battle Raider',
	RM => 'Mercenary Raider',

	S => 'Scout/Courier',
	S2 => 'Enhanced Scout/Courier',
	SL => 'Long Courier',
	T => 'Military Transport',
        TB => 'Cargo Transport',
	TL => 'Clan Transport',
	TM => 'Tug',

	U => 'Packet',
	UF => 'Fast Packet',
	UL => 'Long Packet',
	UG => 'Gunned Packet',

	V => 'Corvette',
	W => 'Barge',
	X => 'Express',
	Y => 'Yacht',
	YF => 'Fast Yacht',
	YL => 'Long Yacht',

	Z => 'Unclassified'
   };

   return $map->{ $missionCode };
}

sub unmapOwner
{
   my $owner = shift;

   my $map = 
   {
        A =>	'Aslan',
        B => 	'Baraccai Technum',
        C => 	'Bilstein',
        D => 	'Droyne',
        E => 	'Kursae',
        H => 	'Hiver',
        I => 	'Imperial',
        K => 	'Kkree',
        L => 	'Valtra',
        M => 	'Al Morai',
        O => 	'Oberlindes',
        P => 	'Republic of Regina',
        R => 	'Darrian',
        S => 	'Sword Worlds',
        T => 	'Tukera',
        U => 	'Humbolt',
        V => 	'Vargr',
        X => 	'Chamaxi',
        Z => 	'Zhodani',
   };

   return $map->{ $owner } || '?';
}

sub mapOwner
{
   my $owner = shift;

   my $map = 
   {
        'Aslan'                 => 'A',
        'Baraccai Technum'      => 'B',
        'Bilstein'              => 'C',
        'Droyne'                => 'D',
        'Kursae'                => 'E',
        'Hiver'                 => 'H',
        'Imperial'              => 'I',
        'Kkree'                 => 'K',
        'Valtra'                => 'L',
        'Al Morai'              => 'M',
        'Oberlindes'            => 'O',
        'Republic of Regina'    => 'P',
        'Darrian'               => 'R',
        'Sword Worlds'          => 'S',
        'Tukera'                => 'T',
        'Humbolt'               => 'U',
        'Vargr'                 => 'V',
        'Chamaxi'               => 'X',
        'Zhodani'               => 'Z',
   };

   return $map->{ $owner } || '?';
}

sub getComponentMap
{
   my %backwardMap = (

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
   Computer             => 37,
   'Life Support'       => 38,
   'Ops Sensors'        => 39,		# convenience category

   Barracks             => 63,

# Sensor  010xxxxx (64-95)
   'Mining Sensors'     => 64,          # convenience category
   'Survey Sensors'     => 65,          # convenience category
   'Military Sensors'   => 66,		# convenience category

   Communicator         => 67,
   CommPlus             => 68,    
   Scope                => 69, 
   Visor                => 70,  
   HoloVisor            => 71,   
   EMS                  => 72,
   'Neutrino Detector'  => 73, 
   'Scanner'            => 74, 
   Densitometer         => 75, 
   Proximeter           => 76, 
   'Deep Radar'         => 77,
   'Mass Sensor'        => 78,     
   'Grav Sensor'        => 79,    
   'Life Detector'      => 80,   
   'Activity Sensor'    => 81,  
   'Field Sensor'       => 82, 

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
   Missile              => 161,		# basic weapon category
   'Beam Laser'         => 162,		# basic weapon category
   'Pulse Laser'        => 163,		# basic weapon category
   'Mining Laser'       => 164,
   'Particle Accelerator' => 165,	# basic weapon category
   'Meson Gun'          => 166,
   'Plasma Gun'         => 167,
   'Fusion Gun'         => 168, 
   'Jump Damper'        => 169,
   'KK Missile'         => 170,  
   CommCaster           => 171,
   DataCaster           => 172,
   Ortillery            => 173,  
   'Salvo Rack'         => 174,  
   'Tractor/Pressor'    => 175,

# Defense 110xxxxx (192-223)
   Sandcaster           => 192,		# basic defense category
   Jammer               => 193,
   'Stealth Mask'       => 194,         # map to Jammer
   'Grav Scrambler'     => 195,
   'Mag Scrambler'      => 196,
   'Black Globe'        => 197,
   'Nuclear Damper'     => 198,
   'Meson Screen'       => 199,         # map to Damper

# Exotic  111xxxxx (224-255)
   Disintegrator        => 224,
   'Jump Inducer'       => 225,
   'Stasis Gun'         => 226,
   'AM Missile'         => 227,
   'Proton Screen'      => 228,
   'White Globe'        => 229,
   'Stasis Globe'       => 230,
   );

   my @forwardMap = ();
   foreach my $key (keys %backwardMap)
   { 
      my $value = $backwardMap{ $key };
      $forwardMap[ $value ] = $key;
   }
   return (\%backwardMap, \@forwardMap);
}

1;

