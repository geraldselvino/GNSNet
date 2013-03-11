/**********************************************************
* Copyright (c) 2013, Gerald Selvino <geraldsv@outlook.com>
* All rights reserved.
*
* This is the implementation class of UdpUserSocket, use to
* send and receive UDP packets/messages.
***********************************************************/
#pragma once

#define UDPRECVBUF_SIZE 9999 /** The size of the recieve buffer in bytes */
#define CLIENT          0    /** Indicates a client */    
#define SERVER          1    /** Indicates a server */

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

    public ref class UdpUserSocketImpl : public Socket
    {
    public:
        UdpUserSocketImpl();
        /**
         * Virtual, so that inheriter can destroy it 
         * using this class' pointer
         */
        virtual ~UdpUserSocketImpl();

    public:
        /**
         * Interfaces to the outside world 
         */
        bool Send(String^ const% SendData);
        bool Send(String^ const% SendData, String^ const% HostName, int PortNo);
        bool Send(String^ const% SendData, int Count);
        bool Recv(String^% pData);
        bool DiscoverClientName();
        bool GetClientName(String^% HostName, String^% PortNo);
        bool CreateSocket(int PortNo);
        bool CreateSocket(int PortNo, String^ const% HostName);
        bool ShutDown();
        int  LastError(){ return m_nLastError; }
        bool IsEnableSocket(){ return m_SockStatus; }

    private:
        /**
         * Internal methods only 
         */
        bool GetRecord(String^% pRecord, int RecvCount);
        bool SetClientName(String^ const% HostName, String^ const% PortNo);

    private:
        /**
         * Encapsulate members as private 
         */
        String^        m_HostName;
        String^        m_Port;
        array<Byte>^   m_RecvBuf;
        int            m_DataLen;
        int            m_nLastError;
        bool           m_SockStatus;
        int            m_Kind;
        EndPoint^      remoteEP;
    };
}
