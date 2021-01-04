5 COLOR 15,0 :?CHR$(147)	:REM LT GREY ON BLACK
10 DIM BU$(30) :REM LINE BUFFER
15 DIM DX(16),DY(16),DC(16),DR(16) :REM DESTINATION VECTORS
20 LOAD "ALLSHIPS.BIN"      ,8, 3,$A000 :REM BANK 3
25 LOAD "BANNER.BIN"        ,8, 4,$A000 :REM BANK 4
30 LOAD "SPINWARD.SPACE.BIN",8,11,$A000 :REM BANK 11 AND UP
35 GOSUB 2540
40 GOSUB 1090
45 C4 = 100000 :REM CR
50 A1 = 19         :REM COL
55 A2 = 10         :REM ROW
60 A3 = 2        :REM RANGE
65 A5 = 0    :REM DEST.AROW
70 A4 = 0    :REM DEST.ACOL
75    GOSUB 1955
80    GOSUB 1150
85    GOTO 75
90 REM
95 REM ------------------- INITIALIZE ENEMY ------------------
100 REM
105 REM  FOR NOW, LET'S JUST USE A SIMPLE DAMAGE LEVEL
110 B5 = INT(RND(1)*8+2) :REM 200 TO 900 TONS
115 B6 = 0
120 RETURN
125 REM --------------------- COMBAT ----------------------
130 B1 = 1
135 B2 = 30
140 B3 = 30
145 B4 = 30
150 GOSUB 110
155 ? HR$ "  SENSOP'S REPORT" :? HR$;
160 ? "  KEPTIN, VE ARE FACING A SIZE" B5 "CORSAIR\X11"
165 REM --------------------- COMBAT LOOP ------------------
170    ? HR$ "  HELM REPORT, ROUND"; B1: ? HR$;
175    ? "  CAPTAIN, SHALL WE:\X11"
180    ? "    1 - ATTACK!"
185    ? "    2 - DEPLOY REPAIR DETAIL!"   
190    ? "    3 - DEPLOY MEDICAL DETAIL!"
195    ? "    4 - MAKE AN EMERGENCY JUMP!"
200    ? "    5 - SIGNAL SURRENDER!\X11"
205    INPUT "  YOUR COMMAND"; CM$ :CM = VAL(CM$)
210    IF CM < 1 OR CM > 5 GOTO 205
215    ? CHR$(147);
220    B2 = 30 :B3 = 30 :B4 = 30
225    IF CM = 1 THEN B2 = 60
230    IF CM = 2 THEN B3 = 60
235    IF CM = 3 THEN B4 = 60
240    IF CM = 4 THEN GOSUB 840 :IF EJ=1 THEN RETURN
245    IF CM = 5 GOSUB 960 :RETURN
250    GOSUB 300  :INPUT "   PRESS <RETURN>"; YN$
255    GOSUB 600  :INPUT "   PRESS <RETURN>"; YN$
260    GOSUB 700 :INPUT "   PRESS <RETURN>"; YN$
265    GOSUB 515
270    GOSUB 450 :IF WW=1 THEN RETURN
275    B1 = B1 + 1
280 GOTO 170
285 REM
290 REM -------------------- BATTLE ACTION -----------------
295 REM
300 ? HR$ "  BATTLE REPORT" :?HR$;
305 FOR X = 6 TO C0
310    Q=0
315    FOR P = 1 TO B9(X)
320       Q = Q + INT(RND(1)*6)
325    NEXT
330    IF Q > 8 GOTO 365
335    D = INT(RND(1)*B2)
340    ? "   TURRET" X-5;
345    IF D < 11 THEN ? "NICKS THE ENEMY SHIP."
350    IF D > 10 AND D < 21 THEN ? "HITS THE ENEMY SHIP!"
355    IF D > 20 THEN ? "WALLOPS THE ENEMY SHIP!!"
360    B6 = B6 + D
365 NEXT
370 ?
375 FOR X = 1 TO B5
380    IF INT(RND(1)*6)+INT(RND(1)*6)+2 > 6 GOTO 420
385    T = INT(RND(1)*C0)
390    ? "   OUR " B7$(T) " IS HIT."
395    B9(T) = B9(T)+1
400    IF INT(RND(1)*6)+INT(RND(1)*6)+2 > 6 GOTO 420
405    C = INT(RND(1)*C3)
410    ? "   " C1$(C) " IS HIT."
415    C2(C) = C2(C) + 1
420 NEXT 
425 ?
430 RETURN
435 REM
440 REM --------------- CHECK ENEMY STATUS --------------
445 REM
450 WW = 0
455 IF B6 < B5 * 100 THEN RETURN
460 WW = 1
465 IF B6 - B5 > 100 GOTO 480
470 ? HR$ "  SENSOP'S REPORT" :? HR$;
475 ? "   ZE ENEMY RUNS!  VE HAVE VON!!" :RETURN
480 ? HR$ "  HELM REPORT" :?HR$
485 ? "   THE ENEMY SURRENDERS!"
490 ? "   WE HAVE CLAIMED SALVAGE RIGHTS TO THEIR" B5*100 "TON CORSAIR!"
495 RETURN
500 REM
505 REM --------------- CHECK POWERPLANT STATUS --------------
510 REM
515 IF B9(4) = B8(4)+1 GOTO 565
520 IF B9(4) <= B8(4)+1 THEN RETURN
525 IF C2(2) > 1 GOTO 550
530 ? HR$ "  ENGINEER'S REPORT" :?HR$;
535 ? "  ACH CAPTAIN, THE POWERRR PLANT'S A COMPLETE WRRRECK!"
540 ? "  WE'RE DEAD IN TH' WATERRR!"
545 GOTO 975
550 ? HR$ "  HELM REPORT" :?HR$;
555 ? "  CAPTAIN, THE ENGINES AREN'T RESPONDING. THE SHIP APPEARS TO BE DEAD.\X11"
560 GOTO 975
565 ? HR$ "  ENGINEER'S REPORT" :?HR$;
570 ? "  ACH CAPTAIN, THE POWERRR PLANT'S A MESS."
575 ? "  ONE MORE HIT LIKE THAT AN' WE'RRRE FINISHED!"
580 RETURN
585 REM
590 REM -------------------- REPAIR ACTION -----------------
595 REM
600 ? HR$ "  DAMAGE CONTROL REPORT" :?HR$;
605 IF C2(2) > 1 THEN ? "  THE ENGINEER IS HURT; NO REPORT AVAILABLE.\X11" :RETURN
610 ? "", "COMPONENT",, "STATUS"
615 ? HW$
620 FOR X = 0 TO C0
625    IF B9(X) > 0 AND INT(RND(1)*B3) > 19 THEN B9(X) = B9(X) - 1
630    IF X = 5 THEN ? DL$;
635    ? "   ",
640    ? LEFT$(B7$(X)+"             ",16),
645    A = B8(X)
650    B = B9(X)
655    IF B>0 AND B>A AND X<6 THEN ? "   " + CHR$(28) + "OUT OF ACTION" + CHR$(155)
660    IF B>0 AND (B<=A OR X>5) THEN ? "   " + CHR$(158) + "DAMAGED" + CHR$(155)
665    IF B=0 THEN ? "   " + CHR$(153) + "OK" + CHR$(155)
670 NEXT
675 ?
680 RETURN
685 REM
690 REM -------------------- MEDICAL ACTION -----------------
695 REM
700 ? HR$ "  MEDICAL REPORT" :?HR$;
705 IF C2(3) > 1 THEN ? "  THE MEDIC IS HURT; NO REPORT AVAILABLE.\X11" :RETURN
710 ? "", "CREWMEMBER", "ROLE      ", "WOUNDS"
715 ? HW$
720 FOR X = 0 TO C3
725    IF C2(X) > 0 AND INT(RND(1)*B4) > 19 THEN C2(X)=C2(X)-1
730    IF C2(X) > 0 AND INT(RND(1)*B4) > 29 THEN C2(X)=C2(X)-1
735    IF C2(X) > 0 AND INT(RND(1)*B4) > 39 THEN C2(X)=C2(X)-1
740    IF X = 4 THEN ? DL$;
745    ? "   ",
750    ? LEFT$(C1$(X)+"            ",16),
755    R$ = "          "
760    IF X = 0 THEN R$ = "PILOT     "
765    IF X = 1 THEN R$ = "ASTROGATOR"
770    IF X = 2 THEN R$ = "ENGINEER  "
775    IF X = 3 THEN R$ = "MEDIC     "
780    IF X = 4 THEN R$ = "STEWARD   "
785    ? R$,
790    W$ = CHR$(28) + "OUT OF ACTION" + CHR$(155)
795    IF C2(X) = 0 THEN W$ = CHR$(153) + "OK" + CHR$(155)
800    IF C2(X) = 1 THEN W$ = CHR$(158) + "WOUNDED" + CHR$(155)
805    ? W$
810 NEXT
815 ?
820 RETURN
825 REM
830 REM ------------------ EMERGENCY JUMP --------------------
835 REM
840 IF C2(1) > 0 GOTO 880
845 IF C2(2) > 0 GOTO 895
850 IF B9(3) >= B8(3) GOTO 925
855 IF INT(RND(1)*6)+INT(RND(1)*6)+2 > 6 GOTO 910
860 ? "   AYE, YOU'VE GOT IT, CAPTAIN!"
865 FOR T = 1 TO 1000 : TT = SQR(12345) :NEXT
870 ? "   CAPTAIN, WE'VE SUCCESSFULLY JUMPED TO SAFETY!"
875 EJ=1 :RETURN
880 ? HR$ "   HELM REPORT" :? HR$;
885 ? "   OUR ASTROGATOR IS WOUNDED, AND CANNOT CALCULATE A JUMP.\X11"
890 EJ=0 :RETURN
895 ? HR$ "   HELM REPORT" :? HR$;
900 ? "   OUR ENGINEER IS WOUNDED, AND CANNOT OPERATE THE JUMP DRIVE.\X11"
905 EJ=0 :RETURN
910 ? HR$ "   ENGINEER'S REPORT" :? HR$;
915 ? "   ACH, WE KENNA JUMP YET CAPTAIN, GIMME 20 MORRRE MINUTES!\X11"
920 EJ=0 :RETURN
925 ? HR$ "   ENGINEER'S REPORT" :? HR$;
930 ? "   ACH, THE JUMP DRIVES ARRRE DOON, CAPTAIN!"
935 ? "   WE NE KENNA JUMP UNTIL THEY'RRRE REPAIRRRED!\X11"
940 EJ=0 :RETURN
945 REM
950 REM --------------------- SURRENDER ----------------------
955 REM
960 ? HR$ "  HELM REPORT": ?HR$;
965 ? "  SURRENDER ACKNOWLEDGED. CREW IS ARMING THEMSELVES.\X11"
970 FOR T = 1 TO 1000: TT=SQR(12345) :NEXT
975 ? HR$ "  HELM REPORT": ?HR$;
980 ? "  CAPTAIN, WE HAVE BEEN BOARDED."
985 FOR T = 1 TO 1000: TT=SQR(12345) :NEXT
990 ? "  FIGHTING REPORTED IN ALL AREAS."
995 FOR T = 1 TO 1000: TT=SQR(12345) :NEXT
1000 RR = INT(RND(1)*6)+INT(RND(1)*6)+2
1005 IF RR < 6 THEN ? "  WE HAVE REPELLED THE PIRATES!!" : RETURN
1010 IF RR < 8 THEN ? "  OUR CARGO AND VEHICLES HAVE BEEN TAKEN." :RETURN
1015 IF RR < 10 THEN ? "  WE HAVE BEEN STRANDED ON A MARGINAL WORLD." :END
1020 ? "  THE CREW HAS BEEN SOLD INTO SLAVERY."
1025 END
1030 REM 
1035 REM ---------------- INITIALIZE CREW ----------------
1040 REM
1045 REM  CREW ROLES ARE INDIVIDUALIZED AS FOLLOWS:
1050 REM
1055 REM    0 - PILOT
1060 REM    1 - ASTROGATOR
1065 REM    2 - ENGINEER
1070 REM    3 - MEDIC
1075 REM    4 - STEWARD/FREIGHTMASTER
1080 REM    5 AND UP - GUNNERS
1085 REM
1090 DIM C1$(24), C2(24)
1095 C3 = -1
1100    C3 = C3+1
1105    READ C1$(C3)
1110    C2(C3) = 0
1115    IF C1$(C3) <> "-1" GOTO 1100
1120 C3 = C3 - 1
1125 DATA JAMISON, AKRIND, KODRAY, ARRLANROUGHL, REYNALDI, SHARIK, -1
1130 RETURN
1135 REM
1140 REM -------------------------- SYSTEM -----------------------
1145 REM
1150    COLOR 15,0 :REM WHITE ON BLACK
1155    A0 = 1
1160 REM
1165 REM  THERE IS ONLY ONE EXIT OUT OF THE SYSTEM, AND THAT IS "STARPORT"
1170 REM
1175    ON A0 GOTO 1185, 1260, 1265, 1315, 1365
1180    GOTO 1175
1185 ? CHR$(147) "\X11\X11"
1190 BA$ = "      INSYSTEM"
1195 GOSUB 2565
1200 ?:? SPC(25) "YOU ARE IN ORBIT AROUND ";
1205 GOSUB 1440
1210 ?:?
1215 ?:? SPC(25) "1 - LAND AT STARPORT"
1220 ?:? SPC(25) "2 - WILDERNESS REFUELING"
1225 ?:? SPC(25) "3 - SYSTEM SURVEY"
1230 ?:? SPC(25) "4 - JUMP"
1235 ?:?
1240 GET DE$
1245 IF DE$ < "1" OR DE$ > "4" GOTO 1240
1250 A0 = VAL(DE$)+1
1255 GOTO 1175
1260 RETURN
1265 ? CHR$(147) "\X11\X11"
1270 BA$ = "      REFUEL"
1275 GOSUB 2565
1280 ?:? SPC(25) "<REFUEL OPTIONS>"
1285 ?:? SPC(25) "X: RETURN"
1290 ?:?
1295 GET DE$
1300    IF DE$="X" THEN A0 = 1
1305    IF DE$ < "1" OR DE$ > "X" GOTO 1295
1310 GOTO 1175
1315 ? CHR$(147) "\X11\X11"
1320 BA$ = "      SURVEY"
1325 GOSUB 2565
1330 ?:? SPC(25) "<SURVEY OPTIONS>"
1335 ?:? SPC(25) "X: RETURN"
1340 ?:?
1345 GET DE$
1350    IF DE$="X" THEN A0 = 1
1355    IF DE$ < "1" OR DE$ > "X" GOTO 1345
1360 GOTO 1175
1365 ? CHR$(147) "\X11\X11"
1370 BA$ = "      JUMP"
1375 GOSUB 2565
1380 A1 = A4 
1385 A2 = A5
1390 A4 = 0 :REM UNSET FLIGHT PLAN
1395 A5 = 0
1400 INPUT "PRESS <RETURN>"; DE$
1405 A0 = 1
1410 ?:? SPC(25) "WELCOME TO AROUND ";
1415 GOSUB 1440
1420 ?:?
1425 IF P = 1 THEN GOSUB 130
1430 GOTO 1175
1435 REM MAP.WHERE AM I(COL,ROW)
1440    REM FIRST POINT TO THE RIGHT BANK
1445    BA = INT((A1-1)/4)
1450    POKE $9F61,11+BA
1455    REM NOW CALCULATE THE ACTUAL COLUMN
1460    CA = A1 - 1
1465    CA = 4*(CA/4-INT(CA/4))
1470    AD = $A000 + CA * 32 * 64 + (A2-1) * 32 * 1
1475    FOR X = 10 TO 24: ? CHR$(PEEK(AD+X)); :NEXT
1480    A6 = PEEK(AD+7)
1485    A7 = PEEK(AD+8)
1490    A8 = PEEK(AD+26)
1495    A9 = PEEK(AD+27)
1500    B0 = PEEL(AD+28)
1505    REM CHECK FOR PIRATES
1510    P = 3
1515    IF A6 = "A" THEN P = 1
1520    IF A6 = "B" THEN P = 2
1525    IF A6 = "C" THEN P = 3
1530    IF A6 = "D" THEN P = 4
1535    IF A6 = "E" THEN P = 5
1540    IF A6 = "X" THEN P = 6
1545    IF B0 = "A" THEN P = P + 2
1550    IF B0 = "R" THEN P = P + 4
1555    IF INT(RND(1)*6)+INT(RND(1)*6)+2 < P THEN P = 1 :RETURN
1560    P = 0 :RETURN
1565 REM  GET REGIONAL UWPS(COL,ROW,RNG)
1570 POKE $9F61,11 :REM MAP
1575 DW = 0
1580 FOR CC = A1 - A3 TO A1 + A3
1585 FOR RR = A2 - A3 TO A2 + A3
1590 NEXT RR
1595 NEXT CC
1600    ? CHR$(147) "SHIP LIST\X11"
1605    POKE $9F61,3  :REM SHIPS
1610    AD = $A000
1615    CT = 0
1620    IF PEEK(AD) = 0 THEN 1735
1625    O% = PEEK(AD+28) :REM OWNER
1630    L = PEEK(AD+3)   :REM COMPONENT LENGTH
1635    T = PEEK(AD+24)  :REM SIZE CODE
1640    M1 = PEEK(AD+22) :REM MISSION CODE 1
1645    IF A9 <> O% GOTO 1725
1650    IF M1 = ASC("E") GOTO 1725 :REM NO ESCORTS
1655    IF M1 = ASC("C") GOTO 1725 :REM NO CRUISERS
1660    ? "     ";
1665    ? CHR$(M1);          :REM MISSION CODE 1
1670    ? CHR$(PEEK(AD+23)); :REM MISSION CODE 2
1675    ? " "; 
1680    FOR X = 4 TO 21 : ? CHR$(PEEK(AD+X)); :NEXT
1685    ? T*100;
1690    IF T<10  THEN ? " ";
1695    ? "TONS";
1700    MC = PEEK(AD+27)
1705    ? "  MCR " MC*T
1710    CT=CT+1
1715    IF CT/4 <> INT(CT/4) GOTO 1725
1720    ? "     -----------------------------------------"
1725    AD = AD + 4 + L :REM NEXT RECORD
1730    GOTO 1620
1735    IF CT = 0 THEN ? "     NO SHIPS AVAILABLE."
1740    RETURN
1745 REM
1750 REM ----------------- INITIALIZE SHIP ------------------
1755 REM
1760 REM   "SUBTYPE" IS DRIVE RATING, WEAPON SUBTYPE, OR FUEL AMOUNT.
1765 REM
1770 REM   COMPONENT 0 IS "THE HULL".
1775 REM   COMPONENT 1 IS STREAMLINING, GENERALLY.
1780 REM   COMPONENT 2 IS THE MANEUVER DRIVE.
1785 REM   COMPONENT 3 IS THE JUMP DRIVE.
1790 REM   COMPONENT 4 IS THE POWER PLANT.
1795 REM   COMPONENT 5 IS FUEL TANKAGE.
1800 REM   COMPONENT 6 AND UP ARE DEFENSES.
1805 REM
1810 DIM B7$(40), B8(40), B9(40)
1815 C0=-1
1820    C0=C0+1
1825    READ B7$(C0)
1830    READ B8(C0)
1835    B9(C0) = 0
1840    IF B7$(C0) <> "-1" GOTO 1820
1845 C0 = C0 - 1
1850 DATA HULL,         	0 
1855 DATA STREAMLINING, 	0 
1860 DATA MANEUVER DRIVE,    1 
1865 DATA JUMP DRIVE,        1 
1870 DATA POWER PLANT,       1 
1875 DATA FUEL TANKAGE,      10
1880 DATA TURRET 1,      	2
1885 DATA TURRET 2,     	2
1890 REM DATA TURRET 1L,   21,   TURRET 2L,  22,  TURRET 3L,  23
1895 REM DATA TURRET 1M,   31,   TURRET 2M,  32,  TURRET 3M,  33
1900 REM DATA TURRET 1S,   41,   TURRET 2S,  42,  TURRET 3S,  43
1905 REM
1910 REM DATA BARBETTE L,  53,   BAY L,      54
1915 REM DATA BARBETTE M,  63,   BAY M,      64
1920 REM DATA BARBETTE S,  73,   BAY S,      74
1925 REM DATA BARBETTE A,  83,   BAY A,      84
1930 DATA -1, -1
1935 RETURN
1940 REM
1945 REM -------------------------- STARPORT -----------------------
1950 REM
1955    COLOR 15,0 :REM WHITE ON BLACK
1960    A0 = 1
1965 REM 
1970 REM THERE IS ONLY ONE EXIT OUT OF THE STARPORT, AND THAT IS "EXIT STARPORT"
1975 REM
1980    ON A0 GOTO 1995, 2095, 2320, 2370, 2425, 2470, 2505, 1990
1985    GOTO 1980
1990    RETURN
1995 ? CHR$(147) "\X11\X11"
2000 BA$ = "  KIGUKI"
2005 GOSUB 2565
2010 ?:? SPC(25) "WELCOME TO ";
2015 GOSUB 1440
2020 ?:?
2025 ?:? SPC(25) "1 - FILE FLIGHT PLAN";
2030 IF A4 > 0 THEN ? " (FILED)"
2035 IF A4 = 0 THEN ?
2040 ?:? SPC(25) "2 - COMMODITY EXCHANGE"
2045 ?:? SPC(25) "3 - SHIPYARD"
2050 ?:? SPC(25) "4 - HIRING HALL"
2055 ?:? SPC(25) "5 - SCOUT BASE"
2060 ?:? SPC(25) "6 - NAVAL BASE"
2065 ?:? SPC(25) "7 - EXIT STARPORT"
2070 ?:?
2075 GET DE$
2080 IF DE$ < "1" OR DE$ > "7" GOTO 2075
2085 A0 = VAL(DE$)+1
2090 GOTO 1980
2095 ? CHR$(147) "\X11\X11"
2100 BA$ = "  GIMGU"
2105 GOSUB 2565
2110 DW=1 :REM DESTINATION WORLD COUNT
2115 FOR CC = A1-2 TO A1+2
2120 FOR RR = A2-2 TO A2+2
2125 IF CC < 1 OR RR < 1 GOTO 2135
2130 IF CC <> A1 OR RR <> A2 THEN GOSUB 2225 
2135 NEXT
2140 NEXT
2145 ?:? SPC(25) "X: RETURN"
2150 ?:?
2155 A4 = 0
2160 GET DE$
2165    IF DE$="X" THEN A0 = 1 :GOTO 1980
2170    IF DE$="" GOTO 2160
2175    FP=ASC(DE$)
2180    FP = FP - 64
2185    IF FP < 1 OR FP > DW GOTO 2160
2190    ? "TARGET" FP "SELECTED."
2195    A4 = DC(FP)
2200    A5 = DR(FP)
2205    INPUT "PRESS <RETURN>"; YN$
2210    A0 = 1
2215 GOTO 1980
2220 REM FETCH UWP DATA(CC,RR)
2225    IF DW = 17 THEN RETURN
2230    REM FIRST POINT TO THE RIGHT BANK
2235    BA = INT((CC-1)/4)
2240    POKE $9F61,11 + BA 
2245    REM NOW CALCULATE THE ACTUAL COLUMN
2250    CA = CC-1
2255    CA = 4*(CA/4-INT(CA/4))
2260    AD = $A000 + CA * 32 * 64 + (RR-1) * 32 * 1
2265    IF PEEK(AD+10) = 32 THEN RETURN
2270    DC(DW) = CC 
2275    DR(DW) = RR
2280    ? SPC(24) CHR$(DW+64) " "; :FOR X = 2 TO 27: ? CHR$(PEEK(AD+X)); :NEXT
2285    IF PEEK(AD+28) = ASC(" ") THEN ? " \X99OK\X9B"
2290    IF PEEK(AD+28) = ASC("A") THEN ? " \X9ECAUTION\X9B"
2295    IF PEEK(AD+28) = ASC("R") THEN ? " \X96DANGER\X9B"
2300    IF DW/4 = INT(DW/4) THEN ? SPC(24);
2305    IF DW/4 = INT(DW/4) THEN ? "\X97....................................\X9B"
2310    DW=DW+1
2315 RETURN
2320 ? CHR$(147) "\X11\X11"
2325 BA$ = "  KAR"          
2330 GOSUB 2565
2335 ?:? SPC(25) "<YOUR CARGO AND VALUE>"
2340 ?:? SPC(25) "<SYSTEM CARGO AND VALUE>"
2345 ?:? SPC(25) "X: RETURN"
2350 GET SE$
2355    IF SE$="X" THEN A0 = 1
2360    IF SE$ < "1" OR SE$ > "X" GOTO 2350
2365 GOTO 1980
2370 ? CHR$(147) "\X11\X11"
2375 BA$ = "  KHISDELIKUR"
2380 GOSUB 2565
2385 AL$ = "I" :GOSUB 1600 :REM SHOW IMPERIAL SHIPS
2390 ?:? SPC(25) "<COMPONENT UPGRADES FOR SALE>"
2395 ?:? SPC(25) "<NEW COMPONENTS FOR SALE>"
2400 ?:? SPC(25) "X: RETURN"
2405 GET SE$
2410    IF SE$="X" THEN A0 = 1
2415    IF SE$ < "1" OR SE$ > "X" GOTO 2405
2420 GOTO 1980
2425 ? CHR$(147) "\X11\X11"
2430 BA$ = "  KHIMEKKI"
2435 GOSUB 2565
2440 ?:? SPC(25) "<CHARACTER LIST>"
2445 ?:? SPC(25) "X: RETURN"
2450 GET SE$
2455    IF SE$="X" THEN A0 = 1
2460    IF SE$ < "1" OR SE$ > "X" GOTO 2450
2465 GOTO 1980
2470 ? CHR$(147) "\X11\X11"
2475 BA$ = "  MEDISHE"
2480 GOSUB 2565
2485 ? SPC(25) "THE SCOUTS BUY YOUR SURVEY FOR CR100,000."
2490 GET A$ : IF A$="" GOTO 2490
2495 A0 = 1
2500 GOTO 1980
2505 ? CHR$(147) "\X11\X11"
2510 BA$ = "  KIISHMU"
2515 GOSUB 2565
2520 ? SPC(25) "THE NAVY BUYS YOUR RECONNAISSANCE DATA FOR CR100,000."
2525 GET A$ : IF A$="" GOTO 2525
2530 A0 = 1
2535 GOTO 1980
2540 HR$ = "\X1C"
2545 FOR X = 1 TO 80 :HR$ = HR$ + "\XC3" :NEXT
2550 HR$ = HR$ + "\X9B"
2555 RETURN
2560 REM BANNER(BA$)
2565 POKE $9F61,4 :REM BANNER CODE
2570 ? HR$;
2575 FOR X = 1 TO LEN(BA$)
2580    A = ASC(MID$(BA$,X,1))
2585    POKE $A000,A
2590    SYS $A001
2595 NEXT X
2600 ?:?:?:? HR$
2605 RETURN
2610 REM DISTANCE(C1,R1,C2,R2) -> D%
2615 A1 = R1 + INT(C1/2)
2620 A2 = R2 + INT(C2/2)
2625 D1% = ABS(A1-A2)
2630 D2% = ABS(C1-C2)
2635 D% = ABS( A1 - A2 - C1 + C2 )
2640 IF (D1% >= D2%) AND (D1% >= D%) THEN D% = D1%
2645 IF (D2% >= D1%) AND (D2% >= D%) THEN D% = D2%
2650 RETURN
2655 REM DICE(DM%) -> 2D6+DM% -> DM%
2660 DM% = DM% + 2 + INT(RND(1)*6) + INT(RND(1)*6)
2665 RETURN
2670 REM FLUX() -> FL%
2675 FL% = 6 * (RND(1)-RND(1))
2680 RETURN
2685 REM EHEX2DEC(HX$) -> N%
2690 N%=0
2695 IF HX$ <= "9" THEN N%=ASC(HX$)-48 :RETURN
2700 IF HX$ >= "P" THEN N%=ASC(HX$)-57 :RETURN
2705 IF HX$ >= "J" THEN N%=ASC(HX$)-56 :RETURN
2710 IF HX$ >= "A" THEN N%=ASC(HX$)-55 :RETURN
2715 RETURN
2720 REM   DECOMPRESS BYTE PAIRS OF UWP-TEXT
2725 REM   PASSED IN AS B0%(), COUNT IN B0%(0)
2730 REM   OUTPUT TO O$
2735 REM   ASSUME B0%() IS INITIALIZED
2740 AL$(0) = " ABCDEFGHIJKLMNOPQRSTUVWXYZ?:'-"
2745 AL$(1) = " 0123456789ABCDEFGHILMNORSTUWY-"
2750 O$ = "": V=0: FOR BB = 1 TO B0%(0) STEP 2
2755 B0% = B0%(BB)
2760 B1% = B0%(BB+1)
2765 REM ? BB, B0%, B1%
2770 B2% = B0% AND 31
2775 B3% = (B0%/32) + (B1% AND 3)*8
2780 B4% = (B1% AND 127) / 4
2785 REM TRANSLATE
2790 B% = B2% :GOSUB 2815 :REM DECODE
2795 B% = B3% :GOSUB 2815 :REM DECODE
2800 B% = B4% :GOSUB 2815 :REM DECODE
2805 NEXT
2810 RETURN
2815 IF B% = 31 THEN V = 1 - V :RETURN
2820 REM ? "INDEX: " B%+1 " LETTER: " MID$(AL$(V),B%+1,1)
2825 O$ = O$ + MID$(AL$(V),B%+1,1)
2830 RETURN
