#pragma once

class EventHandler
{
private:
    void(*Handlers[256])();
public:
    EventHandler();
    void Invoke(void *Context);
    EventHandler& operator+=(void(*function)());
    EventHandler& operator-=(void(*function)());
    EventHandler& operator+=(void(*function)(void*));
    EventHandler& operator-=(void(*function)(void*));
    EventHandler& operator()();
    EventHandler& operator()(void* Context);
};