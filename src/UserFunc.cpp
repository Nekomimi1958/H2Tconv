//----------------------------------------------------------------------//
// 汎用共通関数															//
//																		//
//----------------------------------------------------------------------//
#include "UserFunc.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// 指定セパレータの前の文字列を取得
// ※セパレータが見つからない場合は、文字列をそのまま返す
//---------------------------------------------------------------------------
UnicodeString get_tkn(UnicodeString s, UnicodeString sp)
{
	int p = s.Pos(sp);
	return (p==0)? s : s.SubString(1, p - 1);
}
//---------------------------------------------------------------------------
// 指定セパレータの後の文字列を取得
// ※セパレータが見つからない場合は、EmptyStr を返す
//---------------------------------------------------------------------------
UnicodeString get_tkn_r(UnicodeString s, UnicodeString sp)
{
	int p = s.Pos(sp);
	return (p==0)? EmptyStr : s.SubString(p + sp.Length(), s.Length() - (p - sp.Length() + 1));
}

//---------------------------------------------------------------------------
// 指定セパレータ前の文字列を分離して取得
// ※セパレータが見つからない場合は、文字列をそのまま返し、元文字列は空に
//---------------------------------------------------------------------------
UnicodeString split_tkn(UnicodeString &s, UnicodeString sp)
{
	UnicodeString r;
	int p = s.Pos(sp);
	if (p==0) {
		r = s;
		s = EmptyStr;
	}
	else {
		r = s.SubString(1, p - 1);
		s = s.SubString(p + sp.Length(), s.Length() - (p - sp.Length() + 1));
	}
	return r;
}

//---------------------------------------------------------------------------
//大小文字を区別せずに文字列を検索 (2バイト文字対応)
//  wd: 検索語
//   s: 対象文字列
//---------------------------------------------------------------------------
int pos_i(UnicodeString wd, UnicodeString s)
{
	if (wd.IsEmpty()) return 0;
	return s.UpperCase().Pos(wd.UpperCase());
}

//---------------------------------------------------------------------------
//文字列が最後に現れる位置を取得
//---------------------------------------------------------------------------
int pos_r(
	UnicodeString wd,	//検索語
	UnicodeString s)	//対象文字列
{
	if (wd.IsEmpty()) return 0;

	int p  = 0;
	int p0 = 1;
	for (;;) {
		int p1 = PosEx(wd, s, p0);
		if (p1==0) break;
		p  = p1;
		p0 = p + 1;
	}

	return p;
}

//---------------------------------------------------------------------------
// | 区切りリストに指定語が含まれているか？
//---------------------------------------------------------------------------
bool contains_word(UnicodeString lst, UnicodeString wd)
{
	if (!StartsStr("|", lst)) lst.Insert("|" ,1);
	if (!EndsStr("|", lst))   lst += "|";
	return ContainsText(lst, "|" + wd + "|");
}

//---------------------------------------------------------------------------
//全角に変換
//---------------------------------------------------------------------------
UnicodeString to_full_str(UnicodeString s)
{
	const int size_s = s.Length() + 1;
	int size_d = ::LCMapString(::GetUserDefaultLCID(), LCMAP_FULLWIDTH, s.c_str(), size_s, NULL, 0);
	std::unique_ptr<wchar_t> sbuf(new wchar_t[size_d]);
	::ZeroMemory(sbuf.get(), size_d * sizeof(wchar_t));
	::LCMapString(::GetUserDefaultLCID(), LCMAP_FULLWIDTH, s.c_str(), size_s, sbuf.get(), size_d);
	UnicodeString ret_str = sbuf.get();
	return ret_str;
}

//---------------------------------------------------------------------------
//整数を a～z による表記に変換
// a～z, aa～az, ba～bz...
//---------------------------------------------------------------------------
UnicodeString int_to_alpha(int n)
{
	UnicodeString ret_str;
	n--;
	if (n<0)
		ret_str = "0";
	else {
		int n1 = n /26;
		if (n1>0) ret_str.cat_sprintf(_T("%c"), (char)('a' + n1 - 1));
		ret_str.cat_sprintf(_T("%c"), (char)('a' + (n % 26)));
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//整数(1～999)をローマ数字(小文字)に変換
//---------------------------------------------------------------------------
UnicodeString int_to_roman(int n)
{
	UnicodeString ret_str;
	//100の位
	int rn = (n/100)%10;
	if (rn>0) {
		switch (rn) {
		case 1: case 2: case 3:
			ret_str += StringOfChar(_T('c'), rn); break;
		case 4: ret_str += "cd"; break;
		case 5: ret_str += "d";  break;
		case 6: case 7: case 8:
			ret_str.cat_sprintf(_T("d%s"), StringOfChar(_T('c'), rn - 5).c_str()); break;
		case 9: ret_str += "cm"; break;
		}
	}

	//10の位
	rn = (n/10)%10;
	if (rn>0) {
		switch (rn) {
		case 1: case 2: case 3:
			ret_str += StringOfChar(_T('x'), rn); break;
		case 4: ret_str += "xl"; break;
		case 5: ret_str += "l";  break;
		case 6: case 7: case 8:
			ret_str.cat_sprintf(_T("l%s"), StringOfChar(_T('x'), rn - 5).c_str()); break;
		case 9: ret_str += "xc"; break;
		}
	}

	//1の位
	rn = n%10;
	switch (rn) {
	case 1: case 2: case 3:
		ret_str += StringOfChar(_T('i'), rn); break;
	case 4: ret_str += "iv"; break;
	case 5: ret_str += "v";  break;
	case 6: case 7: case 8:
		ret_str.cat_sprintf(_T("v%s"), StringOfChar(_T('i'), rn - 5).c_str()); break;
	case 9: ret_str += "ix"; break;
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//エスケープを元に戻す
//---------------------------------------------------------------------------
UnicodeString esc_to_str(UnicodeString src)
{
	src = ReplaceStr(src, "\\s",	" ");
	src = ReplaceStr(src, "\\t",	"\t");
	src = ReplaceStr(src, "\\n",	"\n");
	src = ReplaceStr(src, "\\r",	"\r");
	src = ReplaceStr(src, "\\\"",	"\"");
	return src;
}
//---------------------------------------------------------------------------
//特殊文字をエスケープに変換
//---------------------------------------------------------------------------
UnicodeString str_to_esc(UnicodeString src)
{
	src = ReplaceStr(src, " ",	"\\s");
	src = ReplaceStr(src, "\t",	"\\t");
	src = ReplaceStr(src, "\n",	"\\n");
	src = ReplaceStr(src, "\r",	"\\r");
	src = ReplaceStr(src, "\"",	"\\\"");
	return src;
}

//---------------------------------------------------------------------------
//全角スペースも考慮したトリミング
//---------------------------------------------------------------------------
UnicodeString trim_ex(UnicodeString src)
{
	for (;;) {
		if (src.IsEmpty()) break;
		if (src.Pos("　")==1) {
			src.Delete(1, 1); continue;
		}
		if (StartsStr(" ", src) || StartsStr("\t", src)) {
			src.Delete(1, 1); continue;
		}
		break;
	}
	for (;;) {
		if (src.IsEmpty()) break;
		int n = src.Length();
		if (n>1) {
			if (src.SubString(n - 1, 1)=="　") {
				src.Delete(n - 1, 1); continue;
			}
		}
		if (src[n]==' ' || src[n]=='\t') {
			src.Delete(n, 1); continue;
		}
		break;
	}
	return src;
}

//---------------------------------------------------------------------------
//先頭と末尾を除外した文字列を取得
//---------------------------------------------------------------------------
UnicodeString exclude_top_end(UnicodeString s)
{
	return s.SubString(2, s.Length() - 2);
}

//---------------------------------------------------------------------------
// " の囲みを外す
//---------------------------------------------------------------------------
UnicodeString del_quot(UnicodeString s)
{
	if (s.Length()>=2 && StartsStr("\"", s) && EndsStr("\"", s)) s = exclude_top_end(s);
	return s;
}

//---------------------------------------------------------------------------
// リストにしたがって文字列を置換
// リスト書式: 検索文字列 TAB 置換文字列
//---------------------------------------------------------------------------
UnicodeString replace_str_by_list(UnicodeString s, TStringList *lst)
{
	for (int i=0; i<lst->Count; i++) {
		UnicodeString t_s = lst->Strings[i];
		UnicodeString f_s = split_tkn(t_s, "\t");
		s = ReplaceStr(s, f_s, t_s);
	}
	return s;
}

//---------------------------------------------------------------------------
//半角換算の文字列長を取得
//---------------------------------------------------------------------------
int str_len_half(UnicodeString s)
{
	AnsiString    s_a = s;
	UnicodeString s_u = s_a;
	if (s!=s_u && s.Length()==s_u.Length()) {
		for (int i=1; i<=s.Length(); i++)
			if (s[i]!=s_u[i] && s_u[i]=='?') s_u[i] = _T('？');
		s_a = s_u;
	}
	return s_a.Length();
}

//---------------------------------------------------------------------------
//ディレクトリ下のマスクに該当する全ファイルを取得
//  pnam: ディレクトリ名
//  mask: マスク
//   lst: 結果を格納する TStrings
// subsw: サブディレクトリも検索	(default = false);
//  subn: サブディレクトリの深さ	(default = 99);
//---------------------------------------------------------------------------
void get_all_files(UnicodeString pnam, UnicodeString mask, TStrings *lst, bool subsw)
{
	if (pnam.IsEmpty()) return;
	if (!DirectoryExists(pnam)) return;
	pnam = IncludeTrailingPathDelimiter(pnam);
	if (mask.IsEmpty()) mask = "*.*";

	TSearchRec sr;
	UnicodeString tmpstr;
	//サブディレクトリを検索
	if (subsw) {
		if (FindFirst(pnam+"*", faDirectory, sr) == 0) {
			do {
				if ((sr.Attr & faDirectory)==0) continue;
				if (sr.Name==".." || sr.Name==".") continue;
				get_all_files(pnam + sr.Name, mask, lst, subsw);
			} while (FindNext(sr) == 0);
			FindClose(sr);
		}
	}
	//ファイルを検索
	if (FindFirst(pnam + mask, faArchive|faNormal|faReadOnly, sr) == 0) {
		do {
			lst->Add(pnam + sr.Name);
		} while (FindNext(sr) == 0);
		FindClose(sr);
	}
}

//---------------------------------------------------------------------------
UnicodeString get_local_name(UnicodeString s)
{
	UnicodeString fnam = s;
	if (StartsText("file:///", fnam)) {
		fnam.Delete(1, 8);
		fnam = ReplaceStr(fnam, "/", "\\");
		int p = fnam.Pos("#");
		if (p>0) fnam = fnam.SubString(1, p - 1);
	}
	else
		fnam = EmptyStr;
	return fnam;
}

//---------------------------------------------------------------------------
//ファイルはHTML文書か？
//---------------------------------------------------------------------------
bool isHtml(UnicodeString fnam)
{
	if (StartsText("file:///", fnam)) fnam = get_local_name(fnam);
	return contains_word(".htm|.html|.sht|.shtm|.shtml|.php", ExtractFileExt(fnam).LowerCase());
}

//---------------------------------------------------------------------------
//fnam にドライブ名がなければ
//実行ファイルのディレクトリからの相対ファイル名に変える
//ファイルが存在しなければ false を返す
//---------------------------------------------------------------------------
bool get_file_name_r(UnicodeString &fnam)
{
	if (fnam.IsEmpty()) return false;
	if (ExtractFileDrive(fnam).IsEmpty()) fnam = ExtractFilePath(Application->ExeName) + fnam;
	return FileExists(fnam);
}

//---------------------------------------------------------------------------
// TUpDown に関連づけられている編集欄の値をチェック
//---------------------------------------------------------------------------
bool check_TUpDown(TUpDown *udp)
{
	bool ret = true;
	if (udp->Associate) {
		TCustomEdit *ep = dynamic_cast<TCustomEdit *>(udp->Associate);
		if (ep) {
			int n = ep->Text.ToIntDef(udp->Min - 1);
			if (n<udp->Min || n>udp->Max) ret = false;
		}
	}
	return ret;
}

//---------------------------------------------------------------------------
// /～/ (正規表現) ?
//---------------------------------------------------------------------------
bool is_regex_slash(UnicodeString s)
{
	return (s.Length()>=2 && StartsStr('/', s) && EndsStr('/', s));
}

//---------------------------------------------------------------------------
//正規表現パターンのチェック
//---------------------------------------------------------------------------
bool chk_RegExPtn(UnicodeString ptn)
{
	if (ptn.IsEmpty()) return false;

	try {
		TRegEx x(ptn, TRegExOptions() << roCompiled);
		return true;
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//キー・マウスバッファをクリア
//---------------------------------------------------------------------------
void ClearKeyBuff(
	bool key_only)	//true = キーバッファのみクリア (default = false)
{
	MSG msg;
	if (key_only) {
		do {
			Sleep(0);
		} while (::PeekMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE));
	}
	else {
		do {
			Sleep(50);
		} while (
			::PeekMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE) ||
			::PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE)
		);
	}
}

//---------------------------------------------------------------------------
//シフト状態を文字列に
//---------------------------------------------------------------------------
UnicodeString get_ShiftStr(TShiftState Shift)
{
	UnicodeString sftstr;
	if (Shift.Contains(ssShift)) sftstr += "Shift+";
	if (Shift.Contains(ssCtrl))  sftstr += "Ctrl+";
	if (Shift.Contains(ssAlt))   sftstr += "Alt+";
	return  sftstr;
}

//---------------------------------------------------------------------------
//キーを文字列に変換
//---------------------------------------------------------------------------
UnicodeString get_KeyStr(WORD Key)
{
	UnicodeString keystr;

	switch (Key) {
	case VK_RETURN: 	keystr = "ENTER";	break;
	case VK_ESCAPE: 	keystr = "ESC";		break;
	case VK_PAUSE:  	keystr = "PAUSE";	break;
	case VK_SPACE:  	keystr = "SPACE";	break;
	case VK_TAB:		keystr = "TAB";		break;
	case VK_LEFT:   	keystr = "LEFT";	break;
	case VK_RIGHT:  	keystr = "RIGHT";	break;
	case VK_DOWN:		keystr = "DOWN";	break;
	case VK_UP:			keystr = "UP";		break;
	case VK_PRIOR:		keystr = "PGUP";	break;
	case VK_NEXT:		keystr = "PGDN";	break;
	case VK_HOME:		keystr = "HOME";	break;
	case VK_END:		keystr = "END";		break;
	case VK_INSERT:		keystr = "INS";		break;
	case VK_DELETE: 	keystr = "DEL";		break;
	case VK_BACK: 		keystr = "BKSP";	break;
	case VK_F1:			keystr = "F1";		break;
	case VK_F2:			keystr = "F2";		break;
	case VK_F3:			keystr = "F3";		break;
	case VK_F4:			keystr = "F4";		break;
	case VK_F5:			keystr = "F5";		break;
	case VK_F6:			keystr = "F6";		break;
	case VK_F7:			keystr = "F7";		break;
	case VK_F8:			keystr = "F8";		break;
	case VK_F9:			keystr = "F9";		break;
	case VK_F10:		keystr = "F10";		break;
	case VK_F11:		keystr = "F11";		break;
	case VK_F12:		keystr = "F12";		break;
	case VK_APPS:		keystr = "APP";		break;
	case VK_OEM_1:		keystr = ":"; 		break;
	case VK_OEM_2:		keystr = "/";		break;
	case VK_OEM_3:		keystr = "@";		break;
	case VK_OEM_4:		keystr = "[";		break;
	case VK_OEM_5:		keystr = "\\";		break;
	case VK_OEM_6:		keystr = "]";		break;
	case VK_OEM_MINUS:  keystr = "-";		break;
	case VK_OEM_PLUS:   keystr = ";";		 break;
	case VK_OEM_COMMA:  keystr = ",";		break;
	case VK_OEM_PERIOD: keystr = ".";		break;
	case VK_OEM_102:    keystr = "＼";		break;

	case VK_NUMPAD0:	keystr = "10Key_0";	break;
	case VK_NUMPAD1:	keystr = "10Key_1";	break;
	case VK_NUMPAD2:	keystr = "10Key_2";	break;
	case VK_NUMPAD3:	keystr = "10Key_3";	break;
	case VK_NUMPAD4:	keystr = "10Key_4";	break;
	case VK_NUMPAD5:	keystr = "10Key_5";	break;
	case VK_NUMPAD6:	keystr = "10Key_6";	break;
	case VK_NUMPAD7:	keystr = "10Key_7";	break;
	case VK_NUMPAD8:	keystr = "10Key_8";	break;
	case VK_NUMPAD9:	keystr = "10Key_9";	break;
	case VK_MULTIPLY:	keystr = "10Key_*";	break;
	case VK_ADD:		keystr = "10Key_+";	break;
	case VK_SUBTRACT:	keystr = "10Key_-";	break;
	case VK_DIVIDE:		keystr = "10Key_/";	break;
	case VK_DECIMAL:	keystr = "10Key_.";	break;
	case VK_OEM_7:	  	keystr = "^";		break;

	default:
		if (_istalnum(Key)) keystr = (char)Key; else keystr = EmptyStr;
	}

	return  keystr;
}
//---------------------------------------------------------------------------
//キーとシフト状態を文字列に
//---------------------------------------------------------------------------
UnicodeString get_KeyStr(WORD Key, TShiftState Shift)
{
	UnicodeString keystr = get_KeyStr(Key);
	return !keystr.IsEmpty()? get_ShiftStr(Shift) + keystr : EmptyStr;
}

//---------------------------------------------------------------------------
//リストボックスの項目に罫線を描画 (背景の反転色)
//---------------------------------------------------------------------------
void draw_ListItemLine(TCustomListBox *lp, int idx)
{
	TPenMode org_md = lp->Canvas->Pen->Mode;
	int		 org_wd = lp->Canvas->Pen->Width;
	lp->Canvas->Pen->Mode  = pmNot;
	lp->Canvas->Pen->Width = 2;
	if (idx!=-1) {
		TRect r = lp->ItemRect(idx);
		int   y = (lp->ItemIndex<idx)? r.Bottom : r.Top;
		lp->Canvas->MoveTo(0, y);
		lp->Canvas->LineTo(lp->Width, y);
	}
	lp->Canvas->Pen->Mode  = org_md;
	lp->Canvas->Pen->Width = org_wd;
}

//---------------------------------------------------------------------------
//アプリケーションの製品バージョン取得
//※ n.n.n.n → n.nn に変換
//---------------------------------------------------------------------------
UnicodeString get_VersionStr(UnicodeString fnam)
{
	UnicodeString verstr;
	unsigned mj, mi, bl;
	if (GetProductVersion(fnam, mj, mi, bl)) verstr.sprintf(_T("%u.%u%u"), mj, mi, bl);
	return verstr;
}
//---------------------------------------------------------------------------
