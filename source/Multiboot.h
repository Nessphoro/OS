#pragma once
/* The magic number passed by a Multiboot-compliant boot loader. */
#define MULTIBOOT_BOOTLOADER_MAGIC      0x2BADB002

/* The size of our stack (16KB). */
#define STACK_SIZE                      0x4000

/* Types. */

/* The Multiboot header. */
struct MultibootHeader
{
    unsigned int Magic;
    unsigned int Flags;
    unsigned int Checksum;
    unsigned int Header_addr;
    unsigned int Load_addr;
    unsigned int Load_end_addr;
    unsigned int Bss_end_addr;
    unsigned int Entry_addr;
};

/* The symbol table for a.out. */
struct AOUTSymbolTable
{
    unsigned int Tabsize;
    unsigned int Strsize;
    unsigned int Addr;
    unsigned int Reserved;
};

struct ELFSectionHeader
{
    unsigned int Name;
    unsigned int Type;
    unsigned int Flags;
    unsigned int Address;
    unsigned int Offset;
    unsigned int Size;
    unsigned int Link;
    unsigned int Info;
    unsigned int AddressAlign;
    unsigned int EntrySize;
};

/* The section header table for ELF. */
struct ELFSectionHeaderTable
{
    unsigned int Num;
    unsigned int Size;
    unsigned int Addr;
    unsigned int Shndx;
};

/* The Multiboot information. */
struct MultibootInfo
{
    unsigned int Flags;
    unsigned int Mem_lower;
    unsigned int Mem_upper;
    unsigned int Boot_device;
    unsigned int Cmdline;
    unsigned int Mods_count;
    unsigned int Mods_addr;
    union
    {
        AOUTSymbolTable Aout_sym;
        ELFSectionHeaderTable Elf_sec;
    } TableUnition;
    unsigned int Mmap_length;
    unsigned int Mmap_addr;
};

/* The module structure. */
struct Module
{
    unsigned int Mod_start;
    unsigned int Mod_end;
    unsigned int String;
    unsigned int Reserved;
};

/* The memory map. Be careful that the offset 0 is base_addr_low
but no size. */
struct MemoryMap
{
    unsigned int Size;
    unsigned int Base_addr_low;
    unsigned int Base_addr_high;
    unsigned int Length_low;
    unsigned int Length_high;
    unsigned int Type;
};