#pragma once
#include "Memory/Memory.h"
#include "Tasking/Tasking.h"
namespace Tasking
{
	class Thread;
	class TaskQueue
	{
	private:
		Thread** Buffer;
		int Size;
		int Start;
		int End;

	public:
		void Allocate()
		{
			Size=1024;
			Buffer=(Thread**)Memory::StaticMalloc(4096,true);
		}

		int Count()
		{
			if(Start<End)
				return Start-End;
			else
				return Size-Start+End;
		}
		
		void Enqueue(Thread* thread)
		{
			Buffer[End++]=thread;
			if(End>=Size)
				End=0;
		}
		Thread* Peek()
		{
			if(Start==End)
				return 0;
			if(Start>=Size)
				Start=0;
			return Buffer[Start];
		}
		Thread* Dequeue()
		{
			if(Start==End)
				return 0;
			if(Start>=Size)
				Start=0;
			return Buffer[Start++];
		}
	};
}