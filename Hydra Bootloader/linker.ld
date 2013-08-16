OUTPUT_FORMAT(binary)
SECTIONS{
    . = 0x500;

	.text : AT(ADDR(.text)) {
	   *(.magic)
       *(.text)
   }

   .data : AT(ADDR(.data)) {
       *(.data)
	   *(.rodata*)
		
	   
   }
   .bss : AT(ADDR(.bss)) {
       _sbss = .;
       *(COMMON)
       *(.bss)
       _ebss = .;
	}
}