/**********************************************************
* Copyright (c) 2013, Gerald Selvino
* <gerald.selvino@protonmail.com> All rights reserved.
*
* This is the implementation class of FtpControl, use as an
* FTP client to FTP Servers, functions supported are
* Download, Upload, and List directory.
***********************************************************/
#pragma once

/**
 * Namespaces of the libraries that this
 * class used
 */
using namespace System;
using namespace System::Net;
using namespace System::IO;
using namespace System::Collections::Generic;
using namespace System::Text;

namespace GNSNet{

    public ref class FtpControlImpl : public Object 
    {
    public:
        FtpControlImpl();
        /**
         * Virtual, so that inheriter can destroy it 
         * using this class' pointer
         */
        virtual ~FtpControlImpl();

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
         * Interfaces to the outside world, template methods
         * to the actual procs found in private section
         */
        bool FileDownload(String^ const% RemotePath, String^ const% RemoteFileName, String^ const% LocalPath);
        bool FileDownload(String^ RemotePath, String^ const% RemoteFileName, String^ LocalPath, String^ LocalFileName);
        bool FileUpload(String^ const% LocalPath, String^ const% LocalFileName, String^ const% RemotePath);
        bool FileUpload(String^ LocalPath, String^ const% LocalFileName, String^ RemotePath, String^ RemoteFileName);
        bool GetFileList(String^ RemotePath, String^ const% AllFileName, LinkedList<String^>^% FileList);

    private:
        /**
         * Internal methods only, these methods contains the logic
         * of downloading, uploading and directory listing
         */
        bool FileDownloadProc(String^ const% RemotePath, String^ const% RemoteFileName, String^ const% LocalPath, String^ const% LocalFileName);
        bool FileUploadProc(String^ const% LocalPath, String^ const% LocalFileName, String^ const% RemotePath, String^ const% RemoteFileName);
        bool GetFileListProc(String^ const% RemotePath, String^ const% AllFileName, LinkedList<String^>^% FileList);

    private:
        /**
         * Encapsulate members as private 
         */
        int            m_ConnectFlag;
        String^        m_HostName;
        String^        m_PortNo;
        String^        m_UserName;
        String^        m_Password;
        FtpWebRequest^ reqFTP;    /**The ftp object that does the actual connection to ftp servers*/
    };
}
