#pragma once
namespace Hardware
{
    namespace CPU
    {
        enum CPUDID_ECX{
            SSE3         = 1 << 0, 
            PCLMUL       = 1 << 1,
            DTES64       = 1 << 2,
            MONITOR      = 1 << 3,  
            DS_CPL       = 1 << 4,  
            VMX          = 1 << 5,  
            SMX          = 1 << 6,  
            EST          = 1 << 7,  
            TM2          = 1 << 8,  
            SSSE3        = 1 << 9,  
            CID          = 1 << 10,
            FMA          = 1 << 12,
            CX16         = 1 << 13, 
            ETPRD        = 1 << 14, 
            PDCM         = 1 << 15, 
            DCA          = 1 << 18, 
            SSE4_1       = 1 << 19, 
            SSE4_2       = 1 << 20, 
            x2APIC       = 1 << 21, 
            MOVBE        = 1 << 22, 
            POPCNT       = 1 << 23, 
            AES          = 1 << 25, 
            XSAVE        = 1 << 26, 
            OSXSAVE      = 1 << 27, 
            AVX          = 1 << 28,
        };
        enum CPUDID_EDX
        {
            FPU          = 1 << 0,  
            VME          = 1 << 1,  
            DE           = 1 << 2,  
            PSE          = 1 << 3,  
            TSC          = 1 << 4,  
            MSR          = 1 << 5,  
            PAE          = 1 << 6,  
            MCE          = 1 << 7,  
            CX8          = 1 << 8,  
            APIC         = 1 << 9,  
            SEP          = 1 << 11, 
            MTRR         = 1 << 12, 
            PGE          = 1 << 13, 
            MCA          = 1 << 14, 
            CMOV         = 1 << 15, 
            PAT          = 1 << 16, 
            PSE36        = 1 << 17, 
            PSN          = 1 << 18, 
            CLF          = 1 << 19, 
            DTES         = 1 << 21, 
            ACPI         = 1 << 22, 
            MMX          = 1 << 23, 
            FXSR         = 1 << 24, 
            SSE          = 1 << 25, 
            SSE2         = 1 << 26, 
            SS           = 1 << 27, 
            HTT          = 1 << 28, 
            TM1          = 1 << 29, 
            IA64         = 1 << 30,
            PBE          = 1 << 31
        };

        inline void Get(unsigned int Code,unsigned int* EAX,unsigned int* EBX,unsigned int* ECX,unsigned int* EDX)
        {
            __asm volatile("cpuid": "=a" (*EAX), "=b" (*EBX), "=c" (*ECX), "=d" (*EDX) : "a" (Code));
        }
    }
}