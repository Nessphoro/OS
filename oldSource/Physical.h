/*
Physical memory allocator
*/

#pragma once
struct MultibootInfo;
namespace Memory
{
    namespace Physical
    {
        bool Initialize(MultibootInfo *multiboot);

        unsigned int GetPage();

        void SetPage(unsigned int page);
        void FreePage(unsigned int page);

        unsigned int TotalFreePages();
    }
}