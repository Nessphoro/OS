#include "Memory/Virtual.h"
#include "Memory/Memory.h"
#include "Boot/IDT.h"
#include "Memory/Physical.h"
#include "Graphics/Graphics.h"
#include "Tasking/Tasking.h"

extern "C" unsigned int SwichDirectoryASM()
{
	Memory::Virtual::Current=Tasking::GetRunning()->ThreadDirectory;
    return Memory::Virtual::GetPhysical(Memory::Virtual::Kernel,(unsigned int)Tasking::GetRunning()->ThreadDirectory->PhysicalTables);
}
namespace Memory
{
    namespace Virtual
    {
        PageDirectory* Kernel;
        PageDirectory* Current;

        void Initialize()
        {
            //Create kernel tables
            Kernel=(PageDirectory*)&KernelDirectory;
            Kernel->Tables[768]=(PageTable*)((Kernel->PhysicalTables[768]&0xFFFFF000)+0xC0000000); //This is how the PD look
			Kernel->Tables[769]=(PageTable*)((Kernel->PhysicalTables[769]&0xFFFFF000)+0xC0000000); //This is how the PD lookss
            Current=Kernel;
            for(int i=0;i<254;i++)
            {
                Kernel->Tables[770+i]=(PageTable*)Memory::StaticMalloc(sizeof(PageTable),true);
                Kernel->PhysicalTables[770+i]=(GetPhysical((unsigned int)Kernel->Tables[770+i])&0xFFFFF000)|0x3;
            }
            //We created all 256 Kernel entries to avoid updating page directories
            Switch(Kernel); //No effect
			
        }
        PageDirectory* CreateShadow(PageDirectory* Directory)
        {
            PageDirectory* newDirectory=(PageDirectory*)Memory::StaticMalloc(sizeof(PageDirectory),true);
            Memory_Copy(newDirectory,Directory,sizeof(PageDirectory));
            return newDirectory;
        }
        void Switch(PageDirectory* Directory)
        {
            Current=Directory;
            unsigned int physical=GetPhysical(Kernel,(unsigned int)Directory->PhysicalTables);
            __asm volatile("movl %0, %%cr3":: "r"(physical));
        }

        Page* GetPage(PageDirectory* Directory,unsigned int Address)
        {
            unsigned int PD=Address>>22;
            unsigned int PT=(Address>>12)&0x03FF;
            
            if((Directory->PhysicalTables[PD]&0x1) == 1)
            {
                //Present
                return &(Directory->Tables[PD]->Pages[PT]);
            }
            else
            {
                Directory->Tables[PD]=(PageTable*)Memory::StaticMalloc(sizeof(PageTable),true);
                Directory->PhysicalTables[PD]=(GetPhysical((unsigned int)Directory->Tables[PD])&0xFFFFF000)|0x3;
                return &(Directory->Tables[PD]->Pages[PT]);
            }
        }
        Page* GetPage(unsigned int Address)
        {
            return GetPage(Current,Address);
        }

        void FreePage(Page* page)
        {
            if(!page->Present)
                return;
            if(page->Secure)
            {
                MapTemporary(page);
                Memory_Clear((void*)KernelTemporary,0x1000);
                UnmapTemporary();
            }
			Memory::Physical::PushPage(page->Physical);
            Memory_Clear(page,4);
        }
        void AllocatePage(Page* page)
        {
            if(page->Present)
                return;
            page->Present=true;
			page->Physical=Memory::Physical::PopPage();
        }
        void MapRange(unsigned int PhysicalStart,unsigned int VirtualStart,unsigned int Pages)
        {
            for(int i=0;i<Pages;i++)
            {
                Page* mapPage=GetPage(VirtualStart+(i<<12));
                mapPage->Physical=(PhysicalStart+(i<<12))>>12;
                mapPage->Present=true;
            }
        }
        void FreeRange(unsigned int VirtualStart,unsigned int Pages)
        {
            for (int i = 0; i < Pages; i++)
            {
                FreePage(GetPage(VirtualStart+(i<<12)));
            }
        }

        void MapTemporary(Page* page)
        {
            Page* TemporalPage=GetPage(KernelTemporary);
            if(TemporalPage->Temporal)
            {
                FreePage(TemporalPage);
            }
            TemporalPage->Present=true;
            TemporalPage->Physical=page->Physical;
            Invalidate(KernelTemporary);
        }
        void MapTemporary(unsigned int physical)
        {
            Page* TemporalPage=GetPage(KernelTemporary);
            if(TemporalPage->Temporal)
            {
                FreePage(TemporalPage);
            }
            TemporalPage->Present=true;
            TemporalPage->Physical=physical>>12;
            Invalidate(KernelTemporary);
        }
        void MapTemporary()
        {
            Page* TemporalPage=GetPage(KernelTemporary);
            if(TemporalPage->Temporal)
            {
                FreePage(TemporalPage);
            }
            AllocatePage(TemporalPage);
            TemporalPage->Temporal=true;
        }
        void UnmapTemporary()
        {
            Page* TemporalPage=GetPage(KernelTemporary);
            if(!TemporalPage->Present)
                return;
            if(TemporalPage->Temporal)
            {
                FreePage(TemporalPage);
            }
            Memory::Memory_Clear(TemporalPage,4);
            Invalidate(KernelTemporary);
        }

        void Invalidate(unsigned int Address)
        {
            __asm volatile("invlpg %0"::"m" (*(char *)Address));
        }

        unsigned int GetPhysical(PageDirectory* Directory,unsigned int Address)
        {
            
            Page* page=GetPage(Directory,Address);
            unsigned int ph=page->Physical<<12;
            ph+=Address&0xFFF;
            return ph;
        }
        unsigned int GetPhysical(unsigned int Address)
        {
            return GetPhysical(Current,Address);
        }

		void PageFault(Registers* registers)
		{
			bool Present=registers->Error&1;
			bool Supervisor=!((registers->Error&4)>>2);

			unsigned int CR2;
			__asm volatile("mov %%cr2, %0": "=r"(CR2));
			
			*Graphics::GetConsole()<<RED<<"[INFO] Page fault at address:"<<CR2<<" Supevisor:"<<Supervisor<<" Present:"<<Present<<endl;
			if(!Present && Supervisor && CR2 >= 0xF0000000)
			{
				*Graphics::GetConsole()<<GREEN<<"[INFO]"<<WHITE<< " Heap demand paging recognized..."<<endl<<WHITE;
				AllocatePage(GetPage(CR2));
				return;
			}
			
			Graphics::ForceRefresh();
			
			__asm("hlt");
		}
    }
}