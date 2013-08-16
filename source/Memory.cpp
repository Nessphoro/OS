#include "Memory/Memory.h"
#include "Graphics/Graphics.h"
#include "Globals.h"

extern unsigned int KernelMemoryEnd;
namespace Memory
{
	unsigned int MemoryStart=(unsigned int)&KernelMemoryEnd;
	void* Memory_Copy(void* Destination,void* Source,unsigned int Count)
	{
		__asm volatile("mov %0, %%edi":: "g"(Destination):"%edi");
		__asm volatile("mov %0, %%esi":: "g"(Source):"%esi");
		__asm volatile("mov %0, %%ecx":: "g"(Count) :"%ecx");
		__asm volatile("rep movsb":::"memory");
		return Destination;
	}

	void* Memory_Set(unsigned int* Destination,unsigned int Value,unsigned int Count)
	{
		__asm volatile("mov %0, %%edi":: "g"(Destination):"%edi");
		__asm volatile("mov %0, %%eax"::"g"(Value):"%eax");
		__asm volatile("mov %0, %%ecx":: "g"(Count) :"%ecx");
		__asm volatile("rep stosl");
		return Destination;
	}
	void* Memory_Set(unsigned short* Destination,unsigned short Value,unsigned int Count)
	{
		__asm volatile("mov %0, %%edi":: "g"(Destination):"%edi");
		__asm volatile("mov %0, %%eax"::"g"(Value):"%eax");
		__asm volatile("mov %0, %%ecx":: "g"(Count) :"%ecx");
		__asm volatile("rep stosw");
		return Destination;
	}
	void* Memory_Set(unsigned char* Destination,unsigned char Value,unsigned int Count)
	{
		__asm volatile("mov %0, %%edi":: "g"(Destination):"%edi");
		__asm volatile("mov %0, %%eax"::"g"(Value):"%eax");
		__asm volatile("mov %0, %%ecx":: "g"(Count) :"%ecx");
		__asm volatile("rep stosb");
		return Destination;
	}

	void* Memory_Clear(void *Destination,unsigned int Count)
	{
		__asm volatile("mov %0, %%edi":: "g"(Destination):"%edi");
		__asm volatile("mov %0, %%eax"::"g"(0):"%eax");
		__asm volatile("mov %0, %%ecx":: "g"(Count) :"%ecx");
		__asm volatile("rep stosb");
		return Destination;
	}

	void* StaticMalloc(unsigned int Size)
	{
		void* Pointer=(void*)MemoryStart;
		MemoryStart+=Size;
		return Pointer;
	}
	void* StaticMalloc(unsigned int Size,bool Aligned)
	{
		if (Aligned) // If the address is not already page-aligned
		{
			if(MemoryStart%4096!=0)
			{
				// Align it.
				MemoryStart &= 0xFFFFF000;
				MemoryStart += 0x1000;
			}
		}
		void* Pointer=(void*)MemoryStart;
		MemoryStart+=Size;
		return Pointer;
	}

	extern "C" void* memcpy(void* a,void* b,unsigned int count)
	{
		return Memory_Copy(a,b,count);
	}

}