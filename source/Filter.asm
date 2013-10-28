;Beware, dragons live here and they aren't afraid to use self modifying code 

; Cash Account Row
; dw 0
; code 20
; gender 1
; age: 7
; dw 1
; money:20
; height:9;

;CONFIG
;dq CAR Start
;dq CAR End
;dd DatabaseStart
;dd DatabaseCount
;dd Flags (bit order start for LSB: age,code,gender,money,height)
;dd Count




;Function _Find
;in
;ESI CONFIG
;EDI output

;out
[GLOBAL _Find]
[BITS 32]
[SECTION .text]
_Find:
	cld
	mov ESI,[ESP+8]
	mov EDI,[ESP+4]
	pusha
	movd mm0, esp ;ESP is general purpose now
	PREFETCHNTA [ESI] ;Config cached
	mov EBP,ESI ;Save config into EBP
	mov ESI, [EBP+16] ;Load database start
	mov EDX, [EBP+24] ; Load test flags

	bsf EAX,EDX ; first jump
	jz End
	cmp EAX,3
	jnge SkipReload
	mov dword [_LoopJump+1],Reload ;Loop jumps to Reload
DoPartTwo:
	
	mov EBX,[JumpArray+EAX*4]; Get a new jump after reload
	mov [_ReloadJump+1],EBX; Reload jumps to the correct insturctions
	jg InjectSearchParameters ;If EAX (Search filter) is >3 (4) then we're done here
	BTR EDX,EAX ;Reset that search filter flags
	BSF EAX,EDX ;Find a new one
	cmovnz EAX,[JumpArray+4*4] ;Money was the one we just did, so now jump to Height
	cmovz EAX,[JumpArray+4*5] ;Money was the on we just did, however it is the last one
	mov [_MoneyJump+1],EAX
InjectSearchParameters:
	;Age injection code
	mov EDX,[EBP] ;low bound
	mov ECX,[EBP+8] ;High bound
	;Executed in parallel 
	and EDX,MASK_Age
	and ECX,MASK_Age

	shr EDX, 21
	shr ECX, 21

	mov [Age+1],EDX
	sub ECX,EDX
	mov [Age+6],ECX
	;Code injection code :/
	mov EDX,[EBP] ;low bound
	mov ECX,[EBP+8] ;High bound
	;Executed in parallel 
	and EDX,MASK_Code
	and ECX,MASK_Code

	mov [Code+1],EDX
	sub ECX,EDX
	mov [Code+6],ECX

	;Gender injection code (lol)
	mov EDX,[EBP] ;low bound only
	and EDX,MASK_Gender
	mov [Gender+1],EDX
	
	;Money injection code (I wish)
	mov EDX,[EBP+4] ;low bound
	mov ECX,[EBP+12] ;High bound
	;Executed in parallel 
	and EDX,MASK_Money
	and ECX,MASK_Money

	mov [Money+1],EDX
	sub ECX,EDX
	mov [Money+6],ECX

	;Height injection code
	mov EDX,[EBP+4] ;low bound
	mov ECX,[EBP+12] ;High bound
	;Executed in parallel 
	and EDX,MASK_Height
	and ECX,MASK_Height

	shr EDX, 20
	shr ECX, 20

	mov [Height+1],EDX
	sub ECX,EDX
	mov [Height+6],ECX

	jmp _Loop
SkipReload:
	;Code is unrolled btw
	mov EBX,[JumpArray+EAX*4]; Get the first filter
	mov dword [_LoopJump+1],EBX ;Loop jumps to the first filter
	mov EBX,EAX
	BTR EDX,EAX
	BSF EAX,EDX
	jz FinishChain
	;No? We got more to done
	mov ECX,[JumpArray+EAX*4]; Get the next filter
	
	mov ESP,[SMOCEArray+EBX*4] ;Load the location of the jump for the previous filter
	
	cmp EAX,3 ;before we do anything check if we can do the part two
	cmovge ECX, [JumpArray+4*6] ;Hax 
	
	mov [ESP+1],ECX
	
	jge DoPartTwo
	
	
	mov EBX,EAX
	BTR EDX,EAX
	BSF EAX,EDX
	jz FinishChain
	;No? We got more to done
	mov ECX,[JumpArray+EAX*4]; Get the next filter
	mov ESP,[SMOCEArray+EBX*4] ;Load the location of the jump for the previous filter
	cmp EAX,3 ;before we do anything check if we can do the part two
	cmovge ECX, [JumpArray+4*6] ;Hax 
	mov [ESP+1],ECX
	jge DoPartTwo

	mov EBX,EAX
	BTR EDX,EAX
	BSF EAX,EDX
	jz FinishChain
	;No? We got more to done
	mov ECX,[JumpArray+EAX*4]; Get the next filter
	mov ESP,[SMOCEArray+EBX*4] ;Load the location of the jump for the previous filter
	cmp EAX,3 ;before we do anything check if we can do the part two
	cmovge ECX, [JumpArray+4*6] ;Hax 
	mov [ESP+1],ECX
	jge DoPartTwo

	mov EBX,EAX
	BTR EDX,EAX
	BSF EAX,EDX
	jz FinishChain

FinishChain:
	cmp EBX,4
	je InjectSearchParameters
	cmp EBX,2
	ja NoReload
	mov EBX,[SMOCEArray+EBX*4]
	mov dword [EBX+1],Reload
	mov dword [_ReloadJump+1],Success
	jmp InjectSearchParameters
NoReload:
	mov EBX,[SMOCEArray+EBX*4]
	mov dword [EBX+1],Success
	jmp InjectSearchParameters
_Loop:
	lodsd
	mov ebx,eax
_LoopJump	jmp 0x8:0xDEADC0DE

Age:
			mov edx,0xFFFFFFFF ;Will be injected
			mov ecx,0xFFFFFFFF ;Will be injected
			and eax,MASK_Age
			shr eax,21
			sub eax,edx
			cmp eax,ecx
			ja FailNoReload
_AgeJump	jmp 0x8:0xDEADC0DE

Code:
			mov edx,0xFFFFFFFF ;Will be injected
			mov ecx,0xFFFFFFFF ;Will be injected

			mov eax,ebx
			and eax,MASK_Code
			sub eax,edx
			cmp eax,ecx
			ja FailNoReload
			
_CodeJump	jmp 0x8:0xDEADC0DE

Gender: 
			mov edx,0xFFFFFFFF ;Will be injected (But we only use the lower bound)

			mov eax,ebx
			and eax,MASK_Gender
			cmp eax,edx
			jne FailNoReload
;Load cash account row part 2

Reload:
			
			lodsd
			mov ebx,eax
_ReloadJump jmp 0x8:0xDEADC0DE

Money:
			mov edx,0xFFFFFFFF ;Will be injected
			mov ecx,0xFFFFFFFF ;Will be injected
			
			mov eax,ebx
			and eax,MASK_Money
			sub eax,edx
			cmp eax,ecx
			
			ja Fail

_MoneyJump	jmp 0x8:0xDEADC0DE

Height:
			mov edx,0xFFFFFFFF ;Will be injected
			mov ecx,0xFFFFFFFF ;Will be injected

			mov eax,ebx
			and eax,MASK_Height
			shr eax,20
			sub eax,edx
			cmp eax,ecx
			ja Fail
			

Success:
inc dword [EBP+28]
sub ESI, 8
movsd
movsd

Fail:
dec dword [EBP+20]
jnz _Loop
jmp End

FailNoReload:
lodsd
dec dword [EBP+20]
jnz _Loop




End:
;done
movd esp,mm0
popa
ret

[SECTION .data]
JumpArray:
	dd Age
	dd Code
	dd Gender
	dd Money
	dd Height
	dd Success
	dd Reload ;just so I can hack
SMOCEArray:
	dd _AgeJump
	dd _CodeJump
	dd _ReloadJump
	dd _MoneyJump
	dd 0xDEADC0DE

MASK_Code equ   0000000011111111111111111111b
MASK_Gender equ 0000000100000000000000000000b ;Actually bit test
MASK_Age equ    1111111000000000000000000000b
MASK_Money equ  00000000011111111111111111111b
MASK_Height equ 11111111100000000000000000000b