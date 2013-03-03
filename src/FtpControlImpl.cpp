#include <Stdafx.h> //include to allow precompiled header in Visual Studio
#include "FtpControl.h"
#include "FtpControlImpl.h"

/**
 * Definition of the Handle class(FtpControl)
 */

GNSNet::FtpControl::FtpControl()
: pImplFtpControl(gcnew FtpControlImpl())
{
}

GNSNet::FtpControl::~FtpControl() 
{
   delete pImplFtpControl;
}

void GNSNet::FtpControl::SetHostInfo(String^ const% HostName, String^ const% PortNo, String^ const% UserName, String^ const% Password)
{
    return pImplFtpControl->SetHostInfo(HostName, PortNo, UserName, Password);
}

void GNSNet::FtpControl::SetHostName(String^ const% HostName)
{
    return pImplFtpControl->SetHostName(HostName);
}

void GNSNet::FtpControl::SetLogInfo(int LogKind)
{
    return pImplFtpControl->SetLogInfo(LogKind);
}

void GNSNet::FtpControl::SetLogInfo(int LogKind, int LogOutFlag)
{
    return pImplFtpControl->SetLogInfo(LogKind, LogOutFlag);
}

bool GNSNet::FtpControl::FtpConnect()
{
    return pImplFtpControl->FtpConnect();
}

bool GNSNet::FtpControl::FtpDisconnect()
{
    return pImplFtpControl->FtpDisconnect();
}

bool GNSNet::FtpControl::FileDownload(String^ const% RemotePath, String^ const% LocalPath, String^ const% RemoteFileName)
{
    return pImplFtpControl->FileDownload(RemotePath, LocalPath, RemoteFileName);
}

bool GNSNet::FtpControl::FileDownload(String^ RemotePath, String^ LocalPath, String^ const% RemoteFileName, String^ LocalFileName)
{
    return pImplFtpControl->FileDownload(RemotePath, LocalPath, RemoteFileName, LocalFileName);
}

bool GNSNet::FtpControl::FileUpload(String^ const% RemotePath, String^ const% LocalPath, String^ const% RemoteFileName)
{
    return pImplFtpControl->FileUpload(RemotePath, LocalPath, RemoteFileName);
}

bool GNSNet::FtpControl::FileUpload(String^ RemotePath, String^ LocalPath, String^ const% RemoteFileName, String^ LocalFileName)
{
    return pImplFtpControl->FileUpload(RemotePath, LocalPath, RemoteFileName, LocalFileName);
}

bool GNSNet::FtpControl::GetFileList(String^ RemotePath, String^ const% AllFileName, LinkedList<String^>^% FileList)
{
    return pImplFtpControl->GetFileList(RemotePath, AllFileName, FileList);
}

/**
 * Definition of the Body class(FtpControlImpl)
 */

GNSNet::FtpControlImpl::FtpControlImpl()
: m_ConnectFlag(false)
{
}

GNSNet::FtpControlImpl::~FtpControlImpl() 
{
    FtpDisconnect();
}

void GNSNet::FtpControlImpl::SetHostInfo(String^ const% HostName, String^ const% PortNo, String^ const% UserName, String^ const% Password)
{
    m_HostName  = HostName;
    m_PortNo    = PortNo;
    m_UserName  = UserName;
    m_Password  = Password;
}

void GNSNet::FtpControlImpl::SetHostName(String^ const% HostName)
{
    m_HostName = HostName;
}

void GNSNet::FtpControlImpl::SetLogInfo(int LogKind)
{
    SetLogInfo(LogKind, false);
}

void GNSNet::FtpControlImpl::SetLogInfo(int LogKind, int LogOutFlag)
{
    m_LogKind = LogKind;
    m_LogOutFlag = LogOutFlag;
}

bool GNSNet::FtpControlImpl::FtpConnect()
{
    String^ Log;

    if(m_ConnectFlag){
        return true;
    }

    try{
        reqFTP = dynamic_cast<FtpWebRequest^>(FtpWebRequest::Create(gcnew Uri("ftp://" + m_HostName + ":" + m_PortNo + "/")));
        reqFTP->Credentials = gcnew NetworkCredential(m_UserName, m_Password);
        reqFTP->KeepAlive = false;
        m_ConnectFlag = true;
    }
    catch(Exception^ e){
        m_ConnectFlag = false;
        Log = "FtpConnect() throws an exception! " + e->Message;
        LogOut(Log);
        return false;
    }

    Log = String::Format("FtpConnect() successfuly connected at [{0}:{1}]", m_HostName, m_PortNo);
    LogOut(Log);

    return true;
}

bool GNSNet::FtpControlImpl::FtpDisconnect()
{
    if(!m_ConnectFlag) return true;

    try{
        reqFTP->Abort();
        delete reqFTP;
        m_ConnectFlag = false;
        return true;
    }
    catch(Exception^ e){
        LogOut(e->Message);
        return false;
    }
}

bool GNSNet::FtpControlImpl::FileDownload(String^ const% RemotePath, String^ const% LocalPath, String^ const% RemoteFileName)
{
    return(FileDownload(RemotePath, LocalPath, RemoteFileName, ""));
}

bool GNSNet::FtpControlImpl::FileDownload(String^ RemotePath, String^ LocalPath, String^ const% RemoteFileName, String^ LocalFileName)
{
    if(LocalFileName == ""){
        LocalFileName = RemoteFileName;
    }

    String^ Log;

    Log = String::Format("Starting file download...");
    LogOut(Log, LOGMODE::LOGMODE_START);
    Log = String::Format("Source file: {0}{1} -> Destination file: {2}{3}", RemotePath, RemoteFileName, LocalPath, LocalFileName);
    LogOut(Log);

    char From = '\\';
    char To = '/';
    RemotePath = RemotePath->Replace(From, To);
    LocalPath = LocalPath->Replace(From, To);

    bool ret = FileDownloadProc(RemotePath, LocalPath, RemoteFileName, LocalFileName);

    if(ret){
        Log = String::Format("File download successful!");
    } 
    else{
        Log = String::Format("File download failed!");
    }

    LogOut(Log, LOGMODE::LOGMODE_END);

    return ret;
}

bool GNSNet::FtpControlImpl::FileUpload(String^ const% RemotePath, String^ const% LocalPath, String^ const% RemoteFileName)
{
    return(FileUpload(RemotePath, LocalPath, RemoteFileName, ""));
}

bool GNSNet::FtpControlImpl::FileUpload(String^ RemotePath, String^ LocalPath, String^ const% RemoteFileName, String^ LocalFileName)
{
    if(LocalFileName == ""){
        LocalFileName = RemoteFileName;
    }

    String^ Log;
    Log = String::Format("Starting file upload...");
    LogOut(Log, LOGMODE::LOGMODE_START);
    Log = String::Format("Destination file: {0}{1} <- Source file: {2}{3}", RemotePath, RemoteFileName, LocalPath, LocalFileName);
    LogOut(Log);

    char From = '\\';
    char To = '/';
    RemotePath = RemotePath->Replace(From, To);
    LocalPath = LocalPath->Replace(From, To);

    bool ret = FileUploadProc(RemotePath, LocalPath, RemoteFileName, LocalFileName);

    if(ret){
        Log = String::Format("File upload successful!");
    }
    else{
        Log = String::Format("File upload failed!");
    }

    LogOut(Log, LOGMODE::LOGMODE_END);

    return ret;
}

bool GNSNet::FtpControlImpl::GetFileList(String^ RemotePath, String^ const% AllFileName, LinkedList<String^>^% FileList)
{
    String^ Log;
    Log = String::Format("Listing Directory...");
    LogOut(Log, LOGMODE::LOGMODE_START);
    Log = String::Format("Directory path: {0}{1}", RemotePath, AllFileName);
    LogOut(Log);

    char From = '\\';
    char To = '/';
    RemotePath = RemotePath->Replace(From, To);

    bool ret = GetFileListProc(RemotePath, AllFileName, FileList);

    if(ret){
        Log = String::Format("Directory list successful!");
    }
    else{
        Log = String::Format("Directory list failed!");
    }

    LogOut(Log, LOGMODE::LOGMODE_END);

    return ret;
}

bool GNSNet::FtpControlImpl::FileDownloadProc(String^ const% RemotePath, String^ const% LocalPath, String^ const% RemoteFileName)
{
    return(FileDownloadProc(RemotePath, LocalPath, RemoteFileName, ""));
}

bool GNSNet::FtpControlImpl::FileDownloadProc(String^ const% RemotePath, String^ const% LocalPath, String^ const% RemoteFileName, String^ LocalFileName)
{
    if(LocalFileName == ""){
        LocalFileName = RemoteFileName;
    }

    bool ret = FtpConnect();
    if(!ret){
        return false;
    }

    if(!m_ConnectFlag){
        LogOut("FileDownload() – error! there is no connection established");
        return false;
    }

    try{
        FileStream^ outputStream = gcnew FileStream(LocalPath + LocalFileName, FileMode::Create);

        reqFTP = dynamic_cast<FtpWebRequest^>(FtpWebRequest::Create(gcnew Uri("ftp://" + m_HostName + ":" + m_PortNo + "/" + RemotePath + RemoteFileName)));
        reqFTP->Method = WebRequestMethods::Ftp::DownloadFile;
        reqFTP->UseBinary = true;
        reqFTP->Credentials = gcnew NetworkCredential(m_UserName, m_Password);
        reqFTP->KeepAlive = false;
        
        FtpWebResponse^ response = dynamic_cast<FtpWebResponse^>(reqFTP->GetResponse());
        Stream^ ftpStream = response->GetResponseStream();
        
        int bufferSize = 2048;
        int readCount;
        array<Byte>^ buffer = gcnew array<Byte>(bufferSize);

        readCount = ftpStream->Read(buffer, 0, bufferSize);
        while(readCount>0){
            outputStream->Write(buffer, 0, readCount);
            readCount = ftpStream->Read(buffer, 0, bufferSize);
        }

        ftpStream->Close();
        outputStream->Close();
        response->Close();

        FtpDisconnect();
    }
    catch (Exception^ e){
        LogOut(RemoteFileName + " download failed! " + e->Message);
        return false;
    }
    
    return true;
}

bool GNSNet::FtpControlImpl::FileUploadProc(String^ const% RemotePath, String^ const% LocalPath, String^ const% RemoteFileName)
{
    return(FileUploadProc(RemotePath, LocalPath, RemoteFileName, ""));
}

bool GNSNet::FtpControlImpl::FileUploadProc(String^ const% RemotePath, String^ const% LocalPath, String^ const% RemoteFileName, String^ LocalFileName)
{
    if(LocalFileName == ""){
        LocalFileName = RemoteFileName;
    }

    bool ret = FtpConnect();
    if(!ret){
        return false;
    }

    if(!m_ConnectFlag){
        LogOut("FileUpload() – error! there is no connection established");
        return false;
    }

    try{
        FileInfo^ fileInf = gcnew FileInfo(LocalPath + LocalFileName);

        reqFTP = dynamic_cast<FtpWebRequest^>(FtpWebRequest::Create(gcnew Uri("ftp://" + m_HostName + ":" + m_PortNo + "/" + RemotePath + RemoteFileName)));
        reqFTP->Credentials = gcnew NetworkCredential(m_UserName, m_Password);
        reqFTP->KeepAlive = false;
        reqFTP->Method = WebRequestMethods::Ftp::UploadFile;
        reqFTP->UseBinary = true;
        reqFTP->ContentLength = fileInf->Length;

        int buffLength = 2048;
        array<Byte>^ buff = gcnew array<Byte>(buffLength);
        int contentLen;

        FileStream^ fs = fileInf->OpenRead();
        Stream^ strm = reqFTP->GetRequestStream();

        contentLen = fs->Read(buff, 0, buffLength);

        while (contentLen!=0){
            strm->Write(buff, 0, contentLen);
            contentLen = fs->Read(buff, 0, buffLength);
        }

        strm->Close();
        fs->Close();

        FtpDisconnect();
    }
    catch(Exception^ e){
        LogOut(RemoteFileName + " upload failed! " + e->Message);
        return false; 
    }

    return true;
}

bool GNSNet::FtpControlImpl::GetFileListProc(String^ const% RemotePath, String^ const% AllFileName, LinkedList<String^>^% FileList)
{
    bool ret = FtpConnect();
    if(!ret){
        return false;
    }

    if(!m_ConnectFlag){
        LogOut("GetFileListProc() – error! there is no connection established");
        return false;
    }

    try{
        StringBuilder^ result = gcnew StringBuilder();

        reqFTP = dynamic_cast<FtpWebRequest^>(FtpWebRequest::Create(gcnew Uri("ftp://" + m_HostName + ":" + m_PortNo + "/" + RemotePath + AllFileName)));
        reqFTP->UseBinary = true;
        reqFTP->Credentials = gcnew NetworkCredential(m_UserName, m_Password);
        reqFTP->KeepAlive = false;
        reqFTP->Method = WebRequestMethods::Ftp::ListDirectory;

        WebResponse^ response = reqFTP->GetResponse();
        Encoding^ l_encode = Encoding::GetEncoding("shift_jis");    
        StreamReader^ reader = gcnew StreamReader(response->GetResponseStream(), l_encode);

        String^ line = reader->ReadLine();

        while(line!=nullptr){
            result->Append(line);
            result->Append("\n");
            line = reader->ReadLine();
        }

        //remove the trailing '\n'
        result->Remove(result->ToString()->LastIndexOf('\n'), 1);
        reader->Close();
        response->Close();
        array<String^>^ tmpList = result->ToString()->Split('\n');

        for(int i=0 ; i<=tmpList->GetUpperBound(0); i++){
            FileList->AddLast(tmpList[i]);
        }

        FtpDisconnect();
    }
    catch(Exception^ e){
        LogOut(AllFileName + " list failed! " + e->Message);
        return false;
    }

    return true;
}

void GNSNet::FtpControlImpl::LogOut(String^ const% Log)
{
    LogOut(Log, LOGMODE::LOGMODE_NONE);
}

void GNSNet::FtpControlImpl::LogOut(String^ const% Log, LOGMODE LogMode)
{
    String^ Header;
    
    switch(LogMode){
    case LOGMODE::LOGMODE_START:
        Header = "„¡";
        break;
    case LOGMODE::LOGMODE_END:
        Header = "„¤";
        break;
    default:
        Header = "„ ";
        break;
    }

    String^ log = String::Format("{0}{1}", Header, Log);
}
