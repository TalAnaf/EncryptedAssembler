; file ps.as
.entry LIST 
.extern W 
.define sz = 2 

mcr MACRO1
    add r1, r2
    sub r3, r4
endmcr

MAIN:	mov r3, LIST[sz]

LOOP: 	jmp W 
	prn #-5
	mov STR[5], STR[2] 
	sub r1, r4
	cmp K, #sz
	bne W
L1: 	inc L3 

mcr MACRO2
    cmp r1, #0
    bne END
endmcr

.entry LOOP
	bne LOOP 
END: hlt
MACRO1
.define len = 4
STR: .string "abcdef" 
LIST: .data 6, -9, len 
K: .data 22 
.extern L3
