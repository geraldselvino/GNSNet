#include "test_TcpClient.h"
#include "ClientSocket.h"

namespace GNSNetTest{

	void TestTcpClient::Init(){
        m_Client = GNSNet::GNSNetFactory::getTcpClientSocket();
	}
        
    void TestTcpClient::Connect(){
        Assert::AreNotEqual(nullptr, m_Client);
        bool ret = m_Client->Connect("localhost", 7788);
        Assert::AreEqual(true, ret);
        ret = m_Client->IsConnect();
        Assert::AreEqual(true, ret);
        Console::WriteLine("Connection successful");
        ret = m_Client->Disconnect();
        Assert::AreEqual(true, ret);
        Console::WriteLine("Disconnected successfully");
    }

    void TestTcpClient::GetClientName(){
        Assert::AreNotEqual(nullptr, m_Client);
        bool ret = m_Client->Connect("localhost", 7788);
        Assert::AreEqual(true, ret);
        ret = m_Client->IsConnect();
        Assert::AreEqual(true, ret);
        Console::WriteLine("Connection successful");
        ret = m_Client->DiscoverClientName();
        Assert::AreEqual(true, ret);
        String^ t_host;
        String^ t_port;
        ret = m_Client->GetClientName(t_host, t_port);
        Assert::AreEqual(true, ret);
        Assert::AreNotEqual("", t_host);
        Assert::AreNotEqual("", t_port);
        Console::WriteLine("Hostname: {0}", t_host);
        Console::WriteLine("Portnum: {0}", t_port);
        ret = m_Client->Disconnect();
        Assert::AreEqual(true, ret);
        Console::WriteLine("Disconnected successfully");
    }

    void TestTcpClient::Send(){
        Assert::AreNotEqual(nullptr, m_Client);
        bool ret = m_Client->Connect("localhost", 7788);
        Assert::AreEqual(true, ret);
        ret = m_Client->IsConnect();
        Assert::AreEqual(true, ret);
        Console::WriteLine("Connection successful");
        ret = m_Client->Send("Test data to send");
        Assert::AreEqual(true, ret);
        Console::WriteLine("Test message sent successfully");
        ThreadStart^ tStart = gcnew ThreadStart(this, &TestTcpClient::threadProc);
        Thread^ t_Trd = gcnew Thread(tStart);
        t_Trd->Priority = ThreadPriority::Normal;
        t_Trd->IsBackground = true;
        t_Trd->Start();
    }

    void TestTcpClient::SetKeepAlive(){
        Assert::AreNotEqual(nullptr, m_Client);
        bool ret = m_Client->Connect("localhost", 7788);
        Assert::AreEqual(true, ret);
        ret = m_Client->IsConnect();
        Assert::AreEqual(true, ret);
        Console::WriteLine("Connection successful");
        ret = m_Client->SetKeepAliveOption(1);
        Assert::AreEqual(true, ret);
        ret = m_Client->SetKeepAlive();
        Assert::AreEqual(true, ret);
        ret = m_Client->Disconnect();
        Assert::AreEqual(true, ret);
        Console::WriteLine("Disconnected successfully");
    }

    void TestTcpClient::threadProc(){
        String^ t_MsgRecv;
        bool ret = m_Client->Recv(t_MsgRecv);
        Assert::AreEqual(true, ret);
        Assert::AreNotEqual("", t_MsgRecv);
        Console::WriteLine("Received data: {0}", t_MsgRecv);
        ret = m_Client->Disconnect();
        Assert::AreEqual(true, ret);
        Console::WriteLine("Disconnected successfully");
    }
}

