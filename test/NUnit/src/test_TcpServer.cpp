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
    }

	void TestTcpServer::StartService(){
        Assert::AreNotEqual(nullptr, m_Server);
        bool ret = m_Server->DestroySocket();
        Assert::AreEqual(true, ret);
        ret = m_Server->StartService(7788);
        Assert::AreEqual(true, ret);
        Console::WriteLine("Server started successfully");
	}

    void TestTcpServer::ReadMessage(){
        Assert::AreNotEqual(nullptr, m_Server);
        bool ret = m_Server->DestroySocket();
        Assert::AreEqual(true, ret);
        ret = m_Server->StartService(7788);
        Assert::AreEqual(true, ret);
        Console::WriteLine("Server started successfully");
        Thread^ tr = gcnew Thread(gcnew ThreadStart(this, &TestTcpServer::threadproc));
        tr->IsBackground = true;
        tr->Priority = ThreadPriority::Normal;
        tr->Start();
	}

    void TestTcpServer::threadproc(){
        while(true){
            Thread::Sleep(1);
            GNSNet::ReceiveParam^ t_Msg = m_Server->ReadMessage();
            if(t_Msg==nullptr)continue;
            Assert::AreNotEqual(nullptr, t_Msg);
            Console::WriteLine("Data: {0}", t_Msg->RecvData);
            Console::WriteLine("ClientNo: {0}", t_Msg->ClientNo);
            bool ret = m_Server->Send(t_Msg->RecvData, t_Msg->ClientNo);
            Assert::AreEqual(true, ret);
            Console::WriteLine("Received message echoed back");
            break;
        }
	}
}

