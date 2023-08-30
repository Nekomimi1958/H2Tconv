object H2TconvForm: TH2TconvForm
  Left = 283
  Top = 150
  HelpContext = 1
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'H2Tconv'
  ClientHeight = 550
  ClientWidth = 590
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  KeyPreview = True
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  DesignSize = (
    590
    550)
  TextHeight = 15
  object StatusBar1: TStatusBar
    Left = 0
    Top = 531
    Width = 590
    Height = 19
    DoubleBuffered = True
    Panels = <
      item
        Width = 100
      end
      item
        Width = 50
      end>
    ParentDoubleBuffered = False
    SimplePanel = True
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 590
    Height = 531
    ActivePage = TabSheet1
    Align = alClient
    TabHeight = 24
    TabOrder = 0
    TabWidth = 100
    OnChange = PageControl1Change
    object TabSheet1: TTabSheet
      HelpContext = 4
      Caption = #20837#20986#21147
      object Panel1_2: TPanel
        Left = 0
        Top = 292
        Width = 582
        Height = 205
        Align = alBottom
        BevelOuter = bvNone
        BorderWidth = 4
        TabOrder = 1
        DesignSize = (
          582
          205)
        object ArrowImg: TVirtualImage
          Left = 273
          Top = 0
          Width = 37
          Height = 16
          ImageCollection = ImgArwCollection
          ImageWidth = 0
          ImageHeight = 0
          ImageIndex = 0
          ImageName = 'arraw'
        end
        object GrpBox1_2: TGroupBox
          Left = 4
          Top = 18
          Width = 574
          Height = 82
          Anchors = [akLeft, akRight, akBottom]
          Caption = #12486#12461#12473#12488#20986#21147#20808'(&O)'
          TabOrder = 0
          DesignSize = (
            574
            82)
          object SamDirRadioBtn: TRadioButton
            Tag = 1
            Left = 128
            Top = 22
            Width = 81
            Height = 17
            Caption = #20803#12392#21516#12376
            TabOrder = 1
            OnClick = DestRadioBtnClick
          end
          object SetDirRadioBtn: TRadioButton
            Tag = 2
            Left = 216
            Top = 22
            Width = 98
            Height = 17
            Caption = #22580#25152#25351#23450'(&L)'
            TabOrder = 2
            OnClick = DestRadioBtnClick
          end
          object DstDirEdit: TEdit
            Left = 320
            Top = 20
            Width = 210
            Height = 23
            Anchors = [akLeft, akTop, akRight]
            TabOrder = 3
            OnChange = DstDirEditChange
          end
          object RefDstBtn: TButton
            Left = 532
            Top = 19
            Width = 32
            Height = 23
            Anchors = [akTop, akRight]
            Caption = '...'
            TabOrder = 4
            OnClick = RefDstBtnClick
          end
          object ClipRadioBtn: TRadioButton
            Left = 12
            Top = 22
            Width = 113
            Height = 17
            Caption = #12463#12522#12483#12503#12508#12540#12489
            TabOrder = 0
            OnClick = DestRadioBtnClick
          end
          object JoinCheck: TCheckBox
            Left = 16
            Top = 55
            Width = 121
            Height = 17
            Caption = #19968#12388#12395#36899#32080'(&C)'
            TabOrder = 5
            OnClick = JoinCheckClick
          end
          object FilNamEdit: TLabeledEdit
            Left = 216
            Top = 53
            Width = 289
            Height = 23
            Anchors = [akLeft, akTop, akRight]
            EditLabel.Width = 64
            EditLabel.Height = 23
            EditLabel.Caption = #12501#12449#12452#12523#21517'(&F)'
            LabelPosition = lpLeft
            TabOrder = 6
            Text = ''
            OnChange = FilNamEditChange
          end
          object FilExtEdit: TLabeledEdit
            Left = 516
            Top = 53
            Width = 48
            Height = 23
            Hint = #25313#24373#23376
            Anchors = [akTop, akRight]
            EditLabel.Width = 3
            EditLabel.Height = 23
            EditLabel.Caption = '.'
            LabelPosition = lpLeft
            TabOrder = 7
            Text = ''
            OnChange = FilExtEditChange
          end
        end
        object GrpBox1_3: TGroupBox
          Left = 4
          Top = 115
          Width = 478
          Height = 82
          Anchors = [akLeft, akTop, akRight]
          Caption = #22793#25563#24460'(&E)'
          TabOrder = 1
          DesignSize = (
            478
            82)
          object OpenAppCheck: TCheckBox
            Left = 16
            Top = 52
            Width = 105
            Height = 17
            Anchors = [akLeft, akBottom]
            Caption = #12450#12503#12522#12391#38283#12367
            TabOrder = 1
          end
          object RefAppBtn: TButton
            Left = 438
            Top = 49
            Width = 32
            Height = 23
            Anchors = [akRight, akBottom]
            Caption = '...'
            TabOrder = 3
            OnClick = RefAppBtnClick
          end
          object AppNameEdit: TEdit
            Left = 128
            Top = 50
            Width = 308
            Height = 23
            Anchors = [akLeft, akRight, akBottom]
            TabOrder = 2
          end
          object KillCheck: TCheckBox
            Left = 16
            Top = 22
            Width = 161
            Height = 17
            Caption = #20803#12501#12449#12452#12523#12434#21066#38500'(&K)'
            TabOrder = 0
          end
        end
        object ConvBtn: TBitBtn
          Left = 502
          Top = 125
          Width = 72
          Height = 72
          Action = ConvertAction
          Anchors = [akRight, akBottom]
          Default = True
          Images = VirtualImgBtnList
          Layout = blGlyphTop
          Margin = 4
          TabOrder = 3
        end
        object ConvBtnC: TBitBtn
          Left = 488
          Top = 112
          Width = 48
          Height = 42
          Action = ConvertAction2
          Caption = #38283#22987
          Default = True
          TabOrder = 2
        end
      end
      object Panel1_1: TPanel
        Left = 0
        Top = 0
        Width = 582
        Height = 292
        Align = alClient
        BevelOuter = bvNone
        BorderWidth = 4
        TabOrder = 0
        object GrpBox1_1: TGroupBox
          Left = 4
          Top = 4
          Width = 570
          Height = 284
          Align = alLeft
          Anchors = [akLeft, akTop, akRight, akBottom]
          Caption = #20837#21147'HTML'#25991#26360'(&I)'
          TabOrder = 0
          object Panel1_1a: TPanel
            Left = 2
            Top = 17
            Width = 566
            Height = 235
            Align = alClient
            BevelOuter = bvNone
            BorderWidth = 4
            TabOrder = 0
            object HtmFileList: TListBox
              Left = 4
              Top = 4
              Width = 558
              Height = 227
              Align = alClient
              DragMode = dmAutomatic
              ItemHeight = 15
              MultiSelect = True
              PopupMenu = PopupMenu2
              TabOrder = 0
              OnClick = HtmFileListClick
              OnDragDrop = HtmFileListDragDrop
              OnDragOver = HtmFileListDragOver
              OnEndDrag = HtmFileListEndDrag
              OnKeyDown = HtmFileListKeyDown
              OnStartDrag = HtmFileListStartDrag
            end
          end
          object Panel1_1b: TPanel
            Left = 2
            Top = 252
            Width = 566
            Height = 30
            Align = alBottom
            BevelOuter = bvNone
            TabOrder = 1
            DesignSize = (
              566
              30)
            object DowItemBtn: TButton
              Left = 487
              Top = 0
              Width = 55
              Height = 25
              Anchors = []
              TabOrder = 7
              OnClick = DowItemBtnClick
            end
            object UpItemBtn: TButton
              Left = 426
              Top = 0
              Width = 55
              Height = 25
              Anchors = []
              TabOrder = 6
              OnClick = UpItemBtnClick
            end
            object SortBtn: TButton
              Left = 367
              Top = 0
              Width = 55
              Height = 25
              Anchors = []
              TabOrder = 5
              OnClick = SortBtnClick
            end
            object ClrBtn: TButton
              Left = 295
              Top = 0
              Width = 55
              Height = 25
              Anchors = []
              TabOrder = 4
              OnClick = ClrBtnClick
            end
            object DelBtn: TButton
              Left = 238
              Top = 0
              Width = 55
              Height = 25
              Anchors = []
              TabOrder = 3
              OnClick = DelBtnClick
            end
            object PasteBtn: TButton
              Left = 154
              Top = 0
              Width = 70
              Height = 25
              Anchors = []
              TabOrder = 2
              OnClick = PasteBtnClick
            end
            object InputBtn: TButton
              Left = 82
              Top = 0
              Width = 70
              Height = 25
              Anchors = []
              TabOrder = 1
              OnClick = InputBtnClick
            end
            object AddBtn: TButton
              Left = 8
              Top = 0
              Width = 70
              Height = 25
              Anchors = [akLeft, akBottom]
              TabOrder = 0
              OnClick = AddBtnClick
            end
          end
        end
      end
    end
    object TabSheet2: TTabSheet
      HelpContext = 5
      Caption = #22793#25563#12398#35373#23450
      ImageIndex = 1
      object GrpBox2_3: TGroupBox
        Left = 8
        Top = 60
        Width = 564
        Height = 50
        Caption = #35211#20986#12375'(&H)'
        TabOrder = 1
        object H1Edit: TLabeledEdit
          Left = 40
          Top = 20
          Width = 60
          Height = 23
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = 'h1'
          LabelPosition = lpLeft
          TabOrder = 0
          Text = ''
        end
        object H2Edit: TLabeledEdit
          Left = 131
          Top = 20
          Width = 60
          Height = 23
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = 'h2'
          LabelPosition = lpLeft
          TabOrder = 1
          Text = ''
        end
        object H3Edit: TLabeledEdit
          Left = 221
          Top = 20
          Width = 60
          Height = 23
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = 'h3'
          LabelPosition = lpLeft
          TabOrder = 2
          Text = ''
        end
        object H4Edit: TLabeledEdit
          Left = 312
          Top = 20
          Width = 60
          Height = 23
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = 'h4'
          LabelPosition = lpLeft
          TabOrder = 3
          Text = ''
        end
        object H5Edit: TLabeledEdit
          Left = 402
          Top = 20
          Width = 60
          Height = 23
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = 'h5'
          LabelPosition = lpLeft
          TabOrder = 4
          Text = ''
        end
        object H6Edit: TLabeledEdit
          Left = 493
          Top = 20
          Width = 60
          Height = 23
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = 'h6'
          LabelPosition = lpLeft
          TabOrder = 5
          Text = ''
        end
      end
      object GrpBox2_1: TGroupBox
        Left = 8
        Top = 4
        Width = 564
        Height = 50
        Caption = #34892
        TabOrder = 0
        object PstHdrLabel: TLabel
          Left = 412
          Top = 23
          Width = 104
          Height = 15
          Caption = #35211#20986#12375#24460#12434#31354#12369#12394#12356
        end
        object LnWidUpDown: TUpDown
          Left = 80
          Top = 20
          Width = 16
          Height = 23
          Associate = LnWidEdit
          Min = 1
          Max = 250
          Position = 1
          TabOrder = 1
        end
        object BlkLmtUpDown: TUpDown
          Left = 348
          Top = 20
          Width = 16
          Height = 23
          Associate = BlkLmtEdit
          Min = 1
          Position = 1
          TabOrder = 4
        end
        object FrcCrCheck: TCheckBox
          Left = 112
          Top = 22
          Width = 97
          Height = 17
          Caption = #26689#25968#12391#25913#34892
          TabOrder = 2
        end
        object PstHdrCheck: TCheckBox
          Left = 396
          Top = 22
          Width = 152
          Height = 17
          TabOrder = 5
          StyleElements = [seClient, seBorder]
        end
        object BlkLmtEdit: TLabeledEdit
          Left = 308
          Top = 20
          Width = 40
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 78
          EditLabel.Height = 23
          EditLabel.Caption = #36899#32154#31354#34892#21046#38480
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 3
          Text = '1'
          OnChange = BlkLmtEditaChange
          OnExit = NumberEditExit
        end
        object LnWidEdit: TLabeledEdit
          Left = 40
          Top = 20
          Width = 40
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 26
          EditLabel.Height = 23
          EditLabel.Caption = #26689#25968
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 0
          Text = '1'
          OnChange = LnWidEditChange
          OnExit = NumberEditExit
        end
      end
      object GrpBox2_2: TGroupBox
        Left = 424
        Top = 116
        Width = 148
        Height = 50
        Caption = 'HR'#32619#32218
        TabOrder = 5
        object HrStrEdit: TLabeledEdit
          Left = 77
          Top = 20
          Width = 60
          Height = 23
          EditLabel.Width = 52
          EditLabel.Height = 23
          EditLabel.Caption = #32619#32218#25991#23383
          LabelPosition = lpLeft
          TabOrder = 0
          Text = ''
        end
      end
      object GrpBox2_7: TGroupBox
        Left = 8
        Top = 172
        Width = 400
        Height = 50
        Caption = #31623#26465#26360#12365'(&K)'
        TabOrder = 6
        object ZenNoLabel: TLabel
          Left = 296
          Top = 23
          Width = 63
          Height = 15
          Caption = #25968#23383#12399#20840#35282
        end
        object ZenNoCheck: TCheckBox
          Left = 280
          Top = 22
          Width = 105
          Height = 17
          TabOrder = 2
        end
        object IndentEdit: TLabeledEdit
          Left = 64
          Top = 20
          Width = 60
          Height = 23
          EditLabel.Width = 49
          EditLabel.Height = 23
          EditLabel.Caption = #12452#12531#12487#12531#12488
          LabelPosition = lpLeft
          TabOrder = 0
          Text = ''
        end
        object MarkEdit: TLabeledEdit
          Left = 189
          Top = 20
          Width = 60
          Height = 23
          EditLabel.Width = 30
          EditLabel.Height = 23
          EditLabel.Caption = #12510#12540#12463
          LabelPosition = lpLeft
          TabOrder = 1
          Text = ''
        end
      end
      object GrpBox2_9: TGroupBox
        Left = 8
        Top = 228
        Width = 564
        Height = 50
        Caption = #30011#20687'(&I)'
        TabOrder = 8
        object AltCheck: TCheckBox
          Left = 16
          Top = 22
          Width = 93
          Height = 17
          Caption = #20195#26367#25991#23383#21015
          TabOrder = 0
        end
        object AltKetEdit: TEdit
          Left = 517
          Top = 20
          Width = 36
          Height = 23
          TabOrder = 4
        end
        object ImgSrcCheck: TCheckBox
          Left = 280
          Top = 22
          Width = 84
          Height = 17
          Caption = #12501#12449#12452#12523#21517
          TabOrder = 2
        end
        object DefAltEdit: TLabeledEdit
          Left = 172
          Top = 20
          Width = 77
          Height = 23
          EditLabel.Width = 49
          EditLabel.Height = 23
          EditLabel.Caption = #12487#12501#12457#12523#12488
          LabelPosition = lpLeft
          TabOrder = 1
          Text = ''
        end
        object AltBraEdit: TLabeledEdit
          Left = 476
          Top = 20
          Width = 36
          Height = 23
          EditLabel.Width = 25
          EditLabel.Height = 23
          EditLabel.Caption = #22258#12415
          LabelPosition = lpLeft
          TabOrder = 3
          Text = ''
        end
      end
      object GrpBox2_10: TGroupBox
        Left = 8
        Top = 284
        Width = 564
        Height = 50
        Caption = #12522#12531#12463'(&L)'
        TabOrder = 9
        object LinkCrLabel: TLabel
          Left = 296
          Top = 24
          Width = 72
          Height = 15
          Caption = #21069#24460#12395#25913#34892#12434
        end
        object LinkCheck: TCheckBox
          Left = 16
          Top = 23
          Width = 89
          Height = 17
          Caption = #25407#20837#12377#12427
          TabOrder = 0
        end
        object LnkKetEdit: TEdit
          Left = 517
          Top = 21
          Width = 36
          Height = 23
          TabOrder = 4
        end
        object ExtLnkCheck: TCheckBox
          Left = 132
          Top = 23
          Width = 121
          Height = 17
          Caption = #22806#37096#12522#12531#12463#12398#12415
          TabOrder = 1
        end
        object LinkCrCheck: TCheckBox
          Left = 280
          Top = 23
          Width = 111
          Height = 17
          TabOrder = 2
        end
        object LnkBraEdit: TLabeledEdit
          Left = 476
          Top = 21
          Width = 36
          Height = 23
          EditLabel.Width = 25
          EditLabel.Height = 23
          EditLabel.Caption = #22258#12415
          LabelPosition = lpLeft
          TabOrder = 3
          Text = ''
        end
      end
      object GrpBox2_4: TGroupBox
        Left = 8
        Top = 116
        Width = 128
        Height = 50
        Caption = #22826#23383'(&B)'
        TabOrder = 2
        object B_KetEdit: TEdit
          Left = 82
          Top = 20
          Width = 38
          Height = 23
          TabOrder = 1
        end
        object B_BraEdit: TLabeledEdit
          Left = 40
          Top = 20
          Width = 38
          Height = 23
          EditLabel.Width = 25
          EditLabel.Height = 23
          EditLabel.Caption = #22258#12415
          LabelPosition = lpLeft
          TabOrder = 0
          Text = ''
        end
      end
      object GrpBox2_5: TGroupBox
        Left = 144
        Top = 116
        Width = 128
        Height = 50
        Caption = #26012#20307'(&I)'
        TabOrder = 3
        object I_KetEdit: TEdit
          Left = 82
          Top = 20
          Width = 38
          Height = 23
          TabOrder = 1
        end
        object I_BraEdit: TLabeledEdit
          Left = 40
          Top = 20
          Width = 38
          Height = 23
          EditLabel.Width = 25
          EditLabel.Height = 23
          EditLabel.Caption = #22258#12415
          LabelPosition = lpLeft
          TabOrder = 0
          Text = ''
        end
      end
      object GrpBox2_6: TGroupBox
        Left = 280
        Top = 116
        Width = 128
        Height = 50
        Caption = #19979#32218'(&U)'
        TabOrder = 4
        object U_KetEdit: TEdit
          Left = 82
          Top = 20
          Width = 38
          Height = 23
          TabOrder = 1
        end
        object U_BraEdit: TLabeledEdit
          Left = 40
          Top = 20
          Width = 38
          Height = 23
          EditLabel.Width = 25
          EditLabel.Height = 23
          EditLabel.Caption = #22258#12415
          LabelPosition = lpLeft
          TabOrder = 0
          Text = ''
        end
      end
      object GrpBox2_8: TGroupBox
        Left = 424
        Top = 172
        Width = 148
        Height = 50
        Caption = #34920'(&T)'
        TabOrder = 7
        object CelSepEdit: TLabeledEdit
          Left = 77
          Top = 20
          Width = 60
          Height = 23
          EditLabel.Width = 56
          EditLabel.Height = 23
          EditLabel.Caption = #12475#12523#21306#20999#12426
          LabelPosition = lpLeft
          TabOrder = 0
          Text = ''
        end
      end
      object GrpBox2_11: TGroupBox
        Left = 8
        Top = 340
        Width = 564
        Height = 50
        Caption = #12467#12531#12486#12531#12484#21306#20999#12426#32619#32218
        TabOrder = 10
        object InsHrClsEdit: TLabeledEdit
          Left = 172
          Top = 20
          Width = 121
          Height = 23
          Hint = '; '#12391#21306#20999#12387#12390#35079#25968#25351#23450#21487#33021
          EditLabel.Width = 60
          EditLabel.Height = 23
          EditLabel.Caption = '<div> class'
          LabelPosition = lpLeft
          TabOrder = 1
          Text = ''
        end
        object InsHrNavCheckBox: TCheckBox
          Left = 471
          Top = 22
          Width = 72
          Height = 17
          Caption = '<nav>'
          TabOrder = 4
        end
        object InsHrArtCheckBox: TCheckBox
          Left = 393
          Top = 22
          Width = 76
          Height = 17
          Caption = '<article>'
          TabOrder = 3
        end
        object InsHrSctCheckBox: TCheckBox
          Left = 311
          Top = 22
          Width = 80
          Height = 17
          Caption = '<section>'
          TabOrder = 2
        end
        object InsLineStrEdit: TLabeledEdit
          Left = 64
          Top = 20
          Width = 36
          Height = 23
          EditLabel.Width = 52
          EditLabel.Height = 23
          EditLabel.Caption = #32619#32218#25991#23383
          LabelPosition = lpLeft
          TabOrder = 0
          Text = ''
        end
      end
      object GrpBox2_12: TGroupBox
        Left = 8
        Top = 396
        Width = 564
        Height = 50
        Caption = #25351#23450#37096#20998#12398#21066#38500
        TabOrder = 11
        object DelBlkClsEdit: TLabeledEdit
          Left = 40
          Top = 20
          Width = 240
          Height = 23
          Hint = '; '#12391#21306#20999#12387#12390#35079#25968#25351#23450#21487#33021
          EditLabel.Width = 25
          EditLabel.Height = 23
          EditLabel.Caption = 'class'
          LabelPosition = lpLeft
          TabOrder = 0
          Text = ''
        end
        object DelBlkIdEdit: TLabeledEdit
          Left = 308
          Top = 20
          Width = 240
          Height = 23
          Hint = '; '#12391#21306#20999#12387#12390#35079#25968#25351#23450#21487#33021
          EditLabel.Width = 10
          EditLabel.Height = 23
          EditLabel.Caption = 'id'
          LabelPosition = lpLeft
          TabOrder = 1
          Text = ''
        end
      end
      object MarkdownCheck: TCheckBox
        Left = 20
        Top = 462
        Width = 541
        Height = 17
        Caption = 'Markdown '#35352#27861#12434#26377#21177#12395#12377#12427'('#35373#23450#20869#23481#12364#12487#12501#12457#12523#12488#12395#21021#26399#21270#12373#12428#12414#12377')'#12290
        TabOrder = 12
        OnClick = MarkdownCheckClick
      end
    end
    object TabSheet5: TTabSheet
      HelpContext = 12
      Caption = #32622#25563#12398#35373#23450
      ImageIndex = 4
      object Panel1: TPanel
        Left = 0
        Top = 368
        Width = 582
        Height = 129
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 0
        DesignSize = (
          582
          129)
        object AddItemBtn: TButton
          Left = 8
          Top = 97
          Width = 55
          Height = 24
          Action = AddRepAction
          TabOrder = 2
        end
        object UpRepBtn: TButton
          Left = 183
          Top = 97
          Width = 30
          Height = 24
          Action = UpRepAction
          TabOrder = 5
        end
        object DownRepBtn: TButton
          Left = 215
          Top = 97
          Width = 30
          Height = 24
          Action = DownRepAction
          TabOrder = 6
        end
        object DelItemBtn: TButton
          Left = 122
          Top = 97
          Width = 55
          Height = 24
          Action = DelRepAction
          TabOrder = 4
        end
        object ChgItemBtn: TButton
          Left = 65
          Top = 97
          Width = 55
          Height = 24
          Action = ChgRepAction
          TabOrder = 3
        end
        object FromStrEdit: TLabeledEdit
          Left = 8
          Top = 24
          Width = 565
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          BevelEdges = [beLeft, beTop, beRight]
          EditLabel.Width = 203
          EditLabel.Height = 15
          EditLabel.Caption = #26908#32034#25991#23383#21015' (/ '#65374' / '#12391#22258#12416#12392#27491#35215#34920#29694')'
          TabOrder = 0
          Text = ''
        end
        object ToStrEdit: TLabeledEdit
          Left = 8
          Top = 66
          Width = 565
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          BevelEdges = [beLeft, beTop, beRight]
          EditLabel.Width = 65
          EditLabel.Height = 15
          EditLabel.Caption = #32622#25563#25991#23383#21015
          TabOrder = 1
          Text = ''
        end
      end
      object RepCheckListBox: TCheckListBox
        Left = 0
        Top = 0
        Width = 582
        Height = 368
        Align = alClient
        Style = lbOwnerDrawFixed
        TabOrder = 1
        OnClick = RepCheckListBoxClick
        OnDrawItem = RepCheckListBoxDrawItem
      end
    end
    object TabSheet3: TTabSheet
      HelpContext = 6
      Caption = #12504#12483#12480'/'#12501#12483#12479
      ImageIndex = 2
      object Panel2: TPanel
        Left = 0
        Top = 239
        Width = 582
        Height = 258
        Align = alClient
        BevelOuter = bvNone
        Caption = 'Panel2'
        TabOrder = 1
        DesignSize = (
          582
          258)
        object GrpBox3_2: TGroupBox
          Left = 8
          Top = 6
          Width = 563
          Height = 246
          Anchors = [akLeft, akTop, akRight, akBottom]
          Caption = #12501#12483#12479'(&F)'
          TabOrder = 0
          DesignSize = (
            563
            246)
          object InsFtCheck: TCheckBox
            Left = 16
            Top = 20
            Width = 90
            Height = 17
            Caption = #25407#20837#12377#12427
            TabOrder = 0
          end
          object FootMemo: TMemo
            Left = 16
            Top = 46
            Width = 531
            Height = 192
            Anchors = [akLeft, akTop, akRight, akBottom]
            ScrollBars = ssVertical
            TabOrder = 2
            WordWrap = False
          end
          object RefMac2Btn: TButton
            Tag = 1
            Left = 369
            Top = 16
            Width = 160
            Height = 25
            Anchors = [akTop, akRight]
            Caption = #26360#24335#25991#23383#21015#12434#21442#29031'...'
            TabOrder = 1
            OnClick = RefMac1BtnClick
          end
        end
      end
      object Panel3: TPanel
        Left = 0
        Top = 0
        Width = 582
        Height = 239
        Align = alTop
        BevelOuter = bvNone
        Caption = 'Panel3'
        TabOrder = 0
        DesignSize = (
          582
          239)
        object GrpBox3_1: TGroupBox
          Left = 8
          Top = 4
          Width = 563
          Height = 229
          Anchors = [akLeft, akTop, akRight, akBottom]
          Caption = #12504#12483#12480'(&H)'
          TabOrder = 0
          DesignSize = (
            563
            229)
          object InsHdCheck: TCheckBox
            Left = 16
            Top = 20
            Width = 90
            Height = 17
            Caption = #25407#20837#12377#12427
            TabOrder = 0
          end
          object HeadMemo: TMemo
            Left = 16
            Top = 46
            Width = 531
            Height = 175
            Anchors = [akLeft, akTop, akRight, akBottom]
            ScrollBars = ssVertical
            TabOrder = 2
            WordWrap = False
          end
          object RefMac1Btn: TButton
            Left = 369
            Top = 16
            Width = 160
            Height = 25
            Anchors = [akTop, akRight]
            Caption = #26360#24335#25991#23383#21015#12434#21442#29031'...'
            TabOrder = 1
            OnClick = RefMac1BtnClick
          end
        end
      end
    end
    object TabSheet4: TTabSheet
      HelpContext = 7
      Caption = #12381#12398#20182#12398#35373#23450
      ImageIndex = 4
      DesignSize = (
        582
        497)
      object Label21: TLabel
        Left = 24
        Top = 334
        Width = 107
        Height = 15
        Caption = #34920#31034#20999#12426#26367#12360#12398#22522#28857
      end
      object Label23: TLabel
        Left = 24
        Top = 166
        Width = 55
        Height = 15
        Caption = #20986#21147#12467#12540#12489
      end
      object RefSndBtn: TButton
        Left = 493
        Top = 248
        Width = 40
        Height = 23
        Anchors = [akTop, akRight]
        Caption = '...'
        TabOrder = 12
        OnClick = RefSndBtnClick
      end
      object UseTrashCheck: TCheckBox
        Left = 196
        Top = 206
        Width = 449
        Height = 17
        Caption = #21066#38500#12395#12372#12415#31665#12434#20351#12358
        TabOrder = 10
      end
      object SureKillCheck: TCheckBox
        Left = 24
        Top = 206
        Width = 166
        Height = 17
        Caption = #21066#38500#12398#38555#12395#30906#35469#12377#12427
        TabOrder = 9
      end
      object TopMostCheck: TCheckBox
        Left = 24
        Top = 299
        Width = 137
        Height = 17
        Caption = #24120#12395#25163#21069#12395#34920#31034
        TabOrder = 14
        OnClick = TopMostCheckClick
      end
      object CmpPosCombo: TComboBox
        Left = 142
        Top = 331
        Width = 110
        Height = 23
        Style = csDropDownList
        TabOrder = 15
        Items.Strings = (
          #24038#19978
          #21491#19978
          #21491#19979
          #24038#19979)
      end
      object DropSubCheck: TCheckBox
        Left = 24
        Top = 17
        Width = 449
        Height = 17
        Caption = #12487#12451#12524#12463#12488#12522#12398#12489#12525#12483#12503#26178#12395#38542#23652#19979#12418#26908#32034
        TabOrder = 0
      end
      object Tit2NamCheck: TCheckBox
        Left = 24
        Top = 75
        Width = 249
        Height = 17
        Caption = #12479#12452#12488#12523#12363#12425#20986#21147#12501#12449#12452#12523#21517#12434#12388#12367#12427
        TabOrder = 2
        OnClick = Tit2NamCheckClick
      end
      object AddNoCheck: TCheckBox
        Left = 24
        Top = 131
        Width = 449
        Height = 17
        Caption = #20986#21147#12501#12449#12452#12523#21517#12364#37325#35079#12377#12427#12392#12365#36899#30058#12434#20184#21152
        TabOrder = 7
      end
      object TitLmtUpDown: TUpDown
        Left = 148
        Top = 100
        Width = 16
        Height = 23
        Associate = TitLmtEdit
        Min = 8
        Max = 200
        Position = 8
        TabOrder = 4
      end
      object TitCvExChCheck: TCheckBox
        Left = 196
        Top = 102
        Width = 137
        Height = 17
        Caption = '.\/:*?"<>| '#8594#20840#35282
        TabOrder = 5
      end
      object TitCvSpcCheck: TCheckBox
        Left = 359
        Top = 102
        Width = 82
        Height = 17
        Caption = #31354#30333' '#8594' _'
        TabOrder = 6
      end
      object CodePageComboBox: TComboBox
        Left = 87
        Top = 163
        Width = 165
        Height = 23
        Style = csDropDownList
        TabOrder = 8
      end
      object TitLmtEdit: TLabeledEdit
        Left = 108
        Top = 100
        Width = 40
        Height = 23
        Alignment = taRightJustify
        EditLabel.Width = 52
        EditLabel.Height = 23
        EditLabel.Caption = #23383#25968#21046#38480
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 3
        Text = '8'
        OnChange = TitLmtEditChange
        OnExit = NumberEditExit
      end
      object EndSoundEdit: TLabeledEdit
        Left = 87
        Top = 249
        Width = 404
        Height = 23
        Anchors = [akLeft, akTop, akRight]
        EditLabel.Width = 39
        EditLabel.Height = 23
        EditLabel.Caption = #32066#20102#38899
        LabelPosition = lpLeft
        TabOrder = 11
        Text = ''
      end
      object PlySndBtn: TButton
        Left = 534
        Top = 248
        Width = 32
        Height = 23
        Action = PlaySoundAction
        Anchors = [akTop, akRight]
        TabOrder = 13
      end
      object NaturalCheck: TCheckBox
        Left = 24
        Top = 46
        Width = 449
        Height = 17
        Caption = #33258#28982#38918#12391#12477#12540#12488#12377#12427
        TabOrder = 1
        OnClick = NaturalCheckClick
      end
    end
  end
  object CmpBtnPanel: TPanel
    Left = 532
    Top = 1
    Width = 54
    Height = 24
    Anchors = [akTop, akRight]
    AutoSize = True
    BevelOuter = bvNone
    TabOrder = 1
    object CompactBtn: TSpeedButton
      Left = 30
      Top = 0
      Width = 24
      Height = 24
      Align = alRight
      Caption = #9660
      Flat = True
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
      Font.Style = []
      ParentFont = False
      OnClick = CompactBtnClick
    end
    object MenuBtn: TSpeedButton
      Left = 0
      Top = 0
      Width = 24
      Height = 24
      Action = MenuAction
      Align = alLeft
      ImageIndex = 0
      ImageName = 'nekobtn'
      Images = VirtualImgIcoList
      HotImageName = 'ImageCollection2'
      Flat = True
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
      Font.Style = []
      ParentFont = False
      ExplicitLeft = 2
    end
  end
  object OpenDialog1: TOpenDialog
    Options = [ofHideReadOnly, ofAllowMultiSelect, ofEnableSizing]
    Left = 431
    Top = 215
  end
  object PopupMenu1: TPopupMenu
    AutoHotkeys = maManual
    Left = 509
    Top = 66
    object PopTitleItem: TMenuItem
      Caption = '$TITLE '#12479#12452#12488#12523'(&T)'
      OnClick = PopRefItemClick
    end
    object PopDescItem: TMenuItem
      Caption = '$DESCR '#25991#26360#12398#27010#35201' (&G)'
      OnClick = PopRefItemClick
    end
    object PopKeywdItem: TMenuItem
      Caption = '$KEYWD '#12461#12540#12527#12540#12489' (&K)'
      OnClick = PopRefItemClick
    end
    object Sep_p_1: TMenuItem
      Caption = '-'
    end
    object PopFnameItem: TMenuItem
      Caption = '$FNAME '#12501#12449#12452#12523#21517'/URL(&F)'
      OnClick = PopRefItemClick
    end
    object PopFtimeItem: TMenuItem
      Caption = '$FTIME '#12501#12449#12452#12523#12398#26085#20184#12539#26178#21051' (&S)'
      OnClick = PopRefItemClick
    end
    object PopDatTimItem: TMenuItem
      Caption = '$DTIME '#29694#22312#12398#26085#20184#12539#26178#21051'(&D)'
      OnClick = PopRefItemClick
    end
    object Sep_p_2: TMenuItem
      Caption = '-'
    end
    object PopLine1Item: TMenuItem
      Caption = '$LINE1 '#32619#32218' --  (&1)'
      OnClick = PopRefItemClick
    end
    object PopLine2Item: TMenuItem
      Caption = '$LINE2 '#32619#32218' '#9472'  (&2)'
      OnClick = PopRefItemClick
    end
    object PopLine3Item: TMenuItem
      Caption = '$LINE3 '#32619#32218' '#9473'  (&3)'
      OnClick = PopRefItemClick
    end
  end
  object ActionList1: TActionList
    Images = VirtualImgIcoList
    Left = 276
    Top = 66
    object ConvertAction2: TAction
      Caption = #38283#22987
      OnExecute = ConvertActionExecute
      OnUpdate = ConvertActionUpdate
    end
    object PlaySoundAction: TAction
      Caption = #9654
      OnExecute = PlaySoundActionExecute
      OnUpdate = PlaySoundActionUpdate
    end
    object AddRepAction: TAction
      Caption = #36861#21152
      OnExecute = AddRepActionExecute
      OnUpdate = AddRepActionUpdate
    end
    object ChgRepAction: TAction
      Caption = #22793#26356
      OnExecute = ChgRepActionExecute
      OnUpdate = ChgRepActionUpdate
    end
    object DelRepAction: TAction
      Caption = #21066#38500
      OnExecute = DelRepActionExecute
      OnUpdate = DelRepActionUpdate
    end
    object UpRepAction: TAction
      Caption = #8593
      OnExecute = UpRepActionExecute
      OnUpdate = UpRepActionUpdate
    end
    object DownRepAction: TAction
      Caption = #8595
      OnExecute = DownRepActionExecute
      OnUpdate = DownRepActionUpdate
    end
    object CopyListAction: TAction
      Caption = #12467#12500#12540'(&C)'
      ImageIndex = 4
      ImageName = '11_copy'
      ShortCut = 16451
      OnExecute = CopyListActionExecute
      OnUpdate = CopyListActionUpdate
    end
    object SelectAllAction: TAction
      Caption = #12377#12409#12390#12434#36984#25246'(&A)'
      OnExecute = SelectAllActionExecute
      OnUpdate = SelectAllActionUpdate
    end
    object SaveListAction: TAction
      Caption = #19968#35239#12395#21517#21069#12434#20184#12369#12390#20445#23384'(&S)...'
      ImageIndex = 3
      ImageName = '03_filesaveas'
      OnExecute = SaveListActionExecute
      OnUpdate = SaveListActionUpdate
    end
    object LoadListAction: TAction
      Caption = #19968#35239#12434#12501#12449#12452#12523#12363#12425#35501#12415#36796#12416'(&L)...'
      ImageIndex = 1
      ImageName = '01_fileopen'
      OnExecute = LoadListActionExecute
    end
    object SaveIniAction: TAction
      Caption = #35373#23450#12501#12449#12452#12523#12434#19978#26360#12365#20445#23384'(&S)'
      ImageIndex = 2
      ImageName = '02_filesave'
      OnExecute = SaveIniActionExecute
      OnUpdate = SaveIniActionUpdate
    end
    object MenuAction: TAction
      Hint = #12513#12491#12517#12540
      ShortCut = 32845
      OnExecute = MenuActionExecute
      OnUpdate = MenuActionUpdate
    end
  end
  object ToolMenu: TPopupMenu
    HelpContext = 8
    Images = VirtualImgIcoList
    OwnerDraw = True
    OnPopup = ToolMenuPopup
    Left = 431
    Top = 66
    object LoadIniItem: TMenuItem
      Caption = #35373#23450#12501#12449#12452#12523#12434#35501#12415#36796#12416'(&L)...'
      ImageIndex = 1
      ImageName = '01_fileopen'
      OnClick = LoadIniItemClick
    end
    object SaveIniItem: TMenuItem
      Action = SaveIniAction
    end
    object SaveAsIniItem: TMenuItem
      Caption = #35373#23450#12395#21517#21069#12434#20184#12369#12390#20445#23384'(&A)...'
      ImageIndex = 3
      ImageName = '03_filesaveas'
      OnClick = SaveAsIniItemClick
    end
    object FileHistItem: TMenuItem
      Caption = #26368#36817#38283#12356#12383#35373#23450#12501#12449#12452#12523
      OnClick = FileHistItemClick
    end
    object Sep_m_1: TMenuItem
      Caption = '-'
    end
    object DefaultIniItem: TMenuItem
      Caption = #35373#23450#12434#12487#12501#12457#12523#12488#12395#25147#12377'(&R)'
      OnClick = DefaultIniItemClick
    end
    object Sep_m_2: TMenuItem
      Caption = '-'
    end
    object HelpItem: TMenuItem
      Caption = #12504#12523#12503'(&H)'
      ImageIndex = 5
      ImageName = '35_help'
      OnClick = HelpItemClick
    end
    object AboutItem: TMenuItem
      Caption = #12496#12540#12472#12519#12531#24773#22577'(&V)'
      object AbautInf1Item: TMenuItem
        Tag = 1001
        OnDrawItem = AbautInfItemDrawItem
        OnMeasureItem = AbautMeasureItem
      end
      object AboutInf2Item: TMenuItem
        Tag = 1002
        OnDrawItem = AbautInfItemDrawItem
        OnMeasureItem = AbautMeasureItem
      end
      object GoWebItem: TMenuItem
        Caption = 'Web'#12469#12452#12488#12408'(&W)'
        OnClick = GoWebItemClick
      end
    end
  end
  object SaveDialog1: TSaveDialog
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 509
    Top = 215
  end
  object ApplicationEvents1: TApplicationEvents
    OnMessage = ApplicationEvents1Message
    Left = 356
    Top = 127
  end
  object PopupMenu2: TPopupMenu
    Images = VirtualImgIcoList
    Left = 510
    Top = 127
    object C1: TMenuItem
      Action = CopyListAction
    end
    object SelectAllItem: TMenuItem
      Action = SelectAllAction
      ShortCut = 16449
    end
    object Sep_1: TMenuItem
      Caption = '-'
    end
    object SaveListItem: TMenuItem
      Action = SaveListAction
    end
    object L1: TMenuItem
      Action = LoadListAction
    end
  end
  object ScrollTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = ScrollTimerTimer
    Left = 330
    Top = 215
  end
  object ImgArwCollection: TImageCollection
    Images = <
      item
        Name = 'arraw'
        SourceImages = <
          item
            Image.Data = {
              89504E470D0A1A0A0000000D49484452000000250000001008060000009129B6
              1D0000000774494D4507E7081C013A1E334EAB51000000097048597300000EC2
              00000EC20115284A800000000467414D410000B18F0BFC610500000099494441
              5478DAEDD6010A80200C05D02DBAB7F3E436B122A3D2AF69058DA408654F6323
              162120C421B3D7559A442FCE9B4B34A2090CB68B90089C3F94ECBC75FCA84FA3
              7C65382DA9D70C25393D2961AB15651F3E1C9E87BF715CE1DA87F485E98C095F
              2C3C1EF42961DF546C07DC1EB38D93E6D90E778549A0EEC7E5603251F5380403
              A2705C09A61095C6D5602A51318E85D65F9A1ACC12135A617E78F6E2D52C0000
              000049454E44AE426082}
          end>
      end>
    Left = 170
    Top = 131
  end
  object VirtualImgArwList: TVirtualImageList
    AutoFill = True
    Images = <
      item
        CollectionIndex = 0
        CollectionName = 'arraw'
        Name = 'arraw'
      end>
    ImageCollection = ImgArwCollection
    Width = 37
    Left = 58
    Top = 131
  end
  object VirtualImgIcoList: TVirtualImageList
    AutoFill = True
    Images = <
      item
        CollectionIndex = 0
        CollectionName = 'nekobtn'
        Name = 'nekobtn'
      end
      item
        CollectionIndex = 1
        CollectionName = '01_fileopen'
        Name = '01_fileopen'
      end
      item
        CollectionIndex = 2
        CollectionName = '02_filesave'
        Name = '02_filesave'
      end
      item
        CollectionIndex = 3
        CollectionName = '03_filesaveas'
        Name = '03_filesaveas'
      end
      item
        CollectionIndex = 4
        CollectionName = '11_copy'
        Name = '11_copy'
      end
      item
        CollectionIndex = 5
        CollectionName = '35_help'
        Name = '35_help'
      end>
    ImageCollection = ImgIcoCollection
    Width = 20
    Height = 20
    Left = 58
    Top = 67
  end
  object ImgIcoCollection: TImageCollection
    Images = <
      item
        Name = 'nekobtn'
        SourceImages = <
          item
            Image.Data = {
              89504E470D0A1A0A0000000D49484452000000140000001408060000008D891D
              0D0000000774494D4507E7081C020224E09B7041000000097048597300001EC2
              00001EC2016ED0753E0000000467414D410000B18F0BFC61050000009D494441
              5478DAD553D10E80200884E6FFFFB2A1A99D0A66CDB5BA871A20271C48F475B0
              27DF3B833B41E27CE58738398D4C70DA2CB6245BFE367F5BDDF272C2E51A16C2
              7C583B385D5D1E0A0A6E896D107415874F35BD440A07ECF69BA9C79FA3016657
              05B1C51D63C624D2F68E8E56AA8A35B8DC4AB8FDCE5030A7D838E551AB5D1C34
              6EF27EF05254C2765033ED96B8A62168D2058764F4F891BD881D57E468254E96
              60670000000049454E44AE426082}
          end>
      end
      item
        Name = '01_fileopen'
        SourceImages = <
          item
            Image.Data = {
              89504E470D0A1A0A0000000D49484452000000140000001408060000008D891D
              0D0000000774494D4507E70803022D2CF42687D7000000097048597300000B12
              00000B1201D2DD7EFC0000000467414D410000B18F0BFC6105000000B4494441
              5478DAED94D10D83300C4463C45E74B48C16264B7DB416B6B145DAF25309FF1C
              41F1F9710AA15ACBA5355D6BF70F86B35E709EDD6FE077F49521CCB8F809FDBB
              1251CF9AA361B35D5A336834440FF3A62EC3BC39D6930CCF4C9848EDB3B90B69
              4A8866F46B6DAD152410A9184F1161D6BC2C8FB2AEB16AE4F2FEFDFAABB6499B
              B2C99062BFF81C08F17920CA4822F567C9104247C9A09AEE4028D98D9299ECC4
              63BFBE2A4FAAE5D3F2079BEEFBF0E77A02A1A4D677E39827670000000049454E
              44AE426082}
          end>
      end
      item
        Name = '02_filesave'
        SourceImages = <
          item
            Image.Data = {
              89504E470D0A1A0A0000000D49484452000000140000001408060000008D891D
              0D0000000774494D4507E70803022D1E3CF1D657000000097048597300000B12
              00000B1201D2DD7EFC0000000467414D410000B18F0BFC61050000009E494441
              5478DACD94010E85200886A179306F26DD4C4FC68358AD165A6DBCD63F693AE1
              5BFAAB4804A19A62717F00A6E390981EEE81E513AE63DCD74B9F411B2030F310
              84A80CCB95BA0DE82C19E1BECEB90E906F80FAB9DF77F925A0DA262E8E023AC7
              2BF9C0229F32FE159F375832CD60C7C28965CE57EA03096AAD4BA872CEDB94F5
              CB43A048614456A817A7B506570A77198F66053F0E11FAFE4DF90113292DCA0B
              1F34600000000049454E44AE426082}
          end>
      end
      item
        Name = '03_filesaveas'
        SourceImages = <
          item
            Image.Data = {
              89504E470D0A1A0A0000000D49484452000000140000001408060000008D891D
              0D0000000774494D4507E70803022D0F5641F6A5000000097048597300000B12
              00000B1201D2DD7EFC0000000467414D410000B18F0BFC6105000000A6494441
              5478DACD940D0E80200885A179306F2657F364E44FB64C2C2DDA7AAB4D0DBEE4
              39442250D5A28BFB0068EA29314D7A90E309CB1C8FF961CC101F4060E64B1062
              64E4D890B703859211C6D5C60A401E00F56343C93443B8553A14E75CE5CD9D7F
              5D0342EE725E88B06CF833351EBE8189C073B9F107E51D91916047E8AC9F46A3
              CCAA22B953D2A7D456446EDB2980F73E8DADB550C75D768AE20ED52F070DFDFF
              825D0140BF3C30757A1D3B0000000049454E44AE426082}
          end>
      end
      item
        Name = '11_copy'
        SourceImages = <
          item
            Image.Data = {
              89504E470D0A1A0A0000000D49484452000000140000001408060000008D891D
              0D0000000774494D4507E70809011204F4B65B2C000000097048597300000B12
              00000B1201D2DD7EFC0000000467414D410000B18F0BFC6105000000AC494441
              5478DAAD950D0E80200885A5752F8F661ECD9391AC5CFE20A2C966B485CF0FDE
              2AB82EB3358EBD72C69CE92692A254189FC3342122B24B73E052CB1A5195604E
              AA9E618A10025B68AD5D13CC3702B43EE46D73460D099F4EE90245A6C348BC16
              15095FC94EFE887351C50C4BB29413793D1615A1F79E58E22AB3730A53DA80B8
              B1257CB293053957670941FADAD0C07B2E576EF74DF94BB87D868A7719077992
              90374AA8DFFD0BB8010C2C6AE5BBE9E89F0000000049454E44AE426082}
          end>
      end
      item
        Name = '35_help'
        SourceImages = <
          item
            Image.Data = {
              89504E470D0A1A0A0000000D49484452000000140000001408060000008D891D
              0D0000000774494D4507E70807081406A3ECD13A000000097048597300000B12
              00000B1201D2DD7EFC0000000467414D410000B18F0BFC6105000000CE494441
              5478DAC5D4810D84200C05D0D638189B019BC1643D2B567B40814B4C0E623482
              4F7E312201C19B6D7B55FB1B1821121F0838ADCF1464C8933FAFC3D1191DC138
              DA14C122C6668C716EC7F3B804D698A7F08C6130E16D053BEFC507D2B880660D
              CD982194A3135DC7DE576B26ABEAC5D56DAF6FF4305E9986044B298173CE8E1C
              AE5E63DE43172BC3E55392B1AF5D96C8886846D2189F19346B289189CA4B6A78
              8635E0BDA26B4A0DCFB0069409774D2A38E73CC4BA914730071861CDA69893D4
              2E8EB065F097F6FA0FF603927598F0D93F5E0B0000000049454E44AE426082}
          end>
      end>
    Left = 170
    Top = 67
  end
  object ImgBtnCollection: TImageCollection
    Images = <
      item
        Name = 'startbtn'
        SourceImages = <
          item
            Image.Data = {
              89504E470D0A1A0A0000000D4948445200000040000000400806000000AA6971
              DE0000000774494D4507E7081C01161773A3785B000000097048597300001EC1
              00001EC101C36954530000000467414D410000B18F0BFC610500000262494441
              5478DAED5BD192C32008AC99FBF0FEB97779B0F50C28CA02C9243B934E6A0C85
              4509189BDEEFD7ADF16321F48FD44CB4A528393D24E408280A67E25AFAF6191A
              809223C186343E334A1763F28BF6AA851C57028AD212F49447C97127E0CA5013
              30E3B502CA7B2839EE045C1D0F01D10A44E321402B604F48663392F43A263228
              39EE045C1DA84448ECBD9ED75072DC09A895E7342AD7464AA3E448C116439A4A
              2CBA1A9CB9EF4080672586C68AEE5B2BC0B312431BBFA2FBD60A90E06C246874
              7F1E83FB475425868056F7DB8F009345D1113C163B4F4300652CF9980A22C574
              0AD48FA6FAA040F5F388319F446836980C727A36219956F02B931D0D1ADDE123
              609490CCC27A34FC4B85A54C32F53CCCEB33BFABD5BD4D85559598F5841DBC53
              58D21D520DAE24231A8CCA615535380B6FE3A5244871FB4C50454094F77784BF
              1B8C341E49022415E62662165E6FE548DB1180C6805102844C9050801643A390
              9C84EDB5C7B973144E31023273CEF54396884B047001B0978949AE53FD8BE1DC
              BDDA40089D0259D8BEF20EB09653C8404C05F882C8C8B8DE9A7D16F4EDB59F82
              80DA4B2B818B0A7A9624988E00EA3C0BEFE58C45AF91AD06C1D4533657E7547B
              EF9E11492DB445914931A4F592E7F2309C80D564858AF41E300B82DC77CAB0D6
              F896044B322004CC787B9409AECA5D85A61C9EDED48446F826A948129E253110
              42F6096A81DC24E5BE4DEE4CC6C308A895BB92F13B607940512C192D965A18BF
              C3E2E5689A5DF818196E65FC0EABBFCD414683A5E105D61B243EA3A13E3863DB
              C3638788C71699831151DB612840FF3879453C9960B402D178088856201ABFC4
              A03596A24CA6430000000049454E44AE426082}
          end>
      end>
    Left = 170
    Top = 195
  end
  object VirtualImgBtnList: TVirtualImageList
    AutoFill = True
    Images = <
      item
        CollectionIndex = 0
        CollectionName = 'startbtn'
        Name = 'startbtn'
      end>
    ImageCollection = ImgBtnCollection
    Width = 64
    Height = 64
    Left = 58
    Top = 195
  end
  object ActionList2: TActionList
    Images = VirtualImgBtnList
    Left = 354
    Top = 67
    object ConvertAction: TAction
      ImageIndex = 0
      ImageName = 'startbtn'
      OnExecute = ConvertActionExecute
      OnUpdate = ConvertActionUpdate
    end
  end
end
