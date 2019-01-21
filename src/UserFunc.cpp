//----------------------------------------------------------------------//
// �ėp���ʊ֐�															//
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
// �w��Z�p���[�^�̑O�̕�������擾
// ���Z�p���[�^��������Ȃ��ꍇ�́A����������̂܂ܕԂ�
//---------------------------------------------------------------------------
UnicodeString get_tkn(UnicodeString s, UnicodeString sp)
{
	int p = s.Pos(sp);
	return (p==0)? s : s.SubString(1, p - 1);
}
//---------------------------------------------------------------------------
// �w��Z�p���[�^�̌�̕�������擾
// ���Z�p���[�^��������Ȃ��ꍇ�́AEmptyStr ��Ԃ�
//---------------------------------------------------------------------------
UnicodeString get_tkn_r(UnicodeString s, UnicodeString sp)
{
	int p = s.Pos(sp);
	return (p==0)? EmptyStr : s.SubString(p + sp.Length(), s.Length() - (p - sp.Length() + 1));
}

//---------------------------------------------------------------------------
// �w��Z�p���[�^�O�̕�����𕪗����Ď擾
// ���Z�p���[�^��������Ȃ��ꍇ�́A����������̂܂ܕԂ��A��������͋��
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
//�召��������ʂ����ɕ���������� (2�o�C�g�����Ή�)
//  wd: ������
//   s: �Ώە�����
//---------------------------------------------------------------------------
int pos_i(UnicodeString wd, UnicodeString s)
{
	if (wd.IsEmpty()) return 0;
	return s.UpperCase().Pos(wd.UpperCase());
}

//---------------------------------------------------------------------------
// | ��؂胊�X�g�Ɏw��ꂪ�܂܂�Ă��邩�H
//---------------------------------------------------------------------------
bool contains_word(UnicodeString lst, UnicodeString wd)
{
	if (!StartsStr("|", lst)) lst.Insert("|" ,1);
	if (!EndsStr("|", lst))   lst.UCAT_TSTR("|");
	return ContainsText(lst, "|" + wd + "|");
}

//---------------------------------------------------------------------------
//�S�p�ɕϊ�
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
//������ a�`z �ɂ��\�L�ɕϊ�
// a�`z, aa�`az, ba�`bz...
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
//����(1�`999)�����[�}����(������)�ɕϊ�
//---------------------------------------------------------------------------
UnicodeString int_to_roman(int n)
{
	UnicodeString ret_str;
	//100�̈�
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

	//10�̈�
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

	//1�̈�
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
//�G�X�P�[�v�����ɖ߂�
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
//���ꕶ�����G�X�P�[�v�ɕϊ�
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
//�S�p�X�y�[�X���l�������g���~���O
//---------------------------------------------------------------------------
UnicodeString trim_ex(UnicodeString src)
{
	for (;;) {
		if (src.IsEmpty()) break;
		if (src.Pos("�@")==1) {
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
			if (src.SubString(n - 1, 1)=="�@") {
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
//�擪�Ɩ��������O������������擾
//---------------------------------------------------------------------------
UnicodeString exclude_top_end(UnicodeString s)
{
	return s.SubString(2, s.Length() - 2);
}

//---------------------------------------------------------------------------
// " �݂̈͂��O��
//---------------------------------------------------------------------------
UnicodeString del_quot(UnicodeString s)
{
	if (s.Length()>=2 && StartsStr("\"", s) && EndsStr("\"", s)) s = exclude_top_end(s);
	return s;
}

//---------------------------------------------------------------------------
// ���X�g�ɂ��������ĕ������u��
// ���X�g����: ���������� TAB �u��������
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
//���p���Z�̕����񒷂��擾
//---------------------------------------------------------------------------
int str_len_half(UnicodeString s)
{
	AnsiString    s_a = s;
	UnicodeString s_u = s_a;
	if (s!=s_u && s.Length()==s_u.Length()) {
		for (int i=1; i<=s.Length(); i++)
			if (s[i]!=s_u[i] && s_u[i]=='?') s_u[i] = _T('�H');
		s_a = s_u;
	}
	return s_a.Length();
}

//---------------------------------------------------------------------------
//�f�B���N�g�����̃}�X�N�ɊY������S�t�@�C�����擾
//  pnam: �f�B���N�g����
//  mask: �}�X�N
//   lst: ���ʂ��i�[���� TStrings
// subsw: �T�u�f�B���N�g��������	(default = false);
//  subn: �T�u�f�B���N�g���̐[��	(default = 99);
//---------------------------------------------------------------------------
void get_all_files(UnicodeString pnam, UnicodeString mask, TStrings *lst, bool subsw)
{
	if (pnam.IsEmpty()) return;
	if (!DirectoryExists(pnam)) return;
	pnam = IncludeTrailingPathDelimiter(pnam);
	if (mask.IsEmpty()) mask = "*.*";

	TSearchRec sr;
	UnicodeString tmpstr;
	//�T�u�f�B���N�g��������
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
	//�t�@�C��������
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
//�t�@�C����HTML�������H
//---------------------------------------------------------------------------
bool isHtml(UnicodeString fnam)
{
	if (StartsText("file:///", fnam)) fnam = get_local_name(fnam);
	return contains_word(".htm|.html|.sht|.shtm|.shtml|.php", ExtractFileExt(fnam).LowerCase());
}

//---------------------------------------------------------------------------
//fnam �Ƀh���C�u�����Ȃ����
//���s�t�@�C���̃f�B���N�g������̑��΃t�@�C�����ɕς���
//�t�@�C�������݂��Ȃ���� false ��Ԃ�
//---------------------------------------------------------------------------
bool get_file_name_r(UnicodeString &fnam)
{
	if (fnam.IsEmpty()) return false;
	if (ExtractFileDrive(fnam).IsEmpty()) fnam = ExtractFilePath(Application->ExeName) + fnam;
	return FileExists(fnam);
}

//---------------------------------------------------------------------------
// TUpDown �Ɋ֘A�Â����Ă���ҏW���̒l���`�F�b�N
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
// /�`/ (���K�\��) ?
//---------------------------------------------------------------------------
bool is_regex_slash(UnicodeString s)
{
	return (s.Length()>=2 && StartsStr('/', s) && EndsStr('/', s));
}

//---------------------------------------------------------------------------
//���K�\���p�^�[���̃`�F�b�N
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
//�L�[�E�}�E�X�o�b�t�@���N���A
//---------------------------------------------------------------------------
void ClearKeyBuff(
	bool key_only)	//true = �L�[�o�b�t�@�̂݃N���A (default = false)
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
//�A�v���P�[�V�����̐��i�o�[�W�����擾
//�� n.n.n.n �� n.nn �ɕϊ�
//---------------------------------------------------------------------------
UnicodeString get_VersionStr(UnicodeString fnam)
{
	UnicodeString verstr;
	unsigned mj, mi, bl;
	if (GetProductVersion(fnam, mj, mi, bl)) verstr.sprintf(_T("%u.%u%u"), mj, mi, bl);
	return verstr;
}
//---------------------------------------------------------------------------
