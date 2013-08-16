#include "Memory/Physical.h"
#include "Memory/Memory.h"
#include "Graphics/Graphics.h"
#include "Multiboot.h"
#include "Globals.h"

#define NO_LARGE

namespace Memory
{
	namespace Physical
	{
#ifndef NO_LARGE
		Zone large;
#endif
		Zone medium;
		Zone small;

		bool Initialize(MultibootInfo *multiboot)
		{

			MultibootInfo* mbi=multiboot;

			unsigned int TotalKilobytes=mbi->Mem_lower+mbi->Mem_upper;
			unsigned int TotalPages=(TotalKilobytes*1024)/4096;
			//Fix the addresses
			mbi->Mmap_addr+=KernelVirtualBase;
			small.Initialize(0,4096);
			medium.Initialize(4096,TotalPages-4096);

			MemoryMap* mmap = (MemoryMap*)(mbi->Mmap_addr);
			while((unsigned int)mmap < mbi->Mmap_addr + mbi->Mmap_length) 
			{
				if(mmap->Type==1)
				{
					//Usable 

					unsigned int StartPage=mmap->Base_addr_low>>12;
					unsigned int EndPage=(mmap->Base_addr_low+mmap->Length_low)>>12;
					for(;StartPage<EndPage;StartPage++)
					{
						if(StartPage>=2048)
							PushPage(StartPage);
					}
				}

				mmap = (MemoryMap*) ( (unsigned int)mmap + mmap->Size + sizeof(unsigned int) );
			}
			return true;
		}
		unsigned int PopPage()
		{
			unsigned int page;
#ifndef NO_LARGE
			page=large.PopPage();
			if(page!=-1)
				return page;
#endif
			page=medium.PopPage();
			if(page!=-1)
				return page;
			page=small.PopPage();
			return page;
		}
		void PushPage(unsigned int page)
		{
			if(page<4096)
				small.PushPage(page);
			else if(page<1048576)
				medium.PushPage(page);
#ifndef NO_LARGE
			else
				large.PushPage(page);
#endif
		}

		/*
		unsigned int TotalFreePages()
		{
		unsigned int Pages=0;
		for (int i = 0; i < 0x8000; i++)
		{
		if(Bitmap[i]==0)
		{
		Pages+=32;
		}
		else
		{
		for (int k = 0; k < 32; k++)
		{
		unsigned int toTest = 0x1 << k;
		if ( !(Bitmap[i]&toTest) )
		{
		Pages++;
		}
		}
		}
		}

		return Pages;
		}
		*/
	}
}



unsigned int Memory::Physical::Zone::PopPage()
{
	if(this->StackCount>0)
	{
		this->StackCount--;
		return (*--this->Stack);
	}
	return -1;
}
void Memory::Physical::Zone::PushPage(unsigned int page)
{
	this->StackCount++;
	*this->Stack++=page;
}

void Memory::Physical::Zone::Initialize(unsigned int start,unsigned int end)
{
	this->Stack=(unsigned int*)Memory::StaticMalloc((end-start)*4);
	this->StackCount=0;
	this->ZoneStart=start;
	this->ZoneEnd=end;
}