#include "Graphics/Graphics.h"
#include "Graphics/Console.h"
#include "Kernel.h"
#include "Globals.h"
#include "Memory/Memory.h"
#include "Tasking/Tasking.h"
#include "Hardware/Clock.h"
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
        __asm volatile("hlt":::"memory");
    }
	
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
    Hardware::PIC::Initialize();
    Hardware::Clock::Initialize();
    //Hardware::ACPI::Initialize();
    Symbols::Initialize(BootData);
	
    Tasking::Initialize();
    Console.WriteLineSubsystem("Systems initialized","Kernel",1);
	Memory::Logical::Initialize();
	
	
	unsigned int someData1=(unsigned int)Memory::Logical::Allocate(16);
	unsigned int someData2=(unsigned int)Memory::Logical::Allocate(16);
		Console<<"First chunk: "<<someData1<<endl;
	Console<<"Second chunk: "<<someData2<<endl;
	Memory::Logical::Deallocate((unsigned int*)someData1);

	unsigned int someData3=(unsigned int)Memory::Logical::Allocate(16);

	Console<<"Third chunk: "<<someData3<<endl;
	Tasking::Thread* thread = Tasking::CreateThread(Tasking::KernelProcess,IdleThread);
	Graphics::ForceRefresh();
}



extern "C" void KernelEnd()
{
    for(;;)
    {
		
        __asm volatile("hlt");
    }
}