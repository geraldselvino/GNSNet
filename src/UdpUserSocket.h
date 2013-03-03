/**********************************************************
* Copyright (c) 2013, Gerald Selvino <geraldsv@outlook.com>
* All rights reserved.
*
* This is the handle class to the body (UdpUserSocketImpl)
***********************************************************/
#pragma once

/**
 * Namespaces of the libraries that this
 * class used
 */
using namespace System;

namespace GNSNet{
    /**
     * Forward declare UdpUserSocketImpl, because
     * we just need a reference of it here
     */
    ref class UdpUserSocketImpl;

    public ref class UdpUserSocket
    {
    public:
        UdpUserSocket();
        /**
         * Virtual, so that inheriter can destroy it 
         * using this class' pointer
         */
        virtual ~UdpUserSocket();

    public:
        /**
         * Interfaces to the outside world 
         */
        bool Send(String^ const% SendData);
        bool Send(String^ const% SendData, String^ const% HostName, int PortNo);
        bool Send(String^ const% SendData, int Count);
        bool Recv(String^% pData);
        bool GetClientName();
        bool GetClientName(String^% HostName, String^% PortNo);
        bool CreateSocket(int PortNo);
        bool CreateSocket(int PortNo, String^ const% HostName);
        bool ShutDown();
        int  LastError();
        bool IsEnableSocket();

    private:
        /**
         * Reference/Pointer to the implementation of UdpUserSocket
         */
        UdpUserSocketImpl^        pImplUdpUserSocket;
    };
}
