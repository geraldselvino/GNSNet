/**********************************************************
* Copyright (c) 2013, Gerald Selvino <geraldsv@outlook.com>
* All rights reserved.
*
* This is the implementation class of ClientSocket, use to
* create a TCP/IP client. Provides the standard socket
* methods for client connections.
***********************************************************/
#pragma once

#define RECVBUF_SIZE  1024 /** The size of the recieve buffer in bytes */
#define CLIENT        0    /** Indicates a client */
#define SERVER        1    /** Indicates a server */

/**
 * Namespaces of the libraries that this
 * class used
 */
using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Threading;
using namespace System::Text;

namespace GNSNet{

    public ref class ClientSocket : public Object
    {
    public:
        ClientSocket();
        /**
         * Virtual, so that inheriter can destroy it 
         * using this class' pointer
         */
        virtual ~ClientSocket();

    public:
        /**
         * Interfaces to the outside world 
         */
        bool Connect(String^ const% HostName, int Port); /**Connect to server*/
        bool Accept(ClientSocket^% pSocket);             
        bool Disconnect();                            
        bool Send(String^ const% SendData);
        bool Send(String^ const% SendData, int Count);
        bool Recv(String^% pData);
        bool IsConnect(){ return m_Connect; }
        int  LastError(){ return m_nLastError; }
        bool GetClientName();
        bool GetClientName(String^% HostName, String^% PortNo);
        bool CreateSocket(int PortNo);
        bool CreateSocket();
        bool SetKeepAliveOption(int Enable);
        bool SetKeepAlive();

    protected:
        /**
         * This is protected, since inheriter may implement 
         * methods that will directly access the Socket member
         */
        Socket^ m_s; 

    private:
        /**
         * Internal methods only 
         */
        bool GetRecord(String^% pRecord, int RecvCount);
        bool SetClientName(String^ const% HostName, String^ const% PortNo);
        void InitSocket();

    private:
        /**
         * Encapsulate members as private 
         */
        String^       m_HostName;
        String^       m_Port;
        array<Byte>^  m_RecvBuf; /**The receive buffer*/
        int           m_DataLen; /**Length of received data*/
        int           m_nLastError; /**Stores the last encountered error*/
        int           m_Kind;       /**Stores the type of socket(client,server)*/
        int           m_KeepAliveOption;
        bool          m_Connect;    /**Indicates if the socket is still connected*/
    };
}

