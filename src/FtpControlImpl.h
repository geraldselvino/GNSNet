/**********************************************************
* Copyright (c) 2013, Gerald Selvino <geraldsv@outlook.com>
* All rights reserved.
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
    /**
     * Mode of logging
     */
    enum class LOGMODE{
        LOGMODE_NONE,
        LOGMODE_START,
        LOGMODE_END,
    };

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
        void SetLogInfo(int LogKind);
        void SetLogInfo(int LogKind, int LogOutFlag);
        void SetHostInfo(String^ const% HostName, String^ const% PortNo, String^ const% UserName, String^ const% Password);
        void SetHostName(String^ const% HostName);
        bool FtpConnect();
        bool FtpDisconnect();

    public:
        /**
         * Interfaces to the outside world, template methods
         * to the actual procs found in private section
         */
        bool FileDownload(String^ const% RemotePath, String^ const% LocalPath, String^ const% RemoteFileName);
        bool FileDownload(String^ RemotePath, String^ LocalPath, String^ const% RemoteFileName, String^ LocalFileName);
        bool FileUpload(String^ const% RemotePath, String^ const% LocalPath, String^ const% RemoteFileName);
        bool FileUpload(String^ RemotePath, String^ LocalPath, String^ const% RemoteFileName, String^ LocalFileName);
        bool GetFileList(String^ RemotePath, String^ const% AllFileName, LinkedList<String^>^% FileList);
    
    protected:
        /**
         * Loging method, overridable, since logging format is subjective
         */
        virtual void LogOut(String^ const% Log, LOGMODE LogMode); 
        virtual void LogOut(String^ const% Log); 

    private:
        /**
         * Internal methods only, these methods contains the logic
         * of downloading, uploading and directory listing
         */
        bool FileDownloadProc(String^ const% RemotePath, String^ const% LocalPath, String^ const% RemoteFileName);
        bool FileDownloadProc(String^ const% RemotePath, String^ const% LocalPath, String^ const% RemoteFileName, String^ LocalFileName);
        bool FileUploadProc(String^ const% RemotePath, String^ const% LocalPath, String^ const% RemoteFileName);
        bool FileUploadProc(String^ const% RemotePath, String^ const% LocalPath, String^ const% RemoteFileName, String^ LocalFileName);
        bool GetFileListProc(String^ const% RemotePath, String^ const% AllFileName, LinkedList<String^>^% FileList);

    private:
        /**
         * Encapsulate members as private 
         */
        int            m_LogOutFlag;
        int            m_ConnectFlag;
        int            m_LogKind;
        String^        m_HostName;
        String^        m_PortNo;
        String^        m_UserName;
        String^        m_Password;
        FtpWebRequest^ reqFTP;    /**The ftp object that does the actual connection to ftp servers*/
    };
}
