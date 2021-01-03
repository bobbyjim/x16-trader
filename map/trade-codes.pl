#!/usr/bin/env perl

my $epochs = shift || 1000;

my @sp = qw/- - A A A B B C C D E E X/;
my @hex = (0..9, 'A'..'H', 'J');

my %tradeCodes = ();

for (1..$epochs)
{
   print "$_\n" unless $_ % 100000;

   my $siz = int(rand(6))+int(rand(6));
   $siz = 9+int(rand(6)+1) if $siz == 10;

   my $atm = range( flux($siz), 0, 15 );
   $atm = 0 if $siz = 0;

   my $dm = 0;
   $dm = -4 if $atm < 2 || $atm > 9;
   my $hyd = range( flux($atm+$dm), 0, 10 );

   my $pop = int(rand(6))+int(rand(6));
   $pop = roll2dm2(5) if $pop == 10;
   
   my $gov = range(flux($pop), 0, 15 );
   my $law = range(flux($gov), 0, 18 );

   my $uwp = $hex[$siz] . $hex[$atm] . $hex[$hyd] . $hex[$pop] . $hex[$gov] . $hex[$law];
   my $tradeClasses = getTradeClasses($uwp);

   $tradeCodes{ $tradeClasses }++;
}

my $i = 0;
for my $tc (sort keys %tradeCodes)
{
   $i++;
   printf "%3d. %-15s : %-4s  %d\n", $i, $tc, $tradeCodes{$tc}*100/$epochs, $tradeCodes{$tc};
}

for my $tc (sort keys %tradeCodes)
{
   print "   '$tc',\n";
}

my $line = 950;
for $tc (sort keys %tradeCodes)
{
   print $line, ' DATA ', $tc, "\n";
   ++$line;
}

sub range
{
   my $val = shift;

   my $min = shift;
   my $max = shift;
   $val = $min if $val < $min;
   $val = $max if $val > $max;
   return $val;
}

sub roll2dm2 # 0-10
{
   return int(rand(6))+int(rand(6));
}

sub roll1d
{
   my $dm = shift || 0;
   return int(rand(6)+1) + $dm;
}

sub flux # -5 to +5
{
   my $dm = shift || 0;
   return int(rand(6))-int(rand(6)) + $dm;
}

sub getTradeClasses
{
   my $uwp = shift;

   my @tc = ();

   push @tc, 'Ag' if $uwp =~ /^.[4-9][4-8][567]../;
   push @tc, 'As' if $uwp =~ /^000.../;
   push @tc, 'Ba' if $uwp =~ /^...000/;
   push @tc, 'De' if $uwp =~ /^.[2-9]0.../;
   push @tc, 'Fl' if $uwp =~ /^.[ABC][1-A].../;
   push @tc, 'Hi' if $uwp =~ /^...[9A-F]../;
   push @tc, 'Na' if $uwp =~ /^.[0123][0123][6-9A-F]/;
   push @tc, 'In' if $uwp =~ /^.[012479ABC].[9A-F]../;
   push @tc, 'Lo' if $uwp =~ /^...[123]../;
   push @tc, 'Ni' if $uwp =~ /^...[456]../;
   push @tc, 'Po' if $uwp =~ /^.[2345][0123].../;
   push @tc, 'Ri' if $uwp =~ /^.[68].[678]../;
   push @tc, 'Va' if $uwp =~ /^.0..../;

   return join ' ', sort @tc;
}

