/**********************************************************
* Copyright (c) 2013, Gerald Selvino <geraldsv@outlook.com>
* All rights reserved.
*
* This is a factory class implementation for ClientSocket
***********************************************************/
#pragma once

#include "NetworkFactory.h"
#include "ClientSocket.h"

namespace GNSNet{

    ref class TcpClientFactory: public NetworkFactory
    {
    public:
        /**
         * Virtual, so that inheriter can destroy it 
         * using this class' pointer
         */
        virtual ~TcpClientFactory(){}

    protected:
        /**
         * Implements the Instance() pure virtual
         * method from NetworkFactory, which returns
         * an instance of ClientSocket
         */
        virtual Object^ Instance()override{
            return gcnew ClientSocket();
        }
    };
}