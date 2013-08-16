global loader
global _KernelDirectory
extern _KernelEnd             
extern _KernelStart

MODULEALIGN equ  1<<0                  ; align loaded modules on page boundaries
MEMINFO     equ  1<<1                   ; provide memory map
FLAGS       equ  MODULEALIGN | MEMINFO  ; this is the Multiboot 'flag' field
MAGIC       equ    0x1BADB002           ; 'magic number' lets bootloader find the header
CHECKSUM    equ -(MAGIC + FLAGS)        ; checksum required
STACKSIZE equ 0x8000                ; that's eh 32 kb, I think.


VGA_MISC_WRITE		EQU	3C2h
VGA_SEQ_INDEX		EQU	3C4h
VGA_SEQ_DATA		EQU	3C5h

VGA_CRTC_INDEX		EQU	3D4h
VGA_CRTC_DATA		EQU	3D5h
VGA_INSTAT_READ		EQU	3DAh

NUM_SEQ_REGS		EQU	5
NUM_CRTC_REGS		EQU	25

KERNEL_VIRTUAL_BASE equ 0xC0000000 
KERNEL_PAGE_NUMBER equ (KERNEL_VIRTUAL_BASE >> 12)
section .data
regs_90x60:
; MISC
	db 0E7h
; SEQuencer
	db 03h, 01h, 03h, 00h, 02h
; CRTC
	db  6Bh, 59h,  5Ah, 82h, 60h,  8Dh, 0Bh,  3Eh,
	db  00h, 47h,  06h, 07h, 00h,  00h, 00h,  00h,
	db 0EAh, 0Ch, 0DFh, 2Dh, 08h, 0E8h, 05h, 0A3h,
	db 0FFh
align 0x1000
IndetityTable1:
	times 1024 dd 0
IndetityTable2:
	times 1024 dd 0
align 0x1000
_KernelDirectory:
Directory:
	times 2048 dd 0



section .magic
align 4
MultiBootHeader:
   dd MAGIC
   dd FLAGS
   dd CHECKSUM
; reserve initial kernel stack space


section .text
loader:
   cli
   ;Create the identity table
   push eax
   xor ecx,ecx
   CreationLoop:
	mov eax, ecx
	shl eax,12
	or eax,0x3
	MOV [IndetityTable1-KERNEL_VIRTUAL_BASE+ecx*4],eax

	inc ecx
	cmp ecx,1024
	jne CreationLoop
	
	mov ecx,0
	CreationLoop2:
	mov eax, ecx
  add eax, 1024
	shl eax,12
	or eax,0x3
	MOV [IndetityTable2-KERNEL_VIRTUAL_BASE+ecx*4],eax

	inc ecx
	cmp ecx,1024
	jne CreationLoop2

	pop eax


   mov ecx,IndetityTable1
   sub ecx,KERNEL_VIRTUAL_BASE
   and ecx,0xFFFFF000
   or ecx,0x3
   ;Now map 
   mov [(Directory - KERNEL_VIRTUAL_BASE)],ecx
   mov [(Directory - KERNEL_VIRTUAL_BASE+0xC00)],ecx
   ;Map 2
   mov ecx,IndetityTable2
   sub ecx,KERNEL_VIRTUAL_BASE
   and ecx,0xFFFFF000
   or ecx,0x3
   mov [(Directory - KERNEL_VIRTUAL_BASE+0xC00 + 4)],ecx

   ;Aaand set the directory
   mov ecx, (Directory - KERNEL_VIRTUAL_BASE)
   mov cr3, ecx
   mov ecx, cr0
   or ecx,0x80000000
   mov cr0,ecx
   ;We're paging now, jump 3GB forward
   lea ecx,[StartInHigherHalf]
   jmp ecx
   
StartInHigherHalf:
   mov DWORD [(Directory - KERNEL_VIRTUAL_BASE)],0
   mov ecx, (Directory - KERNEL_VIRTUAL_BASE)
   ;And flush all that junk we don't need anymore
   mov cr3,ecx
   mov ebp,stack+STACKSIZE
   mov esp, stack+STACKSIZE
   ;Set up the stack and push some things to the kernel
   push eax
   ;And do a favor settings the right address for the BootData from GRUB/Whatever
   add ebx,KERNEL_VIRTUAL_BASE
   push ebx
   call  _KernelStart                       ; call kernel proper
   add esp,8
   call _KernelEnd
   cli
hang:
	hlt                               ; halt machine should kernel return
   jmp   hang 

[GLOBAL _GDT_Flush]    ; Allows the C code to call gdt_flush().

_GDT_Flush:
    mov eax, [esp+4]  ; Get the pointer to the GDT, passed as a parameter.
    lgdt [eax]        ; Load the new GDT pointer

    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax        ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush   ; 0x08 is the offset to our code segment: Far jump!
.flush:
    ret

[GLOBAL _ReadEIP]
_ReadEIP:
   pop eax
   jmp eax       
section .bss

align 4
stack:
   resb STACKSIZE                 