use strict;
use autodie;
use ComponentMap;

my $file = shift or die "SYNOPSIS: $0 filename\n";

my $headerLength = 29;    # header is 'ACS' + len + 25

open my $fp, '<:raw', $file;
read $fp, my $buffer, $headerLength;
read $fp, my $comps,  256;      # components are 4b each
close $fp;

=pod
   header 'A3C' => "ACS", buffer-length
          'A18'    class;                      
          'A2'     uc $hdr{mission};               
          'CC'     hull, tl
          'C'      demand + comfort<<4
          'C'      $hdr{mcrp};
          'A'      uc $owner;                      
=cut

my ($acs, $len, $name, $mission, $siz, $tl, $demandComfort, $mcrp, $owner)
   = unpack "A3 C A18 A2 CC C C A", $buffer;

$len -= $headerLength;

my $vol = $siz  * 100;
my $mcr = $mcrp * $siz; 

my $comfort = int($demandComfort/16);
my $demand  = $demandComfort % 16;
$comfort -= 5;
$demand -= 5;

$owner = ComponentMap::unmapOwner($owner); 

my $missionDesc = ComponentMap::mapMission($mission);

my $brief = "$owner $missionDesc";
my $n = length $brief;
my $c = length $comps;
my $l = length $name;

print<<EODAT;

     Name: $name
       TL: $tl
     Tons: $vol
  Mission: $mission ($missionDesc)
    Owner: $owner
  Comfort: $comfort
   Demand: $demand
      MCr: $mcr = $mcrp%
EODAT

my ($componentMapRef, $componentArrayRef) = ComponentMap::getComponentMap();
my @componentMap = @$componentArrayRef;
my @comps = unpack( 'C112', $comps );

while (@comps)
{
   my $id = shift @comps;
   my $qu = shift @comps;
   my $r  = shift @comps;
   my $st = shift @comps;

   #print "$id $qu $r $st\n";

   next if $st == 0;

   my $name = @componentMap[$id];
   my $grp = $id >> 5;
   $qu -= 33;

   printf "%d %3d %-16s : q=%-3d  r=%-3d  st=%-3d", $grp, $id, $name, $qu, $r, $st;
   print ' ', decodeThing($id,$r,$siz);
   print "\n";
}


sub decodeThing
{
   my $id  = shift;
   my $r   = shift;
   my $siz = shift;
 
   if ($id == 0) # cfg
   {
      my $cfg = chr($r);
      $cfg = 'Cluster'       if $cfg eq 'C';
      $cfg = 'Braced'        if $cfg eq 'B';
      $cfg = 'Planetoid'     if $cfg eq 'P';
      $cfg = 'Unstreamlined' if $cfg eq 'U';
      $cfg = 'Streamlined'   if $cfg eq 'S';
      $cfg = 'Airframe'      if $cfg eq 'A';
      $cfg = 'Lift Body'     if $cfg eq 'L';
      return $cfg;
   }

   if ($id == 1) # landers
   {
      my $landers = chr($r);
      $landers = '-'         if $landers !~ /[LSW]/;
      $landers = 'Landers'   if $landers eq 'L';
      $landers = 'Skids'     if $landers eq 'S';
      $landers = 'Wheels'    if $landers eq 'W';
      return $landers;
   }

   if ($id == 33) # volume-divided values
   {
      my $volume = $r * $siz;
      return "$volume tons cargo";
   }

   if ($id == 32) # volume-divided values
   {
      my $volume = $r * $siz;
      return "$volume tons fuel";
   }
}

