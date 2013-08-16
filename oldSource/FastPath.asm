[GLOBAL _PhysicalGetPage]
[EXTERN _Superblock]
[EXTERN _Bitmap]
_PhysicalGetPage:
push ESI
mov ESI,[_Superblock]
mov ECX, 32 ;Superblock has 32 dwords
innerLoopSuper:
	mov EDX,32 ;Convert to zero offset
	sub EDX,ECX
	mov EAX,[ESI+EDX*4] ;Load the value
	not EAX; Reverse
	bsf EAX,EAX ;Find first free bitmap block
	jnz scanBitmap ;if we found, jump
loop innerLoopSuper
pop ESI
mov eax,-1
ret
scanBitmap:
shl EDX,5
add EAX,EDX
shl EAX,5
mov ESI,[_Bitmap]
lea ESI,[ESI+EAX*4]
mov ECX,32 ;Bitmapblock has 32 dwords
innerLoopBitmap:
	mov EDX,32 ;Convert to zero offset
	sub EDX,ECX
	mov EDX,[ESI+EDX*4] ;Load the value
	not EDX; Reverse
	bsf EDX,EDX ;Find first free page
	jnz returnPage ;if we found, jump
loop innerLoopBitmap
pop ESI
mov eax,-1
ret
returnPage:
pop ESI
shl EAX,5
add EAX,ECX
sub EAX,32
add EAX,EDX ; Add page number

ret
