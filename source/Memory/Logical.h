#pragma once
#define HeapBase 0xF0000000
#define ChunkSize 0x200000
namespace Memory
{
	namespace Logical
	{
		struct Block
		{
			unsigned int Large:1;
			unsigned int Head:1;
			unsigned int Free:1;
			unsigned int Empty:1;
			unsigned int Size:19;
			unsigned int Length:9;
			unsigned int TotalAllocations;
			//Bitmap
			unsigned int AllocationBitmap[];
		};

		void Initialize();

		void* Allocate(unsigned int size);
		void Deallocate(void* pointer);
	}
}