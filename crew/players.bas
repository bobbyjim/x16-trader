5 A9 = 10
10 DIM A8$(A9), A8(A9), A0(A9), A1(A9), A2(A9), CH(A9,5)
15 DIM A1$(25), A7(15), A7$(15), HX$(33)
20 A0$(0) = "DEAD"
25 A0$(1) = "ALIVE"
30 FOR X = 0 TO 25 :READ A1$(X) :NEXT
35 DATA ASLAN, BWAP, CASSILDAN, DROYNE, EBOKIN, FLORIAN
40 DATA GEONEE, HUMAN, HIVER, JONKEEREEN, CLONE
45 DATA LLELLEWYLOLLY, AMINDII, VILANI, SOLOMANI
50 DATA SYNTHETIC, CETIAN, DARRIAN, SUERRAT, TESSLAM
55 DATA VIRUSHI, VARGR, KURSAE
60 DATA XARUAH, YILEAN, ZHODANI
65 A3$(0) = "A"
70 A3$(1) = "G"
75 A3$(2) = "L"
80 A3$(3) = "M"
85 A3$(4) = "N"
90 A3$(5) = "O"
95 A3$(6) = "R"
100 A3$(7) = "S"
105 A4$(0) = "SOLDIER"
110 A4$(1) = "AGENT"
115 A4$(2) = "SCHOLAR"
120 A4$(3) = "MERCHANT"
125 A4$(4) = "SPACER"
130 A4$(5) = "ROGUE"
135 A4$(6) = "MARINE"
140 A4$(7) = "SCOUT"
145 A6$(0) = "E1"
150 A6$(1) = "E2"
155 A6$(2) = "E3"
160 A6$(3) = "E4"
165 A6$(4) = "O1"
170 A6$(5) = "O2"
175 A6$(6) = "O3"
180 A6$(7) = "O4"
185 FOR X = 0 TO 15 :READ A7$(X) :NEXT
190 DATA ADMIN, ASTROGATOR, DIPLOMAT, ENGINEER
195 DATA FIGHTER, GUNNER, HOSTILE ENV, MEDIC
200 DATA PILOT, RECON, SENSOP, STREETWISE
205 DATA SURVEY, SURVIVAL, TRADER, VACC
210 FOR X = 0 TO 33 :READ HX$(X) :NEXT
215 DATA 0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F,G,H,J,K,L,M,N,P,Q,R,S,T,U,V,W,X,Y,Z
220 INPUT "PRESS A KEY TO BEGIN"; K$
225 X=RND(-TI)
230 FOR SE = 0 TO A9
235    GOSUB 605
240 NEXT
245 REM 1=WHITE,3=AQUA,5=GREEN,7=YELLOW,13-15 OK TOO
250 COLOR 3,0
255 ? CHR$(147)
260 ? SPC(20) "CHARACTER REGISTER" :?
265 FOR P = 0 TO A9
270    ? SPC(5) LEFT$(A8$(P)+"                ",16);
275    ? MID$(STR$(P)+"   ",2,3);
280    ? " ";
285    FOR X = 0 TO 5
290       ? HX$(CH(P,X));
295    NEXT X
300    ? " ";
305    ? LEFT$(A0$(A0(P))+"      ",6);
310    ? LEFT$(A1$(A1(P))+"                ",16); 
315    ? A4$(A2(P));
320    ?:?
325 NEXT P
330 ?:INPUT "     VIEW #"; SE
335 IF SE < 0 OR SE > A9 GOTO 245
340 FOR X = 0 TO 15: A7(X)=0 :NEXT X
345 REM BUILD RANDOM SEED, SLOPPILY
350 RR = CH(SE,0)
355 RR = RR + CH(SE,1)    * $10
360 RR = RR + CH(SE,2)    * $100
365 RR = RR + CH(SE,3)    * $1000
370 RR = RR + CH(SE,4)    * $10000
375 RR = RR + CH(SE,5)    * $100000
380 RR = RR + A1(SE)   * $1000000
385 RR = RR + A2(SE) * $10000000
390 X=RND(-RR)
395 A5 = INT(RND(1)*7)
400 A6  = INT(RND(1)*A5)
405 REM
410 REM ASSIGN SKILLS *ROUGHLY* 2 LEVELS AT A TIME
415 REM
420 A7 = A5 * 2 + INT(A6 / 2)
425 FOR X = 0 TO A7
430    N = INT(RND(1)*16)
435    A7(N) = A7(N) + INT(RND(1)*4+1)
440 NEXT X
445 ?:?
450 COLOR 5,0 :REM GREEN ON BLACK
455 ? SPC(5) "NAME:   " LEFT$(A8$(SE)+"               ",16);
460 ? SPC(10) "(" A0$(A0(SE)) " " A1$(A1(SE)) ")"
465 ? SPC(5) "CAREER: " A4$(A2(SE)) "," A5 "TERMS, RANK " A6$(A6)
470 ? SPC(5) "UPP:    ";
475 FOR X = 0 TO 5
480    ? HX$(CH(SE,X));
485 NEXT X
490 ?:? SPC(5) "SKILLS: "
495 Y = 0 :CR = 0
500 FOR X = 0 TO 15
505    IF A7(X) = 0 GOTO 530
510    IF Y/2 = INT(Y/2)  THEN ? SPC(20) A7(X) LEFT$(A7$(X)+"          ",16),,
515    IF Y/2 <> INT(Y/2) THEN ? A7(X) A7$(X)
520    Y=Y+1
525    CR=CR+A7(X)
530 NEXT X
535 ? SPC(5) "SALARY: CR " CR * 100
540 ?:?
545 COLOR 7,0 :REM YELLOW ON BLACK
550 ? SPC(5) "(D)ELETE (I)NPUT (K)EEP (R)ENAME?"
555 GET K$ 
560 IF K$ = "K" GOTO 585
565 IF K$ = "D" THEN GOSUB 605 :GOTO 245
570 IF K$ = "I" THEN GOSUB 650 :GOTO 245
575 IF K$ = "R" THEN GOSUB 770 :GOTO 245
580 GOTO 555
585 IF A8$(SE) = "" THEN INPUT "     ENTER NAME"; A8$(SE)
590 GOTO 245
595 END
600 REM ENTRY NUM IS IN SE
605 A8$(SE) = ""
610 A0(SE) = 1
615 A1(SE)   = INT(RND(1)*26)
620 A2(SE) = INT(RND(1)*8)
625 FOR X = 0 TO 5 
630    CH(SE,X) = INT(RND(1)*6)+INT(RND(1)*6)+2
635 NEXT X
640 RETURN
645 REM ENTRY NUM IS IN SE
650 A8$(SE) = ""
655 ?
660 INPUT "     RACE (A-Z) ........."; RE$
665 IF RE$ < "A" OR RE$ > "Z" THEN RETURN :REM INVALID
670 A1(SE) = ASC(RE$)-65
675 INPUT "     CAREER [AGLMNORS] .."; CA$
680 IF CA$ = "A" THEN A2(SE) = 0 :GOTO 725
685 IF CA$ = "G" THEN A2(SE) = 1 :GOTO 725
690 IF CA$ = "L" THEN A2(SE) = 2 :GOTO 725
695 IF CA$ = "M" THEN A2(SE) = 3 :GOTO 725
700 IF CA$ = "N" THEN A2(SE) = 4 :GOTO 725
705 IF CA$ = "O" THEN A2(SE) = 5 :GOTO 725
710 IF CA$ = "R" THEN A2(SE) = 6 :GOTO 725
715 IF CA$ = "S" THEN A2(SE) = 7 :GOTO 725
720 RETURN :REM INVALID
725 INPUT "     UPP ................"; UP$
730 IF LEN(UP$) <> 6 THEN RETURN
735 FOR X = 0 TO 5
740    V$ = MID$(UP$,X+1,1)
745    IF V$ <= "9" THEN CH(SE,X) = VAL(V$)
750    IF V$ >= "A" THEN CH(SE,X) = ASC(V$) - 55
755 NEXT
760 INPUT "     NAME ..............."; A8$(SE)
765 RETURN
770 INPUT "     NAME"; A8$(SE)
775 RETURN
