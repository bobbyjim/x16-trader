5 EN=5000
10 DIM A0$(60)
15 A0$(0)="\X13"
20 FOR X=1 TO 60: A0$(X)=A0$(X-1)+"\X11" :NEXT
25 A1=60
30 A2=56
35 COLOR 15,0: ?CHR$(147)
40 FOR S=1 TO 50 :REM STARS
45    X = INT(RND(1)*A1+1)
50    Y = INT(RND(1)*A2+1)
55    PRINT A0$(Y) TAB(X) "."
60 NEXT
65 X = INT(A1/2)
70 Y = INT(A2/2)
75 ? A0$(Y) TAB(X) "Ï   Ð"
80 ? "\X11" TAB(X-5) "ÛÛÀÀÀ     ÀÀÀÛÛ"
85 ? "\X11" TAB(X) "Ì   º"
90 PRINT A0$(A2+1);
95 ? "Ï£";
100 FOR X=1 TO A1: ? "£"; :NEXT
105 ? "Ð\X9D\X94£"
110 ? "¥ENERGY-  ";LEFT$(STR$(EN)+"      ",6);"        HITS-   0    §\X9D\X94"
115 ? "Ì€";
120 FOR X=1 TO A1: ? "€"; :NEXT
125 ? "º\X9D\X94€\X91"
130 ? A0$(0);
