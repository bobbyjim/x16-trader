5 POKE $9F61,1
10 LOAD "ALLSHIPS.BIN",8,1,$A000
15 AD=$A003 :REM DATA STARTS HERE
20 REM ------------------------------------
25 REM
30 REM   LOOP AND DUMP SHIP DATA.
35 REM
40 REM ------------------------------------
45    LN=PEEK(AD)
50    ? CHR$(PEEK(AD+42)) " "; :REM OWNER
55    REM ------------------------------------
60    REM
65    REM  READ SHIP NAME
70    REM
75    REM ------------------------------------
80    FOR X = 1 TO 23: ? CHR$(PEEK(AD+X));:NEXT
85    M$ = CHR$(PEEK(AD+25))
90    SZ = PEEK(AD+27) :REM SIZ
95    CF$ = CHR$(PEEK(AD+28)) :REM CFG
100    MV = PEEK(AD+29) :REM MANEUVER
105    JP = PEEK(AD+30) :REM JUMP
110    HP = PEEK(AD+31) :REM HOP
115    REM ------------------------------------
120    REM
125    REM  BUILD THE "QUICK SHIP PROFILE" 
130    REM
135    REM ------------------------------------
140    QS$ = M$ + "-" + CHR$(64+SZ) + CF$
145    QS$ = QS$ + CHR$(48+MV)
150    IF HP=0 THEN QS$=QS$ + CHR$(48+JP)
155    IF HP>0 THEN QS$=QS$ + CHR$(48+HP)
160    PW=MV :IF JP>PV THEN PW = JP
165    IF HP>PW THEN PW=HP
170    QS$ = QS$ + CHR$(48+PW)
175    IF HP>0 THEN QS$=QS$ + "H"
180    ? QS$ " ";
185    REM -----------------------------------------
190    REM
195    REM  READ AND DECODE THE REST OF THE HEADER.
200    REM
205    REM -----------------------------------------
210    ? "M/" CHR$(48+PEEK(AD+32)) " "; :REM COMPUTER
215    ? CHR$(PEEK(AD+33));  :REM LANDERS
220    AV = PEEK(AD+34)
225    ? AV; :IF AV<10 THEN ? " ";
230    ? PEEK(AD+35)-5;      :REM COMFORT
235    ? PEEK(AD+36)-5;      :REM DEMAND
240    REM ------------------------------------------
245    REM
250    REM  BECAUSE WE LACK A PRINTF(), WE HAVE TO
255    REM  CONVERT NUMBERS TO STRINGS AND THEN PAD
260    REM  THEM WITH SPACES AS NEEDED.
265    REM
270    REM ------------------------------------------
275    CA$ = STR$(SZ * PEEK(AD+37))
280    IF LEN(CA$)=2 THEN CA$=" " + CA$
285    IF LEN(CA$)=3 THEN CA$=" " + CA$
290    FU$ = STR$(SZ * PEEK(AD+38))
295    IF LEN(FU$)=2 THEN FU$=" " + FU$
300    IF LEN(FU$)=3 THEN FU$=" " + FU$
305    LO$ = STR$(PEEK(AD+39))
310    IF LEN(LO$)=2 THEN LO$=" " + LO$
315    IF LEN(LO$)=3 THEN LO$=" " + LO$
320    SR$ = STR$(PEEK(AD+40))
325    IF LEN(SR$)=2 THEN SR$=" " + SR$
330    ? CA$ FU$ LO$ SR$;
335    ? " MCR" SZ * PEEK(AD+41);   :REM MCR
340    ?
345    REM --------------------------------------
350    REM 
355    REM  OKAY WE MADE IT THROUGH A HEADER.
360    REM  SKIP THE COMPONENT LIST; I THINK WE'D
365    REM  ONLY WANT TO DISPLAY THAT IF THE USER
370    REM  SPECIFICALLY CHOOSES A CRAFT TO 
375    REM  INSPECT.
380    REM 
385    REM --------------------------------------
390    AD = AD + LN + 6
395    REM  THE SCREEN IS FULL AT $B900.  NO PAGING YET.
400    IF AD > $B900 THEN END
405    GOTO 45
