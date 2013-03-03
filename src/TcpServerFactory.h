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
        virtual ~TcpServerFactory(){
            delete m_Object;
        }

    protected:
        /**
         * Implements the Instance() pure virtual
         * method from NetworkFactory, which returns
         * an instance of ServerSocket
         */
        virtual Object^ Instance()override{
            m_Object = gcnew ServerSocket();
            return m_Object;
        }

    private:
        Object^ m_Object;

    };
}