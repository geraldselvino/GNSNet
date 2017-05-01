/**********************************************************
* Copyright (c) 2013, Gerald Selvino
* <gerald.selvino@protonmail.com> All rights reserved.
*
* Abstract Factory class, use as a base class for the 
* implementation factories.
***********************************************************/
#pragma once

namespace GNSNet{

    ref class NetworkFactory abstract
    {
    public:
        NetworkFactory(void){}
        /**
         * Virtual, so that inheriter can destroy it 
         * using this class' pointer
         */
        virtual ~NetworkFactory(void){}

    public:
        /**
         * Non-Virtual Interface/Template method
         * which calls the Instance() method from
         * the implementation factories
         */
        Object^ getInstance(){
            return Instance();
        }

    protected:
        /**
         * Pure virtual method to be implemented
         * by the implementation factories, this is
         * to simulate a Virtual Constructor(Idiom)
         */
        virtual Object^ Instance() = 0;
    };
}
