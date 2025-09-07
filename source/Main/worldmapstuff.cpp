


ncp_jump(0x020CE1E4, 8)
  void worldmapmangel() {asm(R"(
    ADD		R0, #0x8100
	BX		LR
)");}

ncp_call(0x020D5354, 8)                              
  void worldmapactorspeed() {asm(R"(
    MOV     R0, #0x1800
   BX      LR
)");}


ncp_call(0x020D6BC8, 8)                              
  void worldmapmovment() {asm(R"(
    MOV      R2, #0
    BX       LR
)");}


ncp_jump(0x020D2718, 9)   
  void NewFileSceneDissable() {asm(R"(
B      0x020D272C
)");}
// Makes camera always follow player! somewhat jank
ncp_jump(0x20d7880,8) // WMController::onRender
  static void setWMCamPosition() {asm(R"(
    ldr     r0, =0x20d8174         @ global camPosition
    ldr     r0, [r0]
    ldr     r1, [r4, #0xe20]    @ cursorPosition.x
    str     r1, [r0, #4]
    ldr     r1, [r4, #0xe24]    @ cursorPosition.y
    str     r1, [r0, #8]
    ldr     r1, [r4, #0xe28]    @ cursorPosition.z
    str     r1, [r0, #12]
    b       0x20d7898
)");}