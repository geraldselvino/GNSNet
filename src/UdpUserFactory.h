/**********************************************************
* Copyright (c) 2013, Gerald Selvino <geraldsv@outlook.com>
* All rights reserved.
*
* This is a factory class implementation for UdpUserSocket
***********************************************************/
#pragma once

#include "NetworkFactory.h"
#include "UdpUserSocket.h"

namespace GNSNet{

    ref class UdpUserFactory: public NetworkFactory
    {
    public:
        /**
         * Virtual, so that inheriter can destroy it 
         * using this class' pointer
         */
        virtual ~UdpUserFactory(){
            delete m_Object;
        }

    protected:
        /**
         * Implements the Instance() pure virtual
         * method from NetworkFactory, which returns
         * an instance of UdpUserSocket
         */
        virtual Object^ Instance()override{
            m_Object = gcnew UdpUserSocket();
            return m_Object;
        }

    private:
        Object^ m_Object;

    };
}