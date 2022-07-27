//----------------------------------------------------------------------//
// ファイル履歴	(UsrIniFile を使用)										//
//																		//
//----------------------------------------------------------------------//
#ifndef USE_COMMON_PCH
#include <vcl.h>
#include <System.StrUtils.hpp>
#pragma hdrstop
#endif

#include "file_hist.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
FileHistory::FileHistory(TMenuItem *menu, TNotifyEvent on_click)
{
	HistMenu = menu;
	HistList = new TStringList();

	if (HistMenu) {
		for (int i=0; i<MAX_FILEHISTORY; i++) {
			TMenuItem *mp = new TMenuItem(HistMenu);
			mp->OnClick   = on_click;
			mp->Tag 	  = i;
			HistMenu->Add(mp);
		}
	}

	ShowDir = true;
	ShowRel = false;
}
//---------------------------------------------------------------------------
FileHistory::~FileHistory()
{
	delete HistList;
}

//---------------------------------------------------------------------------
//履歴の読込
//---------------------------------------------------------------------------
void FileHistory::LoadHistory(UsrIniFile *ini_file, UnicodeString sct)
{
	if (sct.IsEmpty()) sct = "FileHistory";

	for (int i=0; i<MAX_FILEHISTORY; i++) {
		UnicodeString histstr = ini_file->ReadString(sct, "File" + IntToStr(i), EmptyStr);
		if (histstr.IsEmpty()) break;
		if (FileExists(histstr)) HistList->Add(histstr);
	}
}
//---------------------------------------------------------------------------
//履歴を保存
//---------------------------------------------------------------------------
void FileHistory::SaveHistory(UsrIniFile *ini_file, UnicodeString sct)
{
	if (sct.IsEmpty()) sct = "FileHistory";

	for (int i=0; i<MAX_FILEHISTORY; i++) {
		UnicodeString histstr = EmptyStr;
		if (i<HistList->Count) histstr = HistList->Strings[i];
		ini_file->WriteString(sct, "File" + IntToStr(i), histstr);
	}
}

//---------------------------------------------------------------------------
//メニュー項目の更新
//---------------------------------------------------------------------------
void FileHistory::UpdateMenu()
{
	if (!HistMenu) return;

	UnicodeString exe_path = ReplaceStr(ExtractFilePath(Application->ExeName), "\\.\\", "\\");
	TStringList *lp = HistList;
	for (int i=0; i<HistMenu->Count; i++) {
		TMenuItem *mp = HistMenu->Items[i];
		mp->Visible = false;
		if (i<lp->Count) {
			if (!lp->Strings[i].IsEmpty()) {
				UnicodeString fnam = lp->Strings[i];
				if (ShowRel && StartsText(exe_path, fnam)) fnam.Delete(1, exe_path.Length());
				if (!ShowDir) fnam = ExtractFileName(fnam);
				mp->Caption = ((i<10)? ("&" + IntToStr((i + 1)%10) + ": ") : UnicodeString("   ")) + fnam;
				mp->Visible = true;
			}
		}
	}

	HistMenu->Enabled = (lp->Count>0);
}
//---------------------------------------------------------------------------
//履歴に追加
//---------------------------------------------------------------------------
void FileHistory::AddHistory(UnicodeString fnam)
{
	int idx = -1;
	for (int i=0; i<HistList->Count; i++) {
		if (SameText(HistList->Strings[i], fnam)) {
			idx	= i; break;
		}
	}
	if (idx!=-1) HistList->Delete(idx);	//前の履歴を消して
	HistList->Insert(0, fnam);			//先頭に追加
}
//---------------------------------------------------------------------------
//指定インデックスのファイル名を取得
//---------------------------------------------------------------------------
UnicodeString FileHistory::GetHistory(int idx)
{
	UnicodeString fnam = EmptyStr;
	TStringList *lp = HistList;
	if (idx < lp->Count) fnam = lp->Strings[idx];
	return fnam;
}
//---------------------------------------------------------------------------
