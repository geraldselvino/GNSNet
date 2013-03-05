/**********************************************************
* Copyright (c) 2013, Gerald Selvino <geraldsv@outlook.com>
* All rights reserved.
*
* The main factory class, used to obtain an instance of 
* the network classes
***********************************************************/
#pragma once

namespace GNSNet{

    ref class ClientSocket;
    ref class ServerSocket;
    ref class UdpUserSocket;
    ref class FtpControl;
    ref class NetworkFactory;

    public ref class GNSNetFactory sealed
    {
    public:
        /**
         * The factory methods, which returns
         * a reference to the instance of a
         * network class.
         */
        static ClientSocket^ getTcpClientSocket(); /**Factory method for ClientSocket*/
        static ServerSocket^ getTcpServerSocket(); /**Factory method for ServerSocket*/
        static UdpUserSocket^ getUdpUserSocket();  /**Factory method for UdpUserSocket*/
        static FtpControl^ getFtpControl();        /**Factory method for FtpControl*/

    private:
        /**
         * Make the default constructor, destructor, copy constructor,
         * assignment operator private, so that this factory
         * class cannot be instantiated, copied and assigned
         */
        GNSNetFactory(){}
        ~GNSNetFactory(){}
        GNSNetFactory(GNSNetFactory^ const% other){}
        GNSNetFactory^ operator=(GNSNetFactory^ const% other){return nullptr;}
    };
}