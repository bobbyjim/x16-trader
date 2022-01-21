5 DIM A0$(50)
10 A0$(0) = "\X13"
15 FOR X = 1 TO 50 :A0$(X)=A0$(X-1)+"\X11" :NEXT
20 DIM S$(20)
25 Y=0
30 READ S$(Y) :IF S$(Y)<>"-1" THEN Y=Y+1 :GOTO 30
35 ?CHR$(147);
40 FOR I=0 TO 16 :?S$(I) :NEXT
45 X=16 :Y=4
50 ? A0$(Y) TAB(X) CHR$(94)
55 NY=Y :NX=X
60 GET A$
65 IF A$="I" THEN NY=Y-1 :GOTO 90
70 IF A$="K" THEN NY=Y+1 :GOTO 90
75 IF A$="J" THEN NX=X-1 :GOTO 90
80 IF A$="L" THEN NX=X+1 :GOTO 90
85 GOTO 55
90 IF MID$(S$(NY),NX+1,1)="P" GOTO 240
95 IF MID$(S$(NY),NX+1,1)="A" GOTO 250
100 IF MID$(S$(NY),NX+1,1)="S" GOTO 260
105 IF MID$(S$(NY),NX+1,1)="[" GOTO 270
110 IF MID$(S$(NY),NX+1,1)="]" GOTO 280
115 IF MID$(S$(NY),NX+1,1)="!" GOTO 290
120 IF MID$(S$(NY),NX+1,1)<>"." GOTO 55
125 ? A0$(Y) TAB(X) "."
130 Y=NY :X=NX
135 ? A0$(Y) TAB(X) CHR$(94)
140 GOTO 50
145 END
150 DATA "     
155 DATA "
160 DATA "               P---A
165 DATA "               .....
170 DATA "               S....
175 DATA "               .....
180 DATA "                 .
185 DATA "                 .
190 DATA "               .....
195 DATA "              ].....[
200 DATA "               .....
205 DATA "               .....
210 DATA "               .....
215 DATA "               .!!!.
220 DATA "
225 DATA "
230 DATA "
235 DATA -1
240 PRINT A0$(50) "PILOT       "
245 GOTO 55
250 PRINT A0$(50) "ASTROGATION "
255 GOTO 55
260 PRINT A0$(50) "SENSOP      "
265 GOTO 55
270 PRINT A0$(50) "STARPORT    "
275 GOTO 55
280 PRINT A0$(50) "MARKET      "
285 GOTO 55
290 PRINT A0$(50) "JUMP        "
295 GOTO 55
