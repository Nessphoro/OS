#pragma once

namespace Memory
{
    /*Performs a region-to-region copy*/
    extern void* Memory_Copy(void* Destination,void* Source,unsigned int Count);

    /*Sets a region of memory to a specified value*/
    extern void* Memory_Set(unsigned int* Destination,unsigned int Value,unsigned int Count);
    /*Sets a region of memory to a specified value*/
    extern void* Memory_Set(unsigned short* Destination,unsigned short Value,unsigned int Count);
    /*Sets a region of memory to a specified value*/
    extern void* Memory_Set(unsigned char* Destination,unsigned char Value,unsigned int Count);

    /*Performs a memory clear*/
    extern void* Memory_Clear(void *Destination,unsigned int Count);


    extern void* StaticMalloc(unsigned int Size);
    extern void* StaticMalloc(unsigned int Size,bool Aligned);
}