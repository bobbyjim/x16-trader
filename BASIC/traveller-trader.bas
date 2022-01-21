5 COLOR 15,0 :?CHR$(147)	:REM LT GREY ON BLACK
10 DIM BU$(30) :REM LINE BUFFER
15 DIM DX(36),DY(36),DC(36),DR(36) :REM DESTINATION VECTORS
20 LOAD "MARKET.BIN"        ,8, 2,$A000 :REM BANK 2
25 LOAD "ALLSHIPS.BIN"      ,8, 3,$A000 :REM BANK 3
30 LOAD "BANNER.BIN"        ,8, 4,$A000 :REM BANK 4
35 LOAD "SPINWARD.SPACE.BIN",8,11,$A000 :REM BANK 11 AND UP
40 GOSUB 2915
45 GOSUB 1320
50 GOSUB 2185
55 GOSUB 105
60 C6 = 100 :REM KCR
65 A1 = 19         :REM COL
70 A2 = 10         :REM ROW
75 A3 = 4        :REM RANGE
80 A5 = 0    :REM DEST.AROW
85 A4 = 0    :REM DEST.ACOL
90    GOSUB 2330
95    GOSUB 1385
100    GOTO 90
105 DIM C8$(10), C9(10), D0(10), D1(10)
110 C7 = 4
115 C8$(1)="REGINA" :C9(1)=1 :D0(1)=12 :D1(1)=5
120 C8$(2)="RHYLANOR" :C9(2)=2 :D0(2)=15 :D1(2)=5
125 C8$(3)="DJINNI" :C9(3)=3 :D0(3)=10 :D1(3)=5
130 C8$(4)="MACENE" :C9(4)=4 :D0(4)=13 :D1(4)=5
135 RETURN
140 ?:?
145 FOR X=1 TO C7
150 ? SPC(8), C8$(X), C9(X), D0(X), D1(X)
155 NEXT X
160 ?:INPUT "PRESS <RETURN>"; YN$
165 RETURN
170 REM ------------------- INITIALIZE ENEMY ------------------
175 B9 = INT(RND(1)*5)+INT(RND(1)*4)+2 :REM 200 TO 900 TONS
180 C0 = 0
185 RETURN
190 REM ---------------------- COMBAT -----------------------
195 REM
200 REM  THE MAJOR OUTCOME OF COMBAT IS SURVIVAL OR DESTRUCTION.
205 REM
210 REM  DESTRUCTION IS THE LOSS OF THE SHIP, AND PERHAPS THE
215 REM  LIVES OF THE CREW.
220 REM  
225 REM  SURVIVAL IS ENUMERATED AS:
230 REM
235 REM   2-9 - YOU'VE WON, AND CAPTURED A 200-900 TON CORSAIR
240 REM   1   - YOU'VE RUN VIA A SUCCESSFUL EMERGENCY JUMP
245 REM   0   - THE ENEMY IS REPELLED AND RUNS AWAY
250 REM  -1   - THE ENEMY PLUNDERS YOUR CARGO AND VEHICLES
255 REM
260 B5 = 1
265 B6 = 30
270 B7 = 30
275 B8 = 30
280 C5 = 0
285 GOSUB 175
290 ? HR$ "  SENSOP'S REPORT" :? HR$;
295 ? "  WE ARE FACING A" B9 * 100 "TON CORSAIR\X11"
300 GOSUB 365
305 IF C5 = -1 THEN GOSUB 320
310 IF C5 > 1  THEN GOSUB 325
315 RETURN
320 RETURN
325 ? HG$ "  INSYSTEM SALVAGE REPORT" :?HG$;
330 S = C5 + INT(RND(1)*C5)
335 ?
340 ? "   THANK YOU FOR DEFEATING THIS SCUM. AS A REWARD, YOUR SHARE"
345 ? "   OF THE CORSAIR'S SALVAGE VALUE COMES TO MCR" C5 "."
350 C6 = C6 + S * 1000
355 RETURN
360 REM --------------------- COMBAT LOOP ------------------
365    ? HR$ "  HELM REPORT, ROUND"; B5: ? HR$;
370    ? "  CAPTAIN, SHALL WE:\X11"
375    ? "    1 - ALL-OUT ATTACK!"
380    ? "    2 - DEPLOY REPAIR DETAIL!"   
385    ? "    3 - DEPLOY MEDICAL DETAIL!"
390    ? "    4 - MAKE AN EMERGENCY JUMP!"
395    ? "    5 - SIGNAL SURRENDER!\X11"
400    ? "  YOUR COMMAND?"
405    GET CM$ :CM = VAL(CM$)
410    IF CM < 1 OR CM > 5 GOTO 405
415    ? CHR$(147);
420    B6 = 30 :B7 = 30 :B8 = 30
425    IF CM = 1 THEN B6 = 60
430    IF CM = 2 THEN B7 = 60
435    IF CM = 3 THEN B8 = 60
440    IF CM = 4 THEN GOSUB 1070 :IF C5=1 THEN RETURN
445    IF CM = 5 GOTO 1190
450    GOSUB 500  :INPUT "   PRESS <RETURN>"; YN$
455    GOSUB 800  :INPUT "   PRESS <RETURN>"; YN$
460    GOSUB 920 :INPUT "   PRESS <RETURN>"; YN$
465    GOSUB 710 :IF C5=1 THEN RETURN
470    GOSUB 645 :IF WW=1 THEN RETURN
475    B5 = B5 + 1
480 GOTO 365
485 REM
490 REM -------------------- BATTLE ACTION -----------------
495 REM
500 ? HS$ :? "  BATTLE REPORT":?HS$
505 FOR X = 6 TO C4
510    Q=0
515    FOR P = 1 TO C3(X)
520       Q = Q + INT(RND(1)*6)
525    NEXT
530    IF Q > 8 GOTO 545
535    D = INT(RND(1)*B6)
540    C0 = C0 + D
545 NEXT
550 ? "   TURRETS INFLICT" D "DAMAGE."
555 TD = C0 / (B9 * 100)
560 IF TD > 0.25 THEN ?:? "   THE CORSAIR'S HULL IS SHOWING SIGNS OF DAMAGE."
565 IF TD > 0.5  THEN ?:? "   FIRE IS DETECTED IN THE CORSAIR."
570 IF TD > 0.75 THEN ?:? "   THE CORSAIR IS SHOWING SERIOUS DAMAGE."
575 ?
580 FOR X = 1 TO B9
585    IF INT(RND(1)*6)+INT(RND(1)*6)+2 > 6 GOTO 615
590    T = INT(RND(1)*C4)
595    C3(T) = C3(T)+1
600    IF INT(RND(1)*6)+INT(RND(1)*6)+2 > 6 GOTO 615
605    C = INT(RND(1)*B3)
610    B2(C) = B2(C) + 1
615 NEXT 
620 ?
625 RETURN
630 REM
635 REM --------------- CHECK ENEMY STATUS --------------
640 REM
645 WW = 0
650 IF C0 < B9 * 100 THEN RETURN
655 WW = 1
660 IF C0 - B9 * 100 > 100 GOTO 675
665 ? HR$ "  SENSOP'S REPORT" :? HR$;
670 ? "   ZE ENEMY RUNS!  VE HAVE VON!!" :RETURN
675 ? HR$ "  HELM REPORT" :?HR$
680 ? "   THE ENEMY SURRENDERS!"
685 ? "   WE HAVE CLAIMED SALVAGE RIGHTS TO THEIR" B9*100 "TON CORSAIR!"
690 RETURN
695 REM
700 REM --------------- CHECK POWERPLANT STATUS --------------
705 REM
710 IF C3(4) = C2(4)+1 GOTO 765
715 IF C3(4) <= C2(4)+1 THEN RETURN
720 IF B2(2) > 1 GOTO 745
725 ? HR$ "  ENGINEER'S REPORT" :?HR$;
730 ? "\X11\X9E  ACH CAPTAIN, THE POWERRR PLANT'S A COMPLETE WRRRECK!"
735 ? "  WE'RE DEAD IN TH' WATERRR!\X9B\X11"
740 GOTO 1205
745 ? HR$ "  HELM REPORT" :?HR$;
750 ? "\X11\X93  CAPTAIN, THE ENGINES AREN'T RESPONDING. ";
755 ? "THE SHIP APPEARS TO BE DEAD.\X9B\X11"
760 GOTO 1205
765 ? HR$ "  *** ENGINEER'S WARNING ***" :?HR$;
770 ? "\X11\X9E  ACH CAPTAIN, THE POWERRR PLANT'S A MESS."
775 ? "  ONE MORE HIT LIKE THAT AN' WE'RRRE FINISHED!\X9B\X11"
780 RETURN
785 REM
790 REM -------------------- REPAIR ACTION -----------------
795 REM
800 ? HS$: ? "  DAMAGE CONTROL REPORT" :?HS$
805 IF B2(2)>1 THEN?"  THE ENGINEER IS HURT; NO REPORT AVAILABLE.\X11":RETURN
810 ? "", "COMPONENT",, "STATUS"
815 ? HW$
820 FOR X = 0 TO C4
825    IF C3(X) > 0 AND INT(RND(1)*B7) > 19 THEN C3(X) = C3(X) - 1
830    IF C3(X) > 0 AND INT(RND(1)*B7) > 29 THEN C3(X) = C3(X) - 1
835    IF C3(X) > 0 AND INT(RND(1)*B7) > 39 THEN C3(X) = C3(X) - 1
840    IF X = 5 THEN ? DL$;
845    ? "   ",
850    ? LEFT$(C1$(X)+"             ",16),
855    A = C2(X)
860    B = C3(X)
865    IF B>0 AND B>A AND X<6 THEN ? "   " + CHR$(28);
870    IF B>0 AND B>A AND X<6 THEN ? "OUT OF ACTION" + CHR$(155)
875    IF B>0 AND (B<=A OR X>5) THEN ? "   " + CHR$(158);
880    IF B>0 AND (B<=A OR X>5) THEN ? "DAMAGED" + CHR$(155)
885    IF B=0 THEN ? "   " + CHR$(153) + "OK" + CHR$(155)
890 NEXT
895 ?
900 RETURN
905 REM
910 REM -------------------- MEDICAL ACTION -----------------
915 REM
920 ? HS$: "  MEDICAL REPORT" :?HS$
925 IF B2(3)>1 THEN?"  THE MEDIC IS HURT; NO REPORT AVAILABLE.\X11":RETURN
930 ? "", "CREWMEMBER", "ROLE      ", "WOUNDS"
935 ? HS$
940 FOR X = 0 TO B3
945    IF B2(X) > 2 GOTO 965
950    IF B2(X) > 0 AND INT(RND(1)*B8) > 19 THEN B2(X)=B2(X)-1
955    IF B2(X) > 0 AND INT(RND(1)*B8) > 29 THEN B2(X)=B2(X)-1
960    IF B2(X) > 0 AND INT(RND(1)*B8) > 39 THEN B2(X)=B2(X)-1
965    IF X = 4 THEN ? DL$;
970    ? "   ",
975    ? LEFT$(B1$(X)+"            ",16),
980    R$ = "          "
985    IF X = 0 THEN R$ = "PILOT     "
990    IF X = 1 THEN R$ = "ASTROGATOR"
995    IF X = 2 THEN R$ = "ENGINEER  "
1000    IF X = 3 THEN R$ = "MEDIC     "
1005    IF X = 4 THEN R$ = "STEWARD   "
1010    ? R$,
1015    W$ = CHR$(28) + "DEAD" + CHR$(155)
1020    IF B2(X) = 0 THEN W$ = CHR$(153) + "OK" + CHR$(155)
1025    IF B2(X) = 1 THEN W$ = CHR$(158) + "WOUNDED" + CHR$(155)
1030    IF B2(X) = 2 THEN W$ = CHR$(28) + "OUT OF ACTION" + CHR$(155)
1035    ? W$
1040 NEXT
1045 ?
1050 RETURN
1055 REM
1060 REM ------------------ EMERGENCY JUMP --------------------
1065 REM
1070 IF B2(1) > 0 GOTO 1110
1075 IF B2(2) > 0 GOTO 1125
1080 IF C3(3) >= C2(3) GOTO 1155
1085 IF INT(RND(1)*6)+INT(RND(1)*6)+2 > 6 GOTO 1140
1090 ? "   AYE, YOU'VE GOT IT, CAPTAIN!"
1095 FOR T = 1 TO 1000 : TT = SQR(12345) :NEXT
1100 ? "   CAPTAIN, WE'VE SUCCESSFULLY JUMPED TO SAFETY!"
1105 C5=1 :RETURN
1110 ? HS$: ? "   HELM REPORT" :? HS$
1115 ? "   OUR ASTROGATOR IS INCAPACITATED, AND CANNOT CALCULATE A JUMP.\X11"
1120 C5=0 :RETURN
1125 ? HS$: ? "   HELM REPORT" :? HS$
1130 ? "   OUR ENGINEER IS INCAPACITATED, AND CANNOT OPERATE THE JUMP DRIVE.\X11"
1135 C5=0 :RETURN
1140 ? HS$:? "   ENGINEER'S REPORT" :? HS$
1145 ? "   ACH, WE KENNA JUMP YET CAPTAIN, GIMME 20 MORRRE MINUTES!\X11"
1150 C5=0 :RETURN
1155 ? HS$: ?"   ENGINEER'S REPORT" :? HS$
1160 ? "   ACH, THE JUMP DRIVES ARRRE DOON, CAPTAIN!"
1165 ? "   WE NE KENNA JUMP UNTIL THEY'RRRE REPAIRRRED!\X11"
1170 C5=0 :RETURN
1175 REM
1180 REM --------------------- SURRENDER ----------------------
1185 REM
1190 ? HS$:? "  HELM REPORT": ?HS$
1195 ? "  SURRENDER ACKNOWLEDGED. CREW IS ARMING THEMSELVES.\X11"
1200 FOR T = 1 TO 1000: TT=SQR(12345) :NEXT
1205 ? HS$:? "  HELM REPORT": ?HS$
1210 ? "  CAPTAIN, WE HAVE BEEN BOARDED."
1215 FOR T = 1 TO 1000: TT=SQR(12345) :NEXT
1220 ? "  FIGHTING REPORTED IN ALL AREAS."
1225 FOR T = 1 TO 1000: TT=SQR(12345) :NEXT
1230 C5 = -1
1235 RR = INT(RND(1)*6)+INT(RND(1)*6)+2
1240 IF RR < 6 THEN ? "  WE HAVE REPELLED THE PIRATES!!" :C5 = 1 :RETURN
1245 IF RR < 8 THEN ? "  OUR CARGO AND VEHICLES HAVE BEEN TAKEN." :RETURN
1250 IF RR < 10 THEN ? "  WE HAVE BEEN STRANDED ON A MARGINAL WORLD." :END
1255 ? "  THE CREW HAS BEEN SOLD INTO SLAVERY." :END
1260 REM 
1265 REM ---------------- INITIALIZE CREW ----------------
1270 REM
1275 REM  CREW ROLES ARE INDIVIDUALIZED AS FOLLOWS:
1280 REM
1285 REM    0 - PILOT
1290 REM    1 - ASTROGATOR
1295 REM    2 - ENGINEER
1300 REM    3 - MEDIC
1305 REM    4 - STEWARD/FREIGHTMASTER
1310 REM    5 AND UP - GUNNERS
1315 REM
1320 DIM B1$(24), B2(24), B4(24)
1325 B3 = -1
1330    B3 = B3+1
1335    READ B1$(B3)
1340    B2(B3) = 0
1345    B4 = B3
1350    IF B1$(B3) <> "-1" GOTO 1330
1355 B3 = B3 - 1
1360 DATA JAMISON, AKRIND, KODRAY, ARRLANROUGHL, REYNALDI, SHARIK, -1
1365 RETURN
1370 REM
1375 REM -------------------------- SYSTEM -----------------------
1380 REM
1385    COLOR 15,0 :REM WHITE ON BLACK
1390    A0 = 1
1395 REM
1400 REM  THERE IS ONLY ONE EXIT OUT OF THE SYSTEM, AND THAT IS "STARPORT"
1405 REM
1410    ON A0 GOTO 1420, 1495, 1500, 1590, 1670
1415    GOTO 1410
1420 ? CHR$(147) "\X11\X11"
1425 BA$ = "      INSYSTEM"
1430 GOSUB 2950
1435 ?:? SPC(25) "YOU ARE IN ORBIT AROUND ";
1440 GOSUB 1815
1445 ?:?
1450 ?:? SPC(25) "1 - LAND AT STARPORT"
1455 ?:? SPC(25) "2 - WILDERNESS REFUELING"
1460 ?:? SPC(25) "3 - SYSTEM SURVEY"
1465 ?:? SPC(25) "4 - JUMP"
1470 ?:?
1475 GET DE$
1480 IF DE$ < "1" OR DE$ > "4" GOTO 1475
1485 A0 = VAL(DE$)+1
1490 GOTO 1410
1495 RETURN
1500 ? CHR$(147) "\X11\X11"
1505 BA$ = "      REFUEL"
1510 GOSUB 2950
1515 ?:? SPC(25) "I: ICE REFUELING"
1520 ?:? SPC(25) "G: GAS GIANT REFUELING"
1525 ?:? SPC(25) "W: WILDERNESS REFUEL"
1530 ?:? SPC(25) "X: RETURN"
1535 ?:?
1540 GET DE$
1545    IF DE$="X" THEN A0 = 1
1550    IF DE$ < "1" OR DE$ > "X" GOTO 1540
1555 ?:? "REFUELING";
1560 FORX=0TO7
1565    ? ".";
1570    FORZ=0TO199:Y=SQR(X):NEXT
1575 NEXT
1580 ? "DONE."
1585 GOTO 1410
1590 ? CHR$(147) "\X11\X11"
1595 BA$ = "      SURVEY"
1600 GOSUB 2950
1605 ?:? SPC(25) "<SURVEY OPTIONS>"
1610 ?:? SPC(25) "X: RETURN"
1615 ?:?
1620 GET DE$
1625    IF DE$="X" THEN A0 = 1
1630    IF DE$ < "1" OR DE$ > "X" GOTO 1620
1635 ?:? "SURVEYING";
1640 FORX=0TO7
1645    ? ".";
1650    FORZ=0TO199:Y=SQR(X):NEXT
1655 NEXT
1660 ? "DONE."
1665 GOTO 1410
1670 ? CHR$(147) "\X11\X11"
1675 BA$ = "      JUMP"
1680 GOSUB 2950
1685 ? "   INITIATING JUMP PROGRAM."
1690 FORX=0TO499:Y=SQR(X):NEXT
1695 ? "   JUMP DRIVES CHARGING."
1700 FORX=0TO499:Y=SQR(X):NEXT
1705 ? "   ENTERING JUMPSPACE."
1710 FORX=0TO499:Y=SQR(X):NEXT
1715 FORY=1TO6
1720    ? "      DAY" Y
1725    FORX=0TO999:Z=SQR(X):NEXT
1730 NEXT
1735 ? "      DAY 7: EXIT PRECIPITATION."
1740 FORX=0TO499:Y=SQR(X):NEXT
1745 ? "   SCANNING SYSTEM."
1750 FORX=0TO499:Y=SQR(X):NEXT
1755 A1 = A4 
1760 A2 = A5
1765 A4 = 0 :REM UNSET FLIGHT PLAN
1770 A5 = 0
1775 ?:? SPC(25) "WELCOME TO ";
1780 GOSUB 1815
1785 ?:?
1790 IF P = 1 THEN P = 0 :GOSUB 260
1795 INPUT "PRESS <RETURN>"; YN$
1800 A0 = 1
1805 GOTO 1410
1810 REM MAP.WHERE AM I(COL,ROW)
1815    REM FIRST POINT TO THE RIGHT BANK
1820    BA = INT((A1-1)/4)
1825    POKE $9F61,11+BA
1830    REM NOW CALCULATE THE ACTUAL COLUMN
1835    CA = A1 - 1
1840    CA = 4*(CA/4-INT(CA/4))
1845    AD = $A000 + CA * 32 * 64 + (A2-1) * 32 * 1
1850    FOR X = 10 TO 24: ? CHR$(PEEK(AD+X)); :NEXT
1855    A6 = PEEK(AD+7)
1860    A7 = PEEK(AD+8)
1865    A8 = PEEK(AD+26)
1870    A9 = PEEK(AD+27)
1875    B0 = PEEK(AD+28)
1880    REM CHECK FOR PIRATES
1885    P = 3
1890    IF A6 = 65 THEN P = 1
1895    IF A6 = 66 THEN P = 2
1900    IF A6 = 67 THEN P = 3
1905    IF A6 = 68 THEN P = 4
1910    IF A6 = 69 THEN P = 5
1915    IF A6 = 88 THEN P = 6
1920    IF B0 = 65 THEN P = P + 2
1925    IF B0 = 82 THEN P = P + 4
1930    IF INT(RND(1)*6)+INT(RND(1)*6)+2 < P THEN P = 1 :RETURN
1935    P = 0 :RETURN
1940 REM  GET REGIONAL UWPS(COL,ROW,RNG)
1945 POKE $9F61,11 :REM MAP
1950 DW = 0
1955 FOR CC = A1 - A3 TO A1 + A3
1960 FOR RR = A2 - A3 TO A2 + A3
1965 NEXT RR
1970 NEXT CC
1975    ? CHR$(147) "SHIP LIST\X11"
1980    POKE $9F61,3  :REM SHIPS
1985    AD = $A000
1990    CT = 0
1995    IF PEEK(AD) = 0 THEN 2110
2000    O% = PEEK(AD+28) :REM OWNER
2005    L = PEEK(AD+3)   :REM COMPONENT LENGTH
2010    T = PEEK(AD+24)  :REM SIZE CODE
2015    M1 = PEEK(AD+22) :REM MISSION CODE 1
2020    IF A9 <> O% GOTO 2100
2025    IF M1 = ASC("E") GOTO 2100 :REM NO ESCORTS
2030    IF M1 = ASC("C") GOTO 2100 :REM NO CRUISERS
2035    ? "     ";
2040    ? CHR$(M1);          :REM MISSION CODE 1
2045    ? CHR$(PEEK(AD+23)); :REM MISSION CODE 2
2050    ? " "; 
2055    FOR X = 4 TO 21 : ? CHR$(PEEK(AD+X)); :NEXT
2060    ? T*100;
2065    IF T<10  THEN ? " ";
2070    ? "TONS";
2075    MC = PEEK(AD+27)
2080    ? "  MCR " MC*T
2085    CT=CT+1
2090    IF CT/4 <> INT(CT/4) GOTO 2100
2095    ? "     -----------------------------------------"
2100    AD = AD + 4 + L :REM NEXT RECORD
2105    GOTO 1995
2110    IF CT = 0 THEN ? "     NO SHIPS AVAILABLE."
2115    RETURN
2120 REM
2125 REM ----------------- INITIALIZE SHIP ------------------
2130 REM
2135 REM   "SUBTYPE" IS DRIVE RATING, WEAPON SUBTYPE, OR FUEL AMOUNT.
2140 REM
2145 REM   COMPONENT 0 IS "THE HULL".
2150 REM   COMPONENT 1 IS STREAMLINING, GENERALLY.
2155 REM   COMPONENT 2 IS THE MANEUVER DRIVE.
2160 REM   COMPONENT 3 IS THE JUMP DRIVE.
2165 REM   COMPONENT 4 IS THE POWER PLANT.
2170 REM   COMPONENT 5 IS FUEL TANKAGE.
2175 REM   COMPONENT 6 AND UP ARE DEFENSES.
2180 REM
2185 DIM C1$(40), C2(40), C3(40)
2190 C4=-1
2195    C4=C4+1
2200    READ C1$(C4)
2205    READ C2(C4)
2210    C3(C4) = 0
2215    IF C1$(C4) <> "-1" GOTO 2195
2220 C4 = C4 - 1
2225 DATA HULL,         	0 
2230 DATA STREAMLINING, 	0 
2235 DATA MANEUVER DRIVE,    1 
2240 DATA JUMP DRIVE,        1 
2245 DATA POWER PLANT,       1 
2250 DATA FUEL TANKAGE,      10
2255 DATA TURRET 1,      	2
2260 DATA TURRET 2,     	2
2265 REM DATA TURRET 1L,   21,   TURRET 2L,  22,  TURRET 3L,  23
2270 REM DATA TURRET 1M,   31,   TURRET 2M,  32,  TURRET 3M,  33
2275 REM DATA TURRET 1S,   41,   TURRET 2S,  42,  TURRET 3S,  43
2280 REM
2285 REM DATA BARBETTE L,  53,   BAY L,      54
2290 REM DATA BARBETTE M,  63,   BAY M,      64
2295 REM DATA BARBETTE S,  73,   BAY S,      74
2300 REM DATA BARBETTE A,  83,   BAY A,      84
2305 DATA -1, -1
2310 RETURN
2315 REM
2320 REM -------------------------- STARPORT -----------------------
2325 REM
2330    COLOR 15,0 :REM WHITE ON BLACK
2335    A0 = 1
2340 REM 
2345 REM THERE IS ONLY ONE EXIT OUT OF THE STARPORT, AND THAT IS "EXIT STARPORT"
2350 REM
2355    ON A0 GOTO 2370, 2470, 2695, 2745, 2800, 2845, 2880, 2365
2360    GOTO 2355
2365    RETURN
2370 ? CHR$(147) "\X11\X11"
2375 BA$ = "  CONCOURSE"
2380 GOSUB 2950
2385 ?:? SPC(25) "WELCOME TO ";
2390 GOSUB 1815
2395 ?:?
2400 ?:? SPC(25) "1 - FILE FLIGHT PLAN";
2405 IF A4 > 0 THEN ? " (FILED)"
2410 IF A4 = 0 THEN ?
2415 ?:? SPC(25) "2 - COMMODITY EXCHANGE"
2420 ?:? SPC(25) "3 - SHIPYARD"
2425 ?:? SPC(25) "4 - HIRING HALL"
2430 ?:? SPC(25) "5 - SCOUT BASE"
2435 ?:? SPC(25) "6 - NAVAL BASE"
2440 ?:? SPC(25) "7 - EXIT STARPORT"
2445 ?:?
2450 GET DE$
2455 IF DE$ < "1" OR DE$ > "7" GOTO 2450
2460 A0 = VAL(DE$)+1
2465 GOTO 2355
2470 ? CHR$(147) "\X11\X11"
2475 BA$ = "  FLIGHT PLAN"
2480 GOSUB 2950
2485 DW=1 :REM DESTINATION WORLD COUNT
2490 FOR CC = A1-A3 TO A1+A3
2495 FOR RR = A2-A3 TO A2+A3
2500 IF CC < 1 OR RR < 1 GOTO 2510
2505 IF CC <> A1 OR RR <> A2 THEN GOSUB 2600 
2510 NEXT
2515 NEXT
2520 ?:? SPC(25) "X: RETURN"
2525 ?:?
2530 A4 = 0
2535 GET DE$
2540    IF DE$="X" THEN A0 = 1 :GOTO 2355
2545    IF DE$="" GOTO 2535
2550    FP=ASC(DE$)
2555    FP = FP - 47
2560    IF FP < 1 OR FP > DW GOTO 2535
2565    ? "TARGET" FP "SELECTED."
2570    A4 = DC(FP)
2575    A5 = DR(FP)
2580    INPUT "PRESS <RETURN>"; YN$
2585    A0 = 1
2590 GOTO 2355
2595 REM FETCH UWP DATA(CC,RR)
2600    IF DW = 37 THEN RETURN
2605    REM FIRST POINT TO THE RIGHT BANK
2610    BA = INT((CC-1)/4)
2615    POKE $9F61,11 + BA 
2620    REM NOW CALCULATE THE ACTUAL COLUMN
2625    CA = CC-1
2630    CA = 4*(CA/4-INT(CA/4))
2635    AD = $A000 + CA * 32 * 64 + (RR-1) * 32 * 1
2640    IF PEEK(AD+10) = 32 THEN RETURN
2645    DC(DW) = CC 
2650    DR(DW) = RR
2655    ? SPC(24) CHR$(DW+47) " "; :FOR X = 2 TO 27: ? CHR$(PEEK(AD+X)); :NEXT
2660    IF PEEK(AD+28) = ASC(" ") THEN ? " \X99OK\X9B"
2665    IF PEEK(AD+28) = ASC("A") THEN ? " \X9ECAUTION\X9B"
2670    IF PEEK(AD+28) = ASC("R") THEN ? " \X96DANGER\X9B"
2675    IF DW/4 = INT(DW/4) THEN ? SPC(24);
2680    IF DW/4 = INT(DW/4) THEN ? "\X97....................................\X9B"
2685    DW=DW+1
2690 RETURN
2695 ? CHR$(147) "\X11\X11"
2700 BA$ = "  MARKET"          
2705 GOSUB 2950
2710 GOSUB 140
2715 ?:? SPC(25) "<SYSTEM CARGO AND VALUE>"
2720 ?:? SPC(25) "X: RETURN"
2725 GET SE$
2730    IF SE$="X" THEN A0 = 1
2735    IF SE$ < "1" OR SE$ > "X" GOTO 2725
2740 GOTO 2355
2745 ? CHR$(147) "\X11\X11"
2750 BA$ = "  SHIPYARD"
2755 GOSUB 2950
2760 AL$ = "I" :GOSUB 1975 :REM SHOW IMPERIAL SHIPS
2765 ?:? SPC(25) "<COMPONENT UPGRADES FOR SALE>"
2770 ?:? SPC(25) "<NEW COMPONENTS FOR SALE>"
2775 ?:? SPC(25) "X: RETURN"
2780 GET SE$
2785    IF SE$="X" THEN A0 = 1
2790    IF SE$ < "1" OR SE$ > "X" GOTO 2780
2795 GOTO 2355
2800 ? CHR$(147) "\X11\X11"
2805 BA$ = "  HIRING HALL"
2810 GOSUB 2950
2815 ?:? SPC(25) "<CHARACTER LIST>"
2820 ?:? SPC(25) "X: RETURN"
2825 GET SE$
2830    IF SE$="X" THEN A0 = 1
2835    IF SE$ < "1" OR SE$ > "X" GOTO 2825
2840 GOTO 2355
2845 ? CHR$(147) "\X11\X11"
2850 BA$ = "  SCOUT BASE"
2855 GOSUB 2950
2860 ? SPC(25) "THE SCOUTS BUY YOUR SURVEY FOR CR100,000."
2865 GET A$ : IF A$="" GOTO 2865
2870 A0 = 1
2875 GOTO 2355
2880 ? CHR$(147) "\X11\X11"
2885 BA$ = "  NAVY BASE"
2890 GOSUB 2950
2895 ? SPC(25) "THE NAVY BUYS YOUR RECONNAISSANCE DATA FOR CR100,000."
2900 GET A$ : IF A$="" GOTO 2900
2905 A0 = 1
2910 GOTO 2355
2915 HH$ = ""
2920 FOR X = 1 TO 40 :HH$ = HH$ + "\XC3" :NEXT
2925 HG$ = "\X99" + HH$ + HH$ + "\X9B"
2930 HR$ = "\X1C" + HH$ + HH$ + "\X9B"
2935 HS$ = "\X1C" + HH$ + "\X9B"
2940 RETURN
2945 REM BANNER(BA$)
2950 POKE $9F61,4 :REM BANNER CODE
2955 ? HR$;
2960 FOR X = 1 TO LEN(BA$)
2965    A = ASC(MID$(BA$,X,1))
2970    POKE $A000,A
2975    SYS $A001
2980 NEXT X
2985 ?:?:?:? HR$
2990 RETURN
2995 REM DISTANCE(C1,R1,C2,R2) -> D%
3000 A1 = R1 + INT(C1/2)
3005 A2 = R2 + INT(C2/2)
3010 D1% = ABS(A1-A2)
3015 D2% = ABS(C1-C2)
3020 D% = ABS( A1 - A2 - C1 + C2 )
3025 IF (D1% >= D2%) AND (D1% >= D%) THEN D% = D1%
3030 IF (D2% >= D1%) AND (D2% >= D%) THEN D% = D2%
3035 RETURN
3040 REM DICE(DM%) -> 2D6+DM% -> DM%
3045 DM% = DM% + 2 + INT(RND(1)*6) + INT(RND(1)*6)
3050 RETURN
3055 REM FLUX() -> FL%
3060 FL% = 6 * (RND(1)-RND(1))
3065 RETURN
3070 REM EHEX2DEC(HX$) -> N%
3075 N%=0
3080 IF HX$ <= "9" THEN N%=ASC(HX$)-48 :RETURN
3085 IF HX$ >= "P" THEN N%=ASC(HX$)-57 :RETURN
3090 IF HX$ >= "J" THEN N%=ASC(HX$)-56 :RETURN
3095 IF HX$ >= "A" THEN N%=ASC(HX$)-55 :RETURN
3100 RETURN
3105 REM   DECOMPRESS BYTE PAIRS OF UWP-TEXT
3110 REM   PASSED IN AS B0%(), COUNT IN B0%(0)
3115 REM   OUTPUT TO O$
3120 REM   ASSUME B0%() IS INITIALIZED
3125 AL$(0) = " ABCDEFGHIJKLMNOPQRSTUVWXYZ?:'-"
3130 AL$(1) = " 0123456789ABCDEFGHILMNORSTUWY-"
3135 O$ = "": V=0: FOR BB = 1 TO B0%(0) STEP 2
3140 B0% = B0%(BB)
3145 B1% = B0%(BB+1)
3150 REM ? BB, B0%, B1%
3155 B2% = B0% AND 31
3160 B3% = (B0%/32) + (B1% AND 3)*8
3165 B4% = (B1% AND 127) / 4
3170 REM TRANSLATE
3175 B% = B2% :GOSUB 3200 :REM DECODE
3180 B% = B3% :GOSUB 3200 :REM DECODE
3185 B% = B4% :GOSUB 3200 :REM DECODE
3190 NEXT
3195 RETURN
3200 IF B% = 31 THEN V = 1 - V :RETURN
3205 REM ? "INDEX: " B%+1 " LETTER: " MID$(AL$(V),B%+1,1)
3210 O$ = O$ + MID$(AL$(V),B%+1,1)
3215 RETURN
