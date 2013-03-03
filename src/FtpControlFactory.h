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
        virtual ~FtpControlFactory(){
            delete m_Object;
        }

    protected:
        /**
         * Implements the Instance() pure virtual
         * method from NetworkFactory, which returns
         * an instance of FtpControl
         */
        virtual Object^ Instance()override{
            m_Object = gcnew FtpControl();
            return m_Object;
        }

    private:
        Object^ m_Object;

    };
}