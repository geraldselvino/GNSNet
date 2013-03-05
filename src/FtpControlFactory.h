/**********************************************************
* Copyright (c) 2013, Gerald Selvino <geraldsv@outlook.com>
* All rights reserved.
*
* This is a factory class implementation for FtpControl
***********************************************************/
#pragma once

#include "NetworkFactory.h"
#include "FtpControl.h"

namespace GNSNet{

    ref class FtpControlFactory: public NetworkFactory
    {
    public:
        /**
         * Virtual, so that inheriter can destroy it 
         * using this class' pointer
         */
        virtual ~FtpControlFactory(){}

    protected:
        /**
         * Implements the Instance() pure virtual
         * method from NetworkFactory, which returns
         * an instance of FtpControl
         */
        virtual Object^ Instance()override{
            return gcnew FtpControl();
        }
    };
}