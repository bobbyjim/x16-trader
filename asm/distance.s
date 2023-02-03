;
; cl65 -o DISTANCE.PRG -t cx16 -C cx16-asm.cfg distance.s
;

;	.segment "STARTUP"
;	.segment "INIT"
;	.segment "ONCE"
;	.segment "CODE"

r0              =       $02				; general purpose ABI registers
 r0l            =       $02
 r0h            =       $03
r1              =       $04
 r1l            =       $04
 r1h            =       $05
r2              =       $06
 r2l            =       $06
 r2h            =       $07
r3              =       $08
 r3l            =       $08
 r3h            =       $09

Col1			= 		r0l		; 2
Row1			= 		r0h		; 3
Col2			= 		r1l		; 4
Row2			=		r1h		; 5

AA1				=		r2l		; 6
AA2				= 		r2h		; 7

TMP 			= 		r3l		; 8

;                .org    $0801
;                .byte   $0b,$08,$e3,$07 ; cx16 basic sys call.
;                .byte   $9e,"2061"
;                .byte   $00,$00,$00

Main:			
    ; first, figure out the two "diagonal" values AA1 and AA2
    lda Col1	
	lsr			;  Col1/2
	adc Row1	; (Col1/2) + Row1
	sta AA1

	lda Col2	
	lsr			;  Col2/2
	adc Row2	; (Col2/2) + Row2
	sta AA2

  	; now figure out our three candidate distances D1, D2, D3
  	sbc AA1		; AA2 - AA1
	sta TMP

    rts 

; 	bcs gotd1
; 	lda AA1
; 	sbc AA2
; gotd1:	sta D1		; abs(a1-a2)
; 	lda Col2
; 	sub Col1
; 	bcs gotd2
; 	lda Col1
; 	sbc Col2
; gotd2:	sta D2		; abs(c1-c2)
; 	
