#include "UdpUserSocket.h"
#include "UdpUserSocketImpl.h"
#include "ScopeLock.h"

/**
 * Definition of the Handle class(UdpUserSocket)
 */

GNSNet::UdpUserSocket::UdpUserSocket()
: pImplUdpUserSocket(gcnew UdpUserSocketImpl())
{
}

GNSNet::UdpUserSocket::~UdpUserSocket()
{
    delete pImplUdpUserSocket;
}

bool GNSNet::UdpUserSocket::CreateSocket(int PortNo)
{
    return pImplUdpUserSocket->CreateSocket(PortNo);
}

bool GNSNet::UdpUserSocket::CreateSocket(int PortNo, String^ const% HostName)
{
    return pImplUdpUserSocket->CreateSocket(PortNo, HostName);
}

bool GNSNet::UdpUserSocket::ShutDown()
{
    return pImplUdpUserSocket->ShutDown();
}

bool GNSNet::UdpUserSocket::Send(String^ const% SendData)
{
    return pImplUdpUserSocket->Send(SendData);
}

bool GNSNet::UdpUserSocket::Send(String^ const% SendData, String^ const% HostName, int PortNo)
{
    return pImplUdpUserSocket->Send(SendData, HostName, PortNo);
}

bool GNSNet::UdpUserSocket::Send(String^ const% SendData, int Count)
{
    return pImplUdpUserSocket->Send(SendData, Count);
}

bool GNSNet::UdpUserSocket::Recv(String^% pData)
{
    return pImplUdpUserSocket->Recv(pData);
}

bool GNSNet::UdpUserSocket::DiscoverClientName()
{
    return pImplUdpUserSocket->DiscoverClientName();
}

bool GNSNet::UdpUserSocket::GetClientName(String^% HostName, String^% PortNo)
{
    return pImplUdpUserSocket->GetClientName(HostName, PortNo);
}

int GNSNet::UdpUserSocket::LastError()
{
    return pImplUdpUserSocket->LastError();
}
bool GNSNet::UdpUserSocket::IsEnableSocket()
{ 
    return pImplUdpUserSocket->IsEnableSocket();
}

/**
 * Definition of the Body class(UdpUserSocketImpl)
 */

GNSNet::UdpUserSocketImpl::UdpUserSocketImpl()
: Socket(System::Net::Sockets::AddressFamily::InterNetwork, 
         System::Net::Sockets::SocketType::Dgram, 
         System::Net::Sockets::ProtocolType::Udp),
  m_DataLen(0), 
  m_RecvBuf(gcnew array<Byte>(UDPRECVBUF_SIZE)),
  m_SockStatus(false), 
  m_HostName(""), 
  m_Port(""),
  m_nLastError(0)
{
}

GNSNet::UdpUserSocketImpl::~UdpUserSocketImpl()
{
}

bool GNSNet::UdpUserSocketImpl::CreateSocket(int PortNo)
{
    return CreateSocket(PortNo, "");
}

bool GNSNet::UdpUserSocketImpl::CreateSocket(int PortNo, String^ const% HostName)
{
    bool ret = true;

    if(HostName == ""){
        m_Kind = SERVER;
    }
    else{
        m_Kind = CLIENT;
    }

    try{
        if(m_Kind == SERVER){
            IPEndPoint^ anyEndPoint = gcnew IPEndPoint(IPAddress::Any, PortNo);
            Socket::Bind(anyEndPoint);
            remoteEP = dynamic_cast<EndPoint^>(anyEndPoint);
        }
        else{
            IPEndPoint^ serverEndPoint = gcnew IPEndPoint(IPAddress::Parse(HostName), PortNo);
            remoteEP = dynamic_cast<EndPoint^>(serverEndPoint);
        }

        m_SockStatus = true;
        SetClientName(HostName, PortNo.ToString());
    }
    catch(SocketException^ e){
        m_nLastError = e->ErrorCode;
        ret = false;
    }

    return ret;
}

bool GNSNet::UdpUserSocketImpl::ShutDown()
{
    bool ret = true;

    try{
        if(IsEnableSocket()){
            Socket::Shutdown(SocketShutdown::Both);
            m_SockStatus = false;
        }
        SetClientName("", "");
        Socket::Close();
    }
    catch(SocketException^ e){
        m_nLastError = e->ErrorCode;
        ret = false;
    }

    Thread::Sleep(100);
    return ret;
}

bool GNSNet::UdpUserSocketImpl::Send(String^ const% SendData)
{
    bool ret = true;

    if(!IsEnableSocket()){
        return false;
    }

    try{        
        array<Byte>^ t_Data;
        Encoding^ l_encode = Encoding::GetEncoding("utf-32");    
        t_Data = l_encode->GetBytes(SendData);
        Socket::SendTo(t_Data, remoteEP);
    }
    catch(SocketException^ e){
        ret = false;
        m_nLastError = e->ErrorCode;
    }

    return ret;
}

bool GNSNet::UdpUserSocketImpl::Send(String^ const% SendData, String^ const% HostName, int PortNo)
{
    bool ret = true;

    if(!IsEnableSocket()){
        return false;
    }

    try{
        IPEndPoint^ toEndpoint = gcnew IPEndPoint(IPAddress::Parse(HostName), PortNo);

        array<Byte>^ t_Data;
        Encoding^ l_encode = Encoding::GetEncoding("utf-32");
        t_Data = l_encode->GetBytes(SendData);

        Socket::SendTo(t_Data, toEndpoint);
    }
    catch(SocketException^ e){
        ret = false;
        m_nLastError = e->ErrorCode;
    }

    return ret;
}

bool GNSNet::UdpUserSocketImpl::Send(String^ const% SendData, int Count)
{
    bool ret = true;

    if(!IsEnableSocket()){
        return false;
    }

    try{
        array<Byte>^ t_Data;
        Encoding^ l_encode = Encoding::GetEncoding("utf-32");
        t_Data = l_encode->GetBytes(SendData);

        Socket::SendTo(t_Data, Count, SocketFlags::None, remoteEP);
    }
    catch(SocketException^ e){
        ret = false;
        m_nLastError = e->ErrorCode;
    }

    return ret;
}

bool GNSNet::UdpUserSocketImpl::Recv(String^% pData)
{
    ScopeLock l_Lock(m_RecvBuf);

    bool ret = true;
    pData = "";

    if(!IsEnableSocket()){
        return false;
    }

    int RecvCount = m_DataLen;
    while(true){
        if(GetRecord(pData, m_DataLen)){
            break;
        }

        RecvCount = UDPRECVBUF_SIZE;

        try{
            m_DataLen = Socket::ReceiveFrom(m_RecvBuf, RecvCount, SocketFlags::None, remoteEP);
        }
        catch(SocketException^ e){            
            m_nLastError = e->ErrorCode;
            if(m_nLastError != 10040){
                m_DataLen = 0;
                ret = false;
                break;
            }
            else{
                m_DataLen = RecvCount;
                ret = true;
            }        
        }
        catch(InvalidOperationException^){        
            if(!Socket::IsBound){
                m_DataLen = 0;
                ret = true;
            }
            else{
                m_DataLen = 0;
                ret = false;
                break;
            }
        }
    }
    return ret;
}

bool GNSNet::UdpUserSocketImpl::GetRecord(String^% pRecord, int RecvCount)
{
    int RecvDataLen;

    bool ret = false;

    RecvDataLen = RecvCount;

    if(RecvDataLen > 0){
        Encoding^ l_encode = Encoding::GetEncoding("utf-32");    
        pRecord = l_encode->GetString(m_RecvBuf, 0, RecvCount);

        delete m_RecvBuf;
        m_RecvBuf = gcnew array<Byte>(UDPRECVBUF_SIZE);
    
        m_DataLen = 0;
        ret = true;    
    }

    return ret;
}

bool GNSNet::UdpUserSocketImpl::DiscoverClientName()
{
    String^ HostName;
    String^ PortNo;
    bool ret = true;
    try{
        IPEndPoint^ pRemoteEndPoint = dynamic_cast<IPEndPoint^>(remoteEP);
        HostName = pRemoteEndPoint->Address->ToString();
        PortNo = String::Format("{0:0000}",pRemoteEndPoint->Port.ToString());
    }
    catch(SocketException^ e){
        HostName = "";
        PortNo = "";
        m_nLastError = e->ErrorCode;
        ret = false;
    }

    SetClientName(HostName, PortNo);
    return ret;
}

bool GNSNet::UdpUserSocketImpl::GetClientName(String^% HostName, String^% PortNo)
{
    {
        ScopeLock l_Lock(m_HostName);
        HostName = m_HostName;
    }
    {
        ScopeLock l_Lock(m_Port);
        PortNo = m_Port;
    }

    return true;
}

bool GNSNet::UdpUserSocketImpl::SetClientName(String^ const% HostName, String^ const% PortNo)
{
    {
        ScopeLock l_Lock(m_HostName);
        m_HostName = HostName;
    }
    {
        ScopeLock l_Lock(m_Port);
        m_Port = PortNo;
    }

    return true;
}