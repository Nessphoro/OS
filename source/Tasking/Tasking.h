#pragma once
#include "Boot/IDT.h"
#include "Tasking/TaskQueue.h"
#include "Memory/Virtual.h"
namespace Tasking
{
    class Process
    {
	public:
        char* Name;
        unsigned int ID;
        unsigned int NextID;
        Memory::Virtual::PageDirectory* GlobalDirectory;
        class Thread* Threads;
    };
    class Thread
    {
	public:
        unsigned int ESP0;
        Memory::Virtual::PageDirectory* ThreadDirectory;
        unsigned int ID;
        unsigned int TimeLeft;
        bool Blocking;
        Process* Parent;

		Registers* GetTemporaryRegisters()
		{
			Memory::Virtual::Page* StackPage= Memory::Virtual::GetPage(ThreadDirectory,ESP0);
			Memory::Virtual::MapTemporary(StackPage);
			return (Registers*)(KernelTemporary+(ESP0%4096));
		}
    };
    extern Process* KernelProcess;
	extern bool TaskingEnabled;
    void Initialize();

    Thread* CreateThread(Process* process,void(*function)());
    Thread* GetRunning();
	
	void Exit();
    void Switch();
    void GetNextThread(Registers* registers);
}