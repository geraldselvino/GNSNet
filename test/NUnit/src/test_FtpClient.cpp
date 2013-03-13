#include "test_FtpClient.h"
#include "FtpControl.h"

namespace GNSNetTest{

	void TestFtpClient::Init(){
        m_FtpClient = GNSNet::GNSNetFactory::getFtpControl();
	}

    void TestFtpClient::FtpConnect(){
        Assert::AreNotEqual(nullptr, m_FtpClient);
        m_FtpClient->SetHostName("localhost");
        bool ret = m_FtpClient->FtpConnect();
        Assert::AreEqual(true, ret);
        ret = m_FtpClient->FtpDisconnect();
        Assert::AreEqual(true, ret);
    }

    void TestFtpClient::FileDownload(){
        Assert::AreNotEqual(nullptr, m_FtpClient);
        m_FtpClient->SetHostName("localhost");
        bool ret = m_FtpClient->FtpConnect();
        Assert::AreEqual(true, ret);
        ret = m_FtpClient->FileDownload("testpath/", "testfile.txt", "c:/ftptest/");
        Assert::AreEqual(true, ret);
        ret = m_FtpClient->FtpDisconnect();
        Assert::AreEqual(true, ret);
	}

    void TestFtpClient::FileUpload(){
        Assert::AreNotEqual(nullptr, m_FtpClient);
        m_FtpClient->SetHostInfo("localhost", "21", "", "");
        bool ret = m_FtpClient->FtpConnect();
        Assert::AreEqual(true, ret);
        ret = m_FtpClient->FileUpload("c:/ftptest/", "testfile.txt", "testpath/", "upload.txt");
        Assert::AreEqual(true, ret);
        ret = m_FtpClient->FtpDisconnect();
        Assert::AreEqual(true, ret);
	}

    void TestFtpClient::GetFileList(){
        Assert::AreNotEqual(nullptr, m_FtpClient);
        m_FtpClient->SetHostInfo("localhost", "21", "", "");
        bool ret = m_FtpClient->FtpConnect();
        Assert::AreEqual(true, ret);
        LinkedList<String^>^ t_flist = gcnew LinkedList<String^>();
        ret = m_FtpClient->GetFileList("testpath/", "*.*", t_flist);
        Assert::AreEqual(true, ret);
        Assert::Greater(t_flist->Count, 0);
        ret = m_FtpClient->FtpDisconnect();
        Assert::AreEqual(true, ret);
	}
}

