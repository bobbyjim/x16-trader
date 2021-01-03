
10 REM ---------------------------------------
11 REM
15 REM  DI$  THE DIRNAME TO CREATE IT IN
16 REM  FI$  THE FILENAME TO CREATE
17 REM  UW$(8,10) UP TO 80 UWP DATA LINES
19 REM
20 REM ---------------------------------------
40 REM SECTOR DIR AND SUBSECTOR FILE GO HERE
70 GOSUB 1000       :REM LOAD SUBSECTOR
90 C$ = "//"+DI$+"/:"+FI$+",P,W"
100 OPEN 1,8,2,C$
110 FOR Y = 1 TO 10
120 FOR X = 1 TO 8
130 IF UW$(X,Y) <> "" THEN PRINT#1, UW$(X,Y)
140 NEXT X :NEXT Y
150 CLOSE 1

200 REM ------------------------------------------
201 REM
202 REM  CREATE SUBSECTOR FILE AT DI$
203 REM
204 REM ------------------------------------------
210 OPEN 1,8,2,"//DI$/:FI$"
220 INPUT#1,UW$
230 ? MID$(UW$,22)
240 CLOSE 1

999 END

1000 REM -------------------------------------------
1001 REM
1002 REM  LOAD SUBSECTOR DATA DIRECTLY INTO UW$(,)
1003 REM
1004 REM -------------------------------------------
1005 FOR Y = 1 TO 10
1006 FOR X = 1 TO 8
1007 UW$(X,Y)=""
1008 NEXT X :NEXT Y
1999 RETURN


50 DI$="CS.46-49"
60 FI$="SS.C"

1100 UW$(3,10) = "3A A788899-CNS038IM"+CHR$(143)+CHR$(78)+"REGINA"

