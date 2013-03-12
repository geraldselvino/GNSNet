#include "test_UdpClient.h"
#include "UdpUserSocket.h"

namespace GNSNetTest{

	void TestUdpClient::Init(){
        m_UdpClient = GNSNet::GNSNetFactory::getUdpUserSocket();
	}

    void TestUdpClient::CreateSocket(){
        Assert::AreNotEqual(nullptr, m_UdpClient);
        bool ret = m_UdpClient->CreateSocket(8877, "127.0.0.1");
        Assert::AreEqual(true, ret);
        ret = m_UdpClient->ShutDown();
        Assert::AreEqual(true, ret);
    }

    void TestUdpClient::LastError(){
        Assert::AreNotEqual(nullptr, m_UdpClient);
        bool ret = m_UdpClient->CreateSocket(8877, "127.0.0.1");
        Assert::AreEqual(true, ret);
        int t_err = m_UdpClient->LastError();
        Assert::AreEqual(0, t_err);
        ret = m_UdpClient->ShutDown();
        Assert::AreEqual(true, ret);
	}

    void TestUdpClient::GetClientName(){
        Assert::AreNotEqual(nullptr, m_UdpClient);
        bool ret = m_UdpClient->CreateSocket(8877, "127.0.0.1");
        Assert::AreEqual(true, ret);
        String^ t_host;
        String^ t_port;
        ret = m_UdpClient->GetClientName(t_host, t_port);
        Assert::AreEqual(true, ret);
        Assert::AreNotEqual("", t_host);
        Assert::AreNotEqual("", t_port);
        Console::WriteLine("UdpClient=>Host: {0}", t_host);
        Console::WriteLine("UdpClient=>Port: {0}", t_port);
        ret = m_UdpClient->ShutDown();
        Assert::AreEqual(true, ret);
	}

    void TestUdpClient::Send(){
        Assert::AreNotEqual(nullptr, m_UdpClient);
        bool ret = m_UdpClient->CreateSocket(8877, "127.0.0.1");
        Assert::AreEqual(true, ret);
        ret = m_UdpClient->Send("Udp test data to send");
        Assert::AreEqual(true, ret);
        Console::WriteLine("UdpClient=>Send successful");
        Thread^ tr = gcnew Thread(gcnew ThreadStart(this, &TestUdpClient::threadproc));
        tr->IsBackground = true;
        tr->Priority = ThreadPriority::Normal;
        tr->Start();
	}

    void TestUdpClient::threadproc(){
        while(true){
            Thread::Sleep(10);
            String^ t_Recv;
            bool ret = m_UdpClient->Recv(t_Recv);
            Assert::AreEqual(true, ret);
            Assert::AreNotEqual(nullptr, t_Recv);
            Console::WriteLine("UdpClient=>Data: {0}", t_Recv);
            ret = m_UdpClient->ShutDown();
            Assert::AreEqual(true, ret);
            break;
        }
	}
}

