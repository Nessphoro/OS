#pragma once

namespace Hardware
{
    void Outb(unsigned short port,unsigned char data);
	unsigned char Inb(unsigned short port);
}