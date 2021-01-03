use strict;


my $header = uc 'TEMPLE OF APSHAI GAME STATE'; # 31b
my $name   = uc 'MATT HEFFERMAN'; # 15b
my $galx   = 502;
my $galy   = 500;
my $sx     = 3;
my $sy     = 10;
my $life   = 10;

open my $fp, '>', 'TESTGAMESTATE.BIN';
print $fp pack 'ACA31x', 'X', 16, $header;
print $fp pack 'A15x', $name;
print $fp pack 'vv', $galx, $galy;
print $fp pack 'CC', $sx, $sy;
print $fp pack 'C', $life;
print $fp pack 'x16';

close $fp;

