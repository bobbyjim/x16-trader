5 LOAD "SPINWARD-MAP-64B.BIN", 8, 10, $A000
10 INPUT "COL (1-256)"; Y
15 INPUT "ROW (1-64)"; X
20 X=X-1 :Y=Y-1
25 REM O IS ENTRY NUMBER
30 O = Y * 64 + X
35 BA = 10+INT(O/128)
40 O  = O - 128*INT(O/128)
45 POKE $9F61,BA
50 ? "BANK " BA ", ENTRY" O
55 L$="0-------:-------1-------:-------2-------:-------3-------:-------4---"
60 ?  L$
65 FOR Y = 0 TO 29
70    FOR X = 0 TO 63
75       O1 = O*64 +X
80       V1 = PEEK($A000+O1)
85       A1$ = CHR$(V1)
90       IF V1<32 OR V1>91 THEN A1$="."
95       ? A1$;
100    NEXT
105    ?
110    O=O+1
115    IF O=128 THEN BA=BA+1 :O=0 :POKE $9F61,BA
120 NEXT
125 GOTO 10
