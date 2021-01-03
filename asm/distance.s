;
; cl65 -o DISTANCE.PRG -t cx16 -C cx16-asm.cfg distance.s
;

;	.segment "STARTUP"
;	.segment "INIT"
;	.segment "ONCE"
;	.segment "CODE"

	.org 	$0200
	.export	LOADADDR = *

Row1:		.byte 0	; $0200
Col1:		.byte 0 ; $0201
Row2:		.byte 0 ; $0202
Col2: 		.byte 0 ; $0203
Dist:		.byte 0 ; $0204
A1:		.byte 0	; $0205
A2:		.byte 0	; $0206
D1:		.byte 0	; $0207
D2:		.byte 0	; $0208
		.byte 0,0,0,0,0,0,0 	; pad

Main:			; $0210
	; figure out A1 and A2
	lda Col1
	lsr A
	add Row1
	sta A1
	lda Col2
	lsr A
	add Row2
	sta A2
	
	; now figure out D1, D2, D3
	sbc A1
	bcs gotd1
	lda A1
	sbc A2
gotd1:	sta D1		; abs(a1-a2)
	lda Col2
	sub Col1
	bcs gotd2
	lda Col1
	sbc Col2
gotd2:	sta D2		; abs(c1-c2)
	
