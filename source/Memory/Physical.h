/*
Physical memory allocator
*/

#pragma once
struct MultibootInfo;
namespace Memory
{
    namespace Physical
    {
		class Zone
		{
		public:
			Zone()
			{

			}

			void Initialize(unsigned int start,unsigned int end);

			unsigned int PopPage();
			void PushPage(unsigned int page);
		private:
			unsigned int ZoneStart;
			unsigned int ZoneEnd;
			unsigned int* Stack;
			unsigned int StackCount;
		};

        bool Initialize(MultibootInfo *multiboot);

        unsigned int PopPage();
		void PushPage(unsigned int page);

        unsigned int TotalFreePages();
    }
}