//----------------------------------------------------------------------//
// H2Tconv																//
//	���C���t�H�[��														//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include <algorithm>
#include <mmsystem.h>
#include <htmlhelp.h>
#include <Vcl.HtmlHelpViewer.hpp>
#include <Vcl.FileCtrl.hpp>
#include "UIniFile.h"
#include "UserFunc.h"
#include "Unit1.h"

#pragma link "Vcl.HTMLHelpViewer"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TH2TconvForm *H2TconvForm;

HINSTANCE	  hHHctrl	  = NULL;
FUNC_HtmlHelp lpfHtmlHelp = NULL;

//---------------------------------------------------------------------------
//���[�J���t�b�N
//  �_�C�A���O�Ȃǂ̈ʒu����
//---------------------------------------------------------------------------
HHOOK hDlgHook;

//---------------------------------------------------------------------------
LRESULT CALLBACK DlgHookProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code<0) return ::CallNextHookEx(hDlgHook, code, wParam, lParam);

	if (code==HC_ACTION) {
		PCWPSTRUCT sp = (PCWPSTRUCT)lParam;
		if (sp->message==WM_SHOWWINDOW && sp->wParam==1) {
			HWND hWnd = sp->hwnd;

			_TCHAR tbuf[1024];
			UnicodeString tit  = (::GetWindowText(hWnd, tbuf, 1023)>0)? UnicodeString(tbuf) : EmptyStr;
			UnicodeString cnam = (::GetClassName(hWnd, tbuf, 255)>0)? UnicodeString(tbuf) : EmptyStr;
			if (SameStr(cnam, "#32770")
				&&(SameStr(tit, "�m�F") || SameStr(tit, "�x��")
					|| SameStr(tit, "�t�H���_�[�̎Q��") || SameStr(tit, "�I�����̎w��")))
			{
				TControl *cp = H2TconvForm;
				TRect rc;
				::GetWindowRect(hWnd, &rc);
				TPoint p = cp->ClientToScreen(cp->ClientRect.CenteredRect(rc).Location);
				::SetWindowPos(hWnd, HWND_TOP, p.x, p.y, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
			}
		}
	}

	return ::CallNextHookEx(hDlgHook, code, wParam, lParam);
}

//---------------------------------------------------------------------------
// TH2TconvForm �N���X
//---------------------------------------------------------------------------
__fastcall TH2TconvForm::TH2TconvForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::FormCreate(TObject *Sender)
{
	VersionStr = "Version " + get_VersionStr(Application->ExeName);

	hDlgHook = ::SetWindowsHookEx(WH_CALLWNDPROC, DlgHookProc, NULL, ::GetCurrentThreadId());

	DropInitialize(Handle, HtmFileList);

	HC = new HtmConv();
	Converting = ConvReady = false;
	InhUpdateInf = false;

	CodePageComboBox->Items->Text =
		"ShiftJIS\r\n"
		"JIS (ISO-2022-JP)\r\n"
		"EUC-JP\r\n"
		"UTF-8\r\n"
		"UTF-16\r\n";

	//�N���I�v�V�����̏���
	UnicodeString OutNam = EmptyStr;
	UnicodeString OutDir = EmptyStr;
	UnicodeString IniNam = EmptyStr;
	int i = 1;
	while (!ParamStr(i).IsEmpty()) {
		UnicodeString prmbuf = ParamStr(i++).Trim();
		//�I�v�V����
		if (prmbuf.SubString(1, 1)=="-") {
			//�o�̓t�@�C���w��
			if (SameText(prmbuf.SubString(2, 1), "O")) {
				prmbuf.Delete(1, 2);
				if (!prmbuf.IsEmpty()) {
					OutDir = ExtractFileDir(prmbuf);
					OutNam = ExtractFileName(prmbuf);
				}
			}
			//�ݒ�t�@�C���̎w��
			else if (SameText(prmbuf.SubString(2, 1), "I")) {
				prmbuf.Delete(1, 2);
				IniNam = prmbuf;
			}
		}
		//���̓t�@�C����
		else {
			//�f�B���N�g��
			if (::PathIsDirectory(prmbuf.c_str())) {
				get_all_files(prmbuf, "*.*htm*", HtmFileList->Items, true);
			}
			//�t�@�C���AURL
			else {
				UnicodeString fmsk = ExtractFileName(prmbuf);
				if (fmsk.Pos("*")>0 || fmsk.Pos("?")>0)
					get_all_files(ExtractFileDir(prmbuf), fmsk, HtmFileList->Items, false);
				else
					AddStrToList(prmbuf);
			}
		}
	}

	//�ݒ�̓Ǎ�
	IniFile = new UsrIniFile(ChangeFileExt(Application->ExeName, ".INI"));
	if (!IniNam.IsEmpty()){
		get_file_name_r(IniNam);
		LoadIniFile(IniNam);
	}
	else
		LoadIniFile();

	AutoStart = false;
	if (HtmFileList->Items->Count>0) {
		if (!OutDir.IsEmpty()) {
			DstDirEdit->Text = OutDir;
			DestMode = DSTMOD_SELDIR;
		}
		if (!OutNam.IsEmpty()) {
			FilNamEdit->Text = OutNam;
			FilNamEdit->Modified = true;
			if (DestMode==DSTMOD_CLIPBD) DestMode = DSTMOD_ORGDIR;
		}
		UpdateInf(false);
		AutoStart = ConvReady;
	}

	if (AutoStart) {
		//���C����ʂ�\�������A�ϊ����s��ɏI��
		ConvertExecute(NULL);
		Application->Terminate();
	}
	else {
	 	Show();
	}
}
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::FormShow(TObject *Sender)
{
	//�w���v�t�@�C��������
	Application->HelpFile = ChangeFileExt(Application->ExeName, ".CHM");
	hHHctrl = ::LoadLibrary(_T("hhctrl.ocx"));
	if (hHHctrl) lpfHtmlHelp = (FUNC_HtmlHelp)::GetProcAddress(hHHctrl, "HtmlHelpW");
	HelpContext = 4;	//���o��

	//��ʏ�Ԃ𕜌�
	UnicodeString sct = "General";
	CmpPosCombo->ItemIndex = IniFile->ReadInteger(sct, "CompactPos", 0);
	Compact = IniFile->ReadBool(sct, "Compact",	false);
	if (Compact) {
		Constraints->MinWidth  = CMP_MIN_WD;
		Constraints->MinHeight = CMP_MIN_HI;
		Width  = IniFile->ReadInteger(sct, "WinWidthC",  CMP_MIN_WD);
		Height = IniFile->ReadInteger(sct, "WinHeightC", CMP_MIN_HI);
	}
	else {
		Constraints->MinWidth  = NRM_MIN_WD;
		Constraints->MinHeight = NRM_MIN_HI;
		Width  = IniFile->ReadInteger(sct, "WinWidth",  NRM_MIN_WD);
		Height = IniFile->ReadInteger(sct, "WinHeight", NRM_MIN_HI);
	}
	Left = IniFile->ReadInteger(sct, "WinLeft",	0);
	Top  = IniFile->ReadInteger(sct, "WinTop",	0);

	TopMostCheck->Checked	= IniFile->ReadBool(sct, "TopMost", false);
	if (TopMostCheck->Checked)
		::EndDeferWindowPos(DeferWindowPos(BeginDeferWindowPos(1),
			Handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE));

	SetFrmStyle();

	//���̑��̈�ʐݒ�
	LastIniDir = IniFile->ReadString(sct, "LastIniDir",	ExtractFileDir(Application->ExeName));

	ConvBtnC->Parent = H2TconvForm;
}

//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (!AutoStart) {
		//��ʏ�Ԃ�ۑ�
		UnicodeString sct = "General";
		IniFile->WriteInteger(sct, "WinTop",   		Top);
		IniFile->WriteInteger(sct, "WinLeft",  		Left);
		if (Compact) {
			IniFile->WriteInteger(sct, "WinWidthC", 	Width);
			IniFile->WriteInteger(sct, "WinHeightC",	Height);
		}
		else {
			IniFile->WriteInteger(sct, "WinWidth", 	Width);
			IniFile->WriteInteger(sct, "WinHeight",	Height);
		}
		IniFile->WriteBool(   sct, "Compact",		Compact);
		IniFile->WriteInteger(sct, "CompactPos",	CmpPosCombo->ItemIndex);
		IniFile->WriteBool(   sct, "TopMost",		TopMostCheck->Checked);

		//�e��ݒ��ۑ�
		IniFile->WriteString(sct, "LastIniDir",		LastIniDir);

		SaveIniFile();
	}
}

//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::FormDestroy(TObject *Sender)
{
	::UnhookWindowsHookEx(hDlgHook);

	delete HC;
	delete IniFile;

	DropUninitialize();

	if (hHHctrl) ::FreeLibrary(hHHctrl);
}

//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::ApplicationEvents1Message(tagMSG &Msg, bool &Handled)
{
	//�w���v�̃L�[���b�Z�[�W����
	if (lpfHtmlHelp) {
		Handled = (lpfHtmlHelp(NULL, NULL, HH_PRETRANSLATEMESSAGE, (DWORD)&Msg) != NULL);
	}
}

//---------------------------------------------------------------------------
//��ʃ��T�C�Y���̏���
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::FormResize(TObject *Sender)
{
	if (Compact) {
		GrpBox1_1->Width  = ClientWidth - ConvBtnC->Width - 10;
		GrpBox1_1->Height = ClientHeight;
	}

	if (Screen->MonitorCount==1) {
		//�ʒu����ʓ��ɐ���
		if (BoundsRect.Right>Screen->Width)
			Left = Screen->Width - Width;
		else if (Left<0)
			Left = 0;

		if (BoundsRect.Bottom>Screen->Height)
			Top = Screen->Height - Height;
		else if (Top<0)
			Top = 0;
	}

	SetFrmStyle();
}

//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::PageControl1Change(TObject *Sender)
{
	switch (PageControl1->TabIndex) {
	case 0: HelpContext = 4;	break;
	case 1: HelpContext = 5;	break;
	case 2: HelpContext = 12;	break;
	case 3: HelpContext = 6;	break;
	case 4: HelpContext = 7;	break;
	default:
		HelpContext = 0;
	}
}

//---------------------------------------------------------------------
//��ʂ��ړ����ꂽ�Ƃ��̏���
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::WMMove(TMessage &Msg)
{
	if (Screen->MonitorCount==1) {
		//��ʂ���͂ݏo������߂�
		if (BoundsRect.Right>Screen->Width)
			Left = Screen->Width - Width;
		else if (Left<0)
			Left = 0;

		if (BoundsRect.Bottom>Screen->Height)
			Top = Screen->Height - Height;
		else if (Top<0)
			Top = 0;
	}
}

//---------------------------------------------------------------------------
//�h���b�v�̎󂯓���
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::WmDropped(TMessage &msg)
{
	if (!Converting && (DroppedMode==DROPEFFECT_COPY || DroppedMode==DROPEFFECT_LINK)) {
		ConvReady = false;
		UpdateActions();

		int lastn = HtmFileList->Items->Count;
		Screen->Cursor = crHourGlass;
		StatusBar1->SimpleText = "�h���b�v���ڂ��擾��...";

		for (int i=0; i<DroppedList->Count; i++) {
			UnicodeString fnam = DroppedList->Strings[i];
			if (EndsStr("\\", fnam))
				get_all_files(fnam, "*.*htm*", HtmFileList->Items, DropSubCheck->Checked);
			else if (HtmFileList->Items->IndexOf(fnam)==-1)
				HtmFileList->Items->Add(fnam);
		}

		if (lastn==0) {
			HtmFileList->Sorted = true;
			HtmFileList->Sorted = false;
		}

		UpdateInf(true);

		Screen->Cursor = crDefault;
	}

	DroppedList->Clear();
	::SetForegroundWindow(Handle);
}

//---------------------------------------------------------------------------
//�ݒ�̓Ǎ�
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::LoadIniFile(UnicodeString fnam)
{
	UsrIniFile *ini_file;
	if (!fnam.IsEmpty())
		ini_file = new UsrIniFile(fnam);
	else
		ini_file = IniFile;

	UnicodeString sct = "Option";
	LastDir 				= ini_file->ReadString( sct, "LastDir");
	OpenAppCheck->Checked	= ini_file->ReadBool(   sct, "OpenApp");
	AppNameEdit->Text		= ini_file->ReadString( sct, "AppName",		"notepad");
	DestMode				= ini_file->ReadInteger(sct, "Destination",	DSTMOD_CLIPBD);
	UpdateInf(true);
	DstDirEdit->Text		= ini_file->ReadString( sct, "DstDir");
	JoinCheck->Checked		= ini_file->ReadBool(   sct, "JoinText");
	Tit2NamCheck->Checked	= ini_file->ReadBool(   sct, "Title2Name");
	TitLmtUpDown->Position	= ini_file->ReadInteger(sct, "TitleLimit",	40);
	TitLmtEdit->Text		= TitLmtUpDown->Position;
	TitCvExChCheck->Checked	= ini_file->ReadBool(   sct, "TitleCvExCh");
	TitCvSpcCheck->Checked	= ini_file->ReadBool(   sct, "TitleCvSpc");
	Tit2NamCheckClick(NULL);
	AddNoCheck->Checked 	= ini_file->ReadBool(   sct, "AddSerNo");
	KillCheck->Checked		= ini_file->ReadBool(   sct, "KillHtml");
	SureKillCheck->Checked	= ini_file->ReadBool(   sct, "SureKill");
	InsHdCheck->Checked 	= ini_file->ReadBool(   sct, "InsertHead");
	InsFtCheck->Checked 	= ini_file->ReadBool(   sct, "InsertFoot");
	HeadMemo->Lines->Text
		= esc_to_str(ini_file->ReadString(sct, "HeadText", "�^�C�g�� [$TITLE]\\n$LINE2"));
	FootMemo->Lines->Text
		= esc_to_str(ini_file->ReadString(sct, "FootText", EmptyStr));
	EndSoundEdit->Text		= ini_file->ReadString(sct, "EndSound");
	UseTrashCheck->Checked	= ini_file->ReadBool(  sct, "UseTrashCan",		true);
	DropSubCheck->Checked	= ini_file->ReadBool(  sct, "DropSubdir",		true);
	CodePageComboBox->ItemIndex = ini_file->ReadInteger(sct, "OutCodePage",	0);

	sct = "Convert";
	LnWidUpDown->Position	  = ini_file->ReadInteger(sct, "LineWidth",		DEF_LINE_WIDTH);
	LnWidEdit->Text			  = LnWidUpDown->Position;
	FrcCrCheck->Checked 	  = ini_file->ReadBool(   sct, "ForceCr");
	PstHdrCheck->Checked	  = ini_file->ReadBool(   sct, "NoPostHdr");
	BlkLmtUpDown->Position	  = ini_file->ReadInteger(sct, "BlankLnLimit", 	DEF_BLANK_LN_LIMIT);
	BlkLmtEdit->Text		  = BlkLmtUpDown->Position;
	HrStrEdit->Text 		  = ini_file->ReadString( sct, "HrLineStr",		"��");
	H1Edit->Text			  = ini_file->ReadString( sct, "H1Str",			"��");
	H2Edit->Text			  = ini_file->ReadString( sct, "H2Str",			"��");
	H3Edit->Text			  = ini_file->ReadString( sct, "H3Str",			"��");
	H4Edit->Text			  = ini_file->ReadString( sct, "H4Str",			"��");
	H5Edit->Text			  = ini_file->ReadString( sct, "H5Str",			"��");
	H6Edit->Text			  = ini_file->ReadString( sct, "H6Str",			"��");
	B_BraEdit->Text 		  = ini_file->ReadString( sct, "B_BraStr");
	B_KetEdit->Text 		  = ini_file->ReadString( sct, "B_KetStr");
	I_BraEdit->Text 		  = ini_file->ReadString( sct, "I_BraStr");
	I_KetEdit->Text 		  = ini_file->ReadString( sct, "I_KetStr");
	U_BraEdit->Text 		  = ini_file->ReadString( sct, "U_BraStr");
	U_KetEdit->Text 		  = ini_file->ReadString( sct, "U_KetStr");
	IndentEdit->Text		  = ini_file->ReadString( sct, "IndentStr",		"\\s\\s");
	MarkEdit->Text			  = ini_file->ReadString( sct, "UlMarkStr",		"�E");
	ZenNoCheck->Checked 	  = ini_file->ReadBool(   sct, "ZenNumber");
	CelSepEdit->Text		  = ini_file->ReadString( sct, "CellSepStr",	"\\t");
	AltCheck->Checked		  = ini_file->ReadBool(   sct, "UseAltStr");
	ImgSrcCheck->Checked	  = ini_file->ReadBool(   sct, "AddImgSrc");
	AltBraEdit->Text		  = ini_file->ReadString( sct, "AltBraStr",		"[");
	AltKetEdit->Text		  = ini_file->ReadString( sct, "AltKetStr",		"]");
	DefAltEdit->Text		  = ini_file->ReadString( sct, "DefAltStr",		"�摜");
	LinkCheck->Checked		  = ini_file->ReadBool(   sct, "InsertLink");
	ExtLnkCheck->Checked	  = ini_file->ReadBool(   sct, "OnlyExLink",	true);
	LinkCrCheck->Checked	  = ini_file->ReadBool(   sct, "AddCrLink");
	LnkBraEdit->Text		  = ini_file->ReadString( sct, "LinkBraStr",	"�i");
	LnkKetEdit->Text		  = ini_file->ReadString( sct, "LinkKetStr",	"�j");
	InsLineStrEdit->Text	  = ini_file->ReadString( sct, "InsLineStr",	"��");
	InsHrClsEdit->Text		  = ini_file->ReadString( sct, "InsHrClass",	EmptyStr);
	InsHrSctCheckBox->Checked = ini_file->ReadBool(   sct, "InsHrSct");
	InsHrArtCheckBox->Checked = ini_file->ReadBool(   sct, "InsHrArt");
	InsHrNavCheckBox->Checked = ini_file->ReadBool(   sct, "InsHrNav");
	DelBlkClsEdit->Text 	  = ini_file->ReadString( sct, "DelBlkCls");
	DelBlkIdEdit->Text		  = ini_file->ReadString( sct, "DelBlkId");

	sct = "Replace";
	TCheckListBox *lp = RepCheckListBox;
	lp->Clear();
	for (int i=1;;i++) {
		UnicodeString f_str = ini_file->ReadString(sct, "FromStr" + IntToStr(i));
		UnicodeString t_str = ini_file->ReadString(sct, "ToStr" + IntToStr(i));
		if (f_str.IsEmpty()) break;
		int idx = lp->Items->Add(f_str + "\t" + t_str);
		lp->Checked[idx] = ini_file->ReadBool(sct, "Enabled" + IntToStr(i));
	}

	AltCheckClick(NULL);
	LinkCheckClick(NULL);

	if (!fnam.IsEmpty()) {
		Caption = UnicodeString().sprintf(_T("%s - [%s]"), Application->Title.c_str(), ExtractFileName(fnam).c_str());
		delete ini_file;
	}
	else
		Caption = Application->Title;
}
//---------------------------------------------------------------------------
//�ݒ�̕ۑ�
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::SaveIniFile(UnicodeString fnam)
{
	UsrIniFile *ini_file;
	if (!fnam.IsEmpty())
		ini_file = new UsrIniFile(fnam);
	else
		ini_file = IniFile;

	UnicodeString sct = "Option";
	ini_file->WriteInteger(sct, "Destination",	DestMode);
	ini_file->WriteString( sct, "LastDir",		LastDir);
	ini_file->WriteBool(   sct, "OpenApp",		OpenAppCheck->Checked);
	ini_file->WriteString( sct, "AppName",		AppNameEdit->Text);
	ini_file->WriteString( sct, "DstDir", 		DstDirEdit->Text);
	ini_file->WriteBool(   sct, "JoinText",		JoinCheck->Checked);
	ini_file->WriteBool(   sct, "Title2Name",	Tit2NamCheck->Checked);
	ini_file->WriteString( sct, "TitleLimit",	TitLmtEdit->Text);
	ini_file->WriteBool(   sct, "TitleCvExCh",	TitCvExChCheck->Checked);
	ini_file->WriteBool(   sct, "TitleCvSpc",	TitCvSpcCheck->Checked);
	ini_file->WriteBool(   sct, "AddSerNo",		AddNoCheck->Checked);
	ini_file->WriteBool(   sct, "KillHtml",		KillCheck->Checked);
	ini_file->WriteBool(   sct, "SureKill",		SureKillCheck->Checked);
	ini_file->WriteBool(   sct, "InsertHead",	InsHdCheck->Checked);
	ini_file->WriteBool(   sct, "InsertFoot",	InsFtCheck->Checked);
	ini_file->WriteString( sct, "HeadText",		str_to_esc(HeadMemo->Lines->Text));
	ini_file->WriteString( sct, "FootText",		str_to_esc(FootMemo->Lines->Text));
	ini_file->WriteString( sct, "EndSound",		EndSoundEdit->Text);
	ini_file->WriteBool(   sct, "UseTrashCan",	UseTrashCheck->Checked);
	ini_file->WriteBool(   sct, "DropSubdir",	DropSubCheck->Checked);
	ini_file->WriteInteger(sct, "OutCodePage",	CodePageComboBox->ItemIndex);

	sct = "Convert";
	ini_file->WriteString( sct, "LineWidth",	LnWidEdit->Text);
	ini_file->WriteBool(   sct, "ForceCr",		FrcCrCheck->Checked);
	ini_file->WriteString( sct, "BlankLnLimit",	BlkLmtEdit->Text);
	ini_file->WriteBool(   sct, "NoPostHdr",	PstHdrCheck->Checked);
	ini_file->WriteString( sct, "HrLineStr",	HrStrEdit->Text);
	ini_file->WriteString( sct, "H1Str",		H1Edit->Text);
	ini_file->WriteString( sct, "H2Str",		H2Edit->Text);
	ini_file->WriteString( sct, "H3Str",		H3Edit->Text);
	ini_file->WriteString( sct, "H4Str",		H4Edit->Text);
	ini_file->WriteString( sct, "H5Str",		H5Edit->Text);
	ini_file->WriteString( sct, "H6Str",		H6Edit->Text);
	ini_file->WriteString( sct, "B_BraStr",		B_BraEdit->Text);
	ini_file->WriteString( sct, "B_KetStr",		B_KetEdit->Text);
	ini_file->WriteString( sct, "I_BraStr",		I_BraEdit->Text);
	ini_file->WriteString( sct, "I_KetStr",		I_KetEdit->Text);
	ini_file->WriteString( sct, "U_BraStr",		U_BraEdit->Text);
	ini_file->WriteString( sct, "U_KetStr",		U_KetEdit->Text);
	ini_file->WriteString( sct, "IndentStr",	IndentEdit->Text);
	ini_file->WriteString( sct, "UlMarkStr",	MarkEdit->Text);
	ini_file->WriteBool(   sct, "ZenNumber",	ZenNoCheck->Checked);
	ini_file->WriteString( sct, "CellSepStr",	CelSepEdit->Text);
	ini_file->WriteBool(   sct, "UseAltStr",	AltCheck->Checked);
	ini_file->WriteBool(   sct, "AddImgSrc",	ImgSrcCheck->Checked);
	ini_file->WriteString( sct, "AltBraStr",	AltBraEdit->Text);
	ini_file->WriteString( sct, "AltKetStr",	AltKetEdit->Text);
	ini_file->WriteString( sct, "DefAltStr",	DefAltEdit->Text);
	ini_file->WriteBool(   sct, "InsertLink",	LinkCheck->Checked);
	ini_file->WriteBool(   sct, "OnlyExLink", 	ExtLnkCheck->Checked);
	ini_file->WriteBool(   sct, "AddCrLink", 	LinkCrCheck->Checked);
	ini_file->WriteString( sct, "LinkBraStr",	LnkBraEdit->Text);
	ini_file->WriteString( sct, "LinkKetStr",	LnkKetEdit->Text);
	ini_file->WriteString( sct, "InsLineStr",	InsLineStrEdit->Text);
	ini_file->WriteString( sct, "InsHrClass",	InsHrClsEdit->Text);
	ini_file->WriteBool(   sct, "InsHrSct",		InsHrSctCheckBox->Checked);
	ini_file->WriteBool(   sct, "InsHrArt",		InsHrArtCheckBox->Checked);
	ini_file->WriteBool(   sct, "InsHrNav",		InsHrNavCheckBox->Checked);
	ini_file->WriteString( sct, "DelBlkCls",	DelBlkClsEdit->Text);
	ini_file->WriteString( sct, "DelBlkId",		DelBlkIdEdit->Text);

	sct = "Replace";
	ini_file->EraseSection(sct);
	for (int i=0; i<RepCheckListBox->Count; i++) {
		UnicodeString lbuf = RepCheckListBox->Items->Strings[i];
		ini_file->WriteString(sct, "FromStr" + IntToStr(i + 1),	"\"" + get_tkn(lbuf, "\t") + "\"");
		ini_file->WriteString(sct, "ToStr" + IntToStr(i + 1),	"\"" + get_tkn_r(lbuf, "\t") + "\"");
		ini_file->WriteBool(sct, "Enabled" + IntToStr(i + 1),	RepCheckListBox->Checked[i]);
	}

	ini_file->UpdateFile();

	if (!fnam.IsEmpty()) {
		Caption = UnicodeString().sprintf(_T("%s - [%s]"), Application->Title.c_str(), ExtractFileName(fnam).c_str());
		delete ini_file;
	}
}

//---------------------------------------------------------------------
//�t�@�C������URL���ꗗ�ɒǉ�
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::AddStrToList(UnicodeString s)
{
	if (!s.IsEmpty()) {
		s = del_quot(s);
		if (StartsText("file:/", s)) {
			s.Delete(1, 6);
			if (StartsText("//", s)) s.Delete(1, 2);
			s = ReplaceStr(s, "|\\", ":\\");
			s = ReplaceStr(s, "/",	  "\\");
			s = ReplaceStr(s, "%20", " ");
		}

		if (HtmFileList->Items->IndexOf(s)==-1) HtmFileList->Items->Add(s);
		UpdateInf(true);
	}
}
//---------------------------------------------------------------------------
//���̓t�@�C���I��
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::AddBtnClick(TObject *Sender)
{
	OpenDialog1->Title		= "HTML�����̑I��";
	OpenDialog1->Filter 	= "HTML���� (*.*htm*)|*.*HTM*";
	OpenDialog1->DefaultExt = "HTML";
	OpenDialog1->FileName	= "*.*HTM*";
	OpenDialog1->InitialDir = LastDir;

	TListBox *lp = HtmFileList;
	int lastn = lp->Count;
	if (OpenDialog1->Execute()) {
 		for (int i=0; i<OpenDialog1->Files->Count; i++) {
			UnicodeString fnam = OpenDialog1->Files->Strings[i];
			if (lp->Items->IndexOf(fnam)==-1) lp->Items->Add(fnam);
		}
		LastDir = ExtractFileDir(OpenDialog1->FileName);
		if (lastn==0) {
			lp->Sorted = true;
			lp->Sorted = false;
		}
	}

	UpdateInf(true);
}
//---------------------------------------------------------------------------
//URL ����
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::InputBtnClick(TObject *Sender)
{
	UnicodeString urlstr = InputBox(
		"����", "�t�@�C������URL����͂��Ă�������", EmptyStr);
	if (!urlstr.IsEmpty()) AddStrToList(urlstr);
	UpdateInf(true);
}
//---------------------------------------------------------------------------
//�N���b�v�{�[�h����\��t��
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::PasteBtnClick(TObject *Sender)
{
	if (!Clipboard()->HasFormat(CF_TEXT)) return;

	std::unique_ptr<TStringList> tmpbuf(new TStringList());
	tmpbuf->Text = Clipboard()->AsText;
	for (int i=0; i<tmpbuf->Count; i++)
		if (!tmpbuf->Strings[i].IsEmpty()) AddStrToList(tmpbuf->Strings[i]);
	UpdateInf(true);
}
//---------------------------------------------------------------------------
//�폜
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::DelBtnClick(TObject *Sender)
{
	TListBox *lp = HtmFileList;
	int i = 0;
	while (i<lp->Count) {
		if (lp->Selected[i]) lp->Items->Delete(i); else i++;
	}
	UpdateInf(true);
}
//---------------------------------------------------------------------------
//�N���A
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::ClrBtnClick(TObject *Sender)
{
	HtmFileList->Clear();
	FilNamEdit->Text = EmptyStr;
	UpdateInf(true);
}
//---------------------------------------------------------------------------
//�\�[�g
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::SortBtnClick(TObject *Sender)
{
	TListBox *lp = HtmFileList;
	if (lp->Count<2) return;

	Screen->Cursor = crHourGlass;
	UnicodeString lbuf = (lp->ItemIndex!=-1)? lp->Items->Strings[lp->ItemIndex] : EmptyStr;
	lp->Sorted = true;
	lp->Sorted = false;

	if (!lbuf.IsEmpty()) {
		lp->ItemIndex = lp->Items->IndexOf(lbuf);
		lp->ClearSelection();
		lp->Selected[lp->ItemIndex] = true;
	}
	Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------
//����
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::UpItemBtnClick(TObject *Sender)
{
	TListBox *lp = HtmFileList;
	if (lp->Count<2) return;

	int idx = lp->ItemIndex;
	if (idx>0) {
		lp->Items->Move(idx, idx - 1);
		lp->ItemIndex = idx - 1;
		lp->ClearSelection();
		lp->Selected[lp->ItemIndex] = true;
	}
}
//---------------------------------------------------------------------------
//�����
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::DowItemBtnClick(TObject *Sender)
{
	TListBox *lp = HtmFileList;
	if (lp->Count<2) return;

	int idx = lp->ItemIndex;
	if (idx < (lp->Count - 1)) {
		lp->Items->Move(idx, idx + 1);
		lp->ItemIndex = idx + 1;
		lp->ClearSelection();
		lp->Selected[lp->ItemIndex] = true;
	}
}

//---------------------------------------------------------------------------
//�ꗗ��ł̃L�[����
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::HtmFileListKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	switch (Key) {
	case VK_DELETE:
		DelBtnClick(NULL);
		break;
	case 'V':
		if (Shift.Contains(ssCtrl)) PasteBtnClick(NULL);
		break;
	}
}

//---------------------------------------------------------------------------
// URL �Ȃ�t�@�C���ɓK���Ȗ��O�ɕϊ�
//---------------------------------------------------------------------
UnicodeString __fastcall TH2TconvForm::UrlToName(UnicodeString s)
{
	if (StartsText("http://", s) || StartsText("https://", s)) {
		s = get_tkn_r(s, "://");
		int p = s.Pos("?");
		if (p>0) s = s.SubString(1, p - 1);
		s = ReplaceStr(s, "/",	"\\");
		s = ChangeFileExt(s, EmptyStr);
		s = ReplaceStr(s, "\\",	"-");
		s = ReplaceStr(s, ".",	"_");
	}
	return s;
}
//---------------------------------------------------------------------------
// �^�C�g�������񂩂�K���Ȗ��O������
//---------------------------------------------------------------------
UnicodeString __fastcall TH2TconvForm::TitToName(UnicodeString s)
{
	// . \ / : * ? " < > |
	std::unique_ptr<TStringList> lst(new TStringList());
	if (TitCvExChCheck->Checked)
		lst->Text =
			".\t�D\r\n"
			"\\\t��\r\n"
			"/\t�^\r\n"
			":\t�F\r\n"
			"*\t��\r\n"
			"?\t�H\r\n"
			"\"\t�h\r\n"
			"<\t��\r\n"
			">\t��\r\n"
			"|\t�b\r\n";
	else
		lst->Text =
			".\t\r\n"
			"\\\t\r\n"
			"/\t\r\n"
			":\t\r\n"
			"*\t\r\n"
			"?\t\r\n"
			"\"\t\r\n"
			"<\t\r\n"
			">\t\r\n"
			"|\t\r\n";
	s = replace_str_by_list(s, lst.get());

	if (TitCvSpcCheck->Checked) {
		s = ReplaceStr(s, " ",	"_");
		s = ReplaceStr(s, "�@",	"_");
	}

	s = Trim(s);

	int maxl = TitLmtEdit->Text.ToIntDef(8);
	if (s.Length()>maxl) s.SetLength(maxl);

	if (s.IsEmpty()) s = "untitled";
	return s;
}
//---------------------------------------------------------------------------
// �d������ꍇ�Ƀt�@�C�����ɘA�Ԃ�t��
//---------------------------------------------------------------------
UnicodeString __fastcall TH2TconvForm::AddSerNo(UnicodeString s)
{
	UnicodeString ret = s;
	UnicodeString ext = ExtractFileExt(s);
	UnicodeString nam = ChangeFileExt(s, "");
	for (int n=0; true; n++) {
		if (n==0) {
			ret = nam + ext;
			if (!FileExists(ret)) break;
		}
		else {
			ret.sprintf(_T("%s(%u)%s"), nam.c_str(), n, ext.c_str());
			if (!FileExists(ret)) break;
		}
	}
	return ret;
}

//---------------------------------------------------------------------------
//�\���X�^�C���̐ݒ�
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::SetFrmStyle()
{
	int w;
	if (!Compact) {
		ConvBtnC->Visible	  = false;
		StatusBar1->Visible   = true;
		CmpBtnPanel->Parent   = PageControl1;
		CmpBtnPanel->Left	  = PageControl1->Width - CmpBtnPanel->Width;
		CompactBtn->Caption   = "��";
		CompactBtn->Hint	  = "�R���p�N�g�\��";
		GrpBox1_1->Parent	  = Panel1_1;
		GrpBox1_1->Align	  = alClient;
		w					  = 70;
		AddBtn->Left		  = 8;
		AddBtn->Caption 	  = "�ǉ�(&A)...";
		AddBtn->Width		  = w;
		InputBtn->Caption	  = "����(&B)...";
		InputBtn->Width 	  = w;
		InputBtn->Left		  = AddBtn->Left + w + 2;
		PasteBtn->Caption	  = "�\�t(&P)";
		PasteBtn->Width 	  = w;
		PasteBtn->Left		  = InputBtn->Left + w + 2;
		w					  = 55;
		DelBtn->Caption 	  = "����";
		DelBtn->Width		  = w;
		DelBtn->Left		  = PasteBtn->Left + w + 15 + 12;
		ClrBtn->Caption 	  = "�N���A";
		ClrBtn->Width		  = w;
		ClrBtn->Left		  = DelBtn->Left + w + 2;
		SortBtn->Caption	  = "�\�[�g";
		SortBtn->Width		  = w;
		SortBtn->Left		  = ClrBtn->Left + w + 12;
		UpItemBtn->Caption	  = "���";
		UpItemBtn->Width	  = w;
		UpItemBtn->Left 	  = SortBtn->Left + w + 2;
		DowItemBtn->Caption   = "����";
		DowItemBtn->Width	  = w;
		DowItemBtn->Left	  = UpItemBtn->Left + w + 2;
		Panel3->Height		  = TabSheet3->Height/2;
		PageControl1->Visible = true;
	}
	else {
		StatusBar1->Visible   = false;
		PageControl1->Visible = false;
		CmpBtnPanel->Parent   = H2TconvForm;
		CompactBtn->Caption   = "��";
		CompactBtn->Hint	  = "�ʏ�\��";
		GrpBox1_1->Parent	  = H2TconvForm;
		GrpBox1_1->Align	  = alNone;
		GrpBox1_1->Left 	  = 0;
		GrpBox1_1->Top		  = 0;
		GrpBox1_1->Width	  = ClientWidth - ConvBtnC->Width - 10;
		GrpBox1_1->Height	  = ClientHeight;
		ConvBtnC->Left		  = ClientWidth - ConvBtnC->Width - 2;
		ConvBtnC->Top		  = ClientHeight - ConvBtnC->Height - 2;
		ConvBtnC->Visible	  = true;
		w					  = 28;
		AddBtn->Left		  = 4;
		AddBtn->Caption 	  = "��";
		AddBtn->Width		  = w;
		InputBtn->Caption	  = "��";
		InputBtn->Width 	  = w;
		InputBtn->Left		  = AddBtn->Left + w + 1;
		PasteBtn->Caption	  = "�\";
		PasteBtn->Width 	  = w;
		PasteBtn->Left		  = InputBtn->Left + w + 1;
		DelBtn->Caption 	  = "��";
		DelBtn->Width		  = w;
		DelBtn->Left		  = PasteBtn->Left + w + 4;
		ClrBtn->Caption 	  = "��";
		ClrBtn->Width		  = w;
		ClrBtn->Left		  = DelBtn->Left + w + 1;
		SortBtn->Caption	  = "��";
		SortBtn->Width		  = w;
		SortBtn->Left		  = ClrBtn->Left + w + 4;
		UpItemBtn->Caption	  = "��";
		UpItemBtn->Width	  = w;
		UpItemBtn->Left 	  = SortBtn->Left + w + 1;
		DowItemBtn->Caption   = "��";
		DowItemBtn->Width	  = w;
		DowItemBtn->Left	  = UpItemBtn->Left + w + 1;
	}

	SortBtn->Visible	= ((SortBtn->Left + SortBtn->Width) < GrpBox1_1->Width);
	UpItemBtn->Visible	= ((UpItemBtn->Left + UpItemBtn->Width) < GrpBox1_1->Width);
	DowItemBtn->Visible = ((DowItemBtn->Left + DowItemBtn->Width) < GrpBox1_1->Width);
}

//---------------------------------------------------------------------------
//�ݒ���̍X�V
// chgf = true �Ȃ� FilNamEdit->Text �Ƀf�t�H���g��ݒ�
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::UpdateInf(bool chgf)
{
	//���d�Ăяo����}�~
	if (InhUpdateInf) return;
	InhUpdateInf = true;

	TListBox *lp = HtmFileList;
	int HtmLstCnt = lp->Count;
	bool only_one = (DestMode!=DSTMOD_CLIPBD && (HtmLstCnt==1 || JoinCheck->Checked));

	if (chgf) {
		//FilNamEdit->Text�� �f�t�H���g��ݒ�
		UnicodeString fnam = EmptyStr;
		do {
			if (DestMode==DSTMOD_CLIPBD) break;
			if (HtmLstCnt==0) break;

			if (only_one)
				fnam = lp->Items->Strings[0];
			else {
				if (lp->ItemIndex==-1) break;
				fnam = lp->Items->Strings[lp->ItemIndex];
			}
			fnam = UrlToName(fnam);

			if (Tit2NamCheck->Checked) {
				if (isHtml(fnam) && FileExists(fnam)) {
					FilNamEdit->Text = TitToName(HC->GetTitle(fnam)) + ".txt";
					FilNamEdit->Font->Color = clWindowText;
				}
				else {
					FilNamEdit->Text = "(�ϊ����Ƀ^�C�g������擾).txt";
					FilNamEdit->Font->Color = clRed;
				}
			}
			else {
				FilNamEdit->Text = ChangeFileExt(ExtractFileName(fnam), ".txt");
				FilNamEdit->Font->Color = clWindowText;
			}
			FilNamEdit->Modified = false;
		} while (0);
	}

	//URL���܂܂�Ă��邩?
	bool inc_url = false;
	for (int i=0; i<HtmLstCnt; i++) {
		UnicodeString htm = lp->Items->Strings[i];
		if (StartsText("http://", htm) || StartsText("https://", htm)) {
			inc_url = true; break;
		}
	}
	//�S�ē����f�B���N�g����?
	bool same_dir = true;
	OrgDir = EmptyStr;
	for (int i=0; i<lp->Count && same_dir; i++) {
		if (i==0)
			OrgDir = ExtractFileDir(lp->Items->Strings[i]);
		else if (OrgDir!=ExtractFileDir(lp->Items->Strings[i]))
			same_dir = false;
	}

	SamDirRadioBtn->Enabled = !inc_url;

	if (inc_url && (DestMode==DSTMOD_ORGDIR))						  DestMode = DSTMOD_SELDIR;
	if (JoinCheck->Checked && !same_dir && (DestMode==DSTMOD_ORGDIR)) DestMode = DSTMOD_SELDIR;

	ClipRadioBtn->Checked	= (DestMode==DSTMOD_CLIPBD);
	SamDirRadioBtn->Checked = (DestMode==DSTMOD_ORGDIR);
	SetDirRadioBtn->Checked = (DestMode==DSTMOD_SELDIR);

	DstDirEdit->Enabled 	= (DestMode==DSTMOD_SELDIR);
	RefDstBtn->Enabled		= (DestMode==DSTMOD_SELDIR);
	FilNamEdit->Enabled 	= only_one;
	OpenAppCheck->Enabled	= only_one;

	//�ϊ��J�n�\��?
	ConvReady = false;
	do {
		if (HtmLstCnt==0) {
			StatusBar1->SimpleText = "����HTML�������w�肵�Ă�������";
			break;
		}
		if (DestMode!=DSTMOD_CLIPBD && FilNamEdit->Text.IsEmpty()) {
			StatusBar1->SimpleText = "�o�̓t�@�C�������w�肵�Ă�������";
			break;
		}
		if (DestMode==DSTMOD_SELDIR && DstDirEdit->Text.IsEmpty()) {
			StatusBar1->SimpleText = "�o�͏ꏊ���w�肵�Ă�������";
			break;
		}
		StatusBar1->SimpleText
			= UnicodeString().sprintf(_T("%u�̓���HTML�������w�肳��Ă��܂�"), HtmLstCnt);
		ConvReady = true;
	} while (0);

	InhUpdateInf = false;
}

//---------------------------------------------------------------------------
//�R���p�N�g�^�ʏ�\���؂�ւ�
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::CompactBtnClick(TObject *Sender)
{
	int w, h;
	UnicodeString sct = "General";
	if (Compact) {
		Compact = false;
		IniFile->WriteInteger(   sct, "WinWidthC", 	Width);
		IniFile->WriteInteger(   sct, "WinHeightC",	Height);
		w = IniFile->ReadInteger(sct, "WinWidth",	NRM_MIN_WD);
		h = IniFile->ReadInteger(sct, "WinHeight",	NRM_MIN_HI);
	}
	else {
		Compact = true;
		IniFile->WriteInteger(   sct, "WinWidth", 	Width);
		IniFile->WriteInteger(   sct, "WinHeight",	Height);
		w = IniFile->ReadInteger(sct, "WinWidthC",	CMP_MIN_WD);
		h = IniFile->ReadInteger(sct, "WinHeightC",	CMP_MIN_HI);
	}

	TRect rc = BoundsRect;
	switch (CmpPosCombo->ItemIndex) {
	case 1:	//�E��
		rc.Left   = rc.Right - w;
		rc.Bottom = rc.Top + h;
		break;
	case 2:	//�E��
		rc.Left   = rc.Right - w;
		rc.Top	  = rc.Bottom - h;
		break;
	case 3:	//����
		rc.Right  = rc.Left + w;
		rc.Top	  = rc.Bottom - h;
		break;
	default:	//�E��
		rc.Right  = rc.Left + w;
		rc.Bottom = rc.Top + h;
	}

	Constraints->MinWidth  = 0;
	Constraints->MinHeight = 0;
	BoundsRect = rc;

	if (Compact) {
		Constraints->MinWidth  = CMP_MIN_WD;
		Constraints->MinHeight = CMP_MIN_HI;
	}
	else {
		Constraints->MinWidth  = NRM_MIN_WD;
		Constraints->MinHeight = NRM_MIN_HI;
	}
	SetFrmStyle();
}

//---------------------------------------------------------------------------
//�����ύX
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::LnWidEditChange(TObject *Sender)
{
	LnWidEdit->Font->Color = check_TUpDown(LnWidUpDown)? clWindowText : clRed;
}
//---------------------------------------------------------------------------
//��s�����ύX
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::BlkLmtEditaChange(TObject *Sender)
{
	BlkLmtEdit->Font->Color = check_TUpDown(BlkLmtUpDown)? clWindowText : clRed;
}
//---------------------------------------------------------------------------
//��������
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::TitLmtEditChange(TObject *Sender)
{
	TitLmtEdit->Font->Color = check_TUpDown(TitLmtUpDown)? clWindowText : clRed;
}
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::NumberEditExit(TObject *Sender)
{
	TEdit *ep = (TEdit *)Sender;
	if (ep->Font->Color == clRed) {
		UnicodeString msg;  msg.sprintf(_T("���͂������l[%s]�͔͈͂𒴂��Ă��܂�"), ep->Text.c_str());
		Application->MessageBox(msg.c_str(), _T("�x��"), MB_OK + MB_ICONWARNING);
		ep->SetFocus();
	}
}

//---------------------------------------------------------------------------
//�A�v���P�[�V�����̑I��
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::RefAppBtnClick(TObject *Sender)
{
	OpenDialog1->Title		= "�A�v���P�[�V�����̑I��";
	OpenDialog1->Filter 	= "�A�v���P�[�V���� (*.exe)|*.EXE";
	OpenDialog1->DefaultExt = "EXE";
	OpenDialog1->FileName	= "*.EXE";
	OpenDialog1->InitialDir = EmptyStr;
	if (OpenDialog1->Execute()) AppNameEdit->Text = OpenDialog1->FileName;
}
//---------------------------------------------------------------------------
//�o�͏ꏊ�̎w��
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::RefDstBtnClick(TObject *Sender)
{
	UnicodeString dnam = DstDirEdit->Text;
	if (SelectDirectory("�o�͏ꏊ�̎w��", EmptyStr, dnam,
		TSelectDirExtOpts() << sdNewUI << sdShowShares, Screen->ActiveForm))
	{
		DstDirEdit->Text = dnam;
	}
}

//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::AltCheckClick(TObject *Sender)
{
	DefAltEdit->Enabled = AltCheck->Checked;
	AltBraEdit->Enabled = (AltCheck->Checked || ImgSrcCheck->Checked);
	AltKetEdit->Enabled = (AltCheck->Checked || ImgSrcCheck->Checked);
}
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::LinkCheckClick(TObject *Sender)
{
	ExtLnkCheck->Enabled = LinkCheck->Checked;
	LnkBraEdit->Enabled  = LinkCheck->Checked;
	LnkKetEdit->Enabled  = LinkCheck->Checked;
	LinkCrCheck->Enabled = LinkCheck->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::DestRadioBtnClick(TObject *Sender)
{
	DestMode = ((TComponent *)Sender)->Tag;
	UpdateInf(true);
}
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::JoinCheckClick(TObject *Sender)
{
	UpdateInf(true);
}

//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::HtmFileListClick(TObject *Sender)
{
	UpdateInf(true);
}
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::FilNamEditChange(TObject *Sender)
{
	FilNamEdit->Font->Color = clWindowText;
	UpdateInf(false);
}

//---------------------------------------------------------------------------
//�u�����X�g�̕`��
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::RepCheckListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
	TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;

	cv->Brush->Color = State.Contains(odSelected)? clHighlight : clWindow;
	cv->Font->Color  = State.Contains(odSelected)? clHighlightText : clWindowText;
	cv->FillRect(Rect);

	int w_max = 0;
	for (int i=0; i<lp->Count; i++)
		w_max = std::max(w_max, cv->TextWidth(get_tkn(lp->Items->Strings[i], "\t")));

	int xp = Rect.Left + 4;
	int yp = Rect.Top + 2;
	UnicodeString lbuf = lp->Items->Strings[Index];
	cv->TextOut(xp, yp, "\"" + get_tkn(lbuf, "\t") + "\"");
	xp += (w_max + cv->TextWidth("\"\" "));
	cv->TextOut(xp, yp, " �� ");
	xp += cv->TextWidth(" ��  ");
	cv->TextOut(xp, yp, "\"" + get_tkn_r(lbuf, "\t") + "\"");
}
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::RepCheckListBoxClick(TObject *Sender)
{
	TCheckListBox *lp = RepCheckListBox;
	if (lp->ItemIndex!=-1) {
		UnicodeString lbuf = lp->Items->Strings[lp->ItemIndex];
		FromStrEdit->Text  = get_tkn(lbuf, "\t");
		ToStrEdit->Text    = get_tkn_r(lbuf, "\t");
	}
}
//---------------------------------------------------------------------------
//�u���ݒ�̒ǉ�
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::AddRepActionExecute(TObject *Sender)
{
	TCheckListBox *lp = RepCheckListBox;
	int idx = lp->Items->Add(FromStrEdit->Text + "\t" + ToStrEdit->Text);
	lp->Checked[idx] = true;
}
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::AddRepActionUpdate(TObject *Sender)
{
	UnicodeString f_str = FromStrEdit->Text;
	bool regex_ng = (is_regex_slash(f_str) && !chk_RegExPtn(exclude_top_end(f_str)));
	FromStrEdit->Font->Color = regex_ng? clRed : clWindowText;
	((TAction *)Sender)->Enabled = !regex_ng;
}
//---------------------------------------------------------------------------
//�u���ݒ�̕ύX
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::ChgRepActionExecute(TObject *Sender)
{
	TCheckListBox *lp = RepCheckListBox;
	if (lp->ItemIndex!=-1)
		lp->Items->Strings[lp->ItemIndex] = FromStrEdit->Text + "\t" + ToStrEdit->Text;
}
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::ChgRepActionUpdate(TObject *Sender)
{
	UnicodeString f_str = FromStrEdit->Text;
	((TAction *)Sender)->Enabled
		= (RepCheckListBox->ItemIndex!=-1)
			&& (!is_regex_slash(f_str) || chk_RegExPtn(exclude_top_end(f_str)));
}
//---------------------------------------------------------------------------
//�u���ݒ�̍폜
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::DelRepActionExecute(TObject *Sender)
{
	TCheckListBox *lp = RepCheckListBox;
	int idx = lp->ItemIndex;
	if (idx!=-1) {
		lp->Items->Delete(idx);
		lp->ItemIndex = (idx<lp->Count)? idx : lp->Count - 1;
	}
}
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::DelRepActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = (RepCheckListBox->ItemIndex!=-1);
}

//---------------------------------------------------------------------------
//�u�����ڂ�����
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::UpRepActionExecute(TObject *Sender)
{
	TCheckListBox *lp = RepCheckListBox;
	int idx = lp->ItemIndex;
	if (idx>=1 && idx<lp->Count) {
		lp->Items->Move(idx, idx - 1);
		lp->ItemIndex = idx - 1;
	}
}
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::UpRepActionUpdate(TObject *Sender)
{
	TCheckListBox *lp = RepCheckListBox;
	((TAction*)Sender)->Enabled  = (lp->ItemIndex>=1 && lp->ItemIndex<lp->Count);
}
//---------------------------------------------------------------------------
//�u�����ڂ������
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::DownRepActionExecute(TObject *Sender)
{
	TCheckListBox *lp = RepCheckListBox;
	int idx = lp->ItemIndex;
	if (idx>=0 && idx<lp->Count-1) {
		lp->Items->Move(idx, idx + 1);
		lp->ItemIndex = idx + 1;
	}
}
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::DownRepActionUpdate(TObject *Sender)
{
	TCheckListBox *lp = RepCheckListBox;
	((TAction*)Sender)->Enabled = (lp->ItemIndex>=0 && lp->ItemIndex<lp->Count-1);
}

//---------------------------------------------------------------------------
//����������̎Q��
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::RefMac1BtnClick(TObject *Sender)
{
	PopTag = ((TComponent *)Sender)->Tag;
	POINT p;
	::GetCursorPos(&p);
	PopupMenu1->Popup(p.x, p.y);
}
//---------------------------------------------------------------------------
//����������̑}��
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::PopRefItemClick(TObject *Sender)
{
	UnicodeString mstr = ((TMenuItem*)Sender)->Caption;
	int p = mstr.Pos(" ");
	if (p>0) mstr = mstr.SubString(1, p - 1);
	if (PopTag==0)
		HeadMemo->SetSelTextBuf(mstr.c_str());
	else
		FootMemo->SetSelTextBuf(mstr.c_str());
}

//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::Tit2NamCheckClick(TObject *Sender)
{
	TitLmtEdit->Enabled		= Tit2NamCheck->Checked;
	TitLmtUpDown->Enabled   = Tit2NamCheck->Checked;
	TitCvExChCheck->Enabled = Tit2NamCheck->Checked;
	TitCvSpcCheck->Enabled	= Tit2NamCheck->Checked;
}

//---------------------------------------------------------------------------
//�I�����̎w��
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::RefSndBtnClick(TObject *Sender)
{
	OpenDialog1->Title		= "�I�����̎w��";
	OpenDialog1->Filter 	= "WAV�t�@�C�� (*.wav)|*.WAV";
	OpenDialog1->DefaultExt = "WAV";
	OpenDialog1->FileName	= "*.WAV";
	OpenDialog1->InitialDir = ExtractFileDir(EndSoundEdit->Text);
	if (OpenDialog1->Execute()) {
		EndSoundEdit->Text = OpenDialog1->FileName;
		::sndPlaySound(EndSoundEdit->Text.c_str(), SND_ASYNC);
	}
}
//---------------------------------------------------------------------------
//�I�����̃e�X�g�Đ�
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::PlaySoundActionExecute(TObject *Sender)
{
	::sndPlaySound(EndSoundEdit->Text.c_str(), SND_ASYNC);
}
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::PlaySoundActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = !EndSoundEdit->Text.IsEmpty();
}

//---------------------------------------------------------------------------
//��Ɏ�O�ɕ\��
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::TopMostCheckClick(TObject *Sender)
{
	::EndDeferWindowPos(::DeferWindowPos(::BeginDeferWindowPos(1), Handle,
		(TopMostCheck->Checked? HWND_TOPMOST : HWND_NOTOPMOST), 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE));
}

//---------------------------------------------------------------------------
//Web�T�C�g���J��
//---------------------------------------------------------------------
void __fastcall TH2TconvForm::UrlLabelClick(TObject *Sender)
{
	::ShellExecute(NULL, _T("open"), SUPPORT_URL, NULL, NULL, SW_SHOWNORMAL);
}

//---------------------------------------------------------------------------
//�ϊ��J�n
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::ConvertExecute(TObject *Sender)
{
	Converting = true;
	UpdateActions();

	Screen->Cursor = crHourGlass;	//�����v

	HC->TempDir 	 = ExtractFileDir(Application->ExeName);
	HC->LineWidth	 = LnWidEdit->Text.ToIntDef(80);
	HC->ForceCr 	 = FrcCrCheck->Checked;
	HC->BlankLnLimit = BlkLmtEdit->Text.ToIntDef(5);
	HC->NoPstHdr	 = PstHdrCheck->Checked;
	HC->HrLineStr	 = HrStrEdit->Text;
	HC->HeaderStr[0] = esc_to_str(H1Edit->Text);
	HC->HeaderStr[1] = esc_to_str(H2Edit->Text);
	HC->HeaderStr[2] = esc_to_str(H3Edit->Text);
	HC->HeaderStr[3] = esc_to_str(H4Edit->Text);
	HC->HeaderStr[4] = esc_to_str(H5Edit->Text);
	HC->HeaderStr[5] = esc_to_str(H6Edit->Text);
	HC->B_BraStr	 = esc_to_str(B_BraEdit->Text);
	HC->B_KetStr	 = esc_to_str(B_KetEdit->Text);
	HC->I_BraStr	 = esc_to_str(I_BraEdit->Text);
	HC->I_KetStr	 = esc_to_str(I_KetEdit->Text);
	HC->U_BraStr	 = esc_to_str(U_BraEdit->Text);
	HC->U_KetStr	 = esc_to_str(U_KetEdit->Text);
	HC->IndentStr	 = esc_to_str(IndentEdit->Text);
	HC->UlMarkStr	 = esc_to_str(MarkEdit->Text);
	HC->ZenNumber	 = ZenNoCheck->Checked;
	HC->CellSepStr	 = esc_to_str(CelSepEdit->Text);
	HC->UseAlt		 = AltCheck->Checked;
	HC->AddImgSrc	 = ImgSrcCheck->Checked;
	HC->DefAltStr	 = esc_to_str(DefAltEdit->Text);
	HC->AltBraStr	 = esc_to_str(AltBraEdit->Text);
	HC->AltKetStr	 = esc_to_str(AltKetEdit->Text);
	HC->InsLink 	 = LinkCheck->Checked;
	HC->OnlyExLink	 = ExtLnkCheck->Checked;
	HC->AddCrLink	 = LinkCrCheck->Checked;
	HC->LnkBraStr	 = esc_to_str(LnkBraEdit->Text);
	HC->LnkKetStr	 = esc_to_str(LnkKetEdit->Text);
	HC->InsLineStr	 = InsLineStrEdit->Text;
	HC->InsHrClass	 = InsHrClsEdit->Text;
	HC->InsHrSct	 = InsHrSctCheckBox->Checked;
	HC->InsHrArt	 = InsHrArtCheckBox->Checked;
	HC->InsHrNav	 = InsHrNavCheckBox->Checked;
	HC->DelBlkCls	 = DelBlkClsEdit->Text;
	HC->DelBlkId	 = DelBlkIdEdit->Text;

	if (InsHdCheck->Checked) HC->HeadStr = HeadMemo->Lines->Text;
	if (InsFtCheck->Checked) HC->FootStr = FootMemo->Lines->Text;

	//�o�͐�f�B���N�g��
	UnicodeString dstdir = (DestMode==DSTMOD_SELDIR)? DstDirEdit->Text : OrgDir;
	if (!dstdir.IsEmpty()) dstdir = IncludeTrailingPathDelimiter(dstdir);

	switch (CodePageComboBox->ItemIndex) {
	case 1:  HC->OutCodePage = 50220;	break;	//JIS
	case 2:  HC->OutCodePage = 20932;	break;	//EUC
	case 3:  HC->OutCodePage = 65001;	break;	//UTF-8
	case 4:  HC->OutCodePage = 1200;	break;	//UTF-16
	default: HC->OutCodePage = 932;				//ShiftJIS
	}

	try {
		//�u�����X�g�̒��o
		std::unique_ptr<TStringList> rep_lst(new TStringList());
		for (int i=0; i<RepCheckListBox->Count; i++) {
			if (RepCheckListBox->Checked[i]) {
				UnicodeString lbuf = RepCheckListBox->Items->Strings[i];
				UnicodeString f_s = get_tkn(lbuf, "\t");
				if (is_regex_slash(f_s) && !chk_RegExPtn(exclude_top_end(f_s))) continue;
				rep_lst->Add(lbuf);
			}
		}

		TListBox *lp = HtmFileList;
		for (int i=0; i<lp->Count; i++) lp->Selected[i] = false;
		HC->TxtBuf->Clear();

		for (int i=0; i<lp->Count; i++) {
			Application->ProcessMessages();
			if (Active && HIBYTE(::GetAsyncKeyState(VK_ESCAPE))!=0) {
				ClearKeyBuff(true);
				throw EAbort("�ϊ��𒆒f���܂���");
			}

			UnicodeString htmfil = lp->Items->Strings[i];
			lp->Selected[i] = true;
			lp->Repaint();
			StatusBar1->SimpleText = "�ϊ���... " + htmfil;
			if (!JoinCheck->Checked) HC->TxtBuf->Clear();
			if (HC->LoadFile(htmfil)) {;
				//�ϊ�/�u��
				HC->Convert(rep_lst.get());

				if (DestMode!=DSTMOD_CLIPBD) {
					//�o�̓t�@�C������������ꍇ�͂��̓s�x�ۑ�
					if (lp->Count>1 && !JoinCheck->Checked) {
						UnicodeString fnam = Tit2NamCheck->Checked? TitToName(HC->TitleStr) : UrlToName(htmfil);

						if (DestMode==DSTMOD_SELDIR) 
							fnam = dstdir + ExtractFileName(fnam);
						else if (DestMode==DSTMOD_ORGDIR)
							fnam = ExtractFilePath(htmfil) + ExtractFileName(fnam);

						fnam = ChangeFileExt(fnam, ".txt");
						if (AddNoCheck->Checked) fnam = AddSerNo(fnam);
						if (!HC->SaveToFileCP(fnam)) throw EAbort("�o�̓t�@�C���̕ۑ��Ɏ��s���܂���\r\n" + fnam);
					}
					//�o�̓t�@�C������̏ꍇ
					else if (i==0 && (lp->Count==1 || JoinCheck->Checked)) {
						if (Tit2NamCheck->Checked && !FilNamEdit->Modified) {
							FilNamEdit->Text = TitToName(HC->TitleStr) + ".txt";
						}
					}
				}
				lp->Selected[i] = false;
			}
			else
				throw EAbort("����HTML�������擾�ł��܂���\r\n" + htmfil);
		}

		//�ϊ���̏���
		StatusBar1->SimpleText = "�ϊ�����!";
		if (!EndSoundEdit->Text.IsEmpty()) ::sndPlaySound(EndSoundEdit->Text.c_str(), SND_ASYNC);

		//�N���b�v�{�[�h�ɃR�s�[
		if (DestMode==DSTMOD_CLIPBD) {
			Clipboard()->AsText = HC->TxtBuf->Text;
		}
		//�o�̓t�@�C������̏ꍇ
		else if (lp->Count==1 || JoinCheck->Checked) {
			UnicodeString fnam = dstdir + FilNamEdit->Text;
			if (AddNoCheck->Checked) fnam = AddSerNo(fnam);
			if (!HC->SaveToFileCP(fnam)) throw EAbort("�o�̓t�@�C���̕ۑ��Ɏ��s���܂���\r\n" + fnam);
			if (OpenAppCheck->Checked && !AppNameEdit->Text.IsEmpty()) {
				fnam = "\""+ fnam + "\"";
				int res = (int)::ShellExecute(NULL, _T("open"),
					AppNameEdit->Text.c_str(), fnam.c_str(), NULL, SW_SHOWNORMAL);
				if (res<=32) throw EAbort("�o�̓t�@�C�����J���܂���");
			}
		}

		//���t�@�C���̍폜
		do {
			if (!KillCheck->Checked) break;
			int i = 0;
			while (i<lp->Count) {
				UnicodeString htm = lp->Items->Strings[i];
				if (StartsText("http://", htm) || StartsText("https://", htm))
					lp->Items->Delete(i);
				else
					i++;
			}
			if (lp->Count==0) break;
			if (SureKillCheck->Checked) {
				if (Application->MessageBox(_T("HTML�������폜���܂���"), _T("�x��"),
					MB_OKCANCEL + MB_ICONWARNING)!=IDOK)
						break;
			}
			//���ݔ���
			if (UseTrashCheck->Checked) {
				SHFILEOPSTRUCT lpFileOp;
				std::unique_ptr<_TCHAR []> lpszFil(new _TCHAR[lp->Items->Text.Length()+4]);
				_TCHAR *p = lpszFil.get();
				for (int i=0; i<lp->Count; i++) {
					_tcscpy(p, lp->Items->Strings[i].c_str());
					p += (_tcslen(p) + 1);
				}
				*p ='\0';
				lpFileOp.hwnd				   = Handle;
				lpFileOp.wFunc				   = FO_DELETE;
				lpFileOp.pFrom				   = lpszFil.get();
				lpFileOp.pTo				   = NULL;
				lpFileOp.fFlags 			   = FOF_ALLOWUNDO | FOF_NOCONFIRMATION;
				lpFileOp.fAnyOperationsAborted = true;
				lpFileOp.hNameMappings		   = NULL;
				lpFileOp.lpszProgressTitle	   = NULL;
				::SHFileOperation(&lpFileOp);
			}
			//�폜
			else {
				for (int i=0; i<lp->Count; i++) {
					UnicodeString fnam = lp->Items->Strings[i];
					if (!DeleteFile(fnam))
						throw EAbort("���t�@�C�����폜�ł��܂���\r\n" + fnam);
				}
			}

			lp->Clear();
			StatusBar1->SimpleText = "HTML�������폜���܂���!";
		} while (0);

		::Sleep(500);
	}
	catch(EAbort &e) {
		if (e.Message.Pos("���f"))
			Application->MessageBox(e.Message.c_str(), _T("�m�F"), MB_OK);
		else
			Application->MessageBox(e.Message.c_str(), _T("�x��"), MB_OK|MB_ICONEXCLAMATION);
	}

	Screen->Cursor = crDefault;
	ClearKeyBuff();
	Converting = false;
	UpdateInf(false);

	PageControl1->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::ConvertActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = ConvReady && !Converting;
	PageControl1->Enabled = !Converting;

	TListBox *lp = HtmFileList;
	DelBtn->Enabled 	= (lp->SelCount>0);
	ClrBtn->Enabled 	= (lp->Count>0);
	SortBtn->Enabled	= (lp->Count>1);
	UpItemBtn->Enabled	= (lp->SelCount==1) && (lp->Count>1);
	DowItemBtn->Enabled = (lp->SelCount==1) && (lp->Count>1);
}

//---------------------------------------------------------------------------
//���j���[
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::MenuBtnClick(TObject *Sender)
{
	TPoint p = MenuBtn->ClientToScreen(Point(MenuBtn->Left + 8, MenuBtn->Top + 8));
	ToolMenu->Popup(p.x, p.y);
}
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::AbautMeasureItem(TObject *Sender,
	TCanvas *ACanvas, int &Width, int &Height)
{
	//���𒲐�
	if (((TComponent *)Sender)->Tag>1000)
		Width = std::max(Width, 44 + ACanvas->TextWidth(VersionStr));
	else
		Width += 8;
	//�����𒲐�
	Height += 2;
}

//---------------------------------------------------------------------------
//�ݒ�t�@�C�����J��
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::LoadIniItemClick(TObject *Sender)
{
	OpenDialog1->Title		= "�ݒ�t�@�C�����J��";
	OpenDialog1->Filter 	= "�ݒ�t�@�C�� (*.ini)|*.INI";
	OpenDialog1->FileName	= "*.ini";
	OpenDialog1->InitialDir = LastIniDir;
	if (OpenDialog1->Execute()) {
		LoadIniFile(OpenDialog1->FileName);
		LastIniDir = ExtractFileDir(OpenDialog1->FileName);
	}
}
//---------------------------------------------------------------------------
//�ݒ�ɖ��O��t���ĕۑ�
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::SaveIniItemClick(TObject *Sender)
{
	SaveDialog1->Title		= "�ݒ�t�@�C���ɖ��O��t���ĕۑ�";
	SaveDialog1->Filter 	= "�ݒ�t�@�C�� (*.ini)|*.INI";
	SaveDialog1->DefaultExt = "INI";
	SaveDialog1->FileName	= "*.ini";
	SaveDialog1->InitialDir	= LastIniDir;
	if (SaveDialog1->Execute()) {
		SaveIniFile(SaveDialog1->FileName);
		LastIniDir = ExtractFileDir(SaveDialog1->FileName);
	}
}
//---------------------------------------------------------------------------
//�ݒ���f�t�H���g�ɖ߂�
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::DefaultIniItemClick(TObject *Sender)
{
	if (FileExists(IniFile->FileName)) DeleteFile(IniFile->FileName); 
	LoadIniFile();
}
//---------------------------------------------------------------------------
//�w���v
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::HelpItemClick(TObject *Sender)
{
	Application->HelpShowTableOfContents();
}

//---------------------------------------------------------------------------
//�o�[�W��������`��
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::AbautInfItemDrawItem(TObject *Sender,
	TCanvas *ACanvas, TRect &ARect, bool Selected)
{
	TMenuItem *tp =(TMenuItem*)Sender;
	ACanvas->Brush->Color = clBlack;
	ACanvas->FillRect(ARect);
	RECT rc = RECT(ARect);
	TFontStyles fstyle;

	switch (tp->Tag) {
	case 1001:	//�㔼��
		ACanvas->Draw(ARect.Left + 4, ARect.Bottom - 16, Application->Icon);
		fstyle.Clear(); fstyle << fsBold;
		ACanvas->Font->Style = fstyle;
		ACanvas->Font->Color = clWhite;
		ACanvas->TextOut(ARect.Left + 44, ARect.Top + 2, Application->Title);
		DrawEdge(ACanvas->Handle, &rc, BDR_SUNKENOUTER, BF_TOP|BF_LEFT|BF_RIGHT);
		break;
	case 1002:	//������
		ACanvas->Draw(ARect.Left + 4, ARect.Top - 16, Application->Icon);
		ACanvas->Font->Color = clWhite;
		ACanvas->TextOut(ARect.Left + 48, ARect.Top + 2, VersionStr);
		DrawEdge(ACanvas->Handle, &rc, BDR_SUNKENOUTER, BF_BOTTOM|BF_LEFT|BF_RIGHT);
		break;
	}
}

//---------------------------------------------------------------------------
//Web�y�[�W���J��
//---------------------------------------------------------------------------
void __fastcall TH2TconvForm::GoWebItemClick(TObject *Sender)
{
	::ShellExecute(NULL, _T("open"), SUPPORT_URL, NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
