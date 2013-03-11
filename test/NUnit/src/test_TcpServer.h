#pragma once

#include "GNSNetFactory.h"

using namespace System;
using namespace NUnit::Framework;

namespace GNSNetTest{

	[TestFixture]
	public ref class TestTcpServer
	{
	public:
		[SetUp] void Init();

        [Test] void SetKeepAliveOption();
		[Test] void StartService();
        [Test] void ReadMessage();

    private:
        GNSNet::ServerSocket^ m_Server;
        void threadproc();
    };
}
