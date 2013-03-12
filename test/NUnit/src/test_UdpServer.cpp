#include "test_UdpServer.h"
#include "UdpUserSocket.h"

namespace GNSNetTest{

	void TestUdpServer::Init(){
        m_UdpServer = GNSNet::GNSNetFactory::getUdpUserSocket();
	}

    void TestUdpServer::CreateSocket(){
        Assert::AreNotEqual(nullptr, m_UdpServer);
        bool ret = m_UdpServer->CreateSocket(8877);
        Assert::AreEqual(true, ret);
        ret = m_UdpServer->ShutDown();
        Assert::AreEqual(true, ret);
    }

    void TestUdpServer::LastError(){
        Assert::AreNotEqual(nullptr, m_UdpServer);
        bool ret = m_UdpServer->CreateSocket(8877);
        Assert::AreEqual(true, ret);
        int t_err = m_UdpServer->LastError();
        Assert::AreEqual(0, t_err);
        ret = m_UdpServer->ShutDown();
        Assert::AreEqual(true, ret);
	}

    void TestUdpServer::GetClientName(){
        Assert::AreNotEqual(nullptr, m_UdpServer);
        bool ret = m_UdpServer->CreateSocket(8877);
        Assert::AreEqual(true, ret);
        Thread^ tr = gcnew Thread(gcnew ThreadStart(this, &TestUdpServer::threadprocgcn));
        tr->IsBackground = true;
        tr->Priority = ThreadPriority::Normal;
        tr->Start();
        tr->Join();
	}

    void TestUdpServer::Receive(){
        Assert::AreNotEqual(nullptr, m_UdpServer);
        bool ret = m_UdpServer->CreateSocket(8877);
        Assert::AreEqual(true, ret);

        Thread^ tr = gcnew Thread(gcnew ThreadStart(this, &TestUdpServer::threadproc));
        tr->IsBackground = true;
        tr->Priority = ThreadPriority::Normal;
        tr->Start();
	}

    void TestUdpServer::threadproc(){
        while(true){
            Thread::Sleep(10);
            String^ t_Recv;
            bool ret = m_UdpServer->Recv(t_Recv);
            Assert::AreEqual(true, ret);
            if(t_Recv==nullptr)continue;
            Assert::AreNotEqual(nullptr, t_Recv);
            Console::WriteLine("UdpServer=>Data: {0}", t_Recv);
            ret = m_UdpServer->Send(t_Recv);
            Assert::AreEqual(true, ret);
            Console::WriteLine("Send()-recv msg echoed back");
            ret = m_UdpServer->Send(t_Recv, "127.0.0.1", 8877);
            Assert::AreEqual(true, ret);
            Console::WriteLine("Send(,,)-recv msg echoed back");
            ret = m_UdpServer->ShutDown();
            Assert::AreEqual(true, ret);
            break;
        }
	}

    void TestUdpServer::threadprocgcn(){
        while(true){
            Thread::Sleep(10);
            bool ret = m_UdpServer->DiscoverClientName();
            if(!ret)continue;
            Assert::AreEqual(true, ret);
            String^ t_host;
            String^ t_port;
            ret = m_UdpServer->GetClientName(t_host, t_port);
            Assert::AreEqual(true, ret);
            Assert::AreNotEqual("", t_host);
            Assert::AreNotEqual("", t_port);
            Console::WriteLine("UdpServer=>Client: {0}", t_host);
            Console::WriteLine("UdpServer=>Port: {0}", t_port);
            ret = m_UdpServer->ShutDown();
            Assert::AreEqual(true, ret);
            break;
        }
	}
}

