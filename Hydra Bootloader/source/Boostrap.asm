[BITS 16]
[ORG 0x7C00]
xor AX,AX
mov BX,AX
mov CX,AX
mov DI,AX
mov SI,AX
mov DS,AX
mov SS,AX
;Establish a standardized environment
mov AH,0x42
mov SI,DiskPacket
int 0x13
;Load second stage from boot disk
or AH,AH
jnz ErrorLoading
jmp 0x0:0x500 ;Jump to second stage
ErrorLoading:
mov SI, errorMsg
print_loop:
lodsb
or AL,AL
jz hang
mov AH, 0x0E
int 0x10
jmp print_loop

hang:
	hlt
	jmp hang

errorMsg db 'Error loading second stage', 0
ALIGN 4
DiskPacket:
db 16
db 0
TransferSectors:
dw 32
PacketOffset:
dw 0x500
PacketSegment:
dw 0
StartLBA:
dd 1 ;Second stage start
UpperLBA:
dd 0