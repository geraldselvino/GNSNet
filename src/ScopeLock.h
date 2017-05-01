/**********************************************************
* Copyright (c) 2013, Gerald Selvino
* <gerald.selvino@protonmail.com> All rights reserved.
*
* This is a RAII implementation of a mutex lock
***********************************************************/
#pragma once

using namespace System::Threading;

namespace GNSNet{

    public ref class ScopeLock : public IDisposable
    {
    public:

        ScopeLock(Object^ pObj)
        : m_Obj(pObj)
        {
            Monitor::Enter(m_Obj); /**Protect m_Obj*/
        }

        ~ScopeLock(){
            Monitor::Exit(m_Obj);  /**Release m_Obj*/
        }

    private:
        Object^ m_Obj;
    };
}