/**********************************************************
* Copyright (c) 2013, Gerald Selvino <geraldsv@outlook.com>
* All rights reserved.
*
* This is the handle class to the body (ServerSocketImpl)
***********************************************************/
#pragma once

/**
 * Namespaces of the libraries that this
 * class used
 */
using namespace System;

namespace GNSNet{
    /**
     * Forward declare ServerSocketImpl and ReceiveParam, 
     * because we just need a reference of it here
     */
    ref class ServerSocketImpl;
    ref struct ReceiveParam;

    public ref class ServerSocket
    {
    public:
        ServerSocket();
        /**
         * Virtual, so that inheriter can destroy it 
         * using this class' pointer
         */
        virtual ~ServerSocket();

    public:
        /**
         * Interfaces to the outside world 
         */
        bool Accept();
        bool Disconnect(int ClientNo);
        bool Disconnect();
        bool Send(String^ const% SendData, int ClientNo);
        bool Send(String^ const% SendData, int Count, int ClientNo);
        bool Recv(String^% pData, int ClientNo);
        bool IsConnect(int ClientNo);
        bool GetClientName(String^% HostName, String^% PortNo, int ClientNo);
        bool DestroySocket();
        int  LastError(int ClientNo);
        int  LastError();
        bool CreateSocket(int PortNo);
        bool SetKeepAliveOption(int Enable);
        bool StartService(int PortNo);
        ReceiveParam^ ReadMessage();

    private:
        /**
         * Reference/Pointer to the implementation of FtpControl
         */
        ServerSocketImpl^ pImplServerSocket;

    };
}
