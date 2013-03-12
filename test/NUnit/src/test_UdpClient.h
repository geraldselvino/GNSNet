#pragma once

#include "GNSNetFactory.h"

using namespace System;
using namespace NUnit::Framework;
using namespace System::Threading;

namespace GNSNetTest{

	[TestFixture]
	public ref class TestUdpClient
	{
	public:
		[SetUp] void Init();

        [Test] void CreateSocket();
        [Test] void LastError();
        [Test] void GetClientName();
        [Test] void Send();

    private:
        GNSNet::UdpUserSocket^ m_UdpClient;
        void threadproc();
    };
}
