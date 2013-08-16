#include "Memory/Logical.h"
#include "Memory/Virtual.h"
#include "Graphics/Graphics.h"
#include "Hardware/Clock.h"
namespace Memory
{
	namespace Logical
	{
		Block** BlockMap=(Block**)HeapBase;
		Block* ExtraSmall;
		Block* SmallBlocks[32];
		void Initialize()
		{
			for(int i=0;i<0xFFFF;i++)
			{
				Memory::Virtual::GetPage(HeapBase+i*4096)->Present=false;
			}
			//Allocate space for block map
			for(int i=0;i<64;i++)
			{
				Memory::Virtual::AllocatePage(Memory::Virtual::GetPage(HeapBase+i*4096));
			}

			//Initialize Extra Small
			ExtraSmall=(Block*)(HeapBase+1024*1024*2);
			ExtraSmall->Size=4;
			ExtraSmall->Free=true;
			ExtraSmall->Head=true;
			ExtraSmall->Large=false;
			ExtraSmall->Length=0;
			for(int b=0;b<512;b++)
			{
				BlockMap[512+b]=ExtraSmall;
			}

			
			//Lazy initialize blocks
			for(int i=2;i<34;i++)
			{
				Block* tempBlock=SmallBlocks[i-2]=(Block*)(HeapBase+i*1024*1024*2);
				Graphics::ForceRefresh();
				tempBlock->Size=(i-1)*16;
				tempBlock->Free=true;
				tempBlock->Empty=true;
				tempBlock->Length=0;
				tempBlock->Head=true;
				tempBlock->Large=true;
				for(int b=0;b<512;b++)
				{
					BlockMap[i*512+b]=tempBlock;
				}
			}
		}
		
		inline void* FromBlock(Block* block)
		{
			if(!block->Free)
			{
				return 0;
			}
			block->Empty=false;
			block->TotalAllocations++;
			unsigned int Range=1022 + (ChunkSize/block->Size/32/4096 -1 )*1024; 
			
			for(int i=0;i<Range;i++)
			{
				unsigned int bitmap=block->AllocationBitmap[i];
				if(bitmap!=0xFFFFFFFF)
				{
					//Replace with BSF 
					for (int j = 0; j < 32; j++)
					{
						if(!(bitmap>>j&0x1))
						{
							block->AllocationBitmap[i]|=1<<j;
							return (void*)( ((unsigned int)block) + (Range+2)*4 + (i*32+j)*block->Size);
						}
					}
				}
			}
			
			return 0;
		}


		void* Allocate(unsigned int size)
		{
			if(size==0)
				return 0;


			if(size>=4096)
			{

			}
			else if(size>4 && size<=512)
			{
				if(size%16==0)
					return FromBlock(SmallBlocks[size/16 - 1]);
				else
				{
					return FromBlock(SmallBlocks[size/16]);
				}
			}
			else
			{
				return FromBlock(ExtraSmall);
			}
		}
		void Deallocate(void* pointer)
		{
			unsigned int iPointer=(unsigned int)pointer;
			unsigned int iBlock;
			Block* block = BlockMap[(iPointer-HeapBase)>>12];
			iBlock=(unsigned int)block;
			if(block->Large)
			{

			}
			else
			{
				unsigned int Range=(ChunkSize/block->Size/32);
				unsigned int Offset=iPointer-iBlock - Range;
				Offset/=block->Size;
				block->AllocationBitmap[Offset/32]^=(1<<(Offset%32));
			}
		}
	}
}