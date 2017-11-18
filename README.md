GNSNet [C++/CLI Network simplifier]
===================================
Library that simplifies network programming in C++/CLI,
the library provides a very simplified interface for communicating to the 
network using TCP/IP sockets, UDP sockets and also includes a class library that can be used as an FTP client. Includes a simple multi-client TCP/IP server through the StartService() method.

Requirement
===========
Visual Studio   
.NET framework 2.0 or higher

Sample usage
============
**Note:** Check the documentation (presentation/HighLevelDesign&Usage.pdf) and unit test solution (test/NUnit/test_GNSNet.sln) for complete usage information

*A simple example of using this library to create a TCP client/server, check the unit test solution for UDP and FTP usage. Check the documentation for high level design*  

```c++
//Create a TCP server
ServerSocket^ st = GNSNetFactory::getTcpServerSocket();
st->StartService(500);

//Use TCP client to send a string to the server
ClientSocket^ cst = GNSNetFactory ::getTcpClientSocket();
bool ret = cst->Connect(“localhost”, 500);
If(ret){
cst->Send(“hello”);
}
```
