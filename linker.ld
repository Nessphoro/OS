ENTRY(loader)
SECTIONS{
    . = 0xC0100000;

	.text : AT(ADDR(.text) - 0xC0000000) {
	   *(.magic)
       *(.text)
   }

   .data ALIGN (0x1000) : AT(ADDR(.data) - 0xC0000000) {
       *(.data)
	   *(.rodata*)
   }

   

   .bss : AT(ADDR(.bss) - 0xC0000000) {
       _sbss = .;
       *(COMMON)
       *(.bss)
       _ebss = .;
	}
	
	_KernelMemoryEnd=.;
}