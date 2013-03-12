#pragma once

#include "GNSNetFactory.h"

using namespace System;
using namespace NUnit::Framework;
using namespace System::Threading;

namespace GNSNetTest{

	[TestFixture]
	public ref class TestUdpServer
	{
	public:
		[SetUp] void Init();

        [Test] void CreateSocket();
        [Test] void LastError();
        [Test] void Receive();
        [Test] void GetClientName();

    private:
        GNSNet::UdpUserSocket^ m_UdpServer;
        void threadproc();
        void threadprocgcn();
    };
}
