/**********************************************************
* Copyright (c) 2013, Gerald Selvino
* <gerald.selvino@protonmail.com> All rights reserved.
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
        virtual ~UdpUserFactory(){}

    protected:
        /**
         * Implements the Instance() pure virtual
         * method from NetworkFactory, which returns
         * an instance of UdpUserSocket
         */
        virtual Object^ Instance()override{
            return gcnew UdpUserSocket();
        }
    };
}