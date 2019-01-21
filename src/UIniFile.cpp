//----------------------------------------------------------------------//
// INIファイル処理クラス												//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include "UIniFile.h"

//---------------------------------------------------------------------------
UsrIniFile *IniFile = NULL;

//---------------------------------------------------------------------------
UsrIniFile::UsrIniFile(UnicodeString fnam)
{
	FileName = fnam;
	SectionList = new TStringList();
	LoadValues();
}
//---------------------------------------------------------------------------
UsrIniFile::~UsrIniFile()
{
	Clear();
	delete SectionList;
}

//---------------------------------------------------------------------------
TStringList * UsrIniFile::AddSection(UnicodeString sct)
{
	TStringList *slst = new TStringList();
	SectionList->AddObject(sct, (TObject*)slst);
	return slst;
}
//---------------------------------------------------------------------------
void UsrIniFile::Clear()
{
	for (int i=0; i<SectionList->Count; i++) {
		TStringList *klist = (TStringList*)SectionList->Objects[i];
		delete klist;
	}
	SectionList->Clear();
}

//---------------------------------------------------------------------------
// 全情報を一括で読み込む
// ※コメントなどもそのまま取得
//---------------------------------------------------------------------------
void UsrIniFile::LoadValues()
{
	Clear();

	try {
		std::unique_ptr<TStringList> fbuf(new TStringList());
		fbuf->LoadFromFile(FileName);

		TStringList *klist = AddSection(EmptyStr);	//※最初のセクションより前の内容(コメントなど)用
		for (int i=0; i<fbuf->Count; i++) {
			UnicodeString s = fbuf->Strings[i];
			if (s.IsEmpty()) continue;
			int l = s.Length();
			if (l>2 && s[1]=='[' && s[l]==']')
				klist = AddSection(s.SubString(2, l - 2));
			else if (s[1]==';' || s.Pos("=")>0)
				klist->Add(s);
		}
		Modified = false;
	}
	catch (...) {
		Clear();
	}
}

//---------------------------------------------------------------------------
//セクションを削除
//---------------------------------------------------------------------------
void UsrIniFile::EraseSection(UnicodeString sct)
{
	int idx = SectionList->IndexOf(sct);
	if (idx>=0) {
		TStringList *klist = (TStringList*)(SectionList->Objects[idx]);
		delete klist;
		SectionList->Delete(idx);
	}
}

//---------------------------------------------------------------------------
//キーを削除
//---------------------------------------------------------------------------
void UsrIniFile::DeleteKey(UnicodeString sct, UnicodeString key)
{
	int idx = SectionList->IndexOf(sct);
	if (idx>=0) {
		TStringList *klist = (TStringList*)(SectionList->Objects[idx]);
		idx = klist->IndexOfName(key);
		if (idx>=0) klist->Delete(idx);
	}
}

//---------------------------------------------------------------------------
//セクションを読み込む
//---------------------------------------------------------------------------
void UsrIniFile::ReadSection(UnicodeString sct, TStringList *lst)
{
	if (!lst) return;
	int idx = SectionList->IndexOf(sct);
	if (idx>=0) lst->Assign((TStringList*)SectionList->Objects[idx]);
}
//---------------------------------------------------------------------------
//セクションへ複写
//---------------------------------------------------------------------------
void UsrIniFile::AssignSection(UnicodeString sct, TStringList *lst)
{
	if (!lst) return;
	TStringList *klist;
	int idx = SectionList->IndexOf(sct);
	if (idx>=0)
		klist = (TStringList*)SectionList->Objects[idx];
	else
		klist = AddSection(sct);
	klist->Assign(lst);
	Modified = true;
}

//---------------------------------------------------------------------------
// デフォルト(省略可) def = EmptyStr
// ダブルクォーテーションで囲まれていたら外す
//---------------------------------------------------------------------------
UnicodeString UsrIniFile::ReadString(UnicodeString sct, UnicodeString key, UnicodeString def)
{
	UnicodeString ret;
	int idx = SectionList->IndexOf(sct);
	if (idx>=0) {
		TStringList * klist = (TStringList*)(SectionList->Objects[idx]);
		ret = klist->Values[key];
		int l = ret.Length();
		if (l>1) {
			if ((ret[1]=='\"') && (ret[l]=='\"')) ret = ret.SubString(2, l - 2);
		}
	}
	if (ret.IsEmpty()) ret = def;
	return ret;
}
//---------------------------------------------------------------------------
// デフォルト(省略可) def = 0
//---------------------------------------------------------------------------
int UsrIniFile::ReadInteger(UnicodeString sct, UnicodeString key, int def)
{
	return StrToIntDef(ReadString(sct, key, EmptyStr), def);
}
//---------------------------------------------------------------------------
// デフォルト(省略可) def = false
//---------------------------------------------------------------------------
bool UsrIniFile::ReadBool(UnicodeString sct, UnicodeString key, bool def)
{
	return (ReadInteger(sct, key, (int)def) != 0);
}
//---------------------------------------------------------------------------
// デフォルト(省略可) def = clBlack
//---------------------------------------------------------------------------
TColor UsrIniFile::ReadColor(UnicodeString sct, UnicodeString key, TColor def)
{
	return (TColor)StrToIntDef(ReadString(sct, key, EmptyStr), (int)def);
}

//---------------------------------------------------------------------------
//フォントを作成し、情報を読み込んで設定
// デフォルト(省略可) def = NULL
// 作成されたフォントは、最後に破棄すること
//---------------------------------------------------------------------------
TFont* UsrIniFile::ReadFontInf(UnicodeString sct, TFont *def)
{
	TFont *f = new TFont();
	f->Charset = (TFontCharset)ReadInteger(sct, "FontCharset", (int)SHIFTJIS_CHARSET);
	f->Name    = ReadString( sct, "FontName", (def ? def->Name : EmptyStr));
	f->Size    = ReadInteger(sct, "FontSize", (def ? def->Size : 11));
	TFontStyles fstyle = TFontStyles();
	if (ReadBool(sct, "FontBold", (def ? def->Style.Contains(fsBold) : false)))
		fstyle << fsBold;
	if (ReadBool(sct, "FontItalic", (def ? def->Style.Contains(fsItalic) : false)))
		fstyle << fsItalic;
	f->Style = fstyle;
	return f;
}

//---------------------------------------------------------------------------
void UsrIniFile::WriteString(UnicodeString sct, UnicodeString key, UnicodeString v)
{
	TStringList *klist;
	int idx = SectionList->IndexOf(sct);
	if (idx>=0)
		klist = (TStringList*)(SectionList->Objects[idx]);
	else
		klist = AddSection(sct);
	UnicodeString s = key + '=' + v;
	idx = klist->IndexOfName(key);
	if (idx>=0) {
		if (CompareStr(klist->Strings[idx], s)!=0) {
			klist->Strings[idx] = s;
			Modified = true;
		}
	}
	else {
		klist->Add(s);
		Modified = true;
	}
}
//---------------------------------------------------------------------------
void UsrIniFile::WriteInteger(UnicodeString sct, UnicodeString key, int v)
{
	WriteString(sct, key, IntToStr(v));
}
//---------------------------------------------------------------------------
void UsrIniFile::WriteBool(UnicodeString sct, UnicodeString key, bool v)
{
	WriteString(sct, key, v? "1" : "0");
}

//---------------------------------------------------------------------------
void UsrIniFile::WriteFontInf(UnicodeString sct, TFont *f)
{
	WriteString( sct, "FontName",	f->Name);
	WriteInteger(sct, "FontSize",	f->Size);
	WriteBool(   sct, "FontBold",	f->Style.Contains(fsBold));
	WriteBool(   sct, "FontItalic",	f->Style.Contains(fsItalic));
}

//---------------------------------------------------------------------------
//メインフォームの位置・サイズを復元
//---------------------------------------------------------------------------
void UsrIniFile::LoadFormPos(TForm *frm, int w, int h)
{
	UnicodeString sct = "General";
	if (w>0) frm->Width  = ReadInteger(sct, "WinWidth",	w);
	if (h>0) frm->Height = ReadInteger(sct, "WinHeight",	h);
	frm->Left = ReadInteger(sct, "WinLeft",	(Screen->Width - frm->Width)/2);
	frm->Top  = ReadInteger(sct, "WinTop",	(Screen->Height - frm->Height)/2);
	if (Screen->MonitorCount==1) {
		//1画面の場合、画面外に出ないように
		if (frm->Left<0) frm->Left = 0;
		if (frm->Top<0)  frm->Top  = 0;
		if (frm->BoundsRect.Right>Screen->Width)   frm->Left = Screen->Width - frm->Width;
		if (frm->BoundsRect.Bottom>Screen->Height) frm->Top  = Screen->Height - frm->Height;
	}
	frm->WindowState = (TWindowState)ReadInteger(sct, "WinState", (int)wsNormal);
}
//---------------------------------------------------------------------------
//メインフォームの位置・サイズを保存
//---------------------------------------------------------------------------
void UsrIniFile::SaveFormPos(TForm *frm)
{
	UnicodeString sct = "General";
	if (frm->WindowState==wsMinimized) frm->WindowState = wsNormal;
	WriteInteger(sct, "WinState",	(int)frm->WindowState);
	if (frm->WindowState==wsMaximized) frm->WindowState = wsNormal;
	WriteInteger(sct, "WinLeft",	frm->Left);
	WriteInteger(sct, "WinTop",	frm->Top);
	WriteInteger(sct, "WinWidth",	frm->Width);
	WriteInteger(sct, "WinHeight",frm->Height);
}

//---------------------------------------------------------------------------
//位置情報を読込
//---------------------------------------------------------------------------
void UsrIniFile::LoadPosInfo(TForm *frm, int x, int y, int w, int h)
{
	UnicodeString sct = "General";
	frm->Left = ReadInteger(sct, frm->Name + "Left", x);
	frm->Top  = ReadInteger(sct, frm->Name + "Top",  y);

	if ((frm->BorderStyle!=bsDialog) && (w>0) && (h>0)) {
		int ww = ReadInteger(sct, frm->Name + "Width",  w);
		if (ww<=0) ww = w;
		int wh = ReadInteger(sct, frm->Name + "Height", h);
		if (wh<=0) wh = h;
		frm->Width	= ww;
		frm->Height = wh;
	}

	if (Screen->MonitorCount==1) {
		//1画面の場合、画面外は除外
		if (frm->Left<0) frm->Left = 0;
		if (frm->Top<0)  frm->Top  = 0;
		if (frm->BoundsRect.Right>Screen->Width)   frm->Left = Screen->Width - frm->Width;
		if (frm->BoundsRect.Bottom>Screen->Height) frm->Top  = Screen->Height - frm->Height;
	}
}
//---------------------------------------------------------------------------
//位置情報保存
//---------------------------------------------------------------------------
void UsrIniFile::SavePosInfo(TForm *frm)
{
	UnicodeString sct = "General";
	WriteInteger(sct, frm->Name + "Left", frm->Left);
	WriteInteger(sct, frm->Name + "Top",  frm->Top);

	if (frm->BorderStyle!=bsDialog) {
		WriteInteger(sct, frm->Name + "Width",  frm->Width);
		WriteInteger(sct, frm->Name + "Height", frm->Height);
	}
}

//---------------------------------------------------------------------------
// ComboBox の項目を読込
//---------------------------------------------------------------------------
void UsrIniFile::LoadComboBoxItems(TComboBox *cp, UnicodeString sct, int max_items)
{
	cp->Clear();
	if (sct.IsEmpty()) sct = cp->Name;
	for (int i=0; i<max_items; i++) {
		UnicodeString itm_str = ReadString(sct, "Item" + IntToStr(i + 1), EmptyStr);
		if (itm_str.IsEmpty()) break;
		if (cp->Items->IndexOf(itm_str)==-1) cp->Items->Add(itm_str);
	}
}
//---------------------------------------------------------------------------
// ComboBox の項目を保存
//---------------------------------------------------------------------------
void UsrIniFile::SaveComboBoxItems(TComboBox *cp, UnicodeString sct, int max_items)
{
	if (sct.IsEmpty()) sct = cp->Name;
	for (int i=0; i<max_items; i++) {
		UnicodeString itm_str;
		if (i<cp->Items->Count) itm_str = cp->Items->Strings[i];
		WriteString(sct, "Item" + IntToStr(i + 1), itm_str);
	}
}

//---------------------------------------------------------------------------
// INIファイルの更新 (Write～で変更されていたら保存)
//---------------------------------------------------------------------------
void UsrIniFile::UpdateFile()
{
	if (Modified) {
		try {
			std::unique_ptr<TStringList> fbuf(new TStringList());
			for (int i=0; i<SectionList->Count; i++) {
				UnicodeString sct = SectionList->Strings[i];
				if (!sct.IsEmpty()) fbuf->Add("[" + sct + "]");
				TStringList *klist = (TStringList*)SectionList->Objects[i];
				for (int j=0; j<klist->Count; j++) fbuf->Add(klist->Strings[j]);
				if (fbuf->Count>0 && !fbuf->Strings[fbuf->Count - 1].IsEmpty()) fbuf->Add(EmptyStr);
			}
			fbuf->SaveToFile(FileName, TEncoding::UTF8);
			Modified = false;
		}
		catch (...) {
			return;
		}
	}
}
//---------------------------------------------------------------------------
