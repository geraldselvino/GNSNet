/**********************************************************
* Copyright (c) 2013, Gerald Selvino
* <gerald.selvino@protonmail.com> All rights reserved.
*
* This is the handle class to the body (FtpControlImpl)
***********************************************************/
#pragma once

/**
 * Namespaces of the libraries that this
 * class used
 */
using namespace System;
using namespace System::Collections::Generic;

namespace GNSNet{
    /**
     * Forward declare FtpControlImpl, because
     * we just need a reference of it here
     */
    ref class FtpControlImpl;

    public ref class FtpControl 
    {
    public:
        FtpControl();
        /**
         * Virtual, so that inheriter can destroy it 
         * using this class' pointer
         */
        virtual ~FtpControl();

    public:
        /**
         * Interfaces to the outside world 
         */
        void SetHostInfo(String^ const% HostName, String^ const% PortNo, String^ const% UserName, String^ const% Password);
        void SetHostName(String^ const% HostName);
        bool FtpConnect();
        bool FtpDisconnect();

    public:
        /**
         * Interfaces to the outside world 
         */
        bool FileDownload(String^ const% RemotePath, String^ const% RemoteFileName, String^ const% LocalPath);
        bool FileDownload(String^ RemotePath, String^ RemoteFileName, String^ const% LocalPath, String^ LocalFileName);
        bool FileUpload(String^ const% LocalPath, String^ const% LocalFileName, String^ const% RemotePath);
        bool FileUpload(String^ LocalPath, String^ LocalFileName, String^ const% RemotePath, String^ RemoteFileName);
        bool GetFileList(String^ RemotePath, String^ const% AllFileName, LinkedList<String^>^% FileList);

    private:
        /**
         * Reference/Pointer to the implementation of FtpControl
         */
        FtpControlImpl^ pImplFtpControl;
    };
}
