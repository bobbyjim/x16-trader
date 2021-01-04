5 COLOR 15,0 :?CHR$(147)	:REM LT GREY ON BLACK
10 DIM BU$(30) :REM LINE BUFFER
15 DIM DX(16),DY(16),DC(16),DR(16) :REM DESTINATION VECTORS
20 LOAD "ALLSHIPS.BIN"      ,8, 3,$A000 :REM BANK 3
25 LOAD "BANNER.BIN"        ,8, 4,$A000 :REM BANK 4
30 LOAD "SPINWARD.SPACE.BIN",8,11,$A000 :REM BANK 11 AND UP
35 GOSUB 2555
40 GOSUB 1105
45 GOSUB 1825
50 C4 = 100000 :REM CR
55 A1 = 19         :REM COL
60 A2 = 10         :REM ROW
65 A3 = 2        :REM RANGE
70 A5 = 0    :REM DEST.AROW
75 A4 = 0    :REM DEST.ACOL
80    GOSUB 1970
85    GOSUB 1165
90    GOTO 80
95 REM
100 REM ------------------- INITIALIZE ENEMY ------------------
105 REM
110 REM  FOR NOW, LET'S JUST USE A SIMPLE DAMAGE LEVEL
115 B5 = INT(RND(1)*8+2) :REM 200 TO 900 TONS
120 B6 = 0
125 RETURN
130 REM --------------------- COMBAT ----------------------
135 B1 = 1
140 B2 = 30
145 B3 = 30
150 B4 = 30
155 GOSUB 115
160 ? HR$ "  SENSOP'S REPORT" :? HR$;
165 ? "  KEPTIN, VE ARE FACING A SIZE" B5 "CORSAIR\X11"
170 REM --------------------- COMBAT LOOP ------------------
175    ? HR$ "  HELM REPORT, ROUND"; B1: ? HR$;
180    ? "  CAPTAIN, SHALL WE:\X11"
185    ? "    1 - ATTACK!"
190    ? "    2 - DEPLOY REPAIR DETAIL!"   
195    ? "    3 - DEPLOY MEDICAL DETAIL!"
200    ? "    4 - MAKE AN EMERGENCY JUMP!"
205    ? "    5 - SIGNAL SURRENDER!\X11"
210    INPUT "  YOUR COMMAND"; CM$ :CM = VAL(CM$)
215    IF CM < 1 OR CM > 5 GOTO 210
220    ? CHR$(147);
225    B2 = 30 :B3 = 30 :B4 = 30
230    IF CM = 1 THEN B2 = 60
235    IF CM = 2 THEN B3 = 60
240    IF CM = 3 THEN B4 = 60
245    IF CM = 4 THEN GOSUB 855 :IF EJ=1 THEN RETURN
250    IF CM = 5 GOTO 975
255    GOSUB 305  :INPUT "   PRESS <RETURN>"; YN$
260    GOSUB 605  :INPUT "   PRESS <RETURN>"; YN$
265    GOSUB 715 :INPUT "   PRESS <RETURN>"; YN$
270    GOSUB 520
275    GOSUB 455 :IF WW=1 THEN RETURN
280    B1 = B1 + 1
285 GOTO 175
290 REM
295 REM -------------------- BATTLE ACTION -----------------
300 REM
305 ? HR$ "  BATTLE REPORT" :?HR$;
310 FOR X = 6 TO C0
315    Q=0
320    FOR P = 1 TO B9(X)
325       Q = Q + INT(RND(1)*6)
330    NEXT
335    IF Q > 8 GOTO 370
340    D = INT(RND(1)*B2)
345    ? "   TURRET" X-5;
350    IF D < 11 THEN ? "NICKS THE ENEMY SHIP."
355    IF D > 10 AND D < 21 THEN ? "HITS THE ENEMY SHIP!"
360    IF D > 20 THEN ? "WALLOPS THE ENEMY SHIP!!"
365    B6 = B6 + D
370 NEXT
375 ?
380 FOR X = 1 TO B5
385    IF INT(RND(1)*6)+INT(RND(1)*6)+2 > 6 GOTO 425
390    T = INT(RND(1)*C0)
395    ? "   OUR " B7$(T) " IS HIT."
400    B9(T) = B9(T)+1
405    IF INT(RND(1)*6)+INT(RND(1)*6)+2 > 6 GOTO 425
410    C = INT(RND(1)*C3)
415    ? "   " C1$(C) " IS HIT."
420    C2(C) = C2(C) + 1
425 NEXT 
430 ?
435 RETURN
440 REM
445 REM --------------- CHECK ENEMY STATUS --------------
450 REM
455 WW = 0
460 IF B6 < B5 * 100 THEN RETURN
465 WW = 1
470 IF B6 - B5 > 100 GOTO 485
475 ? HR$ "  SENSOP'S REPORT" :? HR$;
480 ? "   ZE ENEMY RUNS!  VE HAVE VON!!" :RETURN
485 ? HR$ "  HELM REPORT" :?HR$
490 ? "   THE ENEMY SURRENDERS!"
495 ? "   WE HAVE CLAIMED SALVAGE RIGHTS TO THEIR" B5*100 "TON CORSAIR!"
500 RETURN
505 REM
510 REM --------------- CHECK POWERPLANT STATUS --------------
515 REM
520 IF B9(4) = B8(4)+1 GOTO 570
525 IF B9(4) <= B8(4)+1 THEN RETURN
530 IF C2(2) > 1 GOTO 555
535 ? HR$ "  ENGINEER'S REPORT" :?HR$;
540 ? "  ACH CAPTAIN, THE POWERRR PLANT'S A COMPLETE WRRRECK!"
545 ? "  WE'RE DEAD IN TH' WATERRR!"
550 GOTO 990
555 ? HR$ "  HELM REPORT" :?HR$;
560 ? "  CAPTAIN, THE ENGINES AREN'T RESPONDING. THE SHIP APPEARS TO BE DEAD.\X11"
565 GOTO 990
570 ? HR$ "  ENGINEER'S REPORT" :?HR$;
575 ? "  ACH CAPTAIN, THE POWERRR PLANT'S A MESS."
580 ? "  ONE MORE HIT LIKE THAT AN' WE'RRRE FINISHED!"
585 RETURN
590 REM
595 REM -------------------- REPAIR ACTION -----------------
600 REM
605 ? HR$ "  DAMAGE CONTROL REPORT" :?HR$;
610 IF C2(2) > 1 THEN ? "  THE ENGINEER IS HURT; NO REPORT AVAILABLE.\X11" :RETURN
615 ? "", "COMPONENT",, "STATUS"
620 ? HW$
625 FOR X = 0 TO C0
630    IF B9(X) > 0 AND INT(RND(1)*B3) > 19 THEN B9(X) = B9(X) - 1
635    IF X = 5 THEN ? DL$;
640    ? "   ",
645    ? LEFT$(B7$(X)+"             ",16),
650    A = B8(X)
655    B = B9(X)
660    IF B>0 AND B>A AND X<6 THEN ? "   " + CHR$(28);
665    IF B>0 AND B>A AND X<6 THEN ? "OUT OF ACTION" + CHR$(155)
670    IF B>0 AND (B<=A OR X>5) THEN ? "   " + CHR$(158);
675    IF B>0 AND (B<=A OR X>5) THEN ? "DAMAGED" + CHR$(155)
680    IF B=0 THEN ? "   " + CHR$(153) + "OK" + CHR$(155)
685 NEXT
690 ?
695 RETURN
700 REM
705 REM -------------------- MEDICAL ACTION -----------------
710 REM
715 ? HR$ "  MEDICAL REPORT" :?HR$;
720 IF C2(3) > 1 THEN ? "  THE MEDIC IS HURT; NO REPORT AVAILABLE.\X11" :RETURN
725 ? "", "CREWMEMBER", "ROLE      ", "WOUNDS"
730 ? HW$
735 FOR X = 0 TO C3
740    IF C2(X) > 0 AND INT(RND(1)*B4) > 19 THEN C2(X)=C2(X)-1
745    IF C2(X) > 0 AND INT(RND(1)*B4) > 29 THEN C2(X)=C2(X)-1
750    IF C2(X) > 0 AND INT(RND(1)*B4) > 39 THEN C2(X)=C2(X)-1
755    IF X = 4 THEN ? DL$;
760    ? "   ",
765    ? LEFT$(C1$(X)+"            ",16),
770    R$ = "          "
775    IF X = 0 THEN R$ = "PILOT     "
780    IF X = 1 THEN R$ = "ASTROGATOR"
785    IF X = 2 THEN R$ = "ENGINEER  "
790    IF X = 3 THEN R$ = "MEDIC     "
795    IF X = 4 THEN R$ = "STEWARD   "
800    ? R$,
805    W$ = CHR$(28) + "OUT OF ACTION" + CHR$(155)
810    IF C2(X) = 0 THEN W$ = CHR$(153) + "OK" + CHR$(155)
815    IF C2(X) = 1 THEN W$ = CHR$(158) + "WOUNDED" + CHR$(155)
820    ? W$
825 NEXT
830 ?
835 RETURN
840 REM
845 REM ------------------ EMERGENCY JUMP --------------------
850 REM
855 IF C2(1) > 0 GOTO 895
860 IF C2(2) > 0 GOTO 910
865 IF B9(3) >= B8(3) GOTO 940
870 IF INT(RND(1)*6)+INT(RND(1)*6)+2 > 6 GOTO 925
875 ? "   AYE, YOU'VE GOT IT, CAPTAIN!"
880 FOR T = 1 TO 1000 : TT = SQR(12345) :NEXT
885 ? "   CAPTAIN, WE'VE SUCCESSFULLY JUMPED TO SAFETY!"
890 EJ=1 :RETURN
895 ? HR$ "   HELM REPORT" :? HR$;
900 ? "   OUR ASTROGATOR IS WOUNDED, AND CANNOT CALCULATE A JUMP.\X11"
905 EJ=0 :RETURN
910 ? HR$ "   HELM REPORT" :? HR$;
915 ? "   OUR ENGINEER IS WOUNDED, AND CANNOT OPERATE THE JUMP DRIVE.\X11"
920 EJ=0 :RETURN
925 ? HR$ "   ENGINEER'S REPORT" :? HR$;
930 ? "   ACH, WE KENNA JUMP YET CAPTAIN, GIMME 20 MORRRE MINUTES!\X11"
935 EJ=0 :RETURN
940 ? HR$ "   ENGINEER'S REPORT" :? HR$;
945 ? "   ACH, THE JUMP DRIVES ARRRE DOON, CAPTAIN!"
950 ? "   WE NE KENNA JUMP UNTIL THEY'RRRE REPAIRRRED!\X11"
955 EJ=0 :RETURN
960 REM
965 REM --------------------- SURRENDER ----------------------
970 REM
975 ? HR$ "  HELM REPORT": ?HR$;
980 ? "  SURRENDER ACKNOWLEDGED. CREW IS ARMING THEMSELVES.\X11"
985 FOR T = 1 TO 1000: TT=SQR(12345) :NEXT
990 ? HR$ "  HELM REPORT": ?HR$;
995 ? "  CAPTAIN, WE HAVE BEEN BOARDED."
1000 FOR T = 1 TO 1000: TT=SQR(12345) :NEXT
1005 ? "  FIGHTING REPORTED IN ALL AREAS."
1010 FOR T = 1 TO 1000: TT=SQR(12345) :NEXT
1015 RR = INT(RND(1)*6)+INT(RND(1)*6)+2
1020 IF RR < 6 THEN ? "  WE HAVE REPELLED THE PIRATES!!" : RETURN
1025 IF RR < 8 THEN ? "  OUR CARGO AND VEHICLES HAVE BEEN TAKEN." :RETURN
1030 IF RR < 10 THEN ? "  WE HAVE BEEN STRANDED ON A MARGINAL WORLD." :END
1035 ? "  THE CREW HAS BEEN SOLD INTO SLAVERY."
1040 END
1045 REM 
1050 REM ---------------- INITIALIZE CREW ----------------
1055 REM
1060 REM  CREW ROLES ARE INDIVIDUALIZED AS FOLLOWS:
1065 REM
1070 REM    0 - PILOT
1075 REM    1 - ASTROGATOR
1080 REM    2 - ENGINEER
1085 REM    3 - MEDIC
1090 REM    4 - STEWARD/FREIGHTMASTER
1095 REM    5 AND UP - GUNNERS
1100 REM
1105 DIM C1$(24), C2(24)
1110 C3 = -1
1115    C3 = C3+1
1120    READ C1$(C3)
1125    C2(C3) = 0
1130    IF C1$(C3) <> "-1" GOTO 1115
1135 C3 = C3 - 1
1140 DATA JAMISON, AKRIND, KODRAY, ARRLANROUGHL, REYNALDI, SHARIK, -1
1145 RETURN
1150 REM
1155 REM -------------------------- SYSTEM -----------------------
1160 REM
1165    COLOR 15,0 :REM WHITE ON BLACK
1170    A0 = 1
1175 REM
1180 REM  THERE IS ONLY ONE EXIT OUT OF THE SYSTEM, AND THAT IS "STARPORT"
1185 REM
1190    ON A0 GOTO 1200, 1275, 1280, 1330, 1380
1195    GOTO 1190
1200 ? CHR$(147) "\X11\X11"
1205 BA$ = "      INSYSTEM"
1210 GOSUB 2580
1215 ?:? SPC(25) "YOU ARE IN ORBIT AROUND ";
1220 GOSUB 1455
1225 ?:?
1230 ?:? SPC(25) "1 - LAND AT STARPORT"
1235 ?:? SPC(25) "2 - WILDERNESS REFUELING"
1240 ?:? SPC(25) "3 - SYSTEM SURVEY"
1245 ?:? SPC(25) "4 - JUMP"
1250 ?:?
1255 GET DE$
1260 IF DE$ < "1" OR DE$ > "4" GOTO 1255
1265 A0 = VAL(DE$)+1
1270 GOTO 1190
1275 RETURN
1280 ? CHR$(147) "\X11\X11"
1285 BA$ = "      REFUEL"
1290 GOSUB 2580
1295 ?:? SPC(25) "<REFUEL OPTIONS>"
1300 ?:? SPC(25) "X: RETURN"
1305 ?:?
1310 GET DE$
1315    IF DE$="X" THEN A0 = 1
1320    IF DE$ < "1" OR DE$ > "X" GOTO 1310
1325 GOTO 1190
1330 ? CHR$(147) "\X11\X11"
1335 BA$ = "      SURVEY"
1340 GOSUB 2580
1345 ?:? SPC(25) "<SURVEY OPTIONS>"
1350 ?:? SPC(25) "X: RETURN"
1355 ?:?
1360 GET DE$
1365    IF DE$="X" THEN A0 = 1
1370    IF DE$ < "1" OR DE$ > "X" GOTO 1360
1375 GOTO 1190
1380 ? CHR$(147) "\X11\X11"
1385 BA$ = "      JUMP"
1390 GOSUB 2580
1395 A1 = A4 
1400 A2 = A5
1405 A4 = 0 :REM UNSET FLIGHT PLAN
1410 A5 = 0
1415 ?:? SPC(25) "WELCOME TO ";
1420 GOSUB 1455
1425 ?:?
1430 IF P = 1 THEN GOSUB 135
1435 INPUT "PRESS <RETURN>"; YN$
1440 A0 = 1
1445 GOTO 1190
1450 REM MAP.WHERE AM I(COL,ROW)
1455    REM FIRST POINT TO THE RIGHT BANK
1460    BA = INT((A1-1)/4)
1465    POKE $9F61,11+BA
1470    REM NOW CALCULATE THE ACTUAL COLUMN
1475    CA = A1 - 1
1480    CA = 4*(CA/4-INT(CA/4))
1485    AD = $A000 + CA * 32 * 64 + (A2-1) * 32 * 1
1490    FOR X = 10 TO 24: ? CHR$(PEEK(AD+X)); :NEXT
1495    A6 = PEEK(AD+7)
1500    A7 = PEEK(AD+8)
1505    A8 = PEEK(AD+26)
1510    A9 = PEEK(AD+27)
1515    B0 = PEEK(AD+28)
1520    REM CHECK FOR PIRATES
1525    P = 3
1530    IF A6 = 65 THEN P = 1
1535    IF A6 = 66 THEN P = 2
1540    IF A6 = 67 THEN P = 3
1545    IF A6 = 68 THEN P = 4
1550    IF A6 = 69 THEN P = 5
1555    IF A6 = 88 THEN P = 6
1560    IF B0 = 65 THEN P = P + 2
1565    IF B0 = 82 THEN P = P + 4
1570    IF INT(RND(1)*6)+INT(RND(1)*6)+2 < P THEN P = 1 :RETURN
1575    P = 0 :RETURN
1580 REM  GET REGIONAL UWPS(COL,ROW,RNG)
1585 POKE $9F61,11 :REM MAP
1590 DW = 0
1595 FOR CC = A1 - A3 TO A1 + A3
1600 FOR RR = A2 - A3 TO A2 + A3
1605 NEXT RR
1610 NEXT CC
1615    ? CHR$(147) "SHIP LIST\X11"
1620    POKE $9F61,3  :REM SHIPS
1625    AD = $A000
1630    CT = 0
1635    IF PEEK(AD) = 0 THEN 1750
1640    O% = PEEK(AD+28) :REM OWNER
1645    L = PEEK(AD+3)   :REM COMPONENT LENGTH
1650    T = PEEK(AD+24)  :REM SIZE CODE
1655    M1 = PEEK(AD+22) :REM MISSION CODE 1
1660    IF A9 <> O% GOTO 1740
1665    IF M1 = ASC("E") GOTO 1740 :REM NO ESCORTS
1670    IF M1 = ASC("C") GOTO 1740 :REM NO CRUISERS
1675    ? "     ";
1680    ? CHR$(M1);          :REM MISSION CODE 1
1685    ? CHR$(PEEK(AD+23)); :REM MISSION CODE 2
1690    ? " "; 
1695    FOR X = 4 TO 21 : ? CHR$(PEEK(AD+X)); :NEXT
1700    ? T*100;
1705    IF T<10  THEN ? " ";
1710    ? "TONS";
1715    MC = PEEK(AD+27)
1720    ? "  MCR " MC*T
1725    CT=CT+1
1730    IF CT/4 <> INT(CT/4) GOTO 1740
1735    ? "     -----------------------------------------"
1740    AD = AD + 4 + L :REM NEXT RECORD
1745    GOTO 1635
1750    IF CT = 0 THEN ? "     NO SHIPS AVAILABLE."
1755    RETURN
1760 REM
1765 REM ----------------- INITIALIZE SHIP ------------------
1770 REM
1775 REM   "SUBTYPE" IS DRIVE RATING, WEAPON SUBTYPE, OR FUEL AMOUNT.
1780 REM
1785 REM   COMPONENT 0 IS "THE HULL".
1790 REM   COMPONENT 1 IS STREAMLINING, GENERALLY.
1795 REM   COMPONENT 2 IS THE MANEUVER DRIVE.
1800 REM   COMPONENT 3 IS THE JUMP DRIVE.
1805 REM   COMPONENT 4 IS THE POWER PLANT.
1810 REM   COMPONENT 5 IS FUEL TANKAGE.
1815 REM   COMPONENT 6 AND UP ARE DEFENSES.
1820 REM
1825 DIM B7$(40), B8(40), B9(40)
1830 C0=-1
1835    C0=C0+1
1840    READ B7$(C0)
1845    READ B8(C0)
1850    B9(C0) = 0
1855    IF B7$(C0) <> "-1" GOTO 1835
1860 C0 = C0 - 1
1865 DATA HULL,         	0 
1870 DATA STREAMLINING, 	0 
1875 DATA MANEUVER DRIVE,    1 
1880 DATA JUMP DRIVE,        1 
1885 DATA POWER PLANT,       1 
1890 DATA FUEL TANKAGE,      10
1895 DATA TURRET 1,      	2
1900 DATA TURRET 2,     	2
1905 REM DATA TURRET 1L,   21,   TURRET 2L,  22,  TURRET 3L,  23
1910 REM DATA TURRET 1M,   31,   TURRET 2M,  32,  TURRET 3M,  33
1915 REM DATA TURRET 1S,   41,   TURRET 2S,  42,  TURRET 3S,  43
1920 REM
1925 REM DATA BARBETTE L,  53,   BAY L,      54
1930 REM DATA BARBETTE M,  63,   BAY M,      64
1935 REM DATA BARBETTE S,  73,   BAY S,      74
1940 REM DATA BARBETTE A,  83,   BAY A,      84
1945 DATA -1, -1
1950 RETURN
1955 REM
1960 REM -------------------------- STARPORT -----------------------
1965 REM
1970    COLOR 15,0 :REM WHITE ON BLACK
1975    A0 = 1
1980 REM 
1985 REM THERE IS ONLY ONE EXIT OUT OF THE STARPORT, AND THAT IS "EXIT STARPORT"
1990 REM
1995    ON A0 GOTO 2010, 2110, 2335, 2385, 2440, 2485, 2520, 2005
2000    GOTO 1995
2005    RETURN
2010 ? CHR$(147) "\X11\X11"
2015 BA$ = "  KIGUKI"
2020 GOSUB 2580
2025 ?:? SPC(25) "WELCOME TO ";
2030 GOSUB 1455
2035 ?:?
2040 ?:? SPC(25) "1 - FILE FLIGHT PLAN";
2045 IF A4 > 0 THEN ? " (FILED)"
2050 IF A4 = 0 THEN ?
2055 ?:? SPC(25) "2 - COMMODITY EXCHANGE"
2060 ?:? SPC(25) "3 - SHIPYARD"
2065 ?:? SPC(25) "4 - HIRING HALL"
2070 ?:? SPC(25) "5 - SCOUT BASE"
2075 ?:? SPC(25) "6 - NAVAL BASE"
2080 ?:? SPC(25) "7 - EXIT STARPORT"
2085 ?:?
2090 GET DE$
2095 IF DE$ < "1" OR DE$ > "7" GOTO 2090
2100 A0 = VAL(DE$)+1
2105 GOTO 1995
2110 ? CHR$(147) "\X11\X11"
2115 BA$ = "  GIMGU"
2120 GOSUB 2580
2125 DW=1 :REM DESTINATION WORLD COUNT
2130 FOR CC = A1-2 TO A1+2
2135 FOR RR = A2-2 TO A2+2
2140 IF CC < 1 OR RR < 1 GOTO 2150
2145 IF CC <> A1 OR RR <> A2 THEN GOSUB 2240 
2150 NEXT
2155 NEXT
2160 ?:? SPC(25) "X: RETURN"
2165 ?:?
2170 A4 = 0
2175 GET DE$
2180    IF DE$="X" THEN A0 = 1 :GOTO 1995
2185    IF DE$="" GOTO 2175
2190    FP=ASC(DE$)
2195    FP = FP - 64
2200    IF FP < 1 OR FP > DW GOTO 2175
2205    ? "TARGET" FP "SELECTED."
2210    A4 = DC(FP)
2215    A5 = DR(FP)
2220    INPUT "PRESS <RETURN>"; YN$
2225    A0 = 1
2230 GOTO 1995
2235 REM FETCH UWP DATA(CC,RR)
2240    IF DW = 17 THEN RETURN
2245    REM FIRST POINT TO THE RIGHT BANK
2250    BA = INT((CC-1)/4)
2255    POKE $9F61,11 + BA 
2260    REM NOW CALCULATE THE ACTUAL COLUMN
2265    CA = CC-1
2270    CA = 4*(CA/4-INT(CA/4))
2275    AD = $A000 + CA * 32 * 64 + (RR-1) * 32 * 1
2280    IF PEEK(AD+10) = 32 THEN RETURN
2285    DC(DW) = CC 
2290    DR(DW) = RR
2295    ? SPC(24) CHR$(DW+64) " "; :FOR X = 2 TO 27: ? CHR$(PEEK(AD+X)); :NEXT
2300    IF PEEK(AD+28) = ASC(" ") THEN ? " \X99OK\X9B"
2305    IF PEEK(AD+28) = ASC("A") THEN ? " \X9ECAUTION\X9B"
2310    IF PEEK(AD+28) = ASC("R") THEN ? " \X96DANGER\X9B"
2315    IF DW/4 = INT(DW/4) THEN ? SPC(24);
2320    IF DW/4 = INT(DW/4) THEN ? "\X97....................................\X9B"
2325    DW=DW+1
2330 RETURN
2335 ? CHR$(147) "\X11\X11"
2340 BA$ = "  KAR"          
2345 GOSUB 2580
2350 ?:? SPC(25) "<YOUR CARGO AND VALUE>"
2355 ?:? SPC(25) "<SYSTEM CARGO AND VALUE>"
2360 ?:? SPC(25) "X: RETURN"
2365 GET SE$
2370    IF SE$="X" THEN A0 = 1
2375    IF SE$ < "1" OR SE$ > "X" GOTO 2365
2380 GOTO 1995
2385 ? CHR$(147) "\X11\X11"
2390 BA$ = "  KHISDELIKUR"
2395 GOSUB 2580
2400 AL$ = "I" :GOSUB 1615 :REM SHOW IMPERIAL SHIPS
2405 ?:? SPC(25) "<COMPONENT UPGRADES FOR SALE>"
2410 ?:? SPC(25) "<NEW COMPONENTS FOR SALE>"
2415 ?:? SPC(25) "X: RETURN"
2420 GET SE$
2425    IF SE$="X" THEN A0 = 1
2430    IF SE$ < "1" OR SE$ > "X" GOTO 2420
2435 GOTO 1995
2440 ? CHR$(147) "\X11\X11"
2445 BA$ = "  KHIMEKKI"
2450 GOSUB 2580
2455 ?:? SPC(25) "<CHARACTER LIST>"
2460 ?:? SPC(25) "X: RETURN"
2465 GET SE$
2470    IF SE$="X" THEN A0 = 1
2475    IF SE$ < "1" OR SE$ > "X" GOTO 2465
2480 GOTO 1995
2485 ? CHR$(147) "\X11\X11"
2490 BA$ = "  MEDISHE"
2495 GOSUB 2580
2500 ? SPC(25) "THE SCOUTS BUY YOUR SURVEY FOR CR100,000."
2505 GET A$ : IF A$="" GOTO 2505
2510 A0 = 1
2515 GOTO 1995
2520 ? CHR$(147) "\X11\X11"
2525 BA$ = "  KIISHMU"
2530 GOSUB 2580
2535 ? SPC(25) "THE NAVY BUYS YOUR RECONNAISSANCE DATA FOR CR100,000."
2540 GET A$ : IF A$="" GOTO 2540
2545 A0 = 1
2550 GOTO 1995
2555 HR$ = "\X1C"
2560 FOR X = 1 TO 80 :HR$ = HR$ + "\XC3" :NEXT
2565 HR$ = HR$ + "\X9B"
2570 RETURN
2575 REM BANNER(BA$)
2580 POKE $9F61,4 :REM BANNER CODE
2585 ? HR$;
2590 FOR X = 1 TO LEN(BA$)
2595    A = ASC(MID$(BA$,X,1))
2600    POKE $A000,A
2605    SYS $A001
2610 NEXT X
2615 ?:?:?:? HR$
2620 RETURN
2625 REM DISTANCE(C1,R1,C2,R2) -> D%
2630 A1 = R1 + INT(C1/2)
2635 A2 = R2 + INT(C2/2)
2640 D1% = ABS(A1-A2)
2645 D2% = ABS(C1-C2)
2650 D% = ABS( A1 - A2 - C1 + C2 )
2655 IF (D1% >= D2%) AND (D1% >= D%) THEN D% = D1%
2660 IF (D2% >= D1%) AND (D2% >= D%) THEN D% = D2%
2665 RETURN
2670 REM DICE(DM%) -> 2D6+DM% -> DM%
2675 DM% = DM% + 2 + INT(RND(1)*6) + INT(RND(1)*6)
2680 RETURN
2685 REM FLUX() -> FL%
2690 FL% = 6 * (RND(1)-RND(1))
2695 RETURN
2700 REM EHEX2DEC(HX$) -> N%
2705 N%=0
2710 IF HX$ <= "9" THEN N%=ASC(HX$)-48 :RETURN
2715 IF HX$ >= "P" THEN N%=ASC(HX$)-57 :RETURN
2720 IF HX$ >= "J" THEN N%=ASC(HX$)-56 :RETURN
2725 IF HX$ >= "A" THEN N%=ASC(HX$)-55 :RETURN
2730 RETURN
2735 REM   DECOMPRESS BYTE PAIRS OF UWP-TEXT
2740 REM   PASSED IN AS B0%(), COUNT IN B0%(0)
2745 REM   OUTPUT TO O$
2750 REM   ASSUME B0%() IS INITIALIZED
2755 AL$(0) = " ABCDEFGHIJKLMNOPQRSTUVWXYZ?:'-"
2760 AL$(1) = " 0123456789ABCDEFGHILMNORSTUWY-"
2765 O$ = "": V=0: FOR BB = 1 TO B0%(0) STEP 2
2770 B0% = B0%(BB)
2775 B1% = B0%(BB+1)
2780 REM ? BB, B0%, B1%
2785 B2% = B0% AND 31
2790 B3% = (B0%/32) + (B1% AND 3)*8
2795 B4% = (B1% AND 127) / 4
2800 REM TRANSLATE
2805 B% = B2% :GOSUB 2830 :REM DECODE
2810 B% = B3% :GOSUB 2830 :REM DECODE
2815 B% = B4% :GOSUB 2830 :REM DECODE
2820 NEXT
2825 RETURN
2830 IF B% = 31 THEN V = 1 - V :RETURN
2835 REM ? "INDEX: " B%+1 " LETTER: " MID$(AL$(V),B%+1,1)
2840 O$ = O$ + MID$(AL$(V),B%+1,1)
2845 RETURN
