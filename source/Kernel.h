#pragma once
struct MultibootInfo;
extern "C" void KernelStart(MultibootInfo *BootData,unsigned int Magic);
extern void InitializeSystems();
extern "C" void KernelEnd();