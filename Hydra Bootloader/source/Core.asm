%INCLUDE "Core_Includes.inc"
[BITS 16]
mov byte [bootDisk],DL
mov AX,0x7000
mov SS,AX
xor AX,AX
mov SP,0xFFFF
mov SI, loadMessage
call Print
mov SI,payLoadLocation
call Print

mov AX,[payLoadLocation]
sub AL,0x30

cmp AH,0x2F; Is the location in a valid format
je LocationChecked 

InvalidLocation:
mov SI,errorLocationCheck
call Print
jmp hang

LocationChecked:
cmp AL,4
jge InvalidLocation

xor AH,AH
mov BX,AX
shl BX,4
add BX,0x7DBE
mov AX,[BX+4] ;load partition bx type
test AL,0xB; check for fat32
je PartitionChecked
mov SI,errorLocationCheck
call Print
PartitionChecked:
hang:
	cli
	hlt
	jmp hang

[SECTION .data]
bootDisk db 0x80
errorLocationCheck db "Invalid package location format",13,10,0
errorInvalidPartitionType db "Invalid partition type",13,10,0
loadMessage db 'Loading package ', 0
payLoadLocation db '0/payload.hbl',13,10,0
ALIGN 4
DiskPacket:
db 16
db 0
TransferSectors:
dw 1
PacketOffset:
dw 00
PacketSegment:
dw 0
StartLBA:
dd 0
UpperLBA:
dd 0
[SECTION .bss]
numberBuffer resd 1