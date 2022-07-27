//----------------------------------------------------------------------//
//	îƒópã§í ä÷êî														//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrFuncH
#define UsrFuncH

//---------------------------------------------------------------------------
#include <System.StrUtils.hpp>
#include <Vcl.ComCtrls.hpp>

UnicodeString get_tkn(UnicodeString s, UnicodeString sp);
UnicodeString get_tkn_r(UnicodeString s, UnicodeString sp);
UnicodeString split_tkn(UnicodeString &s, UnicodeString sp);
int  pos_i(UnicodeString wd, UnicodeString s);
int  pos_r(UnicodeString wd, UnicodeString s);
bool contains_word(UnicodeString lst, UnicodeString wd);
UnicodeString to_full_str(UnicodeString s);
UnicodeString int_to_alpha(int n);
UnicodeString int_to_roman(int n);
UnicodeString esc_to_str(UnicodeString src);
UnicodeString str_to_esc(UnicodeString src);
UnicodeString trim_ex(UnicodeString src);
UnicodeString exclude_top_end(UnicodeString s);
UnicodeString del_quot(UnicodeString s);
UnicodeString replace_str_by_list(UnicodeString s, TStringList *lst);
int  str_len_half(UnicodeString s);

void get_all_files(UnicodeString pnam, UnicodeString mask, TStrings *lst, bool subsw);

UnicodeString get_local_name(UnicodeString s);
bool isHtml(UnicodeString fnam);
bool get_file_name_r(UnicodeString &fnam);

bool check_TUpDown(TUpDown *udp);

bool is_regex_slash(UnicodeString s);
bool chk_RegExPtn(UnicodeString ptn);

void ClearKeyBuff(bool key_only = false);
UnicodeString get_ShiftStr(TShiftState Shift);
UnicodeString get_KeyStr(WORD Key);
UnicodeString get_KeyStr(WORD Key, TShiftState Shift);

void draw_ListItemLine(TCustomListBox *lp, int idx);

UnicodeString get_VersionStr(UnicodeString fnam);

//---------------------------------------------------------------------------
#endif
