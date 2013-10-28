#include "Boot/IDT.h"
#include "Memory/Memory.h"
#include "Memory/Virtual.h"
#include "Graphics/Graphics.h"
#include "Hardware/Hardware.h"
#include "Hardware/Clock.h"
extern "C" void ISR0 ();
extern "C" void ISR1 ();
extern "C" void ISR2 ();
extern "C" void ISR3 ();
extern "C" void ISR4 ();
extern "C" void ISR5 ();
extern "C" void ISR6 ();
extern "C" void ISR7 ();
extern "C" void ISR8 ();
extern "C" void ISR9 ();
extern "C" void ISR10();
extern "C" void ISR11();
extern "C" void ISR12();
extern "C" void ISR13();
extern "C" void ISR14();
extern "C" void ISR15();
extern "C" void ISR16();
extern "C" void ISR17();
extern "C" void ISR18();
extern "C" void ISR19();
extern "C" void ISR20();
extern "C" void ISR21();
extern "C" void ISR22();
extern "C" void ISR23();
extern "C" void ISR24();
extern "C" void ISR25();
extern "C" void ISR26();
extern "C" void ISR27();
extern "C" void ISR28();
extern "C" void ISR29();
extern "C" void ISR30();
extern "C" void ISR31();

extern "C" void IRQ32();
extern "C" void IRQ33();
extern "C" void IRQ34();
extern "C" void IRQ35();
extern "C" void IRQ36();
extern "C" void IRQ37();
extern "C" void IRQ38();
extern "C" void IRQ39();
extern "C" void IRQ40();
extern "C" void IRQ41();
extern "C" void IRQ42();
extern "C" void IRQ43();
extern "C" void IRQ44();
extern "C" void IRQ45();
extern "C" void IRQ46();
extern "C" void IRQ47();

extern "C" void IDT_Flush(void* Pointer);

extern "C" void ExceptionHandler(Registers* registers)
{
    IDT::InternalHandler(registers);
}
extern "C" void IRQHandler(Registers* registers)
{
    IDT::InternalHandler(registers);
    if (registers->INT >= 40)
    {
        Hardware::Outb(0xA0, 0x20);
    }
    Hardware::Outb(0x20, 0x20);
}


namespace IDT
{
    IDT_Entry Entries[256];
    EventHandler EventHandlers[256];
    IDT_Pointer Pointer;


    bool Initialize()
    {
        Pointer.Limit=sizeof(IDT_Entry)*256-1;
        Pointer.Base=&Entries[0];

        Memory::Memory_Clear(Entries,256*sizeof(IDT_Entry));

        Hardware::Outb(0x20, 0x11);
        Hardware::Outb(0xA0, 0x11);
        Hardware::Outb(0x21, 0x20);
        Hardware::Outb(0xA1, 0x28);
        Hardware::Outb(0x21, 0x04);
        Hardware::Outb(0xA1, 0x02);
        Hardware::Outb(0x21, 0x01);
        Hardware::Outb(0xA1, 0x01);
        Hardware::Outb(0x21, 0x0);
        Hardware::Outb(0xA1, 0x0);

        SetGate( 0, (unsigned int)ISR0 , 0x08, 0x8E);
        SetGate( 1, (unsigned int)ISR1 , 0x08, 0x8E);
        SetGate( 2, (unsigned int)ISR2 , 0x08, 0x8E);
        SetGate( 3, (unsigned int)ISR3 , 0x08, 0x8E);
        SetGate( 4, (unsigned int)ISR4 , 0x08, 0x8E);
        SetGate( 5, (unsigned int)ISR5 , 0x08, 0x8E);
        SetGate( 6, (unsigned int)ISR6 , 0x08, 0x8E);
        SetGate( 7, (unsigned int)ISR7 , 0x08, 0x8E);
        SetGate( 8, (unsigned int)ISR8 , 0x08, 0x8E);
        SetGate( 9, (unsigned int)ISR9 , 0x08, 0x8E);
        SetGate(10, (unsigned int)ISR10, 0x08, 0x8E);
        SetGate(11, (unsigned int)ISR11, 0x08, 0x8E);
        SetGate(12, (unsigned int)ISR12, 0x08, 0x8E);
        SetGate(13, (unsigned int)ISR13, 0x08, 0x8E);
        SetGate(14, (unsigned int)ISR14, 0x08, 0x8E);
        SetGate(15, (unsigned int)ISR15, 0x08, 0x8E);
        SetGate(16, (unsigned int)ISR16, 0x08, 0x8E);
        SetGate(17, (unsigned int)ISR17, 0x08, 0x8E);
        SetGate(18, (unsigned int)ISR18, 0x08, 0x8E);
        SetGate(19, (unsigned int)ISR19, 0x08, 0x8E);
        SetGate(20, (unsigned int)ISR20, 0x08, 0x8E);
        SetGate(21, (unsigned int)ISR21, 0x08, 0x8E);
        SetGate(22, (unsigned int)ISR22, 0x08, 0x8E);
        SetGate(23, (unsigned int)ISR23, 0x08, 0x8E);
        SetGate(24, (unsigned int)ISR24, 0x08, 0x8E);
        SetGate(25, (unsigned int)ISR25, 0x08, 0x8E);
        SetGate(26, (unsigned int)ISR26, 0x08, 0x8E);
        SetGate(27, (unsigned int)ISR27, 0x08, 0x8E);
        SetGate(28, (unsigned int)ISR28, 0x08, 0x8E);
        SetGate(29, (unsigned int)ISR29, 0x08, 0x8E);
        SetGate(30, (unsigned int)ISR30, 0x08, 0x8E);
        SetGate(31, (unsigned int)ISR31, 0x08, 0x8E);
        SetGate(32, (unsigned int)IRQ32, 0x08, 0x8E);
        SetGate(33, (unsigned int)IRQ33, 0x08, 0x8E);
        SetGate(34, (unsigned int)IRQ34, 0x08, 0x8E);
        SetGate(35, (unsigned int)IRQ35, 0x08, 0x8E);
        SetGate(36, (unsigned int)IRQ36, 0x08, 0x8E);
        SetGate(37, (unsigned int)IRQ37, 0x08, 0x8E);
        SetGate(38, (unsigned int)IRQ38, 0x08, 0x8E);
        SetGate(39, (unsigned int)IRQ39, 0x08, 0x8E);
        SetGate(40, (unsigned int)IRQ40, 0x08, 0x8E);
        SetGate(41, (unsigned int)IRQ41, 0x08, 0x8E);
        SetGate(42, (unsigned int)IRQ42, 0x08, 0x8E);
        SetGate(43, (unsigned int)IRQ43, 0x08, 0x8E);
        SetGate(44, (unsigned int)IRQ44, 0x08, 0x8E);
        SetGate(45, (unsigned int)IRQ45, 0x08, 0x8E);
        SetGate(46, (unsigned int)IRQ46, 0x08, 0x8E);
        SetGate(47, (unsigned int)IRQ47, 0x08, 0x8E);
        for(int i=0;i<256;i++)
        {
            EventHandlers[i]=EventHandler();
        }
		IDT::EventHandlers[14]+=(void(*)(void*))Memory::Virtual::PageFault;
        IDT_Flush(&Pointer);
        return true;
    }
    bool Deinitialize()
    {
        return false;
    }
    void SetGate(unsigned char number,unsigned int base,unsigned short selector,unsigned char flags)
    {
        Entries[number].Base_Low = base & 0xFFFF;
        Entries[number].Base_High = (base >> 16) & 0xFFFF;

        Entries[number].Selector     = selector;
        Entries[number].Always0 = 0;
        Entries[number].Flags   = flags;
    }
    void InternalHandler(Registers* registers)
    {
        EventHandlers[registers->INT](registers);
    }
}