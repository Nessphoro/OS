[GLOBAL _IDT_Flush]    ; Allows the C code to call idt_flush().

_IDT_Flush:
    mov eax, [esp+4]  ; Get the pointer to the IDT, passed as a parameter. 
    lidt [eax]        ; Load the IDT pointer.
    ret

%macro ISR_NOERRCODE 1  ; define a macro, taking one parameter
   [GLOBAL _ISR%1]        ; %1 accesses the first parameter.
   _ISR%1:
     cli
     push byte 0
     push byte %1
     jmp ISR_Common_Stub
%endmacro

%macro IRQ 1  ; define a macro, taking one parameter
   [GLOBAL _IRQ%1]        ; %1 accesses the first parameter.
   _IRQ%1:
     cli
     push byte 0
     push byte %1
     jmp IRQ_Common_Stub
%endmacro

%macro ISR_ERRCODE 1
   [GLOBAL _ISR%1]
   _ISR%1:
     cli
     push byte %1
     jmp ISR_Common_Stub
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
IRQ 32
IRQ 33
IRQ 34
IRQ 35
IRQ 36
IRQ 37
IRQ 38
IRQ 39
IRQ 40
IRQ 41
IRQ 42
IRQ 43
IRQ 44
IRQ 45
IRQ 46
IRQ 47

[GLOBAL INT_Nesting_Level]
INT_Nesting_Level dd 0


EXTERN _ExceptionHandler
EXTERN _IRQHandler
ISR_Common_Stub:
	pushad                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
	xor eax, eax
    mov ax, ds               ; Lower 16-bits of eax = ds.
    push eax                 ; save the data segment descriptor

    mov ax, 0x10  ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

	inc dword [INT_Nesting_Level]
	push ESP
    call _ExceptionHandler
	ADD ESP,4
	jmp INT_END


IRQ_Common_Stub:
	pushad                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
	xor eax, eax
    mov ax, ds               ; Lower 16-bits of eax = ds.
    push eax                 ; save the data segment descriptor

    mov ax, 0x10  ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

	inc dword [INT_Nesting_Level]
	sti
	push ESP
    call _IRQHandler
	ADD ESP,4


extern _SwitchRequested
extern _GetThread
extern _NextThread
extern _SwichDirectoryASM
INT_END:
	cli
	dec dword [INT_Nesting_Level]
	jnz Finalize
	cmp byte [_SwitchRequested],1
	jne Finalize
	mov byte [_SwitchRequested],0
	sub esp,4
	call _GetThread
	add esp,4
	call _SwichDirectoryASM
	mov cr3,eax
	;We're now in the next threads Virtual Memory
	MOV EAX,[_NextThread] ;Get the Thread struct address, again
	MOV EAX,[EAX]
	MOV ESP,EAX

Finalize:
	pop eax        ; reload the original data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popad                     ; Pops edi,esi,ebp...
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    sti
    iret