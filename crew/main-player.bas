5 DIM RA$(13),RB$(13)
10 DIM SV$(5), SW$(5)
15 DIM MM$(6)
20 DIM SK$(15)
25 DIM SK(15)
30 DIM HX$(15)
35 FOR X = 0 TO 13 :READ RA$(X) :NEXT
40 DATA ASLAN,BWAP,DROYNE,HIVER,SHRIEKER
45 DATA KKREE,LLELLEWYLOLY,AMINDII,DARRIAN
50 DATA SOLOMANI,THREEP,HUMAN,VARGR,ZHODANI
55 FOR X = 0 TO 13 :READ RB$(X) :NEXT
60 DATA A,B,D,H,I,K,L,M,R,S,T,U,V,Z
65 FOR X = 0 TO 5 :READ SV$(X) :NEXT
70 DATA SCOUT,MERCHANT,ARMY,MARINE,NAVY,ROGUE
75 FOR X = 0 TO 5 :READ SW$(X) :NEXT
80 DATA S,M,A,R,N,G
85 FOR X = 0 TO 15 :READ SK$(X) :NEXT
90 DATA ASTROGATOR, PILOT, ENGINEER, MEDIC, GUNNER
95 DATA FIGHTER, RECON, SENSOP, SURVEY, VACC
100 DATA HOSTILE ENV, ADMIN, STREETWISE, DIPLOMAT
105 DATA SURVIVAL, TRADER
110 FOR X = 0 TO 15 :READ HX$(X) :NEXT
115 DATA 0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F
120 REM MEDALS
125 ME$(3) = "MCG" :ME$(4) = "MCUF" :ME$(5) = "SEH"
130 ? "CHARACTER CREATOR"
135 INPUT "PRESS ANY KEY"; PK$
140 R=RND(-TI)
145 ? CHR$(147);
150 RR = INT(RND(1)*14)
155 C1 = INT(RND(1)*6)+INT(RND(1)*6)+2
160 C2 = INT(RND(1)*6)+INT(RND(1)*6)+2
165 C3 = INT(RND(1)*6)+INT(RND(1)*6)+2
170 C4 = INT(RND(1)*6)+INT(RND(1)*6)+2
175 C5 = INT(RND(1)*6)+INT(RND(1)*6)+2
180 C6 = INT(RND(1)*6)+INT(RND(1)*6)+2
185 ? RA$(RR) :?
190 ?:? C1, "STRENGTH       " 
195 ?:? C2, "DEXTERITY      " 
200 ?:? C3, "ENDURANCE      " 
205 ?:? C4, "INTELLIGENCE   " 
210 ?:? C5, "EDUCATION      " 
215 ?:? C6, "SOCIAL STANDING"
220 ?:?
225 TS = INT(RND(1)*5)
230 TT = INT(RND(1)*6+2) :REM TERMS
235 RK=0 :OK=0 :MM=0
240 IF TS>0 THEN RK = INT(RND(1)*TT)
245 IF TS>0 THEN OK = INT(RND(1)*3/2) :REM OFFICER
250 IF TS>0 THEN MM = INT(RND(1)*6)
255 ?:? SV$(TS) "(" TT "TERMS ) "; 
260 IF TS>0 AND OK > 0 THEN ? "O";
265 IF TS>0 AND OK = 0 THEN ? "E";
270 IF TS>0 THEN ? HX$(RK) " ";
275 IF MM > 2 THEN ? ME$(MM)
280 ?:?:?
285 SS = TT * 3
290 IF TS>0 THEN SS = SS + RK           :REM RANK
295 IF TS>0 AND MM > 3 THEN SS = SS + 1 :REM DECORATED
300 IF TS>0 AND OK = 1 THEN SS = SS + 1 :REM OFFICER
305 IF TS=0 THEN SS = SS + TT * 3
310 FOR X = 0 TO 15
315    SK(X) = 0
320 NEXT
325 FOR X = 0 TO SS
330    S = INT(RND(1)*16)
335    SK(S) = SK(S) + 1
340 NEXT
345 FOR X = 0 TO 15
350    IF SK(X) > 0 THEN ? SK(X), SK$(X) :?
355 NEXT
360 ?
365 INPUT "KEEP THIS CHARACTER"; YN$
370 IF YN$="N" GOTO 145
375 IF YN$="Y" GOTO 385
380 GOTO 365
385 INPUT "CHARACTER NAME"; NA$
390 NA$ = LEFT$(NA$+"... ... ... ... ",16)
395 IF NA$="... ... ... ... " GOTO 385
400 D$ = NA$ + RB$(RR)
405 D$ = D$ + "." + HX$(C1) + HX$(C2) + HX$(C3)
410 D$ = D$ +       HX$(C4) + HX$(C5) + HX$(C6)
415 D$ = D$ + "." + SW$(TS) + HX$(TT)
420 IF OK=0 THEN D$=D$ + ".E"
425 IF OK>0 THEN D$=D$ + ".O"
430 D$=D$+HX$(RK)+HX$(MM)
435 FOR X = 0 TO 3
440    D$ = D$ + "-"
445    FOR Y = 0 TO 3
450       D$=D$+HX$(SK(X*4+Y))
455    NEXT
460 NEXT
465 ?:? "DATA STRING TO STORE: " D$
470 OPEN 2,8,2,"0:CHAR-" + NA$ + ",W,SEQ"
475 PRINT#2, NA$,D$
480 CLOSE 2
485 ?:? "CHARACTER FILE CHAR-" NA$ " WRITTEN."
490 END
