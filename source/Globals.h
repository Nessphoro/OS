#pragma once
//#define INVOKE_TRACE
#define KernelVirtualBase 0xC0000000
#define PhysicalToVirtual(physical) (KernelVirtualBase)+(physical) 
#define IndirectNamespaceReference(name,type) namespace name { extern type;}