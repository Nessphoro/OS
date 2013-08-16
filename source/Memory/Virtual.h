#pragma once
#define KernelTemporary 0xD0000000

 extern void* KernelDirectory;
 struct Registers;
namespace Memory
{
    namespace Virtual
    {
        struct Page
        {
            unsigned int Present:1;
            unsigned int Writable:1;
            unsigned int User:1;
            unsigned int WriteThrough:1;
            unsigned int CacheDisabled:1;
            unsigned int Accessed:1;
            unsigned int Dirty:1;
            unsigned int Zero:1;
            unsigned int Global:1;

            unsigned int Secure:1;
            unsigned int Temporal:1;
            unsigned int FreeFlag:1;

            unsigned int Physical:20;
        };
        struct PageTable
        {
            Page Pages[1024];
        };
        struct PageDirectory
        {
            unsigned int PhysicalTables[1024];
            PageTable* Tables[1024];
        };
        extern PageDirectory* Kernel;
        extern PageDirectory* Current;
        void Initialize();
        PageDirectory* CreateShadow(PageDirectory* Directory);
        void Switch(PageDirectory* PageDirectory);
        
        

        Page* GetPage(PageDirectory* Directory,unsigned int Address);
        Page* GetPage(unsigned int Address);

        void FreePage(Page* page);
        void AllocatePage(Page* page);

        void MapRange(unsigned int PhysicalStart,unsigned int VirtualStart,unsigned int Pages);
        void FreeRange(unsigned int VirtualStart,unsigned int Pages);

        void MapTemporary(Page* page);
        void MapTemporary(unsigned int physical);
        void MapTemporary();
        void UnmapTemporary();
        void Invalidate(unsigned int Address);

        unsigned int GetPhysical(PageDirectory* Directory,unsigned int Address);
        unsigned int GetPhysical(unsigned int Address);

		void PageFault(Registers* registers);
    }
}