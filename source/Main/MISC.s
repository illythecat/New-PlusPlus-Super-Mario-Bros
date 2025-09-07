

ncp_call(0x020A2764, 0)
    SUB             R2, R2, #138
    BX              LR

@option for spikeballs to die on wall contact
ncp_call(0x0218CFD8, 121)
    LDR     R0, [R4,#8]
    LSR     R0, R0, #0x14
    ANDS    R0, R0, #0xF
    LDREQ   R0, =Vec3_vtable
    BEQ     0x0218CFDC
    ADD     SP, SP, #0x28
    LDRB    R1, [R4,#0x2BD]
    EOR     R1, #1
    STRB    R1, [R4,#0x2BD]
    MOV     R0, #0
    STR     R0, [SP]
    LDRB    R3, [R4,#0x2BD]
    MOV     R0, R4
    LDR     R12, [R0]
    MOV     R2, #0x300
    LDR     R1, =0x20C4ED4
    MOV     R3, R3,LSL#1
    LDRSH   R1, [R1,R3]
    LDR     R12, [R12,#0xF8]
    RSB     R3, R2, #0
    MOV     R2, #0x3000
    BLX     R12
    ADD     SP, SP, #8
    LDMFD   SP!, {R4,PC}

@score v2
ncp_call(0x020D8D0C, 8)
	LDRB	R2, [R0]
	LDR		R0, =0x216F8E8
	LDRH	R1, [R0, #4]
	ANDS	R1, R1, #0x100
	BNE		.AlternateCalculation
	LDR		R1, [R0]
	STR		R1, [R0, #-0xC0]
	STR		R1, [R0, #0x28]
	STR		R1, [R0, #-0x80]
	STR		R1, [R0, #-0x28]
	STR		R1, [R0, #-0x158]
	STR		R1, [R0, #0x20]
	STR		R1, [R0, #-0x70]
	STR		R1, [R0, #-0xF8]
	STR		R1, [R0, #-0x170]
	ADD		R0, R0, #4
	MOV		R1, #0
	SUB		R1, R1, #0x10000
	ADD		R1, #0x1000
	CMP		R2, #1
	ADDEQ	R1, #0x2000
	STR		R1, [R0, #-0xC0]
	ADD		R1, R1, #2
	STR		R1, [R0, #0x28]
	ADD		R1, R1, #2
	STR		R1, [R0, #-0x80]
	ADD		R1, R1, #2
	STR		R1, [R0, #-0x28]
	ADD		R1, R1, #2
	STR		R1, [R0, #-0x158]
	ADD		R1, R1, #2
	STR		R1, [R0, #0x20]
	ADD		R1, R1, #2
	STR		R1, [R0, #-0x70]
	ADD		R1, R1, #2
	STR		R1, [R0, #-0xF8]
	ADD		R1, R1, #2
	STR		R1, [R0]
	ADD		R1, R1, #2
	STR		R1, [R0, #-0x170]
	BX		LR
.AlternateCalculation:
	ADD		R0, R0, #5
	LDRB	R1, [R0]
	AND		R1, R1, #0x1F
	CMP		R2, #1
	ADDEQ	R1, R1, #0x20
	STRB	R1, [R0, #-0xC0]
	STRB	R1, [R0, #0x28]
	STRB	R1, [R0, #-0x80]
	STRB	R1, [R0, #-0x28]
	STRB	R1, [R0, #-0x158]
	STRB	R1, [R0, #0x20]
	STRB	R1, [R0, #-0x70]
	STRB	R1, [R0, #-0xF8]
	STRB	R1, [R0]
	STRB	R1, [R0, #-0x170]
	BX		LR


@BlueShell changes
ncp_call(0x021111DC, 10)
	STMFD		        SP!, {R6-R7}
	ADD			R7, PC, #0x164
	ADD			LR, PC, #0
	BX			R7
	CMP			R1, R6
	ADDLT		        PC, PC, #0x14
	CMP			R0, #0
	MOVLT		        R0, R6
	RSBLT		        R0, R0, #0
	MOVGE		        R0, R6
	LDMFD		        SP!, {R6-R7}
	B			0x21111F4
	LDMFD		        SP!, {R6-R7}
	B			0x21111FC

ncp_call(0x021114D8, 10)
	STMFD		        SP!, {R6-R7}
	MOV			R6, R5
	SUB			R6, #0x10
	NOP
	LDR			R6, [R6, #0x7B0]
	TST			R6, #0x80
	MOVEQ		        R0, #0
	ADDNE		        R0, R0, #0x80
	CMP			R0, #0
	ADDNE		        PC, PC, #4
	LDMFD		        SP!, {R6-R7}
	B			0x21114E0
	ADD			R7, PC, #0x100
	ADD			LR, PC, #0
	BX			R7
	NOP
	MOV			R7, R5
	SUB			R7, #0x10
	LDR			R1, [R7, #0xC4]
	LDR			R2, [R7, #0xC8]
	CMP			R1, R2
	MOVEQ		        R1, #1
	MOVNE		        R1, #0
	CMP			R2, #0
	RSBLT		        R6, R6, #0
	CMP			R1, #0
	STRNE		        R6, [R7, #0xC4]
	STR			R6, [R7, #0xC8]
	LDMFD		        SP!, {R6-R7}
	B			0x2111528
	
ncp_call(0x0211162C, 10)
	TST			R2, #0x80		@Check if D-Pad_Down is pressed
	ADDNE		        R0, R0, #0x80
	CMP			R0, #0
	BNE			0x2111634
	B			0x211169C
	
ncp_call(0x02111874, 10)
	STMFD		        SP!, {R6-R7}
	ADD			R7, PC, #0xA0
	ADD			LR, PC, #0
	BX			R7
	MOV			R1, R6
	LDMFD		        SP!, {R6-R7}
	B			0x2111878
	
ncp_call(0x021122E8, 10)
	STMFD		        SP!, {R6-R7}
	ADD			R7, PC, #0x84
	ADD			LR, PC, #0
	BX			R7
	CMP			R0, #0
	MOVLT		        R0, R6
	RSBLT		        R0, R0, #0
	MOVGE		        R0, R6
	LDMFD		        SP!, {R6-R7}
	B			0x21122F8
	
ncp_call(0x02112574, 10)
	STMFD		        SP!, {R6-R7}
	ADD			R7, PC, #0x5C
	ADD			LR, PC, #0
	BX			R7
	CMP			R0, #0
	MOVLT		        R0, R6
	RSBLT		        R0, R0, #0
	MOVGE		        R0, R6
	LDMFD		        SP!, {R6-R7}
	B			0x2112584
	
ncp_call(0x02115D14, 10)
	STMFD		        SP!, {R6-R7}
	ADD			R7, PC, #0x68
	ADD			LR, PC, #0
	BX			R7
	CMP			R0, R6
	ADDGE		        PC, PC, #4
	LDMFD		        SP!, {R6-R7}
	B			0x2115D40
	LDMFD		        SP!, {R6-R7}
	MOV			R0, R4
	SUB			R0, #0x10
	NOP
	LDR			R0, [R0, #0x7B0]
	TST			R0, #0x80
	BNE			0x2115D18
	B			0x2115D40
	
@Relative function        
	STMFD		        SP!, {LR}
	LDR			R7, =0x208B350		@Pointer for Starman duration counter
	LDR			R7, [R7]		@Get Starman remaining time
	CMP			R7, #0			@Check if Starman counter is 0
	MOVNE		        R7, #4
	MOV			R6, R5
	SUB			R6, #0x10
	LDR			R6, [R6, #0x264]	@Get Mario's current action
	CMP			R6, #0			@Check if Mario is airborne
	MOVEQ		        R7, #0
	LDR			R6, =0x212E344		@Pointer for running speed
	LDR			R6, [R6, R7]		@Get max speed
	LDMFD		        SP!, {PC}
	
@Relative function 2 because lazy as fuck
	STMFD		        SP!, {LR}
	LDR			R6, =0x203A784
	LDR			R7, =0x208B350		@Pointer for Starman duration counter
	LDR			R7, [R7]		@Get Starman remaining time
	CMP			R7, #0			@Check if Starman counter is 0
	MOVNE		        R7, #4
	MOV			R6, R4
	SUB			R6, #0x10
	LDR			R6, [R6, #0x264]	@Get Mario's current action
	CMP			R6, #0			@Check if Mario is airborne
	MOVEQ		        R7, #0
	LDR			R6, =0x212E344		@Pointer for running speed
	LDR			R6, [R6, R7]		@Get max speed
	LDMFD		        SP!, {PC}


	
ncp_call(0x020D07B4, 10)      @blueshell blocks				
	STMFD	SP!, {LR}
	MOV     R1, R4
	LDR     R4, =#0x0088002B
	CMP     R1, R4
	LDMNEFD SP!, {PC}
	BL      getPtrToMarioOrLuigi
	ADD     R0, R0, #0x700
	LDRB    R0, [R0, #0xAB]
	LDR     R1, =#0x0088002B
	CMP     R0, #4
	LDREQ   R1, =#0x00080000
	CMP     R0, #0
	LDREQ   R1, =#0x00080000
	LDMFD   SP!, {PC}

@Boss Partical Fix
ncp_jump(0x02022D98)
	LDR 	R0, =0x208B19C	        @ Pointer to block 14
	LDR 	R0, [R0]
	LDR 	R0, [R0, #0xF]
	CMP 	R0, #9			
	MOVEQ 	R0, #0x0
	BXEQ 	LR
	CMP 	R0, #2			@ If Bowser
	MOVLT 	R0, #0x0		
	BXLT 	LR
	BEQ  	.Bowser

	LDR 	R1, =0x2026C88	        @ Particle files array
	CMP 	R0, #3			@ If Giga Goomba
	ADDEQ   R0, R1, #0x10
	BXEQ    LR

	CMP 	R0, #4			@ If Petey
	ADDEQ   R0, R1, #0x18
	BXEQ    LR

	CMP 	R0, #5			@ If Mummipokey
	MOVEQ   R0, R1
	BXEQ    LR

	CMP 	R0, #6			@ If Lakithunder
	ADDEQ   R0, R1, #0x28
	BXEQ    LR

	CMP 	R0, #7			@ If Cheep Skipper
	ADDEQ   R0, R1, #0x8
	BXEQ    LR

	ADD     R0, R1, #0x20	        @ Else, Monty Tank
	BX      LR

.Bowser:
	LDR	R0, =0x208B180	        @ Pointer to block 7
	LDR	R1, =0x208B184	        @ Pointer to block 8
	LDR     R0, [R0]
	LDR     R1, [R1]
	ADD     R1, #4			@ Remove padding

	STMFD 	SP!, {R0,LR}
	SUB     R0, R1, R0		@ Get size of block 7
	LDR 	R1, =0xC	
	BL  	FX_Div			@ Get amount of sprites in level
	LDMFD 	SP!, {R0,LR}

	MOV     R12, R1
	LDR 	R3, =0x2026C80

.CheckWhichBowser:		        @ Determine which Bowser sprite is present
	MOV 	R1, R12
	MOV 	R2, #0xC
	MUL 	R1, R2, R1
	LDRH 	R2, [R0,R1]
	CMP 	R2, #0x3A		@If Bowser
	MOVEQ 	R0, R3
	BXEQ  	LR
	CMP 	R2, #0x3F		@If Dry Bowser
	ADDEQ 	R0, R3,#0x38	
	BXEQ  	LR
	CMP  	R12, #0
	SUBGT 	R12, #1
	BGT 	.CheckWhichBowser
	ADD 	R0, R3,#0x40	        @Else, Giant Bowser
	BX 	LR

@Goal Animations
ncp_call(0x02117D18, 0xa)				
	STMFD	SP!, {R0}
	BL      getPtrToPlayerActor
	ADD     R0, #0x700
	ADD     R0, #0xAB
	LDRB    R1, [R0]
	LDMFD	SP!, {R0}
	MOV   R1, #0xC00
	STR   R1, [SP]
	MOV     R1, #0x61
	ADD   R1, #0x2
	B       0x2117D1C
	
ncp_call(0x02117BD0, 10)				
ncp_call(0x02117C78, 10)
	STMFD	SP!, {R0,R1,LR}
	BL      getPtrToPlayerActor
	ADD     R0, #0x700
	ADD     R0, #0xAB
	LDRB    R1, [R0]
	LDMFD	SP!, {R0,R1,LR}
	BX    LR

@Lava Bubbles

ncp_call(0x02148214, 48)                              
	 LDRB 	R0,[R4,#0xD6]
	 CMP 	R0,#0xFF
	 MOVEQ 	R2,#0x2
	 MOVNE 	R2,#0x0
	 MOV 	R0,R4
	 BX 		LR


ncp_call(0x021480D0, 48)                              
	STMFD	SP!, {R1,LR}
	LDR	R1, [R4, #8]
	LSR	R1,R1,#0
	AND 	R1, R1, #0xFF
	CMP 	R1, #0
	STREQH 	R0,[R3,#0xCA]
	LDMEQFD SP!, {R1,PC}
	CMP 	R1, #1
	MOVGE 	R0,#0x5
	STREQH 	R0,[R3,#0xCA]
	LDMEQFD SP!, {R1,PC}
	MUL 	R0, R1, R0
	STRH 	R0, [R3,#0xCA]
	LDMFD 	SP!, {R1,PC}

 
ncp_call(0x02147E1C, 48)
	STMFD 	SP!, {R0,R1}
	LDR 	R0, =0x2088BFC
	LDR 	R0,[R0]
	CMP 	R0,#0x7  @worldID
	MOVEQ 	R1,#0
	STREQB 	R1, [R10,#0x4CE]
	STREQB 	R1, [R10,#0x4D0]
	STREQB 	R1, [R10,#0x4D2]
	LDMFD 	SP!, {R0,R1}
	AND 	R3, R4, #0xFF
	BX 		LR
	
ncp_call(0x02147F14, 48)
	MOV     R4, R0
	LDR		R1, [R4, #8]
	LSR     R1, #0x8
	ANDS 	R1, R1, #0xF
	BXEQ    LR
	MOV     R1, #0x8000
	LDR     R0, [R4, #0x60]
	ADD     R0, R1
	STR     R0, [R4, #0x60]
	BX      LR

ncp_call(0x02148688, 48)
	STMFD 	SP!, {R1,R2}
	LDR 	R0, =0x2088BFC
	LDR 	R0,[R0]
	CMP 	R0,#0x7  @worldID
	LDR 	R1,=0x216FCF0
	MOVNE 	R2, #0x600
	ADDNE 	R2, $154
	MOVEQ 	R2, #0x600
	ADDEQ 	R2, $0xac
	ADDEQ   R2, $0xff
	STR 	R2, [R1]
	LDMFD 	SP!, {R1,R2}
	MOV 	R0,#0x15
	BX 		LR

ncp_call(0x021481BC, 48)
	LDR 	R0, =0x2088BFC
	LDR 	R0,[R0]
	CMP 	R0,#0x7  @worldID
	MOVEQ 	R0, #0x160
	ADDEQ 	R0, R0, #0xD
	MOVNE 	R0, #0x8C
	BX 		LR

@Lava retextures
ncp_call(0x02165a38, 54)
ncp_call(0x02165b9c, 54)
	LDR		R0, =0x02088BFC
	LDR		R0, [R0]
	CMP		R0, #7
	LDREQ           R0, =2128 - 0x83
    BXEQ            LR
	LDR             R0, =0x617
	BX              LR

ncp_call(0x0213f004, 24)
ncp_call(0x0213ec4c, 24)
	LDR		R0, =0x02088BFC
	LDR		R0, [R0]
	CMP		R0, #7
	LDREQ           R0, =2090 - 131
    BXEQ            LR
	LDR             R0, =1414 - 131
	BX              LR
	
@ Buzzy Beetles can hit Grounded Piranha Plants:
ncp_call(0x021765C8,68)					
	LDR   	LR, =0x2178FB4
	LDR   	R0, =0x00F30004  
	STR 	R0, [LR,#0x10]
	B       0x021765CC

@ScaleAble snailcorns
ncp_call(0x02141F74, 0x24)
	LDR 	R1, [R4,#8]
	ANDS    R1, R1, #0xF	
	LDR     R5, =0x2143584
	LDR     R0, =0x00008000
	STREQ   R0, [R5, #4]
	BEQ     .TileCols
	MOV     R2, #0x1200
	MUL     R2, R1
	ADD   	R0, R2
	STR     R0, [R5, #4]

.TileCols:
	CMP		R1, #0
	MOVEQ	R1, #0x1000
	BXEQ    LR
	ADD     R1, #1
	MOV     R0, #0x200
	MUL     R0, R1
	MOV     R1, #0x1000
	ADD     R1, R0, R1
	BX 		LR

@Snow retextures
ncp_call(0x20D7AEC, 10)                                @Snow 1
ncp_call(0x20D7838, 10)
	LDR		R0, =0x02088BFC
	LDR		R0, [R0]
	CMP		R0, #1
	LDREQ           R0, =2095 - 0x83
    BXEQ            LR
	CMP		R0, #4
	LDREQ           R0, =2095 - 0x83
    BXEQ            LR
	LDR             R0, =0x6EB
	BX              LR

ncp_call(0x20D7AC8, 10)                                @Snow 2
ncp_call(0x20D7844, 10) 
	LDR		R0, =0x02088BFC
	LDR		R0, [R0]
	CMP		R0, #1
	LDREQ           R0, =2096 - 0x83
    BXEQ            LR
	CMP		R0, #4
	LDREQ           R0, =2096 - 0x83
    BXEQ            LR
	LDR             R0, =0x6EC
	BX              LR

ncp_call(0x20D7AA4, 10)	                            @Snow 3
ncp_call(0x20D7850, 10)
	LDR		R0, =0x02088BFC
	LDR		R0, [R0]
	CMP		R0, #1
	LDREQ           R0, =2097 - 0x83
    BXEQ            LR
	CMP		R0, #4
	LDREQ           R0, =2097 - 0x83
    BXEQ            LR
	LDR             R0, =0x6ED
	BX              LR






