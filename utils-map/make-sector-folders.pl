use strict;

#10 rem ---------------------------------------
#11 rem
#12 rem  This little utility requires a LOT of
#13 rem  data to be resident in BASIC strings:
#14 rem
#15 rem  fi$  the filename to create
#16 rem  di$  the dirname to create it in
#17 rem  uw$(79) up to 80 UWP data lines
#19 rem
#20 rem ---------------------------------------
#100 open 1,8,2"//di$/:fi$,P,W"
#110 for x = 0 to 79
#120 if uw$(x) <> "" then print#1, uw$(x)
#130 next
#140 close 1

for my $row (47..54)
{
   for my $col (43..54)
   {
      my $dirname = "SEC-$col-$row";
      print<<EOMKDIR;
DOS"MD:$dirname"
EOMKDIR
   }
}
