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
: m_ConnectFlag(false),
  m_HostName("localhost"),
  m_PortNo("21"),
  m_UserName(""),
  m_Password("")
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

bool GNSNet::FtpControlImpl::FtpConnect()
{
    bool ret = true;
    String^ Log;

    if(m_ConnectFlag){
        return true;
    }

    try{
        reqFTP = dynamic_cast<FtpWebRequest^>(FtpWebRequest::Create(gcnew Uri("ftp://" + m_HostName + ":" + m_PortNo + "/")));
        reqFTP->Credentials = gcnew NetworkCredential(m_UserName, m_Password);
        reqFTP->KeepAlive = false;
        m_ConnectFlag = true;
        Log = String::Format("FtpConnect() successfuly connected at [{0}:{1}]", m_HostName, m_PortNo);
        Console::WriteLine(Log);
    }
    catch(Exception^ e){
        m_ConnectFlag = false;
        Log = "FtpConnect() throws an exception! " + e->Message;
        Console::WriteLine(Log);
        ret = false;
    }

    return ret;
}

bool GNSNet::FtpControlImpl::FtpDisconnect()
{
    bool ret = true;
    if(!m_ConnectFlag){
        return true;
    }

    try{
        reqFTP->Abort();
        delete reqFTP;
        m_ConnectFlag = false;
    }
    catch(Exception^ e){
        Console::WriteLine(e->Message);
        ret = false;
    }

    return ret;
}

bool GNSNet::FtpControlImpl::FileDownload(String^ const% RemotePath, String^ const% RemoteFileName, String^ const% LocalPath)
{
    return(FileDownload(RemotePath, RemoteFileName, LocalPath, ""));
}

bool GNSNet::FtpControlImpl::FileDownload(String^ RemotePath, String^ const% RemoteFileName, String^ LocalPath, String^ LocalFileName)
{
    if(LocalFileName == ""){
        LocalFileName = RemoteFileName;
    }

    String^ Log;

    Log = String::Format("Starting file download...");
    Console::WriteLine(Log);
    Log = String::Format("Source file: {0}{1} -> Destination file: {2}{3}", RemotePath, RemoteFileName, LocalPath, LocalFileName);
    Console::WriteLine(Log);

    char From = '\\';
    char To = '/';
    RemotePath = RemotePath->Replace(From, To);
    LocalPath = LocalPath->Replace(From, To);

    bool ret = FileDownloadProc(RemotePath, RemoteFileName, LocalPath, LocalFileName);

    if(ret){
        Log = String::Format("File download successful!");
    } 
    else{
        Log = String::Format("File download failed!");
    }

    Console::WriteLine(Log);

    return ret;
}

bool GNSNet::FtpControlImpl::FileUpload(String^ const% LocalPath, String^ const% LocalFileName, String^ const% RemotePath)
{
    return(FileUpload(LocalPath, LocalFileName, RemotePath, ""));
}

bool GNSNet::FtpControlImpl::FileUpload(String^ LocalPath, String^ const% LocalFileName, String^ RemotePath, String^ RemoteFileName)
{
    if(RemoteFileName == ""){
        RemoteFileName = LocalFileName;
    }

    String^ Log;
    Log = String::Format("Starting file upload...");
    Console::WriteLine(Log);
    Log = String::Format("Destination file: {0}{1} <- Source file: {2}{3}", RemotePath, RemoteFileName, LocalPath, LocalFileName);
    Console::WriteLine(Log);

    char From = '\\';
    char To = '/';
    RemotePath = RemotePath->Replace(From, To);
    LocalPath = LocalPath->Replace(From, To);

    bool ret = FileUploadProc(LocalPath, LocalFileName, RemotePath, RemoteFileName);

    if(ret){
        Log = String::Format("File upload successful!");
    }
    else{
        Log = String::Format("File upload failed!");
    }

    Console::WriteLine(Log);

    return ret;
}

bool GNSNet::FtpControlImpl::GetFileList(String^ RemotePath, String^ const% AllFileName, LinkedList<String^>^% FileList)
{
    String^ Log;
    Log = String::Format("Listing Directory...");
    Console::WriteLine(Log);
    Log = String::Format("Directory path: {0}{1}", RemotePath, AllFileName);
    Console::WriteLine(Log);

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

    Console::WriteLine(Log);

    return ret;
}

bool GNSNet::FtpControlImpl::FileDownloadProc(String^ const% RemotePath, String^ const% RemoteFileName, String^ const% LocalPath, String^ const% LocalFileName)
{
    bool ret = true;

    if(!m_ConnectFlag){
        Console::WriteLine("FileDownload() – error! there is no connection established");
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
    }
    catch (Exception^ e){
        Console::WriteLine(RemoteFileName + " download failed! " + e->Message);
        ret = false;
    }
    
    return ret;
}

bool GNSNet::FtpControlImpl::FileUploadProc(String^ const% LocalPath, String^ const% LocalFileName, String^ const% RemotePath, String^ const% RemoteFileName)
{
    bool ret = true;

    if(!m_ConnectFlag){
        Console::WriteLine("FileUpload() – error! there is no connection established");
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
    }
    catch(Exception^ e){
        Console::WriteLine(RemoteFileName + " upload failed! " + e->Message);
        ret = false; 
    }

    return ret;
}

bool GNSNet::FtpControlImpl::GetFileListProc(String^ const% RemotePath, String^ const% AllFileName, LinkedList<String^>^% FileList)
{
    bool ret = true;
    if(!m_ConnectFlag){
        Console::WriteLine("GetFileListProc() – error! there is no connection established");
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
        Encoding^ l_encode = Encoding::GetEncoding("utf-8");    
        StreamReader^ reader = gcnew StreamReader(response->GetResponseStream(), l_encode);

        String^ line;
        while((line=reader->ReadLine())!=nullptr){
            result->Append(line);
            result->Append("\n");
        }

        //remove the trailing '\n'
        result->Remove(result->ToString()->LastIndexOf('\n'), 1);
        reader->Close();
        response->Close();
        array<String^>^ tmpList = result->ToString()->Split('\n');

        for(int i=0 ; i<=tmpList->GetUpperBound(0); i++){
            FileList->AddLast(tmpList[i]);
        }
    }
    catch(Exception^ e){
        Console::WriteLine(AllFileName + " list failed! " + e->Message);
        ret = false;
    }

    return ret;
}
