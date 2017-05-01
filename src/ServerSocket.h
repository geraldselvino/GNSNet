/**********************************************************
* Copyright (c) 2013, Gerald Selvino
* <gerald.selvino@protonmail.com> All rights reserved.
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

    /**
     * Definition of the ReceiveParam structure, use
     * for storing the message received from a client
     * an its client number, so that the server knows
     * which client to send a reply. This is needed
     * because the messages received are buffered, so 
     * that the server does not reply upon receipt of
     * the message from the client. If it needs to reply,
     * it will know which client to send reply thru the
     * ClientNo member.
     */
    public ref struct ReceiveParam{
        int ClientNo;
        String^ RecvData;
    };

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
        int  LastClient();
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
