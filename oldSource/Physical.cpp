#include "Memory/Physical.h"
#include "Memory/Memory.h"
#include "Graphics/Graphics.h"
#include "Multiboot.h"
#include "Globals.h"

namespace Memory
{
    namespace Physical
    {
		extern "C"
		{
			unsigned int* Superblock;
			unsigned int* Bitmap;
		}

        bool Initialize(MultibootInfo *multiboot)
        {
            Superblock=(unsigned int*)Memory::StaticMalloc(128);
            Bitmap=(unsigned int*)Memory::StaticMalloc(131072);
            //Nothing is free as of yet
            Memory::Memory_Set(Superblock,0xFFFFFFFF,32);
            Memory::Memory_Set(Bitmap,0xFFFFFFFF,0x8000);

            MultibootInfo* mbi=multiboot;

            unsigned int TotalKilobytes=mbi->Mem_lower+mbi->Mem_upper;
            unsigned int TotalPages=(TotalKilobytes*1024)/4096;
            //Fix the addresses
            mbi->Mmap_addr+=KernelVirtualBase;

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
                        FreePage(StartPage);
                    }
                }
                
                mmap = (MemoryMap*) ( (unsigned int)mmap + mmap->Size + sizeof(unsigned int) );
            }
            //First four megs are reserved too
            for(int i=0;i<1024;i++)
            {
                SetPage(i);
            }
            return true;
        }
		extern "C" unsigned int PhysicalGetPage();
        unsigned int GetPage()
        {
			return PhysicalGetPage();

			/*
            for(int i=0;i<32;i++)
            {
                if(Superblock[i]!=0xFFFFFFFF)
                {
                    int Offset=0;
                    for(int j=0;j<32;j++)
                    {
                        unsigned int totest=1<<j;
                        if(!(Superblock[i]&totest))
                        {
                            Offset=j;
                            break;
                        }
                    }
                    Offset=Offset + (i<<5);
                    Offset<<=5;
                    for(int j=0;j<32;j++)
                    {
                        if(Bitmap[Offset+j]!=0xFFFFFFFF)
                        {
                            for (int k = 0; k < 32; k++)
                            {
                                unsigned int toTest = 0x1 << k;
                                if ( !(Bitmap[Offset+j]&toTest) )
                                {
                                    return ((Offset<<5)+j)+k;
                                }
                            }
                        }
                    }

                    return Offset; 
                }
            }

            return -1;
			*/
        }
		
        void SetPage(unsigned int page)
        {
            Bitmap[page>>5]|=(1<<(page%32));
            unsigned int PageStart=(page>>10) * 32;
            bool FreeBlocks=false;
            for(int j=0;j<32;j++)
            {
                if(Bitmap[PageStart+j]!=0xFFFFFFFF)
                {
                    FreeBlocks=true;
                    break;
                }
            }
            if(!FreeBlocks)
            {
                Superblock[page>>15]|=(1<<((page>>10) % 32));
            }
        }
        void FreePage(unsigned int page)
        {
            Superblock[page>>15]&=~(1<<((page>>10) % 32));
            Bitmap[page>>5]&=~(1<<(page%32));
        }

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
    }
}