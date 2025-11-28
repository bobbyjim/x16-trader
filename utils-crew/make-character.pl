use strict;

my $name   = uc shift || SYNOPSIS();
my $race   = uc shift || SYNOPSIS();
my $career = uc shift || SYNOPSIS();
#my $terms  = uc shift || SYNOPSIS();
#my $rank   = uc shift || SYNOPSIS();

die "Race must be A-Z\n" unless $race =~ /^[A-Z]$/;
die "Career must be A-Z\n" unless $career =~ /^[A-Z]$/;
#die "Terms must be in the range [0-6]\n"   unless $terms =~ /^[0-6]$/;
#die "Rank must be of the form [EO][0-5]\n" unless $rank =~ /^[EO][0-5]$/;

my @c;
$c[$_] = sprintf("%X", int(rand()*6)+int(rand()*6)+2) for 0..5;

#my $medal = int(rand()*6)-int(rand()*6);
#   $medal = 0 if $medal < 0;

my $fileBasename = substr($name, 0, 16);
$name = sprintf "%-16s", substr( $fileBasename . '... ... ... ... ', 0, 16 );

#my @career = qw/MERCHANT ARMY MARINE NAVY SCOUT ROGUE/;
#my $career = $career[ rand @career ];
#$career = 'M' if $career =~ /^MER/;
#$career = 'A' if $career =~ /^ARM|SOL/;
#$career = 'R' if $career =~ /^MAR/;
#$career = 'N' if $career =~ /^NAV|SPA/;
#$career = 'S' if $career =~ /^SCO/;
#$career = 'O' if $career =~ /^ROG|OTH/;

#if ($career !~ /^[MARNSO]$/)
#{
#   print "Unknown career.  Default is Merchants.\n";
#   $career = 'M';
#}

#my @weapon = makeThing();
#my @armor  = makeThing();

#print "weapon: @weapon\n";
#print "weapon: @armor\n";

open my $out, '>', "$fileBasename.CHAR";
print $out '--';    # 2c
print $out $name;   # 16c
print $out '.';     # 1c status e.g. "D"ead
print $out $race;   # 1c e.g. "H"uman
print $out $career; # 1c e.g. "M"erchant
print $out @c;      # 6c
print $out '- - - '; # padding to 16c

#print $out '.';     # 1c
#print $out $career; # 1c
#print $out $terms;  # 1c
#print $out '.';     # 1c
#print $out $rank;   # 1c
#print $out $medal;  # 1c
#print $out '.';     # 1c
#print $out @weapon; # 4c
#print $out @armor;  # 4c
#print $out '- - - - - '; # padding to 64 bytes
close $out;

print "$fileBasename.CHAR created.\n";

sub SYNOPSIS
{
print<<EOSYNOPSIS;

   SYNOPSIS: $0 <name> <race> <career>

   Race is a single letter (A-Z), representing the following:

	A: Aslan
	B: Bwap
	C: Cassildan Human
	D: Droyne
	E: Ebokin
	F: Florian Human
	G: Geonee Human
	H: Human
	I: Hiver
	J: Jonkeereen Human
	K: clone (Human)
	L: Llellewyloly
	M: Amindii
	N: Vilani Human
	O: Solomani Human
	P: Synthetic Human
	Q: Cetian
	R: Darrian Human
	S: Suerrat Human
	T: Threep
	U: Virushi
	V: Vargr
	W: Kursae
	X: Xaruah
	Y: Yilean
	Z: Zhodani

   Career is a single letter (A-Z), representing the following:

	A: Army
	G: Agent
	L: Scholar
	M: Merchant
	N: Navy
	O: Rogue/Other
	R: Marine
	S: Scout

EOSYNOPSIS
#   print "\nSYNOPSIS: $0 <name> <race> <career> <terms> <rank>\n";
#   print "\n   Race and Career can be abbreviated to their first 3 letters.\n\n";
  
   die;
}

#
#  Any 'Thing' is made up of a set of fields.
#
#  Category (e.g. Weapon) is decided externally from this record.
#  Class (Type and subtype (descriptor)) is determined here.  So if you
#  have a "Weapon" category, then this record can specify classes
#  of weapons.
#
#  Differentiation from within a class is given by Stage and Burden.
#
#  That should allow plenty of differentiation.
#
sub makeThing
{
   my $type        = chr(rand(1)*45 + ord('0'));
   my $descriptor  = chr(rand(1)*45 + ord('0'));
   my $burden      = chr(rand(1)*45 + ord('0'));
   my $stage       = chr(rand(1)*45 + ord('0'));

   return ($type, $descriptor, $burden, $stage);
}

