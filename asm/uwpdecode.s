;
; cl65 -o UWPDECODE.PRG -t cx16 -C cx16-asm.cfg uwpdecode.s
;
chrout		= $ffd2

;	.segment "STARTUP"
;	.segment "INIT"
;	.segment "ONCE"
;	.segment "CODE"

	.org 	$A000
	.export	LOADADDR = *

; $A000
target0:	.byte 0

; $A001
Main:		lda target0
                cmp #32		; is this a space?
		bne putchar	; no

putspace:      	jsr twospaces	; someone wants a space char
		jsr down
		jsr twoleft
          	jsr twospaces
		jsr down
		jsr twoleft
		jsr twospaces
		bvc done        ; mission accomplished (used to be a JMP)

		;
		; Char to print is POKEd into target0.
		; Manipulate it to get our table index.
		;
putchar:	clc
		sbc #64
                tax             ; and put that in x
		;	
		; Set up our table pointer in $02-$03
		;
		lda table_lo,x
		sta $02
		lda table_hi,x
		sta $03
		;
		; $02-$03 indirectly points to the letter data
		;
 		ldy #0		; my pointer index
                bvc loop 	; used to be a JMP

position:	pha
		jsr down
		jsr twoleft
		jsr chrout ; and a third left
                pla
                iny
                iny

loop:		lda ($02),y
		jsr chrout
                iny
                lda ($02),y
		jsr chrout
		iny
                lda ($02),y
		jsr chrout
		cpy #10
                bcc position

done:           lda #145		; up
		jsr chrout
		jsr chrout 		; twice
     		rts

twospaces:	lda #32  		; space
		jsr chrout
		jsr chrout
		rts
		
twoleft:	lda #157		; left
		jsr chrout
		jsr chrout
		rts

down:		lda #17			; down
		jsr chrout
		rts


;
; Our character data format:
;
; [1 2 3] [c] [4 5 6] [.] [7 8 9] [. . . .] 0
;
; where
;
; 	[1 2 3] are the three PETSCII characters in the top row
;	[c]	is the PETSCII value of the actual character (e.g. 65 for 'A')
;	[4 5 6] are the three PETSCII characters in the middle row
;	[.]	is unused, currently a dot (46).
;	[7 8 9] are the three PETSCII characters in the bottom row
;	[. . . .] are four more dots (46's).
;	Finally, the 16-byte record ends with a zero (0).
; 
alpha:		.byte 192, 192, 201, 65, 213, 192, 219, 46, 202, 192, 203, 46, 46, 46, 46, 0
beta:		.byte 194, 32, 32, 66, 171, 192, 201, 46, 202, 192, 203, 46, 46, 46, 46, 0
cella: 		.byte 213, 192, 192, 67, 202, 192, 192, 46, 32, 32, 32, 46, 46, 46, 46, 0
delta: 		.byte 32, 32, 194, 68, 213, 192, 179, 46, 202, 195, 203, 46, 46, 46, 46, 0
epsilon:	.byte 213, 192, 192, 69, 171, 192, 32, 46, 202, 192, 192, 46, 46, 46, 46, 0
digamma:	.byte 213, 195, 195, 70, 171, 192, 32, 46, 194, 32, 32, 46, 46, 46, 46, 0
gamma:		.byte 213, 192, 201, 71, 202, 192, 219, 46, 32, 192, 203, 46, 46, 46, 46, 0
heta:  		.byte 194, 32, 32, 72, 171, 192, 201, 46, 194, 32, 194, 46, 46, 46, 46, 0
iota:  		.byte 192, 178, 192, 73, 192, 177, 192, 46, 32, 32, 32, 46, 46, 46, 46, 0
jot: 		.byte 192, 178, 192, 74, 192, 203, 32, 46, 32, 32, 32, 46, 46, 46, 46, 0
kappa:		.byte 170, 164, 206, 75, 167, 163, 205, 46, 32, 32, 32, 46, 46, 46, 46, 0
lambda:		.byte 194, 32, 32, 76, 202, 192, 192, 46, 32, 32, 32, 46, 46, 46, 46, 0
mu:  		.byte 178, 178, 201, 77, 194, 194, 194, 46, 32, 32, 32, 46, 46, 46, 46, 0
nu:		.byte 178, 192, 201, 78, 194, 32, 194, 46, 32, 32, 32, 46, 46, 46, 46, 0
omicron:	.byte 213, 192, 201, 79, 202, 192, 203, 46, 32, 32, 32, 46, 46, 46, 46, 0
pi: 		.byte 213, 192, 201, 80, 171, 192, 203, 46, 194, 32, 32, 46, 46, 46, 46, 0
qoph:  		.byte 213, 195, 201, 81, 194, 32, 194, 46, 202, 178, 203, 46, 46, 46, 46, 0
rho: 		.byte 213, 195, 195, 82, 194, 32, 32, 46, 32, 32, 32, 46, 46, 46, 46, 0
sigma: 		.byte 213, 192, 195, 83, 202, 192, 201, 46, 195, 192, 203, 46, 46, 46, 46, 0
tau:		.byte 192, 178, 192, 84, 32, 194, 32, 46, 32, 32, 32, 46, 46, 46, 46, 0
upsilon:       	.byte 194, 32, 194, 85, 202, 192, 203, 46, 32, 32, 32, 46, 46, 46, 46, 0
vau:   		.byte 194, 32, 194, 86, 202, 192, 203, 46, 32, 32, 32, 46, 46, 46, 46, 0
wau:  		.byte 194, 194, 194, 87, 202, 177, 203, 46, 32, 32, 32, 46, 46, 46, 46, 0
xi:  		.byte 205, 164, 206, 88, 206, 163, 205, 46, 32, 32, 32, 46, 46, 46, 46, 0
ypsilon:       	.byte 205, 175, 206, 89, 32, 194, 32, 46, 32, 32, 32, 46, 46, 46, 46, 0
zeta: 		.byte 195, 195, 201, 90, 213, 195, 203, 46, 202, 195, 195, 46, 46, 46, 46, 0

table_lo:	
	.byte <alpha
	.byte <beta
	.byte <cella
	.byte <delta
	.byte <epsilon
	.byte <digamma
	.byte <gamma
	.byte <heta
	.byte <iota
	.byte <jot
	.byte <kappa
	.byte <lambda
	.byte <mu
	.byte <nu
	.byte <omicron
	.byte <pi
	.byte <qoph
	.byte <rho
	.byte <sigma
	.byte <tau
	.byte <upsilon
	.byte <vau
	.byte <wau
	.byte <xi
	.byte <ypsilon
	.byte <zeta

table_hi:
	.byte >alpha
	.byte >beta
	.byte >cella
	.byte >delta
	.byte >epsilon
	.byte >digamma
	.byte >gamma
	.byte >heta
	.byte >iota
	.byte >jot
	.byte >kappa
	.byte >lambda
	.byte >mu
	.byte >nu
	.byte >omicron
	.byte >pi
	.byte >qoph
	.byte >rho
	.byte >sigma
	.byte >tau
	.byte >upsilon
	.byte >vau
	.byte >wau
	.byte >xi
	.byte >ypsilon
	.byte >zeta

