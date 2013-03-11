#pragma once

#include "GNSNetFactory.h"

using namespace System;
using namespace NUnit::Framework;

namespace GNSNetTest{

	[TestFixture]
	public ref class TestTcpClient
	{
	public:
		[SetUp] void Init();

        [Test] void Connect();
		[Test] void GetClientName();
        [Test] void Send();
        [Test] void SetKeepAlive();

    private:
        GNSNet::ClientSocket^ m_Client;
        void threadProc();
    };
}
