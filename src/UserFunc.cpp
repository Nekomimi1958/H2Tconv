//----------------------------------------------------------------------//
// 汎用共通関数															//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include <algorithm>
#include <mbstring.h>
#include <RegularExpressions.hpp>
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
// | 区切りリストに指定語が含まれているか？
//---------------------------------------------------------------------------
bool contains_word(UnicodeString lst, UnicodeString wd)
{
	if (!StartsStr("|", lst)) lst.Insert("|" ,1);
	if (!EndsStr("|", lst))   lst.UCAT_TSTR("|");
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
		case 4: ret_str.UCAT_TSTR("cd"); break;
		case 5: ret_str.UCAT_TSTR("d");  break;
		case 6: case 7: case 8:
			ret_str.cat_sprintf(_T("d%s"), StringOfChar(_T('c'), rn - 5).c_str()); break;
		case 9: ret_str.UCAT_TSTR("cm"); break;
		}
	}

	//10の位
	rn = (n/10)%10;
	if (rn>0) {
		switch (rn) {
		case 1: case 2: case 3:
			ret_str += StringOfChar(_T('x'), rn); break;
		case 4: ret_str.UCAT_TSTR("xl"); break;
		case 5: ret_str.UCAT_TSTR("l");  break;
		case 6: case 7: case 8:
			ret_str.cat_sprintf(_T("l%s"), StringOfChar(_T('x'), rn - 5).c_str()); break;
		case 9: ret_str.UCAT_TSTR("xc"); break;
		}
	}

	//1の位
	rn = n%10;
	switch (rn) {
	case 1: case 2: case 3:
		ret_str += StringOfChar(_T('i'), rn); break;
	case 4: ret_str.UCAT_TSTR("iv"); break;
	case 5: ret_str.UCAT_TSTR("v");  break;
	case 6: case 7: case 8:
		ret_str.cat_sprintf(_T("v%s"), StringOfChar(_T('i'), rn - 5).c_str()); break;
	case 9: ret_str.UCAT_TSTR("ix"); break;
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
