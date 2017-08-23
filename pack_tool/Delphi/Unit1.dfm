object Form1: TForm1
  Left = 2368
  Top = 124
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = #25171#21253
  ClientHeight = 491
  ClientWidth = 754
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  DesignSize = (
    754
    491)
  PixelsPerInch = 96
  TextHeight = 13
  object lbl1: TLabel
    Left = 8
    Top = 16
    Width = 145
    Height = 13
    AutoSize = False
    Caption = #32447#19978#21457#24067#30340#23567#29256#26412#21495':'
  end
  object Label1: TLabel
    Left = 8
    Top = 40
    Width = 129
    Height = 13
    AutoSize = False
    Caption = #24403#21069#21457#24067#30340#23567#29256#26412#21495':'
  end
  object lbl2: TLabel
    Left = 445
    Top = 75
    Width = 65
    Height = 13
    AutoSize = False
    Caption = #22823#29256#26412#21495':'
  end
  object lbl3: TLabel
    Left = 457
    Top = 43
    Width = 57
    Height = 13
    AutoSize = False
    Caption = #20869#22806#32593':'
  end
  object lbl4: TLabel
    Left = 415
    Top = 10
    Width = 88
    Height = 13
    AutoSize = False
    Caption = #36164#28304#26159#21542#21152#23494':'
  end
  object ComboBox1: TComboBox
    Left = 518
    Top = 39
    Width = 145
    Height = 21
    ItemHeight = 13
    ItemIndex = 0
    TabOrder = 0
    Text = #20869#32593
    Items.Strings = (
      #20869#32593
      #22806#32593)
  end
  object Button1: TButton
    Left = 153
    Top = 70
    Width = 135
    Height = 30
    Cursor = crHandPoint
    Hint = #29983#25104#30340#23567#29256#26412#26356#26032#36164#28304#22312#30446#24405' E:/update_resource/'
    Anchors = [akTop]
    Caption = #29983#25104#26356#26032#36164#28304
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clFuchsia
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
    OnClick = Button1Click
  end
  object Edit1: TEdit
    Left = 152
    Top = 11
    Width = 121
    Height = 21
    Hint = #32447#19978#26368#26032#30340#23567#29256#26412#21495
    Enabled = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 2
  end
  object mmo1: TMemo
    Left = 8
    Top = 152
    Width = 737
    Height = 329
    ScrollBars = ssBoth
    TabOrder = 3
  end
  object btn1: TButton
    Left = 670
    Top = 37
    Width = 75
    Height = 25
    Hint = #25171#21253#22806#32593#21152#23494#21253#65292#39318#20808#24471#22312#24038#36793'<'#29983#25104#23567#29256#26412#26356#26032#36164#28304'>'
    Caption = #25171#21253'apk'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 4
    OnClick = btn1Click
  end
  object cbb1: TComboBox
    Left = 518
    Top = 7
    Width = 145
    Height = 21
    ItemHeight = 13
    ItemIndex = 0
    TabOrder = 5
    Text = #19981#21152#23494
    Items.Strings = (
      #19981#21152#23494
      #21152#23494)
  end
  object edt1: TEdit
    Left = 152
    Top = 37
    Width = 121
    Height = 21
    Hint = #29256#26412#21495'3'#20301' '#28857#21495#38548#24320#65292#27599#27425#36882#22686#65292#27880#24847#29256#26412#21495#25353#29031#23383#31526'ASCII'#22823#23567#27604#36739#65292#35268#23450#27599#19968#20301'0-9 '#27604#22914#65306'1.0.9 > 1.0.10'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 6
  end
  object Edit2: TEdit
    Left = 518
    Top = 72
    Width = 145
    Height = 21
    Hint = 'Android AndroidManifest.xml'#19979'versionName'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 7
  end
  object Button2: TButton
    Left = 152
    Top = 109
    Width = 278
    Height = 33
    Hint = 
      #19978#20256#27809#21152#23494#30340#26356#26032#36164#28304#21040#20869#32593#26356#26032#30446#24405'\\DESKTOP-36PF8JG\update_resource\  '#27492#26102#22823#23567#29256#26412#21495#21069#21518#31471#19968 +
      #33268
    Caption = #25171#21253#20869#32593'debug apk'#24182#19978#20256#26356#26032#36164#28304#21040'svn'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 8
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 440
    Top = 109
    Width = 297
    Height = 33
    Hint = 
      #19978#20256#27809#21152#23494#30340#26356#26032#36164#28304#21040#20869#32593#26356#26032#30446#24405'\\DESKTOP-36PF8JG\update_resource\  '#27492#26102#21518#31471#26356#26032#26381#30340#23567#29256#26412 +
      #21495#21152'1'
    Caption = #29983#25104#20869#32593#26356#26032#36164#28304#24182#19978#20256#26356#26032#36164#28304#21040'svn'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 9
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 8
    Top = 69
    Width = 135
    Height = 30
    Hint = #19978#20256#39033#30446#30446#24405#19979'publish/android/BaLe-release-signed.apk'#21040#25163#26426#23384#20648#21345#26681#30446#24405
    Anchors = [akTop]
    Caption = #21457#36865'release apk'#21040#25163#26426
    ParentShowHint = False
    ShowHint = True
    TabOrder = 10
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 9
    Top = 112
    Width = 135
    Height = 30
    Hint = #19978#20256#39033#30446#30446#24405#19979'simulator/android/BaLe-debug.apk'#21040#25163#26426#23384#20648#21345#26681#30446#24405
    Caption = #21457#36865'debug apk'#21040#25163#26426
    ParentShowHint = False
    ShowHint = True
    TabOrder = 11
    OnClick = Button5Click
  end
  object BitBtn1: TBitBtn
    Left = 295
    Top = 70
    Width = 135
    Height = 30
    Hint = #25171#24320#26356#26032#30446#24405
    Caption = #25171#24320#26356#26032#30446#24405
    ParentShowHint = False
    ShowHint = True
    TabOrder = 12
    OnClick = BitBtn1Click
  end
  object tmr1: TTimer
    OnTimer = tmr1Timer
    Left = 280
    Top = 65528
  end
  object XMLDocument1: TXMLDocument
    Active = True
    Left = 312
    Top = 32
    DOMVendorDesc = 'MSXML'
  end
  object IdHTTP1: TIdHTTP
    MaxLineAction = maException
    ReadTimeout = 0
    AllowCookies = True
    ProxyParams.BasicAuthentication = False
    ProxyParams.ProxyPort = 0
    Request.ContentLength = -1
    Request.ContentRangeEnd = 0
    Request.ContentRangeStart = 0
    Request.ContentType = 'text/html'
    Request.Accept = 'text/html, */*'
    Request.BasicAuthentication = False
    Request.UserAgent = 'Mozilla/3.0 (compatible; Indy Library)'
    HTTPOptions = [hoForceEncodeParams]
    Left = 280
    Top = 32
  end
  object XMLDocument2: TXMLDocument
    Left = 312
    DOMVendorDesc = 'MSXML'
  end
  object OpenDialog1: TOpenDialog
    Left = 328
    Top = 8
  end
end
