#include "ServerSocket.h"
#include "ServerSocketImpl.h"
#include "ClientSocket.h"
#include "ScopeLock.h"

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

//namespace GNSNet{
//    ref struct ReceiveParam{
//        int ClientNo;
//        String^ RecvData;
//    };
//}

/**
 * Definition of the Handle class(ServerSocket)
 */

GNSNet::ServerSocket::ServerSocket()
: pImplServerSocket(gcnew ServerSocketImpl())
{
}

GNSNet::ServerSocket::~ServerSocket()
{
    delete pImplServerSocket;
}

bool GNSNet::ServerSocket::CreateSocket(int PortNo)
{
    return pImplServerSocket->CreateSocket(PortNo);
}

bool GNSNet::ServerSocket::DestroySocket()
{
    return pImplServerSocket->DestroySocket();
}

int GNSNet::ServerSocket::LastError()
{
    return pImplServerSocket->LastError();
}

int GNSNet::ServerSocket::LastError(int ClientNo)
{
    return pImplServerSocket->LastError(ClientNo);
}

bool GNSNet::ServerSocket::StartService(int PortNo)
{
    return pImplServerSocket->StartService(PortNo);
}

bool GNSNet::ServerSocket::Accept()
{
    return pImplServerSocket->Accept();
}

bool GNSNet::ServerSocket::Disconnect()
{
    return pImplServerSocket->Disconnect();
}

bool GNSNet::ServerSocket::Disconnect(int ClientNo)
{
    return pImplServerSocket->Disconnect(ClientNo);
}

bool GNSNet::ServerSocket::Send(String^ const% SendData, int ClientNo)
{
    return pImplServerSocket->Send(SendData, ClientNo);
}

bool GNSNet::ServerSocket::Send(String^ const% SendData, int Count, int ClientNo)
{
    return pImplServerSocket->Send(SendData, Count, ClientNo);
}

GNSNet::ReceiveParam^ GNSNet::ServerSocket::ReadMessage()
{
    return pImplServerSocket->ReadMessage();
}

bool GNSNet::ServerSocket::Recv(String^% pData, int ClientNo)
{
    return pImplServerSocket->Recv(pData, ClientNo);
}

bool GNSNet::ServerSocket::IsConnect(int ClientNo)
{
    return pImplServerSocket->IsConnect(ClientNo);
}

bool GNSNet::ServerSocket::GetClientName(String^% HostName, String^% PortNo, int ClientNo)
{
    return pImplServerSocket->GetClientName(HostName, PortNo, ClientNo);
}

bool GNSNet::ServerSocket::SetKeepAliveOption(int Enable)
{
    return pImplServerSocket->SetKeepAliveOption(Enable);
}

/**
 * Definition of the Body class(ServerSocketImpl)
 */

GNSNet::ServerSocketImpl::ServerSocketImpl()
: m_ClientNo(0), 
  m_Server(gcnew ClientSocket()),
  m_ClientHandle(gcnew Dictionary<int, ClientSocket^>()),
  m_RecvMessage(gcnew LinkedList<ReceiveParam^>()),
  m_RecvMessageEvent(gcnew ManualResetEvent(false)),
  m_AcceptSema(gcnew Semaphore(NOOFCLIENT_MAX, NOOFCLIENT_MAX))
{
}

GNSNet::ServerSocketImpl::~ServerSocketImpl()
{
    DestroySocket();
    delete m_Server;
}

bool GNSNet::ServerSocketImpl::CreateSocket(int PortNo)
{
    bool ret = m_Server->CreateSocket(PortNo);
    return ret;
}

bool GNSNet::ServerSocketImpl::DestroySocket()
{
    bool ret = m_Server->Disconnect();
    return ret;
}

int GNSNet::ServerSocketImpl::LastError()
{
    return LastError(-1);
}

int GNSNet::ServerSocketImpl::LastError(int ClientNo)
{
    int ret = 0;

    if(ClientNo == -1){
        ClientNo = m_ClientNo;
    }

    ClientSocket^ m_Client = m_ClientHandle[ClientNo];
    if(m_Client != nullptr){
        ret = m_Client->LastError();
    }

    return ret;
}

bool GNSNet::ServerSocketImpl::StartService(int PortNo)
{
    bool ret = true;

    try{
        if(CreateSocket(PortNo)){
            ThreadStart^ pAcceptProc = gcnew ThreadStart(this, &ServerSocketImpl::ServiceProc);
            Thread^ pAcceptThread = gcnew Thread(pAcceptProc);
            pAcceptThread->Priority = ThreadPriority::Normal;
            pAcceptThread->IsBackground = true;
            pAcceptThread->Start();
        }
        else{
            ret = false;
        }
    }
    catch(SocketException^ e){
        Console::WriteLine(e->ToString());
        ret = false;
    }

    return ret;
}

bool GNSNet::ServerSocketImpl::Accept()
{
    bool ret;

    ClientSocket^ m_Client = gcnew ClientSocket();

    ret = m_Server->Accept(m_Client);

    if(ret){
        m_AcceptSema->WaitOne();
        ++m_ClientNo;
        m_Client->DiscoverClientName();
        m_ClientHandle->Add(m_ClientNo, m_Client);
    }

    return ret;
}

bool GNSNet::ServerSocketImpl::Disconnect()
{
    return Disconnect(-1);
}

bool GNSNet::ServerSocketImpl::Disconnect(int ClientNo)
{
    bool ret = false;

    if(ClientNo == -1){
        ClientNo = m_ClientNo;
    }

    ClientSocket^ m_Client = m_ClientHandle[ClientNo];

    if(m_Client != nullptr){
        --m_ClientNo;
        m_ClientHandle->Remove(ClientNo);
        ret = m_Client->Disconnect();
        m_AcceptSema->Release();
    } 

    return ret;
}

bool GNSNet::ServerSocketImpl::Send(String^ const% SendData, int ClientNo)
{
    bool ret = false;

    ClientSocket^ m_Client = m_ClientHandle[ClientNo];
    if(m_Client != nullptr){
        ret = m_Client->Send(SendData);
    }

    return ret;
}

bool GNSNet::ServerSocketImpl::Send(String^ const% SendData, int Count, int ClientNo)
{
    bool ret = false;

    ClientSocket^ m_Client = m_ClientHandle[ClientNo];
    if(m_Client != nullptr){
        ret = m_Client->Send(SendData, Count);
    }

    return ret;
}

void GNSNet::ServerSocketImpl::ServiceProc()
{
    while(true){    
        if(Accept()){
            ThreadStart^ pReceiveProc = gcnew ThreadStart(this, &ServerSocketImpl::Receive);
            Thread^ pReceiveThread = gcnew Thread(pReceiveProc);
            pReceiveThread->Priority = ThreadPriority::Normal;
            pReceiveThread->IsBackground = true;
            pReceiveThread->Start();
        }
        Thread::Sleep(10);
    }
}

void GNSNet::ServerSocketImpl::Receive()
{
    while(true){
        {
            ScopeLock l_Lock(m_RecvMessage); //mutex lock, only one thread should access the m_RecvMessage
            if(m_RecvMessage->Count < RECVMSG_MAX){

                ReceiveParam^ l_RecvMessage = gcnew ReceiveParam();
                l_RecvMessage->ClientNo = m_ClientNo;

                if(Recv(l_RecvMessage->RecvData, l_RecvMessage->ClientNo)){
                    m_RecvMessage->AddLast(l_RecvMessage);
                }
            }
            else{
                m_RecvMessageEvent->Reset(); //message queue is full so unsignal the Event
                m_RecvMessageEvent->WaitOne(); //Wait here until there is a room at m_RecvData
            }
        }
        Thread::Sleep(10);
    }
}

GNSNet::ReceiveParam^ GNSNet::ServerSocketImpl::ReadMessage()
{
    ScopeLock l_Lock(m_RecvMessage); //mutex lock, only one thread should access the m_RecvMessage

    ReceiveParam^ l_MsgNode = nullptr;

    if(m_RecvMessage->Count>0){
        l_MsgNode = m_RecvMessage->Last->Value;
        m_RecvMessage->RemoveLast();

        m_RecvMessageEvent->Set(); //signal the event, Recv() will continue
    }

    return l_MsgNode;
}

bool GNSNet::ServerSocketImpl::Recv(String^% pData, int ClientNo)
{
    bool ret = false;

    ClientSocket^ m_Client = m_ClientHandle[ClientNo];
    if(m_Client != nullptr){
        ret = m_Client->Recv(pData);
    }

    return ret;
}

bool GNSNet::ServerSocketImpl::IsConnect(int ClientNo)
{
    bool ret = false;

    ClientSocket^ m_Client = m_ClientHandle[ClientNo];
    if(m_Client != nullptr){
        ret = m_Client->IsConnect(); 
    } 

    return ret;
}

bool GNSNet::ServerSocketImpl::GetClientName(String^% HostName, String^% PortNo, int ClientNo)
{
    bool ret = false;

    ClientSocket^ m_Client = m_ClientHandle[ClientNo];

    if(m_Client != nullptr){
        ret = m_Client->GetClientName(HostName, PortNo);
        if(HostName == ""){
            HostName = "localhost";
        }
    }

    return ret;
}

bool GNSNet::ServerSocketImpl::SetKeepAliveOption(int Enable)
{
    return m_Server->SetKeepAliveOption(Enable);
}