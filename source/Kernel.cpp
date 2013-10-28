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

struct CashAccountRow
{
	unsigned int Code:20;
	unsigned int Gender:1;
	unsigned int Age:7;
	unsigned int:0;
	unsigned int Money:20;
	unsigned int Height:9;
};
enum DatabaseFlags
{
	Age_Filter=1,Code_Filter=2,Gender_Filter=4,Money_Filter=8,Height_Filter=16
};
struct DatabaseConfiguration
{
	CashAccountRow Start;
	CashAccountRow End;
	void* DatabaseStart;
	unsigned int DatabaseCount;
	unsigned int Flags;
	unsigned int Count;
};
CashAccountRow Database[5];
DatabaseConfiguration Config;

extern "C" void Find(CashAccountRow* destination,DatabaseConfiguration* config);
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
	
	Console<<"Size of cash row: "<<(unsigned int)sizeof(CashAccountRow)<<endl;
	Database[0].Code=0;
	Database[0].Age=20;
	Database[0].Gender=1;
	Database[0].Height=100;
	Database[0].Money=400;

	Database[1].Code=10;
	Database[1].Age=20;
	Database[1].Gender=1;
	Database[1].Height=120;
	Database[1].Money=6000;

	Database[2].Code=2;
	Database[2].Age=20;
	Database[2].Gender=1;
	Database[2].Height=180;
	Database[2].Money=1000;

	Database[3].Code=1;
	Database[3].Age=20;
	Database[3].Gender=0;
	Database[3].Height=180;
	Database[3].Money=600;

	Database[4].Code=0;
	Database[4].Age=20;
	Database[4].Gender=1;
	Database[4].Height=100;
	Database[4].Money=5000;
	
	Config.Start.Money=0;
	Config.Start.Age=20;
	Config.Start.Gender=0;
	Config.End.Money=400;
	Config.End.Age=20;
	Config.DatabaseCount=5;
	Config.DatabaseStart=Database;
	Config.Flags=Gender_Filter;

	CashAccountRow tempRow[5];
	__asm volatile("cli");
	Find(tempRow,&Config);
	__asm volatile("sti");
	Console<<Config.Count<<endl;
	/*
	unsigned int someData1=(unsigned int)Memory::Logical::Allocate(16);
	unsigned int someData2=(unsigned int)Memory::Logical::Allocate(16);
		Console<<"First chunk: "<<someData1<<endl;
	Console<<"Second chunk: "<<someData2<<endl;
	Memory::Logical::Deallocate((unsigned int*)someData1);

	unsigned int someData3=(unsigned int)Memory::Logical::Allocate(16);

	Console<<"Third chunk: "<<someData3<<endl;
	*/
	//Tasking::Thread* thread = Tasking::CreateThread(Tasking::KernelProcess,IdleThread);
	Graphics::ForceRefresh();
}



extern "C" void KernelEnd()
{
    for(;;)
    {
		
        __asm volatile("hlt");
    }
}