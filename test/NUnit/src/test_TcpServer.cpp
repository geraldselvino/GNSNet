#include "test_TcpServer.h"
#include "ServerSocket.h"
#include "ServerSocketImpl.h"

namespace GNSNetTest{

	void TestTcpServer::Init(){
        m_Server = GNSNet::GNSNetFactory::getTcpServerSocket();
	}

    void TestTcpServer::SetKeepAliveOption(){
        Assert::AreNotEqual(nullptr, m_Server);
        bool ret = m_Server->SetKeepAliveOption(1);
        Assert::AreEqual(true, ret);
        ret = m_Server->DestroySocket();
        Assert::AreEqual(true, ret);
    }

	void TestTcpServer::StartService(){
        Assert::AreNotEqual(nullptr, m_Server);
        bool ret = m_Server->StartService(7788);
        Assert::AreEqual(true, ret);
        Console::WriteLine("Server started successfully");
        Thread^ tr = gcnew Thread(gcnew ThreadStart(this, &TestTcpServer::threadsrvproc));
        tr->IsBackground = true;
        tr->Priority = ThreadPriority::Normal;
        tr->Start();
	}

    void TestTcpServer::ReadMessage(){
        Assert::AreNotEqual(nullptr, m_Server);
        bool ret = m_Server->StartService(7788);
        Assert::AreEqual(true, ret);
        Console::WriteLine("Server started successfully");
        Thread^ tr = gcnew Thread(gcnew ThreadStart(this, &TestTcpServer::threadproc));
        tr->IsBackground = true;
        tr->Priority = ThreadPriority::Normal;
        tr->Start();
	}

    void TestTcpServer::threadproc(){
        while(true){
            Thread::Sleep(10);
            GNSNet::ReceiveParam^ t_Msg = m_Server->ReadMessage();
            if(t_Msg==nullptr)continue;
            Assert::AreNotEqual(nullptr, t_Msg);
            Console::WriteLine("Server=>Data: {0}", t_Msg->RecvData);
            Console::WriteLine("Server=>ClientNo: {0}", t_Msg->ClientNo);
            bool ret = m_Server->Send(t_Msg->RecvData, t_Msg->ClientNo);
            Assert::AreEqual(true, ret);
            Console::WriteLine("Send(,)-recv msg echoed back");
            break;
        }
	}

    void TestTcpServer::threadsrvproc(){
        while(true){
            Thread::Sleep(10);
            String^ t_host;
            String^ t_port;
            bool ret;
            try{
                ret = m_Server->GetClientName(t_host, t_port, m_Server->LastClient());
            }
            catch(KeyNotFoundException^ e){
                Console::WriteLine(e->Message);
                ret = false;
            }
            if(!ret)continue;
            Assert::AreEqual(true, ret);
            Assert::AreNotEqual("", t_host);
            Assert::AreNotEqual("", t_port);
            Console::WriteLine("Server=>Client: {0}", t_host);
            Console::WriteLine("Server=>Port: {0}", t_port);
            break;
        }
	}
}

