use strict;


my $header = uc 'trader-x16 player';
my $name   = uc 'birdrock';
my $galx   = 502;
my $galy   = 500;
my $kcr    = 100;

open my $fp, '>', 'PLAYER.BIN';
print $fp pack 'ACA23x', 'X', 16, $header;
print $fp pack 'A15x', $name;
print $fp pack 'vv', $galx, $galy;
print $fp pack 'v', $kcr;
print $fp pack 'x18';

close $fp;

