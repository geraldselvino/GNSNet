#pragma once

#include "GNSNetFactory.h"

using namespace System;
using namespace NUnit::Framework;
using namespace System::Threading;

namespace GNSNetTest{

	[TestFixture]
	public ref class TestFtpClient
	{
	public:
		[SetUp] void Init();

        [Test] void FtpConnect();
        [Test] void FileDownload();
        [Test] void FileUpload();
        [Test] void GetFileList();

    private:
        GNSNet::FtpControl^ m_FtpClient;
    };
}
