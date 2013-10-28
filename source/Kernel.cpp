#include "Graphics/Graphics.h"
#include "Graphics/Console.h"
#include "Kernel.h"
#include "Globals.h"
#include "Memory/Memory.h"
#include "Tasking/Tasking.h"
#include "Hardware/Clock.h"
#include "Hardware/RTC.h"
#include "Hardware/PIC.h"
#include "Memory/Physical.h"
#include "Boot/GDT.h"
#include "Boot/IDT.h"
#include "Memory/Virtual.h"
#include "Memory/Logical.h"
#include "API/Symbols.h"
#include "Multiboot.h"
class Console Console;

IndirectNamespaceReference(Memory,unsigned int MemoryStart);
void IdleThread()
{
	Console<<RED<<"Idle thread created"<<WHITE<<endl;
    while (true)
    {
		Hardware::Clock::Sleep(Seconds);
		Console << LIGHT_BROWN << "Idle thread tick" << WHITE << endl;
        __asm volatile("hlt":::"memory");
    }
	
}

void Int3(Registers* registers)
{
	Console<<LIGHT_BROWN<<" EAX: "<<registers->EAX<<" EBX: "<<registers->EBX<<" EDX: "<<registers->EDX<<" OldESP:"<<registers->OldESP<<endl;
}
extern "C" void KernelStart(MultibootInfo* BootData,unsigned int Magic)
{
    BootData->TableUnition.Elf_sec.Addr+=KernelVirtualBase;
    Memory::MemoryStart=KernelVirtualBase+0x200000;
    Memory::Memory_Clear((void*)Memory::MemoryStart,(KernelVirtualBase+0x800000)-Memory::MemoryStart);
	
    Console.Initialise_NoGraphics();
    Graphics::Initialise_NoGraphics();
    Graphics::SetConsole(&Console);
    Console<<"Kernel built on: "<<LIGHT_BLUE<<__TIMESTAMP__<<endl<<WHITE;
	
    Console.WriteLineSubsystem("Begin initalization sequence...","Kernel",1);
    Memory::Physical::Initialize(BootData);
    Memory::Virtual::Initialize();
	

    GDT::Initialize();
    IDT::Initialize();
	IDT::EventHandlers[3]+=(void(*)(void*))Int3;
	Memory::Logical::Initialize();
    Hardware::PIC::Initialize();
	Hardware::RTC::Initialize();
    Hardware::Clock::Initialize();
    //Hardware::ACPI::Initialize();
    Symbols::Initialize(BootData);
	
    Tasking::Initialize();

    Console.WriteLineSubsystem("Systems initialized","Kernel",1);
	
	Graphics::ForceRefresh();
}



extern "C" void KernelEnd()
{
    for(;;)
    {
		
        __asm volatile("hlt");
    }
}