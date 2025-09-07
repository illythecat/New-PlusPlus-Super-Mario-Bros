ncp_call(0x20BB6F8, 0)
	MOV     R4, R0
	LDR	R0, =0x02088BFC
	LDR	R0, [R0]


	CMP     R0, #0x4    
	BNE     .CheckWhatLoaded         @If not this world ID  

	

	LDR	R0, =0x02122004         
	LDR     R9, =2102 - 0x83
	MOV     R6, #0x33C
	BL      .Store
	B	0x020BB6FC




	
.CheckWhatLoaded:
	LDR	R0, =0x02122004
	LDR     R7, [R0]
	LDR     R9, =0xC70406F9
	CMP     R7, R9
	BEQ	0x020BB6FC
	MOV     R6, #0x33C
	BL      .Store	
	LDR     R0, =0x021223B8
	LDR     R9, =0x00000407
	MOV     R6, #0x9C
	BL      .Store	
	B	0x020BB6FC
	
.Store:
	MOV     R7, #0
	
.Loop:
	STR     R9, [R0, R7]
	ADD     R7, #0xC
	CMP     R7, R6
	BNE     .Loop
	BX      LR
