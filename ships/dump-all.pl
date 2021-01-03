
my $filter = shift;

my @ships = <*.SHIP>;
@ships = grep( /$filter/, @ships ) if $filter;

foreach my $ship (@ships)
{
   next if $ship =~ /COMPONENT/;
#   print "file: $ship\n";
   print `perl dump-ship.pl '$ship'`;
}
