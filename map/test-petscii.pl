

my $line = 100;
my $i = 0;

print<<EOPRG;

10 FOR X = 1 TO 8
20 READ A\$(X)
30 FOR Y = 1 TO 16
40 B\$ = MID\$(A\$(X),Y,1)
41 B = ASC(B\$) - 48
42 IF B > 95 THEN B = B - 48
50 ? B,
60 NEXT
70 ?
90 NEXT
EOPRG


print "$line DATA \"";
for (0x30 .. 0x5f, 0x90 .. 0xbf, 0x60 .. 0x7f)
{
   if ($i == 16)
   {
      print "\"\n";
      print "$line DATA \"";
      $i = 0;
   }
   printf("\\X%X", $_);
   ++$i;
   ++$line;
}


print "\n";

