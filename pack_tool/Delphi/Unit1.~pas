unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, INIFiles, ShellAPI, ExtCtrls, xmldom,
  XMLIntf, msxmldom, XMLDoc, IdBaseComponent, IdComponent, IdTCPConnection,
  IdTCPClient, IdHTTP, TLHelp32, Buttons;

type
  TForm1 = class(TForm)
    ComboBox1: TComboBox;
    Button1: TButton;
    Edit1: TEdit;
    mmo1: TMemo;
    btn1: TButton;
    lbl1: TLabel;
    cbb1: TComboBox;
    tmr1: TTimer;
    Label1: TLabel;
    edt1: TEdit;
    XMLDocument1: TXMLDocument;
    IdHTTP1: TIdHTTP;
    Edit2: TEdit;
    lbl2: TLabel;
    lbl3: TLabel;
    lbl4: TLabel;
    XMLDocument2: TXMLDocument;
    Button2: TButton;
    Button3: TButton;
    Button4: TButton;
    OpenDialog1: TOpenDialog;
    Button5: TButton;
    BitBtn1: TBitBtn;
    procedure FormCreate(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure btn1Click(Sender: TObject);
    procedure tmr1Timer(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure BitBtn1Click(Sender: TObject);
    
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;
  smallVer, xmlPath, iniPath, batPath, curSmallVer, androidXml,
  batDebugApk, batReleaseApk, batMoveApk, batDebugAndUploadResToSvnApk,
  batDebugAndUploadResToSvn, batPushDebugApkToSdcard, batPushReleaseApkToSdcard,
  manifestUrl, innerGameConfigUrl,
  androidVer, initReleaseStr, cmdParam: string;

  myMemo: TMemo;
  myButton1: TButton;
  myButton2: TButton;
  myButton3: TButton;
  myButton4: TButton;
  myButton5: TButton;
  myButton6: TButton;
  myXmlDoc: TXMLDocument;
  handleType, myReleaseTip: string;

implementation

uses uJSON;

{$R *.dfm}

procedure CheckResult(b: Boolean);  
begin  
  if not b then  
    raise Exception.Create(SysErrorMessage(GetLastError));  
end;  
   
function RunDOS(const CommandLine: string): string;  
var
  HRead, HWrite: THandle;  
  StartInfo: TStartupInfo;  
  ProceInfo: TProcessInformation;  
  b: Boolean;  
  sa: TSecurityAttributes;  
  inS: THandleStream;  
  sRet: TStrings;  
begin  
  Result := '';  
  FillChar(sa, sizeof(sa), 0);  
//设置允许继承，否则在NT和2000下无法取得输出结果  
  sa.nLength := sizeof(sa);  
  sa.bInheritHandle := True;  
  sa.lpSecurityDescriptor := nil;  
  b := CreatePipe(HRead, HWrite, @sa, 0);  
  CheckResult(b);  
   
  FillChar(StartInfo, SizeOf(StartInfo), 0);  
  StartInfo.cb := SizeOf(StartInfo);  
  StartInfo.wShowWindow := SW_HIDE;  
//使用指定的句柄作为标准输入输出的文件句柄,使用指定的显示方式  
  StartInfo.dwFlags := STARTF_USESTDHANDLES or STARTF_USESHOWWINDOW;  
  StartInfo.hStdError := HWrite;  
  StartInfo.hStdInput := GetStdHandle(STD_INPUT_HANDLE); //HRead;  
  StartInfo.hStdOutput := HWrite;  
   
  b := CreateProcess(nil, //lpApplicationName: PChar  
    PChar(CommandLine), //lpCommandLine: PChar  
    nil, //lpProcessAttributes: PSecurityAttributes  
    nil, //lpThreadAttributes: PSecurityAttributes  
    True, //bInheritHandles: BOOL  
    CREATE_NEW_CONSOLE,  
    nil,  
    nil,  
    StartInfo,  
    ProceInfo);  
   
  CheckResult(b);  
  WaitForSingleObject(ProceInfo.hProcess, INFINITE);  
   
  inS := THandleStream.Create(HRead);  
  if inS.Size > 0 then  
  begin  
    sRet := TStringList.Create;  
    sRet.LoadFromStream(inS);  
    Result := sRet.Text;  
    sRet.Free;  
  end;  
  inS.Free;  
   
  CloseHandle(HRead);  
  CloseHandle(HWrite);
end;


procedure generatePackageSuccess();
var
  xmlNode: IXMLNode;
  begin
    if handleType = 'small_package' then
      begin
        myMemo.Lines.Add('更新资源包生成完成.');
      end
    else if handleType = 'apk_release_package' then
      begin
        myMemo.Lines.Add('apk生成完成.');
        // 移动apk
        //RunDosInMemo(batMoveApk, myMemo);
        myMemo.Lines.Add('移动apk到目录apks下.');
        myMemo.Lines.Add('apk为' + myReleaseTip);
        myMemo.Lines.Add('若安装不成功,请自行到项目/public/android下获取apk安装.');
      end
    else if handleType = 'apk_debug_package' then
      begin
        myMemo.Lines.Add('apk 生成完成.');
        // 移动apk
        //RunDosInMemo(batMoveApk, myMemo);
        myMemo.Lines.Add('移动apk到目录apks下.');
        myMemo.Lines.Add('apk为' + myReleaseTip);
      end
    else if handleType = 'apk_debug_package_and_update_res_to_svn' then
      begin
        myMemo.Lines.Add('apk 生成完成.');
        // 移动apk
        //RunDosInMemo(batMoveApk, myMemo);
        myMemo.Lines.Add('移动apk到目录apks下.');
        myMemo.Lines.Add('apk为内网debug apk');
        // 修改AndroidManifest.xml 版本号
        myXmlDoc.LoadFromFile(androidXml);
        myXmlDoc.DocumentElement.Attributes['android:versionName']:= androidVer;
        myXmlDoc.SaveToFile(androidXml);
      end
    else if handleType = 'debug_package_and_update_res_to_svn' then
      begin
        myMemo.Lines.Add('内网更新资源生成完成, 并上传到内网更新服');
        // 移动apk
        //RunDosInMemo(batMoveApk, myMemo);
      end
    else if handleType = 'batPushReleaseApkToSdcard' then
      begin
        myMemo.Lines.Add('成功上传release apk到手机存储卡');
      end
    else if handleType = 'batPushDebugApkToSdcard' then
      begin
        myMemo.Lines.Add('成功上传debug apk到手机存储卡');
      end;
      myButton1.Enabled:= true;
      myButton2.Enabled:= true;
      myButton3.Enabled:= True;
      myButton4.Enabled:= True;
      myButton5.Enabled:= True;
      myButton6.Enabled:= True;
//      复原 ver_info.xml 服务器内外网配置 值
//      myXmlDoc.LoadFromFile(xmlPath);
//      xmlNode:= myXmlDoc.DocumentElement;
//      xmlNode:= xmlNode.ChildNodes[0];
//      xmlNode:= xmlNode.ChildNodes[1];
//      xmlNode:= xmlNode.ChildNodes[0];
//      xmlNode.NodeValue:= initReleaseStr;
//      myXmlDoc.SaveToFile(xmlPath);
end;

procedure RunDosInMemo(Que:String;EnMemo:TMemo);

   const
      CUANTOBUFFER = 500;
  var
     Seguridades         : TSecurityAttributes; 
     PaLeer,PaEscribir   : THandle; 
     start               : TStartUpInfo;
     ProcessInfo         : TProcessInformation; 
     Buffer              : Pchar; 
     BytesRead           : DWord;
     CuandoSale          : DWord; 
  begin
    //安全描述 可以省略
    with Seguridades do 
    begin 
       nlength              := SizeOf(TSecurityAttributes); 
       binherithandle       := true;
       lpsecuritydescriptor := nil; 
    end; 
  
    {Creamos el pipe...} 
    if Createpipe (PaLeer, PaEscribir, @Seguridades, 0) then 
    begin 
       //申请缓冲
       Buffer  := AllocMem(CUANTOBUFFER + 1);
    
       //创建STARTUPINFO 
       FillChar(Start,Sizeof(Start),#0);
       start.cb          := SizeOf(start); 
       start.hStdOutput  := PaEscribir; 
       start.hStdInput   := PaLeer;
       start.dwFlags     := STARTF_USESTDHANDLES + 
                            STARTF_USESHOWWINDOW; 
       start.wShowWindow := SW_HIDE; 
         
       //执行子进程  
      if CreateProcess(nil, 
         PChar(Que),
         @Seguridades, 
         @Seguridades, 
          true, 
          NORMAL_PRIORITY_CLASS, 
         nil, 
         nil, 
          start,
          ProcessInfo) 
      then 
        begin 
          {Espera a que termine la ejecucion} 
          repeat 
             //使用信号量技术来避免CPU时间片被抢占
             CuandoSale := WaitForSingleObject(ProcessInfo.hProcess, 5);
             Application.ProcessMessages;
            //until (CuandoSale <> WAIT_TIMEOUT);
            {Leemos la Pipe}
            //repeat
             BytesRead := 0;

            {Llenamos un troncho de la pipe, igual a nuestro buffer} 
             //执行标准输出
             ReadFile(PaLeer,Buffer[0],CUANTOBUFFER,BytesRead,nil);
            {La convertimos en una string terminada en cero}
             Buffer[BytesRead]:= #0;
            {Convertimos caracteres DOS a ANSI}
             OemToAnsi(Buffer,Buffer);
             EnMemo.Lines.Add(String(Buffer));
          until (CuandoSale <> WAIT_TIMEOUT);//(BytesRead < CUANTOBUFFER);
        end;
       FreeMem(Buffer);

       //释放资源
       CloseHandle(ProcessInfo.hProcess);
       CloseHandle(ProcessInfo.hThread);
       CloseHandle(PaLeer);
       CloseHandle(PaEscribir);
    end;

    generatePackageSuccess();
  end;

// 结束进程
function KillTask(ExeFileName: string): integer;
const
  PROCESS_TERMINATE = $0001;
var
  ContinueLoop: BOOLean;
  FSnapshotHandle: THandle;
  FProcessEntry32: TProcessEntry32;
begin
  Result := 0;
  FSnapshotHandle := CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  FProcessEntry32.dwSize := SizeOf(FProcessEntry32);
  ContinueLoop := Process32First(FSnapshotHandle, FProcessEntry32);
  while integer(ContinueLoop) <> 0 do
  begin
    if ((UpperCase(ExtractFileName(FProcessEntry32.szExeFile)) = UpperCase
          (ExeFileName)) or (UpperCase(FProcessEntry32.szExeFile) = UpperCase
          (ExeFileName))) then
      Result := integer(TerminateProcess(OpenProcess(PROCESS_TERMINATE, BOOL(0)
              , FProcessEntry32.th32ProcessID), 0));
    ContinueLoop := Process32Next(FSnapshotHandle, FProcessEntry32);
  end;
  CloseHandle(FSnapshotHandle);
end;

//procedure RunDosInMemo(Que:String;EnMemo:TMemo);
//
//   const
//      CUANTOBUFFER = 9999;
//  var
//     Seguridades         : TSecurityAttributes; 
//     PaLeer,PaEscribir   : THandle; 
//     start               : TStartUpInfo;
//     ProcessInfo         : TProcessInformation; 
//     Buffer              : Pchar; 
//     BytesRead           : DWord;
//     CuandoSale          : DWord; 
//  begin 
//    //安全描述 可以省略
//    with Seguridades do 
//    begin 
//       nlength              := SizeOf(TSecurityAttributes); 
//       binherithandle       := true;
//       lpsecuritydescriptor := nil; 
//    end; 
//  
//    {Creamos el pipe...} 
//    if Createpipe (PaLeer, PaEscribir, @Seguridades, 0) then 
//    begin 
//       //申请缓冲
//       Buffer  := AllocMem(CUANTOBUFFER + 1);
//    
//       //创建STARTUPINFO 
//       FillChar(Start,Sizeof(Start),#0);
//       start.cb          := SizeOf(start); 
//       start.hStdOutput  := PaEscribir; 
//       start.hStdInput   := PaLeer;
//       start.dwFlags     := STARTF_USESTDHANDLES + 
//                            STARTF_USESHOWWINDOW; 
//       start.wShowWindow := SW_HIDE; 
//         
//       //执行子进程  
//      if CreateProcess(nil, 
//         PChar(Que),
//         @Seguridades, 
//         @Seguridades, 
//          true, 
//          NORMAL_PRIORITY_CLASS, 
//         nil, 
//         nil, 
//          start,
//          ProcessInfo) 
//      then 
//        begin 
//          {Espera a que termine la ejecucion} 
//          repeat 
//             //使用信号量技术来避免CPU时间片被抢占
//             CuandoSale := WaitForSingleObject( ProcessInfo.hProcess,100);
//             Application.ProcessMessages;
//          //until (CuandoSale <> WAIT_TIMEOUT);
//          {Leemos la Pipe} 
//          //repeat
//             BytesRead := 0; 
//
//            {Llenamos un troncho de la pipe, igual a nuestro buffer} 
//             //执行标准输出
//             ReadFile(PaLeer,Buffer[0],CUANTOBUFFER,BytesRead,nil);
//            {La convertimos en una string terminada en cero}
//             Buffer[BytesRead]:= #0;
//            {Convertimos caracteres DOS a ANSI}
//             OemToAnsi(Buffer,Buffer);
//             EnMemo.Lines.Add(String(Buffer));
//          until (CuandoSale <> WAIT_TIMEOUT);//(BytesRead < CUANTOBUFFER);
//        end;
//       FreeMem(Buffer);
//
//       //释放资源
//       CloseHandle(ProcessInfo.hProcess);
//       CloseHandle(ProcessInfo.hThread);
//       CloseHandle(PaLeer);
//       CloseHandle(PaEscribir);
//    end;
//  end;

// 分割字符串
function Split(sSource,sSubStr: WideString): TStringList;
var
    iCurrentPlace: Byte;
    StrList: TStringList;
begin
    StrList := TStringList.Create;
    while Pos(sSubStr, sSource)>0 do  //循环判断是否存在分割符
    begin
        iCurrentPlace := Pos(sSubStr, sSource); //取当前分隔符的位置
        StrList.Add(Copy(sSource, 1, iCurrentPlace-1));   //添加项目
        sSource := Copy(sSource, iCurrentPlace+Length(sSubStr), Length(sSource)-iCurrentPlace);  //减去已添加项和分隔符
    end;
    StrList.Add(sSource); //如果不存在分隔符时，直接作为项目添加
    Result := StrList;
end;

// 修改ver_info.xml 服务器内外网配置
//function modifyGameServerCfg(xmlDoc: XMLDocument) : string;
//var
//  xmlNode: IXMLNode;
//begin
//  // xml的 release 位置不可改变
//  xmlNode:= xmlDoc.DocumentElement;
//  xmlNode := xmlNode.ChildNodes[0];
//  xmlNode := xmlNode.ChildNodes[1];
//  xmlNode := xmlNode.ChildNodes[0];
//  xmlNode.NodeValue:= 'true';
//  Result := xmlNode.XML;
//end;

procedure TForm1.FormCreate(Sender: TObject);
var
  myIniFile: TInifile;
  mainfestData: string;
  jo: JSONObject;
  xmlNode, androidxmlNode: IXMLNode;
  newVer: string;
  strList: TStringList;
  a, b, c: Integer;
  packageUrl_debug, packageUrl_release : string;

begin
  // 结束cmd进程
  if KillTask('cmd.exe') = 0 then
    begin
      //showmessage('请打开任务管理器,结束cmd进程!');
    end;
  // 结束加密进程
  KillTask('php.exe');
  KillTask('python.exe');
  //KillTask('adb.exe');
  //KillTask('conhost.exe');
  KillTask('Robocopy.exe');

  // 内外网地址
  iniPath:= ExtractFilePath(Application.Exename) + 'generate_file_md5\config.ini';
  myIniFile:= TInifile.Create(iniPath);
  packageUrl_debug:= myIniFile.ReadString('UPDATE', 'packageUrl_debug', 'http://192.168.0.111/update_resource/');
  packageUrl_release:= myIniFile.ReadString('UPDATE', 'packageUrl_release', 'http://res.balehd.com/update_resource/');
  myIniFile.Free;

  iniPath:= ExtractFilePath(Application.Exename) + 'small_version.ini';
  //xmlPath:= 'E:/MyWork/trunk/client/BaLe/res/version/ver_info.xml';
  xmlPath:= ExtractFilePath(ExtractFileDir(Application.ExeName)+'x.x')+'BaLe\res\version\ver_info.xml';
  batPath:= ExtractFilePath(Application.Exename) + 'genarate_package.bat';
  batDebugApk:= ExtractFilePath(Application.Exename) + 'generate_not_encrypt_apk.bat';
  batReleaseApk:= ExtractFilePath(Application.Exename) + 'genarate_apk.bat';
  batDebugAndUploadResToSvnApk:= ExtractFilePath(Application.Exename) + 'genarate_package_not_encrypt_1.bat';
  batDebugAndUploadResToSvn:= ExtractFilePath(Application.Exename) + 'genarate_package_not_encrypt_2.bat';
  batMoveApk:= ExtractFilePath(Application.Exename) + 'move_apk_here.bat';
  manifestUrl:= packageUrl_release + 'version_dev.manifest';
  innerGameConfigUrl:= packageUrl_debug + 'game_config.manifest';
  batPushDebugApkToSdcard:= ExtractFilePath(Application.Exename) + 'adb_push_debug_apk_to_sdcard.bat';
  batPushReleaseApkToSdcard:= ExtractFilePath(Application.Exename) + 'adb_push_release_apk_to_sdcard.bat';
  // android
  androidXml:= ExtractFilePath(ExtractFileDir(Application.ExeName)+'x.x')+'BaLe\frameworks\runtime-src\proj.android\AndroidManifest.xml';
  //androidXml:= ExtractFilePath(Application.Exename) + 'AndroidManifest.xml';

  // 显示小版本号
  //myIniFile:= TInifile.Create(iniPath);
  //smallVer:= myIniFile.ReadString('UPDATE', 'version', '1.0.0');
  //Edit1.Text:= smallVer;
  //curSmallVer:= '0';
  //myIniFile.Free;

  // 获取线上最新的小版本
  mainfestData:=idHTTP1.Get(manifestUrl);
  if mainfestData = '' then
    begin
      ShowMessage('获取线上小版本失败, ' + manifestUrl);
      Exit;
    end;

  jo:= JSONObject.Create(mainfestData);
  smallVer:= jo['version'];
  Edit1.Text:= smallVer;
  // 小版本号加1
  strList:= Split(smallVer, '.');
  a:= StrToInt(strList[0]);
  b:= StrToInt(strList[1]);
  c:= StrToInt(strList[2]);
  if c < 9 then
    c:= c + 1
  else if b < 9 then
    begin
      b:= b + 1;
      c:= 0;
    end
  else
    begin
      c:= 0;
      b:= 0;
      a:= a + 1;
    end;
  newVer:= IntToStr(a) + '.' + IntToStr(b) + '.' + IntToStr(c);
  edt1.Text:= newVer;
  curSmallVer:= '0';

  // 判断文件存在
 if not fileexists(xmlPath) then
    begin
      ShowMessage('ver_info.xml配置文件不存');
      exit;
    end;

  // 获取ver_info.xml 服务器内外网配置 值
  XMLDocument1.LoadFromFile(xmlPath);
  xmlNode:= XMLDocument1.DocumentElement;
  xmlNode:= xmlNode.ChildNodes[0];
  xmlNode:= xmlNode.ChildNodes[1];
  xmlNode:= xmlNode.ChildNodes[0];
  initReleaseStr:= xmlNode.NodeValue;

  // 获取AndroidManifest.xml 版本号
  XMLDocument2.LoadFromFile(androidXml);
  androidVer:= XMLDocument2.DocumentElement.Attributes['android:versionName'];
  Edit2.Text:= androidVer;
end;

function MyThreadFun(p: Pointer): Integer; stdcall;
begin
  RunDosInMemo(cmdParam, myMemo);
  Result:= 0;
end;

procedure TForm1.Button1Click(Sender: TObject);
var
  newVer: string;
  strList: TStringList;
  a, b, c: Integer;
  myIniFile: TInifile;
  xmlNode: IXMLNode;
  hThread:Thandle;
  ThreadID: DWord;
  
begin
    mmo1.Lines.Clear();
    // 小版本号
    myIniFile:= TInifile.Create(iniPath);
    //smallVer:= myIniFile.ReadString('UPDATE', 'version', '1.0.0');
    Edit1.Text:= smallVer;

    // 修改ver_info.xml 服务器内外网配置
    // xml的 release 位置不可改变
    XMLDocument1.LoadFromFile(xmlPath);
    xmlNode:= XMLDocument1.DocumentElement;
    xmlNode:= xmlNode.ChildNodes[0];
    xmlNode:= xmlNode.ChildNodes[1];
    xmlNode:= xmlNode.ChildNodes[0];
    xmlNode.NodeValue:= 'true';
    XMLDocument1.SaveToFile(xmlPath);
    mmo1.Lines.Add('服务器地址修改为外网：release = ' + xmlNode.NodeValue);

    // 提示小版本号加1
    strList:= Split(smallVer, '.');
    a:= StrToInt(strList[0]);
    b:= StrToInt(strList[1]);
    c:= StrToInt(strList[2]);
    if c < 9 then
      c:= c + 1
    else if b < 9 then
      begin
        b:= b + 1;
        c:= 0;
      end
    else
      begin
        c:= 0;
        b:= 0;
        a:= a + 1;
      end;

    newVer:= IntToStr(a) + '.' + IntToStr(b) + '.' + IntToStr(c);
    
    if edt1.Text <> '' then
      newVer:= edt1.Text;
    
    if MessageBox(0, pchar('即将生成更新资源,版本号为:' + newVer), '确认版本号', MB_OKCANCEL + MB_ICONQUESTION) = ID_OK then
      begin
        // 修改本地ini小版本号
        myIniFile.WriteString('UPDATE', 'version', newVer);
        myIniFile.Free;
        mmo1.Lines.Add('修改本地ini小版本号为：' + newVer);
        Button1.Enabled:= False;
        btn1.Enabled:= False;
        Button2.Enabled:= False;
        Button3.Enabled:= False;
        Button4.Enabled:= False;
        Button5.Enabled:= False;
        // 当前发布版本
        edt1.Text:= newVer;
        curSmallVer:= newVer;
        mmo1.Lines.Add('加密图片脚本, 拷贝到加密资源到更新svn目录.');
        //RunDosInMemo(batPath, mmo1);

        // 线程处理cmd命令
        cmdParam:= batPath;
        myMemo:= mmo1;
        myButton1:= Button1;
        myButton2:= btn1;
        myButton3:= Button2;
        myButton4:= Button3;
        myButton5:= Button4;
        myButton6:= Button5;
        handleType:= 'small_package';
        CreateThread(nil, 0, @MyThreadFun, nil, 0, ThreadID);
        //WaitForSingleObject(CreateThread(nil, 0, @MyThreadFun, nil, 0, ThreadID), 100000);
        //hthread:= CreateThread(nil, 0, @MyThreadFun, nil, 0, ThreadID);

        //Button1.Enabled:= true;
        //btn1.Enabled:= true;
        //mmo1.Lines.Add('更新资源包生成完成.');
      end
    else
      myIniFile.Free;

    // 复原 ver_info.xml 服务器内外网配置 值
//    XMLDocument1.LoadFromFile(xmlPath);
//    xmlNode:= XMLDocument1.DocumentElement;
//    xmlNode:= xmlNode.ChildNodes[0];
//    xmlNode:= xmlNode.ChildNodes[1];
//    xmlNode:= xmlNode.ChildNodes[0];
//    xmlNode.NodeValue:= initReleaseStr;
//    XMLDocument1.SaveToFile(xmlPath);
end;

procedure TForm1.btn1Click(Sender: TObject);
var
  path, releaseStr, releaseTips: string;
  isEncrpty: Integer;
  xmlNode: IXMLNode;
  hThread:Thandle;
  ThreadID: DWord;

begin
  mmo1.Lines.Clear();
  // 内网
  if ComboBox1.ItemIndex = 0 then
    begin
      releaseStr:= 'false' ;
      releaseTips:= '测试';
    end
  else
    begin
      releaseStr:= 'true' ;
      releaseTips:= '正式';
    end;
    
    // 修改ver_info.xml 服务器内外网配置
    // xml的 release 位置不可改变
    XMLDocument1.LoadFromFile(xmlPath);
    xmlNode:= XMLDocument1.DocumentElement;
    xmlNode := xmlNode.ChildNodes[0];
    xmlNode := xmlNode.ChildNodes[1];
    xmlNode := xmlNode.ChildNodes[0];
    xmlNode.NodeValue:= releaseStr;
    XMLDocument1.SaveToFile(xmlPath);
    mmo1.Lines.Add('服务器地址修改为内网：release = ' + releaseStr);

    // 修改AndroidManifest.xml 版本号
    XMLDocument2.LoadFromFile(androidXml);
    XMLDocument2.DocumentElement.Attributes['android:versionName']:= Edit2.Text;
    XMLDocument2.SaveToFile(androidXml);
    androidVer:= Edit2.Text;

    // 测试包 资源不加密
    if cbb1.ItemIndex = 0 then
      begin
        mmo1.Lines.Add('资源不加密.');
        Button1.Enabled:= False;
        btn1.Enabled:= False;
        Button2.Enabled:= False;
        Button3.Enabled:= False;
        Button4.Enabled:= False;
        Button5.Enabled:= False;        
        //RunDosInMemo(batDebugApk, mmo1);

        // 线程处理cmd命令
        cmdParam:= batDebugApk;
        myMemo:= mmo1;
        myButton1:= Button1;
        myButton2:= btn1;
        myButton3:= Button2;
        myButton4:= Button3;
        myButton5:= Button4;
        myButton6:= Button5;
        handleType:= 'apk_debug_package';
        myReleaseTip:= releaseTips;
        CreateThread(nil, 0, @MyThreadFun, nil, 0, ThreadID);
      end
    else
      begin
        if curSmallVer = '0' then
          begin
            ShowMessage('打正式包，请确保已经生成好了小版本更新资源(encrypt_res)!');
            
            Button1.Enabled:= False;
            btn1.Enabled:= False;
            mmo1.Lines.Add('正在生成apk.');
            //RunDosInMemo(batReleaseApk, mmo1);

            // 线程处理cmd命令
            cmdParam:= batReleaseApk;
            myMemo:= mmo1;
            myButton1:= Button1;
            myButton2:= btn1;
            handleType:= 'apk_release_package';
            myReleaseTip:= releaseTips;
            CreateThread(nil, 0, @MyThreadFun, nil, 0, ThreadID);
          end
        else
          begin
            if MessageBox(0, pchar('即将打包apk,小版本号为:' + curSmallVer), '确认版本号', MB_OKCANCEL + MB_ICONQUESTION) = ID_OK then
              begin
                Button1.Enabled:= False;
                btn1.Enabled:= False;
                mmo1.Lines.Add('正在生成apk.');
                //RunDosInMemo(batReleaseApk, mmo1);

                // 线程处理cmd命令
                cmdParam:= batReleaseApk;
                myMemo:= mmo1;
                myButton1:= Button1;
                myButton2:= btn1;
                handleType:= 'apk_release_package';
                myReleaseTip:= releaseTips;
                CreateThread(nil, 0, @MyThreadFun, nil, 0, ThreadID);

//                mmo1.Lines.Add('apk生成完成.');
//                // 移动apk
//                RunDosInMemo(batMoveApk, mmo1);
//                mmo1.Lines.Add('移动apk到目录apks下.');
//                mmo1.Lines.Add('apk为' + releaseTips);
//                mmo1.Lines.Add('若安装不成功,请自行到项目/public/android下获取apk安装.');
//
//                Button1.Enabled:= true;
//                btn1.Enabled:= true;
              end
          end;
      end;

//     复原 ver_info.xml 服务器内外网配置 值
//    XMLDocument1.LoadFromFile(xmlPath);
//    xmlNode:= XMLDocument1.DocumentElement;
//    xmlNode:= xmlNode.ChildNodes[0];
//    xmlNode:= xmlNode.ChildNodes[1];
//    xmlNode:= xmlNode.ChildNodes[0];
//    xmlNode.NodeValue:= initReleaseStr;
//    XMLDocument1.SaveToFile(xmlPath);

    // 复原AndroidManifest.xml 版本号
//    XMLDocument2.LoadFromFile(androidXml);
//    XMLDocument2.DocumentElement.Attributes['android:versionName']:= androidVer;
//    XMLDocument2.SaveToFile(androidXml);
end;

procedure TForm1.tmr1Timer(Sender: TObject);
  var
    str: string;
begin
    //str:= mmo1.Lines.Strings[mmo1.Lines.Count - 1];
    //if str = 'finish' then
      //showmessage(str);

end;

procedure TForm1.FormDestroy(Sender: TObject);
var
  xmlNode: IXMLNode;
begin
    // 判断文件存在
 if fileexists(xmlPath) then
    begin
      // 复原 ver_info.xml 服务器内外网配置 值
      XMLDocument1.LoadFromFile(xmlPath);
      xmlNode:= XMLDocument1.DocumentElement;
      xmlNode:= xmlNode.ChildNodes[0];
      xmlNode:= xmlNode.ChildNodes[1];
      xmlNode:= xmlNode.ChildNodes[0];
      xmlNode.NodeValue:= initReleaseStr;
      XMLDocument1.SaveToFile(xmlPath);
    end;

    // 复原AndroidManifest.xml 版本号
//    XMLDocument2.LoadFromFile(androidXml);
//    XMLDocument2.DocumentElement.Attributes['android:versionName']:= androidVer;
//    XMLDocument2.SaveToFile(androidXml);
end;

procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
var
  xmlNode: IXMLNode;
begin
  // 结束cmd进程
  if KillTask('cmd.exe') = 0 then
    begin
      //showmessage('请打开任务管理器,结束cmd进程!');
    end;

    // 结束加密进程
    KillTask('php.exe');
    KillTask('python.exe');
    //KillTask('adb.exe');
    //KillTask('conhost.exe');
    KillTask('Robocopy.exe');

  // 判断文件存在
 if fileexists(xmlPath) then
    begin
      // 复原 ver_info.xml 服务器内外网配置 值
      XMLDocument1.LoadFromFile(xmlPath);
      xmlNode:= XMLDocument1.DocumentElement;
      xmlNode:= xmlNode.ChildNodes[0];
      xmlNode:= xmlNode.ChildNodes[1];
      xmlNode:= xmlNode.ChildNodes[0];
      xmlNode.NodeValue:= initReleaseStr;
      XMLDocument1.SaveToFile(xmlPath);
    end;
end;

procedure TForm1.Button2Click(Sender: TObject);
var
  xmlNode: IXMLNode;
  ThreadID: DWord;
  webData: string;
  jo: JSONObject;
  innSmallVer: string;
  
begin
    mmo1.Lines.Clear();
    mmo1.Lines.Add('打包内网debug apk, 并上传更新资源到内网更新服.');
    // 修改ver_info.xml 服务器内外网配置
    // xml的 release 位置不可改变
    XMLDocument1.LoadFromFile(xmlPath);
    xmlNode:= XMLDocument1.DocumentElement;
    xmlNode:= xmlNode.ChildNodes[0];
    xmlNode:= xmlNode.ChildNodes[1];
    xmlNode:= xmlNode.ChildNodes[0];
    xmlNode.NodeValue:= 'false';
    XMLDocument1.SaveToFile(xmlPath);
    mmo1.Lines.Add('服务器地址修改为外网：release = ' + xmlNode.NodeValue);

    // 获取内网大版本
    webData:=idHTTP1.Get(innerGameConfigUrl);
    if webData = '' then
      begin
        ShowMessage('获取内网大版本失败, ' + innerGameConfigUrl);
      end;

    jo:= JSONObject.Create(webData);
    innSmallVer:= jo['version_android'];
    jo:= JSONObject.Create(innSmallVer);
    innSmallVer:= jo['big'];
    ShowMessage('打包debug内网大版本:' + innSmallVer);

    // 修改AndroidManifest.xml 版本号
    XMLDocument2.LoadFromFile(androidXml);
    XMLDocument2.DocumentElement.Attributes['android:versionName']:= innSmallVer;
    XMLDocument2.SaveToFile(androidXml);

    Button1.Enabled:= False;
    btn1.Enabled:= False;
    Button2.Enabled:= False;
    Button3.Enabled:= False;
    Button4.Enabled:= False;
    Button5.Enabled:= False;
    // 线程处理cmd命令
    cmdParam:= batDebugAndUploadResToSvnApk;
    myMemo:= mmo1;
    myButton1:= Button1;
    myButton2:= btn1;
    myButton3:= Button2;
    myButton4:= Button3;
    myButton5:= Button4;
    myButton6:= Button5;
    handleType:= 'apk_debug_package_and_update_res_to_svn';
    CreateThread(nil, 0, @MyThreadFun, nil, 0, ThreadID);
end;

procedure TForm1.Button3Click(Sender: TObject);
var
  xmlNode: IXMLNode;
  ThreadID: DWord;

begin
    mmo1.Lines.Clear();
    mmo1.Lines.Add('生成内网更新资源, 并上传更新资源到内网更新服.');
    // 修改ver_info.xml 服务器内外网配置
    // xml的 release 位置不可改变
    XMLDocument1.LoadFromFile(xmlPath);
    xmlNode:= XMLDocument1.DocumentElement;
    xmlNode:= xmlNode.ChildNodes[0];
    xmlNode:= xmlNode.ChildNodes[1];
    xmlNode:= xmlNode.ChildNodes[0];
    xmlNode.NodeValue:= 'false';
    XMLDocument1.SaveToFile(xmlPath);
    mmo1.Lines.Add('服务器地址修改为外网：release = ' + xmlNode.NodeValue);

    // 修改AndroidManifest.xml 版本号
//    XMLDocument2.LoadFromFile(androidXml);
//    XMLDocument2.DocumentElement.Attributes['android:versionName']:= Edit2.Text;
//    XMLDocument2.SaveToFile(androidXml);

    Button1.Enabled:= False;
    btn1.Enabled:= False;
    Button2.Enabled:= False;
    Button3.Enabled:= False;
    Button4.Enabled:= False;
    Button5.Enabled:= False;

    // 线程处理cmd命令
    cmdParam:= batDebugAndUploadResToSvn;
    myMemo:= mmo1;
    myButton1:= Button1;
    myButton2:= btn1;
    myButton3:= Button2;
    myButton4:= Button3;
    myButton5:= Button4;
    myButton6:= Button5;
    handleType:= 'debug_package_and_update_res_to_svn';
    CreateThread(nil, 0, @MyThreadFun, nil, 0, ThreadID);
end;

procedure TForm1.Button5Click(Sender: TObject);
var
  ThreadID: DWord;

begin
    mmo1.Lines.Clear();
    mmo1.Lines.Add('上传debug apk到手机存储卡.');

    Button1.Enabled:= False;
    btn1.Enabled:= False;
    Button2.Enabled:= False;
    Button3.Enabled:= False;
    Button4.Enabled:= False;
    Button5.Enabled:= False;

    // 线程处理cmd命令
    cmdParam:= batPushDebugApkToSdcard;
    myMemo:= mmo1;
    myButton1:= Button1;
    myButton2:= btn1;
    myButton3:= Button2;
    myButton4:= Button3;
    myButton5:= Button4;
    myButton6:= Button5;
    handleType:= 'batPushDebugApkToSdcard';
    CreateThread(nil, 0, @MyThreadFun, nil, 0, ThreadID);
end;

procedure TForm1.Button4Click(Sender: TObject);
var
  ThreadID: DWord;

begin
    mmo1.Lines.Clear();
    mmo1.Lines.Add('上传release apk到手机存储卡.');

    Button1.Enabled:= False;
    btn1.Enabled:= False;
    Button2.Enabled:= False;
    Button3.Enabled:= False;
    Button4.Enabled:= False;
    Button5.Enabled:= False;

    // 线程处理cmd命令
    cmdParam:= batPushReleaseApkToSdcard;
    myMemo:= mmo1;
    myButton1:= Button1;
    myButton2:= btn1;
    myButton3:= Button2;
    myButton4:= Button3;
    myButton5:= Button4;
    myButton6:= Button5;
    handleType:= 'batPushReleaseApkToSdcard';
    CreateThread(nil, 0, @MyThreadFun, nil, 0, ThreadID);
end;
procedure TForm1.BitBtn1Click(Sender: TObject);
begin
  ShellExecute(Handle, 'open', 'E:\update_resource', 'C:\Windows', nil, 1);
end;

end.
