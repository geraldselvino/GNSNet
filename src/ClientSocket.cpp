#include <Stdafx.h> //include to allow precompiled header in Visual Studio
#include "ClientSocket.h"
#include "ScopeLock.h"

/**
 * Definition of the ClientSocket class
 */

GNSNet::ClientSocket::ClientSocket()
: m_DataLen(0), m_Connect(false), m_RecvBuf(gcnew array<Byte>(RECVBUF_SIZE))
{
    InitSocket();
}

GNSNet::ClientSocket::~ClientSocket()
{
    delete m_s;
}

bool GNSNet::ClientSocket::CreateSocket()
{
    return CreateSocket(-1);
}

void GNSNet::ClientSocket::InitSocket()
{
    if(m_s != nullptr){
        m_s->Close();
    }
    m_s = gcnew Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
    GC::Collect();
}

bool GNSNet::ClientSocket::CreateSocket(int PortNo)
{
    if(PortNo == -1){
        m_Kind = CLIENT;
    }
    else{
        m_Kind = SERVER;
    }
    
    bool ret = true;
    try{
        if(m_Kind == SERVER){
            m_s->SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ReuseAddress, true);
            IPEndPoint^ endPoint = gcnew IPEndPoint(IPAddress::Any, PortNo);
            m_s->Bind(endPoint);
            m_s->Listen(6);
        }
    }
    catch(SocketException^ e){
        m_nLastError = e->ErrorCode;
        ret = false;
    }

    return ret;
}

bool GNSNet::ClientSocket::Connect(String^ const% HostName, int Port)
{
    bool ret = true;

    bool BefStatus = IsConnect();

    if(BefStatus){
        return ret;
    }

    m_DataLen = 0;
    SetClientName("", "");

    try{
        m_s->Connect(HostName, Port);
        m_Connect = m_s->Connected;
    }
    catch(SocketException^ e){
        InitSocket();
        m_nLastError = e->ErrorCode;
        ret = false;
    }

    return ret;
}

bool GNSNet::ClientSocket::Accept(ClientSocket^% pSocket)
{
    bool ret = true;
    try{
        pSocket->m_s = m_s->Accept();
        pSocket->m_Connect = true;
    }
    catch(SocketException^ e){
        m_nLastError = e->ErrorCode;
        ret = false;
    }

    return ret;
}

bool GNSNet::ClientSocket::Disconnect()
{
    bool ret = true;

    try{
        if(IsConnect()){
            m_s->Shutdown(SocketShutdown::Both);
        }
        SetClientName("", "");
        InitSocket();
        m_Connect = false;
        
        Thread::Sleep(50);
    }
    catch(SocketException^ e){
        m_nLastError = e->ErrorCode;
        ret = false;
    }

    return ret;
}

bool GNSNet::ClientSocket::Send(String^ const% SendData)
{
    if(!IsConnect()){
        return false;
    }
    bool ret = true;
    try{
        array<Byte>^ t_Data;
        Encoding^ l_encode = Encoding::GetEncoding("shift_jis");    
        t_Data = l_encode->GetBytes(SendData);
        m_s->Send(t_Data);
    }
    catch(SocketException^ e){
        m_nLastError = e->ErrorCode;
        ret = false;
    }

    return ret;
}

bool GNSNet::ClientSocket::Send(String^ const% SendData, int Count)
{
    if(!IsConnect()){
        return false;
    }
    bool ret = true;
    try{
        array<Byte>^ t_Data;
        Encoding^ l_encode = Encoding::GetEncoding("shift_jis");
        t_Data = l_encode->GetBytes(SendData);
        m_s->Send(t_Data, Count, SocketFlags::None);
    }
    catch(SocketException^ e){
        m_nLastError = e->ErrorCode;
        ret = false;
    }

    return ret;
}

bool GNSNet::ClientSocket::Recv(String^% pData)
{
    ScopeLock l_Lock(m_RecvBuf);

    bool ret = true;
    pData = "";
    if(!IsConnect()){
        return false;
    }

    int RecvCount = m_DataLen;
    while(true){
        if(GetRecord(pData, m_DataLen)){
            break;
        }

        RecvCount = m_RecvBuf->Length;

        try{
            m_DataLen = m_s->Receive(m_RecvBuf, RecvCount, SocketFlags::None);
            if(m_DataLen <= 0){
                ret = false;
                break;
            }
        }
        catch(SocketException^ e){
            m_nLastError = e->ErrorCode;
            m_DataLen = 0;
            return false;
        }
        catch(Exception^){
            return false;
        }
    }

    return ret;
}

bool GNSNet::ClientSocket::GetRecord(String^% pRecord, int RecvCount)
{
    int RecvDataLen;
    bool ret = false;

    RecvDataLen = RecvCount;

    if(RecvDataLen > 0){
        Encoding^ l_encode = Encoding::GetEncoding("shift_jis");    
        pRecord = l_encode->GetString(m_RecvBuf, 0, RecvCount);

        delete m_RecvBuf;
        m_RecvBuf = gcnew array<Byte>(RECVBUF_SIZE);

        m_DataLen = 0;
        ret = true;    
    }

    return ret;
}

bool GNSNet::ClientSocket::GetClientName()
{
    String^ HostName;
    String^ PortNo;
    bool ret = true;
    try{
        IPEndPoint^ pRemoteEndPoint = dynamic_cast<IPEndPoint^>(m_s->RemoteEndPoint);
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

bool GNSNet::ClientSocket::GetClientName(String^% HostName, String^% PortNo)
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

bool GNSNet::ClientSocket::SetClientName(String^ const% HostName, String^ const% PortNo)
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

bool GNSNet::ClientSocket::SetKeepAlive()
{
    bool ret = true;

    int Val = m_KeepAliveOption;

    try{
        m_s->SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::KeepAlive, Val);
    }
    catch(SocketException^ e){
        m_nLastError = e->ErrorCode;
        String^ Log = String::Format("keepaliveoption error {0} value = {1}", m_nLastError, Val);
        ret = false;
    }

    return ret;
}

bool GNSNet::ClientSocket::SetKeepAliveOption(int Enable)
{
    m_KeepAliveOption = Enable;
    return true;
}