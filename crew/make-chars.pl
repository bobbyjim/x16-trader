use strict;

my $header = uc 'trader-x16 char';

my %race = (
	'Aslan'		=> 'A',
	'Bwap'		=> 'B',
	'Droyne'	=> 'D',
	'Hiver'		=> 'H',
	'Shrieker'	=> 'I',
	'Kkree'		=> 'K',
	'Llellewyloly'	=> 'L',
	'Amindii'	=> 'M',
	'Darrian'	=> 'R',
	'Solomani'	=> 'S',
	'Threep'	=> 'T',
	'Human'		=> 'U',
	'Vargr'		=> 'V',
	'Zhodani'	=> 'Z',
);

my %service = (
	'Merchant' 	=> 'M',
	'Army'	 	=> 'A',
	'Soldier'	=> 'A',  # = Army
	'Marines'	=> 'R',
	'Navy'		=> 'N',
	'Scouts'	=> 'S',
	'Noble'		=> 'O',
	'Rogue'		=> 'G',
		
);

# skills list
#
# astrogation
# pilot
# engineer
# medic
# gunner
# fighter

# recon
# sensop
# survey
# vacc
# hostile env

# admin 
# streetwise
# diplomat
# survival 
# trader

my @characterlist = 
(
   {
	name 	=> 'jorn',
	sdeies	=> '987678',
	race	=> $race{ 'Amindii' },
        service => $service{ 'Marines' },
        rank    => 0,
	terms	=> 4,
        medal   => 0,
	armor	=> 'V0',
	wpn	=> 'P0',
	asgn	=> 'A',
	skills	=> '1000000100000000',
   },
   {
	name 	=> 'khaalo',
	sdeies	=> '987678',
	race	=> $race{ 'Human' },
        service => $service{ 'Navy' },
        rank    => 0,
	terms	=> 4,
        medal   => 0,
	armor	=> 'V0',
	wpn	=> 'P0',
	asgn	=> 'E',
	skills	=> '0001000100000000',
   },
   {
	name 	=> 'arrlanroughl',
	sdeies	=> '987678',
	race	=> $race{ 'Vargr' },
        service => $service{ 'Army' },
        rank    => 0,
	terms	=> 4,
        medal   => 0,
	armor	=> 'V0',
	wpn	=> 'P0',
	asgn	=> 'P',
	skills	=> '0100000100000000',
   },
   {
	name 	=> 'throyss',
	sdeies	=> '987678',
	race	=> $race{ 'Droyne' },
        service => $service{ 'Merchants' },
        service => 0,
        rank    => 0,
	terms	=> 4,
        medal   => 0,
	armor	=> 'V0',
	wpn	=> 'P0',
	asgn	=> 'S',
	skills	=> '0010000100000000',
   },
);


my $record = ""; # pack "AC", 'X', 16; # A15x", 'X', 16, $header;
foreach my $cref (@characterlist)
{
   my %c = %$cref;
   $record .= pack "A15x", uc $c{'name'};			
   $record .= pack 'A', uc $c{'race'};
   $record .= pack 'A', uc $c{'terms'};		
   $record .= pack "A6", uc $c{'sdeies'};		
   $record .= pack 'AAAA', uc $c{'$armor'}, uc $c{'wpn'};	
   $record .= pack 'xxx';		
   $record .= pack 'A', uc $c{'asgn'};
   $record .= pack 'x16';			
   $record .= pack "A*", uc $c{'skills'};
}

open OUT, '>', 'CHARACTERS.BIN';
print OUT $record;
close OUT;

print "file CHARACTERS.BIN created\n";

