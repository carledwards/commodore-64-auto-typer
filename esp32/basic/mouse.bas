5 S=54272:POKE S+24,15:POKE S,220:POKE S+1,68:POKE S+5,15:POKE S+6,215
10 POKE S+7,120:POKE S+8,100:POKE S+12,15:POKE S+13,215
15 PRINT"{CLEAR}": V=53248: POKE V+21,1
20 FOR S1=12288 TO 12350:READ Q1: POKE S1,Q1: NEXT
25 FOR S2=12352 TO 12414:READ Q2: POKE S2,Q2: NEXT
30 FOR S3=12416 TO 12478:READ Q3: POKE S3,Q3: NEXT
35 POKE V+39,15:POKE V+1,68
40 PRINTTAB(160)"{WHITE}I AM THE DANCING MOUSE!{LIGHT BLUE}"
45 P=192
50 FOR X = 0 TO 347 STEP 3
55 RX=INT(X/256):LX=X-RX*256
60 POKE V,LX: POKE V+16,RX
70 IF P=192 THEN GOSUB 200
75 IF P=193 THEN GOSUB 300
80 POKE 2040,P:FOR T=1 TO 60:NEXT
85 P=P+1:IF P>194 THEN P=192
90 NEXT
95 END
100 DATA 30,0,120,63,0,252,127,129,254,127,129,254,127,189,254,127,255,254
101 DATA 63,255,252,31,187,248,3,187,192,1,255,128,3,189,192,1,231,128,1,255,0
102 DATA 31,255,0,0,124,0,0,254,0,1,199,32,3,131,224,7,1,192,1,192,0,3,192,0
103 DATA 30,0,120,63,0,252,127,129,254,127,129,254,127,189,254,127,255,254
104 DATA 63,255,252,31,221,248,3,221,192,1,255,128,3,255,192,1,195,128,1,231,3
105 DATA 31,255,255,0,124,0,0,254,0,1,199,0,7,1,128,7,0,204,1,128,124,7,128,56
106 DATA 30,0,120,63,0,252,127,129,254,127,129,254,127,189,254,127,255,254
107 DATA 63,255,252,31,221,248,3,221,192,1,255,134,3,189,204,1,199,152,1,255,48
108 DATA 1,255,224,1,252,0,3,254,0
109 DATA 7,14,0,204,14,0,248,56,0,112,112,0,0,60,0,-1
200 POKE S+4,129: POKE S+4,128:RETURN
300 POKE S+11,129: POKE S+11,128:RETURN
