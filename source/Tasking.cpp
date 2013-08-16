#include "Tasking/Tasking.h"
#include "Memory/Memory.h"
#include "Graphics/Graphics.h"


Tasking::Thread* NextThread;
volatile bool SwitchRequested=false;
extern "C" void GetThread(Registers* registers)
{
    Tasking::GetNextThread(registers);
}
namespace Tasking
{
    TaskQueue Queue;
	Thread* Running;
    Process* KernelProcess;
	bool TaskingEnabled=false;

    void Initialize()
    {
		Queue.Allocate();
        KernelProcess=(Process*)Memory::StaticMalloc(sizeof(Process));
        KernelProcess->GlobalDirectory=Memory::Virtual::Kernel;
        KernelProcess->ID=0;
        KernelProcess->Name="Hydra Operating System Kernel";
        
        Thread* KernelThread=(Thread*)Memory::StaticMalloc(sizeof(Thread));
        KernelThread->Blocking=false;
        KernelThread->ID=KernelProcess->NextID++;
        KernelThread->Parent=KernelProcess;
        KernelThread->ThreadDirectory=KernelProcess->GlobalDirectory;
        KernelThread->TimeLeft=0;
		Running=KernelThread;
		Queue.Enqueue(KernelThread);
        SwitchRequested=true;
		TaskingEnabled=true;
        __asm volatile("sti");
        while (SwitchRequested)
        {
            __asm volatile("nop":::"memory");
        }
    }

    Thread* CreateThread(Process* process,void(*function)())
    {
        __asm volatile("cli");
        Thread* NewThread=(Thread*)Memory::StaticMalloc(sizeof(Thread));
        NewThread->Blocking=false;
        NewThread->ID=process->NextID++;
        NewThread->Parent=process;
       
        NewThread->ThreadDirectory=Memory::Virtual::CreateShadow(process->GlobalDirectory);
        NewThread->TimeLeft=20;
		NewThread->ESP0=0xA0000FFC-sizeof(Registers);
		Memory::Virtual::Page* StackPage= Memory::Virtual::GetPage(NewThread->ThreadDirectory,0xA0000000);
        Memory::Virtual::AllocatePage(StackPage);
		Memory::Virtual::AllocatePage(Memory::Virtual::GetPage(NewThread->ThreadDirectory,0x100000-4));
		Registers* ThreadData=NewThread->GetTemporaryRegisters();
        ThreadData->EFLAGS=0x202;
        ThreadData->CS=0x8;
        ThreadData->DS=
            ThreadData->SS=0x10;
        ThreadData->EIP=(unsigned int)function;
		ThreadData->OldESP=0x100000-4;
		Memory::Virtual::UnmapTemporary();
       

        
		Queue.Enqueue(NewThread);

        __asm volatile("sti");
        return NewThread;
    }
    Thread* GetRunning()
    {
		return Running;
    }
	void Exit()
	{
		Graphics::GetConsole()->WriteLine("Exit requested");
		while (true)
		{
			__asm volatile("hlt":::"memory");
		}
	}
    void GetNextThread(Registers* registers)
    {
		Running->ESP0=(unsigned int)registers;
		if(!Running->Blocking)
			Queue.Enqueue(Running);
		Running=NextThread=Queue.Dequeue();
		if(NextThread==0)
		{
			*Graphics::GetConsole()<<RED<<"Nothing to run";
			Graphics::ForceRefresh();
			__asm("cli");
			__asm("hlt");
		}
        NextThread->TimeLeft=20;
		
    }

    void Switch()
    {
		if(TaskingEnabled)
            SwitchRequested=true;
    }
}