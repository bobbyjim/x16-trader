5 COLOR 5,0
10 PRINT "\X93";
15 A0$=""
20 A1$=""
25 FOR X=1 TO 78
30    A0$=A0$+"£"
35    A1$=A1$+"€"
40 NEXT
45 GOSUB 70 :REM STATUS BAR
50 GOSUB 95 :REM STARCHART
55 GOSUB 280 :REM UWPS
60 END
65 REM STATUS BAR
70 PRINT "  LOCATION: TRANSYLVANIA     DESTINATION: TRANSYLVANIA  "
75 ?
80 PRINT " " A0$
85 RETURN
90 REM STARCHART
95 PRINT"ÕÃÃÃÃÃ®     °ÃÃÃÃÃ®     °ÃÃÃÃÃ®     °ÃÃÃÃÃ®
100 PRINT"Â     Â     Â     Â     Â     Â     Â     Â
105 PRINT"Â     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃÉ
110 PRINT"Â     Â     Â     Â     Â     Â     Â     Â     Â
115 PRINT"«ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     Â
120 PRINT"Â     Â     Â     Â     Â     Â     Â     Â     Â
125 PRINT"Â     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³
130 PRINT"Â     Â     Â     Â     Â     Â     Â     Â     Â
135 PRINT"«ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     Â
140 PRINT"Â     Â     Â     Â     Â     Â     Â     Â     Â
145 PRINT"Â     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³
150 PRINT"Â     Â     Â     Â     Â     Â     Â     Â     Â
155 PRINT"«ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     Â
160 PRINT"Â     Â     Â     Â     Â     Â     Â     Â     Â
165 PRINT"Â     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³
170 PRINT"Â     Â     Â     Â     Â     Â     Â     Â     Â
175 PRINT"«ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     Â
180 PRINT"Â     Â     Â     Â     Â     Â     Â     Â     Â
185 PRINT"Â     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³
190 PRINT"Â     Â     Â     Â     Â     Â     Â     Â     Â
195 PRINT"«ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     Â
200 PRINT"Â     Â     Â     Â     Â     Â     Â     Â     Â
205 PRINT"Â     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³
210 PRINT"Â     Â     Â     Â     Â     Â     Â     Â     Â
215 PRINT"«ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     Â
220 PRINT"Â     Â     Â     Â     Â     Â     Â     Â     Â
225 PRINT"Â     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³
230 PRINT"Â     Â     Â     Â     Â     Â     Â     Â     Â
235 PRINT"«ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     Â
240 PRINT"Â     Â     Â     Â     Â     Â     Â     Â     Â
245 PRINT"Â     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³
250 PRINT"Â     Â     Â     Â     Â     Â     Â     Â     Â
255 PRINT"ÊÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     «ÃÃÃÃÃ³     Â
260 PRINT"      Â     Â     Â     Â     Â     Â     Â     Â
265 PRINT"      ­ÃÃÃÃÃœ     ­ÃÃÃÃÃœ     ­ÃÃÃÃÃœ     ­ÃÃÃÃÃË
270 RETURN
275 REM WORLD LIST
280 PRINT "\X13\X11\X11"
285 FOR X = 1 TO INT(RND(1)*40)
290 PRINT TAB(50) "TRANSYLVANIA 2 AS HI IN NA VA"
295 NEXT
300 RETURN
