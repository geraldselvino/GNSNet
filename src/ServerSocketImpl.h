/**********************************************************
* Copyright (c) 2013, Gerald Selvino <geraldsv@outlook.com>
* All rights reserved.
*
* This is the implementation class of ServerSocket, use to
* create a TCP/IP server. Provides the standard socket 
* functions for creating a server. It also provides a
* seamless server method, which automates all process of
* TCP server creation at the call of one method (StartService),
* the StartService() method handles concurrent connections,
* tracks clients to a dictionary, and does all the dirty 
* tricks of concurrency, synchronization, and queueing. In
* just a call to StartService() method, you have an instant
* server listening, and ready to receive messages, which it
* stores at m_RecvMessage, retrievable by calling ReadMessage()
* client handles are stored at m_ClientHandle.
***********************************************************/
#pragma once

/**
 * Maximum incomming message buffer,
 * this is different from socket receive buffer
 */
#define RECVMSG_MAX 10
#define NOOFCLIENT_MAX 10 /** Maximum number of allowed client connections*/

/**
 * Namespaces of the libraries that this
 * class used
 */
using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Threading;

namespace GNSNet{
    /**
     * Forward declare ClientSocket and ReceiveParam,
     * because we just need a reference of it here
     */
    ref class ClientSocket;
    ref struct ReceiveParam;

    public ref class ServerSocketImpl : public Object
    {
    public:
        ServerSocketImpl();
        /**
         * Virtual, so that inheriter can destroy it 
         * using this class' pointer
         */
        virtual ~ServerSocketImpl();

    public:
        /**
         * Interfaces to the outside world 
         */
        bool Accept();                 /**Accept a connection*/
        bool Disconnect(int ClientNo); /**Disconnect a client*/
        bool Disconnect();             /**Disconnect a client*/
        bool Send(String^ const% SendData, int ClientNo);            /**Send to a client*/
        bool Send(String^ const% SendData, int Count, int ClientNo); /**Send to a client*/
        bool Recv(String^% pData, int ClientNo); /**Receive a data from a client*/
        bool IsConnect(int ClientNo);  /**Returns the status of connection*/
        bool GetClientName(String^% HostName, String^% PortNo, int ClientNo);
        bool DestroySocket();          /**Disconnect the server socket*/
        int  LastError(int ClientNo);  /**Returns the error code that is last encountered*/
        int  LastError();              /**Returns the error code that is last encountered*/
        bool CreateSocket(int PortNo); /**Create a socket, bind and listen*/
        bool SetKeepAliveOption(int Enable);   /**Set the socket option to KeepAlive*/
        virtual bool StartService(int PortNo); /**Seamless server method, overridable, so that 
                                                  developers can implement their own*/
        virtual ReceiveParam^ ReadMessage();   /**Read a message from the head of the queue,
                                                  overridable, so that developers can implement
                                                  their own*/

    protected:
        /**
         * This is protected, since inheriter may implement 
         * methods that will directly access this Socket member
         */
        ClientSocket^ m_Server;

    protected:
        /**
         * These are protected, since inheriter may implement 
         * methods that will directly access the handle to client
         * and the received messages.
         */
        Dictionary<int, ClientSocket^>^ m_ClientHandle; /**Stores the socket handles of each client*/
        LinkedList<ReceiveParam^>^      m_RecvMessage;  /**Where messages from clients are buffered*/

    private:
        /**
         * Internal method only, method run by the Receive and Accept thread 
         */
        void Receive(Object^ pObj);
        void Accept(Object^ pObj);

    private:
        /**
         * Encapsulate members as private 
         */
        int                m_ClientNo;         /**Number of clients counter*/
        String^            m_HostName;         /**Stores the host name*/
        String^            m_Port;             /**Stores the port number*/
        ManualResetEvent^  m_RecvMessageEvent; /**Synchronization Event to unsignal/signal the Recv 
                                                 method to stop/continue*/
        Semaphore^         m_AcceptSema;       /**Semaphore to control the number of client connections*/

    };
}
