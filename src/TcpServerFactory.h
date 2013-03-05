/**********************************************************
* Copyright (c) 2013, Gerald Selvino <geraldsv@outlook.com>
* All rights reserved.
*
* This is a factory class implementation for ServerSocket
***********************************************************/
#pragma once

#include "NetworkFactory.h"
#include "ServerSocket.h"

namespace GNSNet{

    ref class TcpServerFactory: public NetworkFactory
    {
    public:
        /**
         * Virtual, so that inheriter can destroy it 
         * using this class' pointer
         */
        virtual ~TcpServerFactory(){}

    protected:
        /**
         * Implements the Instance() pure virtual
         * method from NetworkFactory, which returns
         * an instance of ServerSocket
         */
        virtual Object^ Instance()override{
            return gcnew ServerSocket();
        }
    };
}