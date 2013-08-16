[Global Print]
[SECTION .text]
Print:
push AX
PrintLoop:
lodsb
or AL,AL
jz PrintReturn
mov AH, 0x0E
int 0x10
jmp PrintLoop
PrintReturn:
pop AX
ret 