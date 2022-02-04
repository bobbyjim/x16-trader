#!/usr/bin/env perl
#use UwpTables;

my $epochs = shift || 1000;
my $search_mode = shift;

die "please use search_mode instead\n  SYNOPSIS: $0 1 SEARCH" if $epochs > 100000000;

my @sp = qw/- - A A A B B C C D E E X/;
my @hex = (0..9, 'A'..'H', 'J');

my %tradeCodes = ();

my ($siz, $atmo, $dm, $hyd, $pop, $gov, $law, $uwp, $tradeClasses);

my $count = 0;

for (1..$epochs)
{
   print "$_\n" unless $_ % 100000;

   $siz = int(rand(6))+int(rand(6));
   $siz = 9+int(rand(6)+1) if $siz == 10;

   if ($siz == 0)
   {
      $atm = 0;
   }
   else
   {
      $atm = int(rand(6))-int(rand(6)) + $siz;
      $atm = $atm<0? 0 :
             $atm>15? 15 :
             $atm;
   }

   if ($siz < 2)
   {
      $hyd = 0;
   }
   else
   {
      $dm = ($atm < 2 || $atm > 9)? -4 : 0;
      $hyd = int(rand(6))-int(rand(6)) + $atm + $dm;
      $hyd = $hyd<0? 0 :
             $hyd>10? 10 :
             $hyd;
   }

   $pop = int(rand(6))+int(rand(6));
   $pop = int(rand(6))+int(rand(6)) + 5 if $pop == 10;
   
   $gov = int(rand(6))-int(rand(6)) + $pop;
   $gov = $gov<0? 0 :
          $gov>15? 15 :
          $gov;

   $law = int(rand(6))-int(rand(6)) + $gov;
   $law = $law<0? 0 :
          $law>18? 18 :
          $law;

   #$uwp = $hex[$siz] . $hex[$atm] . $hex[$hyd] . $hex[$pop] . $hex[$gov] . $hex[$law];
   $uwp = sprintf("%X%X%X%X%X%X", $siz, $atm, $hyd, $pop, $gov, $law);
   $tradeClasses = getTradeClasses($uwp);

   if ($search_mode)
   {
      unless ($tradeCodes{ $tradeClasses })
      {
         ++$tradeCodes{ $tradeClasses };
         print scalar localtime, " : ", scalar(keys %tradeCodes), " : $tradeClasses\n";
      }
      ++$count;
      if ($count < 1000)
      {
         print "$count\n" unless $count % 100;
      }
      elsif ($count < 10000)
      {
         print "$count\n" unless $count % 1000;
      }
      elsif ($count < 100000)
      {
         print "$count\n" unless $count % 10000;
      }
      else
      {
         print "$count\n" unless $count % 1000000;
      }
      redo;
   } 
   
   ++$tradeCodes{ $tradeClasses };
}

my $i = 0;
for my $tc (sort keys %tradeCodes)
{
   $i++;
   printf "%3d. %-15s : %-4s  %d\n", $i, $tc, $tradeCodes{$tc}*100/$epochs, $tradeCodes{$tc};
}

for my $tc (sort keys %tradeCodes)
{
   printf "   %-16s,  # %.2f \n", "'$tc'", $tradeCodes{$tc}/100;
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

