#include <Stdafx.h> //include to allow precompiled header in Visual Studio
#include "GNSNetFactory.h"
#include "TcpClientFactory.h"
#include "TcpServerFactory.h"
#include "UdpUserFactory.h"
#include "FtpControlFactory.h"

GNSNet::ClientSocket^ GNSNet::GNSNetFactory::getTcpClientSocket()
{
    NetworkFactory^ m_Factory = gcnew TcpClientFactory();
    return dynamic_cast<ClientSocket^>(m_Factory->getInstance());
}

GNSNet::ServerSocket^ GNSNet::GNSNetFactory::getTcpServerSocket(){
    NetworkFactory^ m_Factory = gcnew TcpServerFactory();
    return dynamic_cast<ServerSocket^>(m_Factory->getInstance());
}

GNSNet::UdpUserSocket^ GNSNet::GNSNetFactory::getUdpUserSocket(){
    NetworkFactory^ m_Factory = gcnew TcpClientFactory();
    return dynamic_cast<UdpUserSocket^>(m_Factory->getInstance());
}

GNSNet::FtpControl^ GNSNet::GNSNetFactory::getFtpControl(){
    NetworkFactory^ m_Factory = gcnew FtpControlFactory();
    return dynamic_cast<FtpControl^>(m_Factory->getInstance());
}

