//----------------------------------------------------------------------//
// HTML→テキスト変換処理クラス											//
//																		//
//----------------------------------------------------------------------//
#include <wininet.h>
#pragma link "win32\\release\\psdk\\wininet.lib"

#include "UserFunc.h"
#include "htmconv.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//UTF8チェック
//---------------------------------------------------------------------------
int check_UTF8(BYTE *bp, int size)
{
	int cnt = 0;
	try {
		int i = 0;
		while (i<size) {
			if (bp[i]<0x80) {
				i++;
			}
			else {
				int bn;
				if		(0xc2<=bp[i] && bp[i]<=0xdf) bn = 1;
				else if (0xe0<=bp[i] && bp[i]<=0xef) bn = 2;
				else if (0xf0<=bp[i] && bp[i]<=0xf4) bn = 3;
				else Abort();
				i++;	if (i==size) break;
				for (int j=0; j<bn && i<size; j++) {
					if (0x80<=bp[i] && bp[i]<=0xbf) {
						i++;	if (i==size) break;
					}
					else Abort();
				}
				cnt++;
			}
		}
	}
	catch (...) {
		cnt = 0;
	}
	return cnt;
}

//---------------------------------------------------------------------------
//メモリーのコードページを取得
// 戻り値: コードページ (0:不明)
//---------------------------------------------------------------------------
int get_MemoryCodePage(TMemoryStream *ms)
{
	int code_page = 0;

	int src_size = ms->Size;
	if (src_size==0) return 932;

	//予備判定
	BYTE *bp = (BYTE *)ms->Memory;
	int x00_cnt	= 0;	//0x00 の個数
	int z_rpt	= 0;	//0x00 の連続数
	int z_max	= 0;	//0x00 の最大連続数
	int esc_cnt = 0;	//ESC の個数
	int asc_cnt = 0;	//ASCII の個数
	int ltn_cnt = 0;	//ISO8859(Latin-1を想定) の個数
	int b7_cnt	= 0;	//b7==1 の個数

	for (int i=0; i<src_size; i++) {
		BYTE b0 = bp[i];
		if (b0==0x00) {
			x00_cnt++; z_rpt++;
		}
		else {
			if (z_rpt>z_max) z_max = z_rpt;
			z_rpt = 0;
			if		(b0==0x1b) 			 esc_cnt++;
			else if (0x06<b0 && b0<0x7f) asc_cnt++;
			else if (0xa0<=b0)			 ltn_cnt++;
			if ((b0 & 0x80)!=0)			 b7_cnt++;
		}
	}
	if (z_max>3 || (src_size<3 && x00_cnt>0)) return 0;	//バイナリー

	//BOMの有無をチェック
	if		(bp[0]==0xfe && bp[1]==0xff)				code_page = 1201;		//UTF-16BE BOM付
	else if (bp[0]==0xff && bp[1]==0xfe)				code_page = 1200;		//UTF-16 (LE) BOM付
	else if (bp[0]==0xef && bp[1]==0xbb && bp[2]==0xbf)	code_page = CP_UTF8;	//UTF-8 BOM付
	if (code_page>0) return code_page;

	//BOM無し
	if (check_UTF8(bp, src_size)>0) code_page = CP_UTF8;	//UTF-8 BOM無し
	if (code_page>0) return code_page;

	if (x00_cnt==0) {
		if (esc_cnt>0) {
			//ISO-2022-JP?
			try {
				int jis_cnt = 0;
				int flag = 0;
				for (int i=0; i<src_size-1; i++) {
					BYTE b = bp[i];
					switch (flag) {
					case 0:
						if (b==0x1b) flag = 1;
						else if ((b>=0x80 && b<=0x8d) || (b>=0x90 && b<=0xa0) || b==0xfe) Abort();
						break;
					case 1:	//ESC
						flag = (b=='$')? 2 : (b=='(')? 5 : 0;
						break;
					case 2:	//ESC$
						flag = (b=='B' || b=='@')? 3 : 0;
						break;
					case 3:	//ESC$B/@
						if (b==0x1b) flag = 1; else jis_cnt++;
						break;
					case 5:	//ESC(
						flag = (b=='B' || b=='J')? 6 : 0;
						break;
					case 6:	//ESC(B/J
						if (b==0x1b) flag = 1; else jis_cnt++;
						break;
					}
				}
				if (jis_cnt > 0) code_page = 50220;
			}
			catch (...) {
				;
			}
		}
		else {
			//ShiftJIS?
			int sj_cnt	= 0;
			for (int i=0; i<src_size-1; i++) {
				BYTE b0 = bp[i];
				BYTE b1 = bp[i + 1];
				if (((0x81<=b0 && b0<=0x9f) || (0xe0<=b0 && b0<=0xfc))
					&& ((0x40<=b1 && b1<=0x7e) || (0x80<=b1 && b1<=0xfc)))
				{
					sj_cnt += 2;  i++;
				}
			}
			//EUC-JP?
			int euc_cnt = 0;
			for (int i=0; i<src_size-1; i++) {
				BYTE b0 = bp[i];
				BYTE b1 = bp[i + 1];
				if (((0xa1<=b0 && b0<=0xfe) && (0xa1<=b1 && b1<=0xfe))
					|| (b0==0x8e && (0xa1<=b1 && b1<=0xdf)))
				{
					euc_cnt += 2;  i++;
				}
				else if (i<src_size-2) {
					BYTE b2 = bp[i + 2];
					if (b0==0x8f && (0xa1<=b1 && b1<=0xfe) && (0xa1<=b2 && b2<=0xfe)) {
						euc_cnt += 3;  i += 2;
					}
				}
			}
			//UTF-7?
			int utf7_cnt = 0;
			if (b7_cnt==0) {
				int  b64cnt = 0;
				bool is_b64 = false;
				for (int i=0; i<src_size; i++) {
					BYTE b0 = bp[i];
					if (!is_b64) {
						if (b0=='+') {
							b64cnt = 0; is_b64 = true;
						}
					}
					else {
						if (b0=='-') {
							utf7_cnt += b64cnt;
							is_b64 = false;
						}
						else if (!isalnum(b0) && b0!='+' && b0!='/') {
							utf7_cnt = 0; break;
						}
						else b64cnt++;
					}
				}
			}
			if ((euc_cnt + sj_cnt + asc_cnt) > src_size/2) {
				if		(euc_cnt>sj_cnt)	code_page = 20932;
				else if (utf7_cnt>sj_cnt)	code_page = 65000;
				else if (sj_cnt>0)			code_page = 932;
				else if (ltn_cnt>0)			code_page = 1252;
				else if (asc_cnt==src_size)	code_page = 20127;
			}
		}
		if (code_page>0) return code_page;
	}

	//UTF-16か?
	int be_asc = 1, le_asc = 1;
	int be_hk  = 1, le_hk  = 1;		//ひらがな、カタカナ
	src_size -= (src_size%2);
	for (int i=0; i<src_size-1; i+=2) {
		BYTE b0 = bp[i];
		BYTE b1 = bp[i + 1];
		if		(b0==0x00 && 0x1f<b1 && b1<0x7f) be_asc++;
		else if (b1==0x00 && 0x1f<b0 && b0<0x7f) le_asc++;
		else if (b0==0x30 && 0x00<b1 && b1<0xf7) be_hk++;
		else if (b1==0x30 && 0x00<b0 && b0<0xf7) le_hk++;
	}

	int be_point = 0, le_point = 0;
	if		(be_asc/le_asc>10) be_point++;
	else if (le_asc/be_asc>10) le_point++;
	if		(be_hk/le_hk>10)   be_point++;
	else if (le_hk/be_hk>10)   le_point++;

	if		(be_point>le_point) code_page = 1201;	//BE
	else if	(le_point>be_point) code_page = 1200;	//LE

	return code_page;
}

//---------------------------------------------------------------------------
// HtmConv クラス
//---------------------------------------------------------------------------
HtmConv::HtmConv()
{
	HtmBuf = new TStringList();
	TxtBuf = new TStringList();

	TempDir 	 = EmptyStr;
	StatMsg		 = EmptyStr;

	OutCodePage  = 932;		//ShiftJIS

	IsMarkdown	 = false;

	LineWidth	 = DEF_LINE_WIDTH;
	ForceCr 	 = false;
	BlankLnLimit = DEF_BLANK_LN_LIMIT;
	NoPstHdr	 = false;
	HrLineStr	 = "-";
	IndentStr	 = "  ";
	UlMarkStr	 = "・";
	ZenNumber	 = false;

	UseAlt		 = false;
	AddImgSrc	 = false;
	DefAltStr	 = "画像";
	AltBraStr	 = "[";
	AltKetStr	 = "]";
	B_BraStr	 = EmptyStr;
	B_KetStr	 = EmptyStr;
	I_BraStr	 = EmptyStr;
	I_KetStr	 = EmptyStr;
	U_BraStr	 = EmptyStr;
	U_KetStr	 = EmptyStr;
	CellSepStr	 = "\t";

	InsLink 	 = false;
	OnlyExLink	 = true;
	AddCrLink	 = false;
	LnkBraStr	 = "<";
	LnkKetStr	 = ">";

	InsHrSct = InsHrArt = InsHrNav = false;

	DelBlkCls	 = EmptyStr;
	DelBlkId	 = EmptyStr;

	FileName	 = EmptyStr;
	FileTime	 = EmptyStr;

	HeadStr 	 = EmptyStr;
	FootStr 	 = EmptyStr;

	AlignList = new TStringList();
}

//---------------------------------------------------------------------------
HtmConv::~HtmConv()
{
	delete HtmBuf;
	delete TxtBuf;
	delete AlignList;
}

//---------------------------------------------------------------------------
//指定タグで囲まれた部分を削除
//---------------------------------------------------------------------------
void HtmConv::DelTagBlock(TStringList *lst, UnicodeString start_wd, UnicodeString end_wd)
{
	bool flag = false;
	int i = 0;
	while (i<lst->Count) {
		UnicodeString s = lst->Strings[i];
		if (StartsStr("<", s)) s = s.UpperCase();
		if (!flag) {
			if (StartsStr(start_wd, s)==1) {
				flag = (s.Pos(end_wd)==0);
				lst->Delete(i);
			}
			else {
				i++;
			}
		}
		else {
			flag = (s.Pos(end_wd)==0);
			lst->Delete(i);
		}
	}
}

//---------------------------------------------------------------------------
//指定属性のブロックを削除
//---------------------------------------------------------------------------
void HtmConv::DelAtrBlock(TStringList *lst, UnicodeString anam, UnicodeString aval)
{
	UnicodeString tag;
	bool del_sw = false;
	int t_level = 0;
	int i = 0;
	while (i<lst->Count) {
		UnicodeString s = lst->Strings[i];
		if (!del_sw) {
			UnicodeString tnam = GetTag(s);
			if (contains_word("DIV|SECTION|ARTICLE|NAV|TABLE|UL|OL|DL", tnam)) {
				if (SameStr(GetTagAtr(s, tnam, anam), aval)) {
					tag = tnam;
					t_level = 1;
					del_sw = true;
				}
			}
		}
		else {
			UnicodeString tnam = GetTag(s);
			if (StartsStr("/", tnam)) {
				tnam.Delete(1, 1);
				if (SameText(tag, tnam)) t_level--;;
			}
			else {
				if (SameText(tag, tnam)) t_level++;;
			}
		}

		if (del_sw) {
			lst->Delete(i);
			if (t_level==0) del_sw = false;
		}
		else {
			i++;
		}
	}
}

//---------------------------------------------------------------------------
//タグ名を取得
//---------------------------------------------------------------------------
UnicodeString HtmConv::GetTag(UnicodeString s)
{
	if (s.IsEmpty()) return EmptyStr;

	UnicodeString retstr;
	int flag = 0;
	for (int i=1; i<=s.Length(); i++) {
		WideChar c = s[i];
		switch (flag) {
		case 0:
			if (c!='<') flag = -1; else flag = 1;
			break;
		case 1:
			if (c=='>' || c==' ')
				flag = -1;
			else
				retstr += UpperCase(c);
			break;
		}
		if (flag==-1) break;
	}
	return retstr;
}

//---------------------------------------------------------------------------
//要素から指定した属性の値を取得
//---------------------------------------------------------------------------
UnicodeString HtmConv::GetTagAtr(UnicodeString s, UnicodeString t, UnicodeString a)
{
	if (s.IsEmpty()) return EmptyStr;

	UnicodeString retstr, tag, atr, vstr;
	int flag = 0;
	bool qflag;

	for (int i=1; i<=s.Length(); i++) {
		WideChar c = s[i];
		switch (flag) {
		case 0:
			if (c=='<') {
				tag  = EmptyStr;
				flag = 1;
			}
			break;
		case 1:
			if (c=='>') {
				flag = 0;
			}
			else if (c==' ') {	//タグ確定
				tag = tag.Trim();
				atr  = EmptyStr;
				flag = 2;
			}
			else {
				tag.cat_sprintf(_T("%c"), c);
			}
			break;
		case 2:
			if (c=='=') {		//属性名確定
				atr = atr.Trim();
				flag = 3;
			}
			else if (c==' ') atr = EmptyStr;
			else if (c=='>') flag = 0;
			else atr.cat_sprintf(_T("%c"), c);
			break;
		case 3:
			if (c=='>') {
				flag = 0;
			}
			else if (c!=' ') {	//属性値開始
				qflag = (c=='\"');
				if (qflag) vstr = EmptyStr; else vstr = c;
				flag = 4;
			}
			break;
		case 4:
			if ((qflag && c=='\"') || (!qflag && (c==' ' || c=='>'))) {
				if (SameText(tag, t) && SameText(atr, a)) {
					retstr = vstr;
					flag   = -1;	//属性値確定!
				}
				else {
					atr = EmptyStr;
					if (c=='>') flag = 0; else flag = 2;
				}
			}
			else vstr.cat_sprintf(_T("%c"), c);
			break;
		}

		if (flag==-1) break;
	}

	return retstr;
}

//---------------------------------------------------------------------------
//実体参照の解決
//---------------------------------------------------------------------------
UnicodeString HtmConv::RefEntity(UnicodeString s)
{
	if (!ContainsStr(s, "&")) return s;

	//文字実体参照から数値文字参照に一旦変換
	std::unique_ptr<TStringList> ent_lst(new TStringList());

	if (!IsMarkdown) ent_lst->Text = "&lt;\t&#60;\n&gt;\t&#62;\n";

	ent_lst->Text +=
		"&quot;\t&#34;\n"
		"&nbsp;\t&#160;\n"
		"&iexcl;\t&#161;\n"
		"&cent;\t&#162;\n"
		"&pound;\t&#163;\n"
		"&curren;\t&#164;\n"
		"&yen;\t&#165;\n"
		"&brvbar;\t&#166;\n"
		"&sect;\t&#167;\n"
		"&uml;\t&#168;\n"
		"&copy;\t&#169;\n"
		"&ordf;\t&#170;\n"
		"&laquo;\t&#171;\n"
		"&not;\t&#172;\n"
		"&shy;\t&#173;\n"
		"&reg;\t&#174;\n"
		"&macr;\t&#175;\n"
		"&deg;\t&#176;\n"
		"&plusmn;\t&#177;\n"
		"&sup2;\t&#178;\n"
		"&sup3;\t&#179;\n"
		"&acute;\t&#180;\n"
		"&micro;\t&#181;\n"
		"&para;\t&#182;\n"
		"&middot;\t&#183;\n"
		"&cedil;\t&#184;\n"
		"&sup1;\t&#185;\n"
		"&ordm;\t&#186;\n"
		"&raquo;\t&#187;\n"
		"&frac14;\t&#188;\n"
		"&frac12;\t&#189;\n"
		"&frac34;\t&#190;\n"
		"&iquest;\t&#191;\n"
		"&Agrave;\t&#192;\n"
		"&Aacute;\t&#193;\n"
		"&Acirc;\t&#194;\n"
		"&Atilde;\t&#195;\n"
		"&Auml;\t&#196;\n"
		"&Aring;\t&#197;\n"
		"&AElig;\t&#198;\n"
		"&Ccedil;\t&#199;\n"
		"&Egrave;\t&#200;\n"
		"&Eacute;\t&#201;\n"
		"&Ecirc;\t&#202;\n"
		"&Euml;\t&#203;\n"
		"&Igrave;\t&#204;\n"
		"&Iacute;\t&#205;\n"
		"&Icirc;\t&#206;\n"
		"&Iuml;\t&#207;\n"
		"&ETH;\t&#208;\n"
		"&Ntilde;\t&#209;\n"
		"&Ograve;\t&#210;\n"
		"&Oacute;\t&#211;\n"
		"&Ocirc;\t&#212;\n"
		"&Otilde;\t&#213;\n"
		"&Ouml;\t&#214;\n"
		"&times;\t&#215;\n"
		"&Oslash;\t&#216;\n"
		"&Ugrave;\t&#217;\n"
		"&Uacute;\t&#218;\n"
		"&Ucirc;\t&#219;\n"
		"&Uuml;\t&#220;\n"
		"&Yacute;\t&#221;\n"
		"&THORN;\t&#222;\n"
		"&szlig;\t&#223;\n"
		"&agrave;\t&#224;\n"
		"&aacute;\t&#225;\n"
		"&acirc;\t&#226;\n"
		"&atilde;\t&#227;\n"
		"&auml;\t&#228;\n"
		"&aring;\t&#229;\n"
		"&aelig;\t&#230;\n"
		"&ccedil;\t&#231;\n"
		"&egrave;\t&#232;\n"
		"&eacute;\t&#233;\n"
		"&ecirc;\t&#234;\n"
		"&euml;\t&#235;\n"
		"&igrave;\t&#236;\n"
		"&iacute;\t&#237;\n"
		"&icirc;\t&#238;\n"
		"&iuml;\t&#239;\n"
		"&eth;\t&#240;\n"
		"&ntilde;\t&#241;\n"
		"&ograve;\t&#242;\n"
		"&oacute;\t&#243;\n"
		"&ocirc;\t&#244;\n"
		"&otilde;\t&#245;\n"
		"&ouml;\t&#246;\n"
		"&divide;\t&#247;\n"
		"&oslash;\t&#248;\n"
		"&ugrave;\t&#249;\n"
		"&uacute;\t&#250;\n"
		"&ucirc;\t&#251;\n"
		"&uuml;\t&#252;\n"
		"&yacute;\t&#253;\n"
		"&thorn;\t&#254;\n"
		"&yuml;\t&#255;\n"
		"&OElig;\t&#338;\n"
		"&oelig;\t&#339;\n"
		"&Scaron;\t&#352;\n"
		"&scaron;\t&#353;\n"
		"&Yuml;\t&#376;\n"
		"&fnof;\t&#402;\n"
		"&circ;\t&#710;\n"
		"&tilde;\t&#732;\n"
		"&Alpha;\t&#913;\n"
		"&Beta;\t&#914;\n"
		"&Gamma;\t&#915;\n"
		"&Delta;\t&#916;\n"
		"&Epsilon;\t&#917;\n"
		"&Zeta;\t&#918;\n"
		"&Eta;\t&#919;\n"
		"&Theta;\t&#920;\n"
		"&Iota;\t&#921;\n"
		"&Kappa;\t&#922;\n"
		"&Lambda;\t&#923;\n"
		"&Mu;\t&#924;\n"
		"&Nu;\t&#925;\n"
		"&Xi;\t&#926;\n"
		"&Omicron;\t&#927;\n"
		"&Pi;\t&#928;\n"
		"&Rho;\t&#929;\n"
		"&Sigma;\t&#931;\n"
		"&Tau;\t&#932;\n"
		"&Upsilon;\t&#933;\n"
		"&Phi;\t&#934;\n"
		"&Chi;\t&#935;\n"
		"&Psi;\t&#936;\n"
		"&Omega;\t&#937;\n"
		"&alpha;\t&#945;\n"
		"&beta;\t&#946;\n"
		"&gamma;\t&#947;\n"
		"&delta;\t&#948;\n"
		"&epsilon;\t&#949;\n"
		"&zeta;\t&#950;\n"
		"&eta;\t&#951;\n"
		"&theta;\t&#952;\n"
		"&iota;\t&#953;\n"
		"&kappa;\t&#954;\n"
		"&lambda;\t&#955;\n"
		"&mu;\t&#956;\n"
		"&nu;\t&#957;\n"
		"&xi;\t&#958;\n"
		"&omicron;\t&#959;\n"
		"&pi;\t&#960;\n"
		"&rho;\t&#961;\n"
		"&sigmaf;\t&#962;\n"
		"&sigma;\t&#963;\n"
		"&tau;\t&#964;\n"
		"&upsilon;\t&#965;\n"
		"&phi;\t&#966;\n"
		"&chi;\t&#967;\n"
		"&psi;\t&#968;\n"
		"&omega;\t&#969;\n"
		"&thetasym;\t&#977;\n"
		"&upsih;\t&#978;\n"
		"&piv;\t&#982;\n"
		"&bull;\t&#8226;\n"
		"&hellip;\t&#8230;\n"
		"&prime;\t&#8242;\n"
		"&Prime;\t&#8243;\n"
		"&oline;\t&#8254;\n"
		"&frasl;\t&#8260;\n"
		"&weierp;\t&#8472;\n"
		"&image;\t&#8465;\n"
		"&real;\t&#8476;\n"
		"&trade;\t&#8482;\n"
		"&alefsym;\t&#8501;\n"
		"&larr;\t&#8592;\n"
		"&uarr;\t&#8593;\n"
		"&rarr;\t&#8594;\n"
		"&darr;\t&#8595;\n"
		"&harr;\t&#8596;\n"
		"&crarr;\t&#8629;\n"
		"&lArr;\t&#8656;\n"
		"&uArr;\t&#8657;\n"
		"&rArr;\t&#8658;\n"
		"&dArr;\t&#8659;\n"
		"&hArr;\t&#8660;\n"
		"&forall;\t&#8704;\n"
		"&part;\t&#8706;\n"
		"&exist;\t&#8707;\n"
		"&empty;\t&#8709;\n"
		"&nabla;\t&#8711;\n"
		"&isin;\t&#8712;\n"
		"&notin;\t&#8713;\n"
		"&ni;\t&#8715;\n"
		"&prod;\t&#8719;\n"
		"&sum;\t&#8721;\n"
		"&minus;\t&#8722;\n"
		"&lowast;\t&#8727;\n"
		"&radic;\t&#8730;\n"
		"&prop;\t&#8733;\n"
		"&infin;\t&#8734;\n"
		"&ang;\t&#8736;\n"
		"&and;\t&#8743;\n"
		"&or;\t&#8744;\n"
		"&cap;\t&#8745;\n"
		"&cup;\t&#8746;\n"
		"&int;\t&#8747;\n"
		"&there4;\t&#8756;\n"
		"&sim;\t&#8764;\n"
		"&cong;\t&#8773;\n"
		"&asymp;\t&#8776;\n"
		"&ne;\t&#8800;\n"
		"&equiv;\t&#8801;\n"
		"&le;\t&#8804;\n"
		"&ge;\t&#8805;\n"
		"&sub;\t&#8834;\n"
		"&sup;\t&#8835;\n"
		"&nsub;\t&#8836;\n"
		"&sube;\t&#8838;\n"
		"&supe;\t&#8839;\n"
		"&oplus;\t&#8853;\n"
		"&otimes;\t&#8855;\n"
		"&perp;\t&#8869;\n"
		"&sdot;\t&#8901;\n"
		"&lceil;\t&#8968;\n"
		"&rceil;\t&#8969;\n"
		"&lfloor;\t&#8970;\n"
		"&rfloor;\t&#8971;\n"
		"&lang;\t&#9001;\n"
		"&rang;\t&#9002;\n"
		"&loz;\t&#9674;\n"
		"&spades;\t&#9824;\n"
		"&clubs;\t&#9827;\n"
		"&hearts;\t&#9829;\n"
		"&diams;\t&#9830;\n"
		"&ensp;\t&#8194;\n"
		"&emsp;\t&#8195;\n"
		"&thinsp;\t&#8201;\n"
		"&zwnj;\t&#8204;\n"
		"&zwj;\t&#8205;\n"
		"&lrm;\t&#8206;\n"
		"&rlm;\t&#8207;\n"
		"&ndash;\t&#8211;\n"
		"&mdash;\t&#8212;\n"
		"&lsquo;\t&#8216;\n"
		"&rsquo;\t&#8217;\n"
		"&sbquo;\t&#8218;\n"
		"&ldquo;\t&#8220;\n"
		"&rdquo;\t&#8221;\n"
		"&bdquo;\t&#8222;\n"
		"&dagger;\t&#8224;\n"
		"&Dagger;\t&#8225;\n"
		"&permil;\t&#8240;\n"
		"&lsaquo;\t&#8249;\n"
		"&rsaquo;\t&#8250;\n"
		"&euro;\t&#8364;\n";
	s = replace_str_by_list(s, ent_lst.get());

	//非ユニコードの場合、適当な文字に変換
	if (OutCodePage!=1200 && OutCodePage!=1201 && OutCodePage!=65000 && OutCodePage!=65001) {
		ent_lst->Text =
			"&#160;\t \n"		//&nbsp;
			"&#169;\t(c)\n"		//&copy;
			"&#174;\t(R)\n"		//&reg;
			"&#8194;\t \n"		//&ensp;
			"&#8195;\t　\n"		//&emsp;
			"&#8211;\t-\n"		//&ndash;
			"&#8212;\t－\n";	//&mdash;
		s = replace_str_by_list(s, ent_lst.get());
	}

	//数値文字参照を解決
	if (ContainsStr(s, "&")) {
		int stt = 0;
		int p = 1;
		int p0;
		while (p<=s.Length()) {
			if (s[p]=='&') {
				stt = 1;
				p++;
			}
			else {
				switch (stt) {
				case 1:
					if (s[p]=='#') {
						p0 = p + 1;
						stt = 2;
					}
					else {
						stt = 0;
					}
					p++;
					break;
				case 2:
					if (s[p]==';') {
						int cn = s.SubString(p0, p - p0).ToIntDef(0);
						p0 -= 2;
						s.Delete(p0, p - p0 + 1);
						if (cn>=32 && cn<65535)
							s.Insert((WideChar)cn, p0);
						else
							s.Insert("?", p0);
						p = p0 + 1;
						stt = 0;
					}
					else {
						p++;
					}
					break;
				default:
					p++;
				}
			}
		}
	}

	s = ReplaceStr(s, "&amp;", "&");

	return s;
}

//---------------------------------------------------------------------------
//HTML文書のタイトルを取得
//  fnam: HTML文書のファイル名
//---------------------------------------------------------------------------
UnicodeString HtmConv::GetTitle(UnicodeString fnam)
{
	UnicodeString titstr;

	std::unique_ptr<TStringList> fbuf(new TStringList());
	fbuf->LoadFromFile(fnam);
	UnicodeString headstr;
	int p = pos_i("</HEAD>", fbuf->Text);
	if (p>0) headstr = fbuf->Text.SubString(1, p + 6);

	if (!headstr.IsEmpty()) {
		//charset をチェック
		int p = pos_i("charset", headstr);
		UnicodeString charset;
		if (p>0) {
			UnicodeString tmpstr = headstr.SubString(p, headstr.Length() - p + 1);
			tmpstr = get_tkn(get_tkn_r(tmpstr, "="), ">");
			if (!tmpstr.IsEmpty()) {
				p = tmpstr.Pos("\"");
				if (p>0) charset = tmpstr.SubString(1, p - 1);
			}
		}

		//エンコーディングを指定して再読み込み
		if (!charset.IsEmpty()) {
			int code_page = 0;
			if 		(charset.CompareIC("EUC-JP")==0)		code_page = 20932;
			else if (charset.CompareIC("ISO-2022-JP")==0)	code_page = 50220;
			else if (charset.CompareIC("UTF-8")==0)			code_page = CP_UTF8;
			if (code_page!=0) {
				std::unique_ptr<TEncoding> enc(TEncoding::GetEncoding(code_page));
				std::unique_ptr<TStringList> fbuf(new TStringList());
				fbuf->LoadFromFile(fnam, enc.get());
				headstr = EmptyStr;
				int p = pos_i("</HEAD>", fbuf->Text);
				if (p>0) headstr = fbuf->Text.SubString(1, p + 6);
			}
		}

		//コメント部分を削除
		int p0, p1;
		p0 = headstr.Pos("<!--");
		while (p0>0) {
			p1 = headstr.Pos("-->");
			if (p1>p0) {
				headstr.Delete(p0, p1 - p0 + 3);
				p0 = headstr.Pos("<!--");
			}
			else {
				headstr = headstr.SubString(1, p1 - 1);
				break;
			}
		}

		//タイトルを取得
		p0 = pos_i("<TITLE>",  headstr);
		p1 = pos_i("</TITLE>", headstr);
		if (p0>0 && p1>p0) titstr = headstr.SubString(p0 + 7, p1 - p0 - 7);
	}
	return titstr;
}

//---------------------------------------------------------------------------
//ファイルまたはURLからHTMLソースを読み込む
// fnam: HTMLファイル名、ショートカットファイル名、URL
//---------------------------------------------------------------------------
bool HtmConv::LoadFile(UnicodeString fnam)
{
	HtmBuf->Clear();
	StatMsg = EmptyStr;
	UrlStr	= EmptyStr;
	BaseStr = EmptyStr;
	UnicodeString dl_file = EmptyStr;

	if (StartsText("http://", fnam) || StartsText("https://", fnam)) {
		try {
			UrlStr = fnam;
			if (UrlStr.Pos("%")==0) UrlStr = TIdURI::URLEncode(UrlStr, IndyTextEncoding_UTF8());
		}
		catch (Exception &exception) {
			;
		}
	}
	else if (ExtractFileExt(fnam).UpperCase()==".URL") {
		if (FileExists(fnam)) {
			std::unique_ptr<TStringList> tmplst(new TStringList());
			tmplst->LoadFromFile(fnam);
			for (int i=0; i<tmplst->Count; i++) {
				if (!StartsStr("URL=", tmplst->Strings[i])) continue;
				UrlStr = tmplst->Strings[i].Delete(1, 4);
				break;
			}
		}
		else return false;
	}

	if (!UrlStr.IsEmpty()) {
		//インターネットからHTML文書を取得
		BaseStr = UrlStr;
		if (!EndsStr("/", BaseStr)) {
			UnicodeString tmpstr = BaseStr;
			tmpstr.Delete(1, 7);
			if (tmpstr.Pos("/")==0) {
				BaseStr.UCAT_TSTR("/");
			}
			else {
				int i = BaseStr.Length();
				while (i>7) {
					if (BaseStr[i]=='/') break;
					BaseStr.Delete(i--, 1);
				}
			}
		}

		DWORD flag;
		if (InternetGetConnectedState(&flag, 0)) {
			HINTERNET hSession = InternetOpen(
				_T("H2Tconv"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
			if (hSession) {
				HINTERNET hConnect = InternetOpenUrl(hSession,
				    UrlStr.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
				if (hConnect) {
					dl_file = TempDir+"\\$$~DOWLD.tmp";
					{
						std::unique_ptr<TFileStream> dfs(new TFileStream(dl_file, fmCreate));
						DWORD dwSize;
						char ldbuf[8192];
						for (;;) {
							if (!InternetReadFile(hConnect, &ldbuf, 8192, &dwSize)) break;
							if (dwSize==0) break;
							dfs->Write(ldbuf, dwSize);
						}
					}
					InternetCloseHandle(hConnect);
					HtmBuf->LoadFromFile(dl_file);
				}
				else {
					StatMsg = "指定URLを開けせん";
				}
			    InternetCloseHandle(hSession);
			}
			else {
				StatMsg = "接続できません";
			}
			FileName = EmptyStr;
			FileTime = DateTimeToStr(Now());
		}
		else {
			StatMsg = "インターネットに接続されていません";
		}
	}
	//ファイルから読み込み
	else {
		if (FileExists(fnam)) {
			HtmBuf->LoadFromFile(fnam);
			FileName = fnam;
			TDateTime ft; if (!FileAge(fnam, ft)) ft = 0;
			FileTime = DateTimeToStr(ft);
		}
		else {
			return false;
		}
	}

	//必要ならエンコーディングを指定して再読み込み
	if (HtmBuf->Count>0) {
		UnicodeString charset;
		UnicodeString tmpbuf = HtmBuf->Text;
		do {
			//ヘッダを取り出す
			int p = pos_i("</head>", tmpbuf);  if (p==0) break;
			if (p>0) tmpbuf = tmpbuf.SubString(1, p - 1);

			//漢字コードが指定されているか調べる
			std::unique_ptr<TStringList> tmplst(new TStringList());
			tmplst->Text = tmpbuf;
			for (int i=0; i<tmplst->Count; i++) {
				UnicodeString lbuf = tmplst->Strings[i];
				UnicodeString tmpstr = GetTagAtr(lbuf, "META", "charset");
				if (!tmpstr.IsEmpty()) {
					charset = tmpstr; break;
				}
				tmpstr = GetTagAtr(lbuf, "META", "http-equiv");
				if (tmpstr.IsEmpty()) continue;
				tmpstr = GetTagAtr(lbuf, "META", "content");
				if ((p = pos_i("charset", tmpstr)) == 0) continue;;
				tmpstr.Delete(1, p - 1);
				if ((p = tmpstr.Pos("=")) == 0) continue;
				charset = tmpstr.Delete(1, p).Trim();
				break;
			}
		} while (0);

		int code_page = 0;
		if (!charset.IsEmpty()) {
			if		(SameText(charset, "Shift_JIS"))	code_page = 932;
			else if	(SameText(charset, "EUC-JP"))		code_page = 20932;
			else if (SameText(charset, "ISO-2022-JP"))	code_page = 50220;
			else if (SameText(charset, "UTF-8"))		code_page = CP_UTF8;
		}
		//charset が指定されていなかったら自前でコード判定
		if (code_page==0) {
			std::unique_ptr<TMemoryStream> ms(new TMemoryStream());
			ms->LoadFromFile(dl_file.IsEmpty()? fnam : dl_file);
			code_page = get_MemoryCodePage(ms.get());
		}

		//エンコーディングを指定して再読み込み
		if (code_page!=0 && code_page!=932) {
			std::unique_ptr<TEncoding> enc(TEncoding::GetEncoding(code_page));
			HtmBuf->LoadFromFile(dl_file.IsEmpty()? fnam : dl_file, enc.get());
		}
		if (!dl_file.IsEmpty()) DeleteFile(dl_file);
		return true;
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
void HtmConv::AddText(UnicodeString s)
{
	if (s.IsEmpty()) {
		TxtBuf->Add(EmptyStr);
	}
	else {
		if (IsMarkdown) {
			//改行(半角スペース×2)を付加
			if (!TRegEx::IsMatch(s, NO_CRSPC_PTN)) s += "  ";
			//引用
			if (BQ_level>0) s = StringOfChar(_T('>'), BQ_level) + s;
		}
		TxtBuf->Add(s);
	}
}
//---------------------------------------------------------------------------
//TxtLineBuf を整形後 TxtBuf へ
//---------------------------------------------------------------------------
void HtmConv::FlushText()
{
	if (TxtLineBuf.IsEmpty()) return;
	if (!Skip) {
		//特殊記号の変換
		if (!fXMP) TxtLineBuf = RefEntity(TxtLineBuf);

		//アライン
		if (!fPRE && !fXMP && AlignList->Count>0) {
			UnicodeString a = AlignList->Strings[AlignList->Count - 1];
			int len = str_len_half(TxtLineBuf);
			if (len>0 && len<LineWidth) {
				UnicodeString s;
				if		(SameText(a, "center"))
					s = StringOfChar(_T(' '), (LineWidth - len)/2);
				else if (SameText(a, "right"))
					s = StringOfChar(_T(' '), LineWidth - len);
				TxtLineBuf = s + TxtLineBuf;
			}
		}

		if (fPRE || fXMP) {
	 		TxtBuf->Add(TxtLineBuf);
		}
		else if (ForceCr) {
			//指定桁数で強制改行
			UnicodeString tmpstr = TxtLineBuf;
			while (str_len_half(tmpstr)>LineWidth) {
				UnicodeString lbuf;
				while (str_len_half(lbuf)<LineWidth) {
					lbuf += tmpstr.SubString(1, 1);
			 		tmpstr.Delete(1, 1);
			 		if (tmpstr.IsEmpty()) break;
				}
				AddText(lbuf);
			}
			if (!tmpstr.IsEmpty()) {
				UnicodeString s = trim_ex(TxtLineBuf);
				if (trim_ex(TxtLineBuf).IsEmpty()) AddText(EmptyStr); else AddText(tmpstr);
			}
		}
		else {
			UnicodeString s = trim_ex(TxtLineBuf);
			if (trim_ex(TxtLineBuf).IsEmpty()) AddText(EmptyStr); else AddText(TxtLineBuf);
		}
	}
	TxtLineBuf = EmptyStr;
}

//---------------------------------------------------------------------------
//罫線を追加
//---------------------------------------------------------------------------
void HtmConv::AddLine(UnicodeString hrstr, UnicodeString wdstr, UnicodeString alstr)
{
	if (hrstr.IsEmpty()) hrstr = HrLineStr;
	if (hrstr.IsEmpty()) return;

	int hr_w = str_len_half(hrstr);
	int n = std::max(LineWidth/hr_w, 1);
	UnicodeString s;
	if (!wdstr.IsEmpty()) {
		wdstr = wdstr.UpperCase();
		if (EndsStr("%", wdstr)) {
			wdstr.Delete(wdstr.Length(), 1);
			n = std::min(wdstr.ToIntDef(100), 100);
			n = std::max((LineWidth/hr_w)*n/100, 1);
			if (alstr.IsEmpty()) alstr = "center";
			AlignList->Add(alstr);
			for (int i=0; i<n; i++) s += hrstr;
			TxtBuf->Add(s);
			AlignList->Delete(AlignList->Count - 1);
		}
		else {
			for (int i=0; i<n; i++) s += hrstr;
			TxtBuf->Add(s);
		}
	}
	else {
		for (int i=0; i<n; i++) s += hrstr;
		TxtBuf->Add(s);
	}
}

//---------------------------------------------------------------------------
//変換処理
//---------------------------------------------------------------------------
void HtmConv::Convert(
	TStringList *rep_lst)	//置換リスト	(default = NULL)
{
	StatMsg = EmptyStr;

	std::unique_ptr<TStringList> TmpBuf(new TStringList());

	//------------------------------
	//タグを行毎に分解
	//------------------------------
	for (int i=0; i<HtmBuf->Count; i++) {
		UnicodeString s = HtmBuf->Strings[i];
		if (!s.IsEmpty()) {
			for (;;) {
				int p0 = s.Pos("<");
				int p1 = s.Pos(">");
				if (p0>1) {
					int n = (p1>0)? ((p0<p1)? p0 - 1 : p1) : p0 - 1;
					TmpBuf->Add(s.SubString(1, n));
					s.Delete(1, n);
				}
				else if (p1>0) {
					TmpBuf->Add(s.SubString(1, p1));
					s.Delete(1, p1);
				}
				else break;
			}
			if (!s.IsEmpty()) TmpBuf->Add(s);
		}
		TmpBuf->Add("</>");	//改行マーク
	}

	//------------------------------
	//コメント部分を削除
	//------------------------------
	DelTagBlock(TmpBuf.get(), "<!--", "-->");

	//------------------------------
	//script、sytle 部分を削除
	//※コメントアウトされていない場合の対策
	//------------------------------
	DelTagBlock(TmpBuf.get(), "<SCRIPT",	"</SCRIPT>");
	DelTagBlock(TmpBuf.get(), "<NOSCRIPT",	"</NOSCRIPT>");
	DelTagBlock(TmpBuf.get(), "<STYLE",		"</STYLE>");

	//------------------------------
	//行分断されているタグを連結
	//------------------------------
	int i = 0;
	bool flag = false;
	while (i<TmpBuf->Count) {
		UnicodeString s = TmpBuf->Strings[i];
		if (!flag) {
			flag = (StartsStr("<", s) && !EndsStr(">", s));
			i++;
		}
		else {
			if (s=="</>") {
				TmpBuf->Delete(i);
			}
			else {
				if (EndsStr(">", s)) flag = false;
				TmpBuf->Strings[i - 1] = TmpBuf->Strings[i - 1] + " " + TmpBuf->Strings[i];
				TmpBuf->Delete(i);
			}
		}
	}

	//------------------------------
	//余分な半角スペース等を削除
	//------------------------------
	fPRE = fXMP = false;
	i = 0;
	while (i<TmpBuf->Count) {
		UnicodeString s = TmpBuf->Strings[i];
		UnicodeString tag = GetTag(s);
		if (fPRE) {
			if (tag=="/PRE") fPRE = false;
			i++;
		}
		else if (fXMP) {
			if (tag=="/XMP") fXMP = false;
			i++;
		}
		else if (!tag.IsEmpty()) {
			if		(tag=="PRE")  fPRE = true;
			else if (tag=="XMP")  fXMP = true;
			TmpBuf->Strings[i++] = s.Trim();
		}
		else {
			//連続した半角スペースを一つに
			s = s.Trim();
			while (s.Pos("  ")>0) s = ReplaceStr(s, "  ", " ");
			if (s.IsEmpty())
				TmpBuf->Delete(i);
			else
				TmpBuf->Strings[i++] = s;
		}
	}

	//指定クラスのブロックを削除
	TStringDynArray cls_lst = SplitString(DelBlkCls, ";");
	for (int i=0; i<cls_lst.Length; i++) DelAtrBlock(TmpBuf.get(), "class", cls_lst[i]);
	//指定IDのブロックを削除
	TStringDynArray id_lst = SplitString(DelBlkId, ";");
	for (int i=0; i<id_lst.Length; i++) DelAtrBlock(TmpBuf.get(), "id", id_lst[i]);

	//------------------------------
	//変換
	//------------------------------
	AlignList->Clear();
	int  DL_level = 0;
	int  LI_level = 0;
	int  LI_No[100];
	int  tag_level = 0;
	char OL_type[100];
	int  fTR    = 0;
	int  fTITLE = 0;
	int  fP_al  = 0;
	int  fTABLE = 0;
	bool pstHdr = false;

	fPRE = fXMP = false;
	BQ_level = 0;
	Skip = false;

	TxtLineBuf = EmptyStr;
	TitleStr   = EmptyStr;
	UnicodeString HrefStr, DescrStr, KeywdStr, tmpstr;

	int topline = TxtBuf->Count;

	for (int i=0; i<TmpBuf->Count; i++) {
		UnicodeString lbuf = TmpBuf->Strings[i];
		UnicodeString tag  = GetTag(lbuf);
		if (tag.IsEmpty()) {
			pstHdr = false;
			if (fTITLE) {
				TitleStr += lbuf;
			}
			else if (fPRE || fXMP) {
				TxtLineBuf += lbuf;
			}
			else if (!Skip) {
				if (tag_level>0) {
					lbuf = lbuf.TrimLeft();
				}
				else {
					do {
						if (TxtLineBuf.IsEmpty() || lbuf.IsEmpty()) break;
						if (EndsStr(" ", TxtLineBuf)) break;
						if (StartsStr(" ", lbuf)) break;
						TxtLineBuf.UCAT_TSTR(" ");
					} while (0);
				}
				TxtLineBuf += lbuf;
			}
		}
		else {
			//<P> がブレークされたか？
			if (fP_al>0 && contains_word(
				"H1|H2|H3|H4|H5|H6|P|HR|DIV|CENTER|BLOCKQUOTE|TABLE|TR|TH|TD|UL|OL|DL|LI|PRE|XMP", tag))
			{
				FlushText();
				if (AlignList->Count>0) AlignList->Delete(AlignList->Count - 1);
				fP_al--;
			}

			if (tag=="/") {
				if (fPRE || fXMP) {
					UnicodeString s = trim_ex(TxtLineBuf);
					if (s.IsEmpty() && !IsMarkdown) TxtBuf->Add(TxtLineBuf); else FlushText();
				}
			}
			else if (tag=="/XMP") {
				FlushText();
				fXMP = false;
			}
			else if (fXMP) {
				TxtLineBuf += lbuf;
			}
			else if (tag=="B" || tag=="STRONG")  {
				TxtLineBuf += B_BraStr;
				tag_level++;
			}
			else if (tag=="/B" || tag=="/STRONG") {
				TxtLineBuf += B_KetStr;
				DecLevel(tag_level);
			}
			else if (tag=="I" || tag=="EM")  {
				TxtLineBuf += I_BraStr;
				tag_level++;
			}
			else if (tag=="/I" || tag=="/EM") {
				TxtLineBuf += I_KetStr;
				DecLevel(tag_level);
			}
			else if (tag=="U")  {
				TxtLineBuf += U_BraStr;
				tag_level++;
			}
			else if (tag=="/U") {
				TxtLineBuf += U_KetStr;
				DecLevel(tag_level);
			}
			else if (tag=="P") {
				FlushText();
				if (!pstHdr) TxtBuf->Add(EmptyStr);
				pstHdr = false;
				if (fTABLE==0) {
					tmpstr = GetTagAtr(lbuf, tag, "ALIGN");
					if (tmpstr.IsEmpty() && AlignList->Count>0)
						tmpstr = AlignList->Strings[AlignList->Count - 1];
					AlignList->Add(tmpstr);
				}
				else {
					AlignList->Add(EmptyStr);
				}
				fP_al++;
			}
			else if (tag=="/P") {
				FlushText();
				if (fP_al>0) {
					if (AlignList->Count>0) AlignList->Delete(AlignList->Count - 1);
					fP_al--;
				}
			}
			else if (contains_word("BR|BR/|BR /", tag)) {
				if (!TxtLineBuf.IsEmpty()) FlushText(); else TxtBuf->Add(EmptyStr);
				for (int j=1; j<(LI_level + DL_level); j++) TxtLineBuf += IndentStr;
			}
			else if (tag=="PRE") {
				FlushText();
				if (IsMarkdown) {
					TxtBuf->Add(EmptyStr);
					TxtBuf->Add("```");
				}
				fPRE = true;
			}
			else if (tag=="/PRE") {
				FlushText();
				if (IsMarkdown) {
					TxtBuf->Add("```");
					TxtBuf->Add(EmptyStr);
				}
				fPRE = false;
			}
			else if (tag=="XMP") {
				FlushText();
				fXMP = true;
			}
			else if (tag=="BLOCKQUOTE") {
				FlushText();
				if (IsMarkdown && BQ_level==0) TxtBuf->Add(EmptyStr);
				BQ_level++;
			}
			else if (tag=="/BLOCKQUOTE") {
				FlushText();
				if (IsMarkdown)	TxtBuf->Add(EmptyStr);
				DecLevel(BQ_level);
			}
			else if (tag=="CENTER") {
				FlushText();
				AlignList->Add("center");
			}
			else if (tag=="/CENTER") {
				FlushText();
				if (AlignList->Count>0) AlignList->Delete(AlignList->Count - 1);
			}
			else if (tag=="DIV") {
				FlushText();
				//指定クラスの前に罫線挿入
				if (!InsHrClass.IsEmpty()) {
					UnicodeString cnam = GetTagAtr(lbuf, tag, "CLASS");
					if (!cnam.IsEmpty() && ContainsText(";" + InsHrClass + ";", ";" + cnam + ";")) AddLine(InsLineStr);
				}
				AlignList->Add(GetTagAtr(lbuf, tag, "ALIGN"));
			}
			else if (tag=="/DIV") {
				FlushText();
				if (AlignList->Count>0) AlignList->Delete(AlignList->Count - 1);
			}
			else if (tag=="SECTION" || tag=="/SECTION") {
				FlushText();
				if (tag=="SECTION" && InsHrSct)	AddLine(InsLineStr);
			}
			else if (tag=="ARTICLE" || tag=="/ARTICLE") {
				FlushText();
				if (tag=="ARTICLE" && InsHrArt)	AddLine(InsLineStr);
			}
			else if (tag=="NAV" || tag=="/NAV") {
				FlushText();
				if (tag=="NAV" && InsHrNav)		AddLine(InsLineStr);
			}
			else if (contains_word("H1|H2|H3|H4|H5|H6", tag)) {
				FlushText();
				if (!pstHdr && TxtBuf->Count>0) TxtBuf->Add(EmptyStr);
				pstHdr = false;
				TxtLineBuf = HeaderStr[tag.SubString(2, 1).ToIntDef(1) - 1];
				tag_level++;
			}
			else if (contains_word("/H1|/H2|/H3|/H4|/H5|/H6", tag)) {
				FlushText();
				if (IsMarkdown) TxtBuf->Add(EmptyStr);
				if (NoPstHdr) pstHdr = true;
				DecLevel(tag_level);
			}
			else if (tag=="HR") {
				FlushText();
				AddLine(HrLineStr, GetTagAtr(lbuf, tag, "WIDTH"), GetTagAtr(lbuf, tag, "ALIGN"));
			}
			else if (tag=="UL") {
				FlushText();
				if (LI_level==0 && (!pstHdr || IsMarkdown)) TxtBuf->Add(EmptyStr);
				pstHdr = false;
				LI_No[++LI_level] = -1;
			}
			else if (tag=="OL") {
				FlushText();
				if (LI_level==0 && (!pstHdr || IsMarkdown)) TxtBuf->Add(EmptyStr);
				pstHdr = false;
				LI_No[++LI_level] = GetTagAtr(lbuf, tag, "START").ToIntDef(1);
				tmpstr = GetTagAtr(lbuf, tag, "TYPE");
				if (tmpstr.IsEmpty()) OL_type[LI_level] = '1'; else OL_type[LI_level] = tmpstr[1];
			}
			else if (tag=="/UL" || tag=="/OL") {
				FlushText();
				DecLevel(LI_level);
			}
			else if (tag=="LI") {
				FlushText();
				//順序付きリスト
				if (LI_level>0) {
					int n = LI_level + DL_level;
					for (int j=1; j<n; j++) TxtLineBuf += IndentStr;
					if (LI_No[LI_level]>0) {
						n = LI_No[LI_level];
						switch (OL_type[LI_level]) {
						case '1': tmpstr = IntToStr(n); 				break;
						case 'a': tmpstr = int_to_alpha(n); 			break;
						case 'A': tmpstr = int_to_alpha(n).UpperCase(); break;
						case 'i': tmpstr = int_to_roman(n); 			break;
						case 'I': tmpstr = int_to_roman(n).UpperCase(); break;
						}
						tmpstr.UCAT_TSTR(".");
						if (ZenNumber) tmpstr = to_full_str(tmpstr);
						TxtLineBuf += tmpstr;
						LI_No[LI_level]++;
					}
					//順序無しリスト
					else {
						TxtLineBuf += UlMarkStr;
					}
				}
			}
			else if (tag=="DL") {
				FlushText();
				if (DL_level==0 && (!pstHdr || IsMarkdown)) TxtBuf->Add(EmptyStr);
				pstHdr = false;
				DL_level++;
			}
			else if (tag=="/DL") {
				FlushText();
				DecLevel(DL_level);
			}
			else if (tag=="DT") {
				FlushText();
				if (DL_level>0) {
					int n = LI_level + DL_level - 1;
					for (int j=1; j<n; j++) TxtLineBuf += IndentStr;
				}
			}
			else if (tag=="DD") {
				if (DL_level>0) {
					FlushText();
					int n = LI_level + DL_level;
					for (int j=1; j<n; j++) TxtLineBuf += IndentStr;
				}
			}
			else if (tag=="TABLE") {
				FlushText();
				if (!pstHdr || IsMarkdown) TxtBuf->Add(EmptyStr);
				pstHdr = false;
				AlignList->Add(EmptyStr);
				fTABLE++;
			}
			else if (tag=="/TABLE") {
				FlushText();
				if (AlignList->Count>0) AlignList->Delete(AlignList->Count - 1);
				fTABLE--;
			}
			else if (contains_word("CAPTION|/TR|/CAPTION", tag)) {
				FlushText();
			}
			else if (tag=="TD" || tag=="TH") {
				if (fTR>0)
					fTR = 0;
				else
					if (!TxtLineBuf.IsEmpty()) TxtLineBuf += CellSepStr;
			}
			else if (tag=="TR") {
				FlushText();
				fTR = 1;
			}
			else if (tag=="A" && InsLink) {
				HrefStr = GetTagAtr(lbuf, tag, "HREF");
				if (OnlyExLink) {
					if (!StartsStr("http:", HrefStr) && !StartsStr("https:", HrefStr) && !StartsStr("mailto:", HrefStr))
						HrefStr = EmptyStr;
				}
				//可能なら絶対指定に
				do {
					if (HrefStr.IsEmpty() || BaseStr.IsEmpty()) break;
					if (StartsStr("file:", HrefStr)) break;
					if (StartsStr("http:", HrefStr) || StartsStr("https:", HrefStr)) break;
					if (StartsStr("mailto:", HrefStr)) break;
					HrefStr = BaseStr + HrefStr;
				} while (0);

				if (IsMarkdown && !HrefStr.IsEmpty()) TxtLineBuf += "[";
				tag_level++;
			}
			else if (tag=="/A" && InsLink) {
				if (!HrefStr.IsEmpty()) {
					if (IsMarkdown) TxtLineBuf += "]";
					tmpstr = LnkBraStr + HrefStr + LnkKetStr;
					if (AddCrLink) tmpstr = "\r\n" + tmpstr + "\r\n";
					TxtLineBuf += tmpstr;
				}
				HrefStr = EmptyStr;
				DecLevel(tag_level);
			}
			else if (tag=="IMG" && (UseAlt || AddImgSrc)) {
				TxtLineBuf += AltBraStr;
				if (UseAlt) {
					tmpstr = GetTagAtr(lbuf, tag, "ALT");
					if (tmpstr.IsEmpty()) tmpstr = DefAltStr;
					TxtLineBuf += tmpstr;
				}
				if (AddImgSrc) {
					UnicodeString fnam = GetTagAtr(lbuf, tag, "SRC");
					if (!fnam.IsEmpty()) {
						//可能なら絶対指定に
						do {
							if (BaseStr.IsEmpty()) break;
							if (StartsStr("file:", HrefStr) || StartsStr("http:", HrefStr) || StartsStr("https:", HrefStr)) break;
							fnam = BaseStr + fnam;
						} while (0);
						//"(ファイル名)" 後置
						if (IsMarkdown) {
							TxtLineBuf += AltKetStr;
							TxtLineBuf.cat_sprintf(_T("(%s)"), fnam.c_str());
						}
						//ファイル名付加
						else {
							if (UseAlt) TxtLineBuf.UCAT_TSTR(" ");
							TxtLineBuf += fnam;
						}
					}
				}
				if (!IsMarkdown) TxtLineBuf += AltKetStr;
			}
			else if (tag=="HEAD") {
				Skip = true;
			}
			else if (tag=="/HEAD" || tag=="BODY") {
				Skip   = false;
				fTITLE = 0;
			}
			else if (tag=="TITLE") {
				fTITLE ++;
			}
			else if (tag=="/TITLE") {
				fTITLE = 0;
			}
			else if (tag=="BASE") {
				tmpstr = GetTagAtr(lbuf, tag, "HREF");
				if (!tmpstr.IsEmpty()) {
					if (!EndsStr("/", tmpstr)) tmpstr.UCAT_TSTR("/");
					BaseStr = tmpstr;
				}
			}
			else if (tag=="META") {
				tmpstr = GetTagAtr(lbuf, tag, "NAME");
				if (!tmpstr.IsEmpty()) {
					if (tmpstr.CompareIC("description")==0)
						DescrStr = GetTagAtr(lbuf, tag, "CONTENT");
					else
					if (tmpstr.CompareIC("keywords")==0)
						KeywdStr = GetTagAtr(lbuf, tag, "CONTENT");
				}
			}
		}
	}
	FlushText();

	TitleStr = RefEntity(TitleStr);

	//連続空行の制限
	i = topline;
	int blkln = 0;
	while (i<TxtBuf->Count) {
		if (TxtBuf->Strings[i].IsEmpty()) {
			blkln++;
			if (blkln>BlankLnLimit) {
				TxtBuf->Delete(i);
			}
			else if (IsMarkdown && blkln>1 && i>0 && i<TxtBuf->Count-1) {
				if (TRegEx::IsMatch(TxtBuf->Strings[i + 1], NO_CRSPC_PTN)) {
					if (TxtBuf->Strings[i - 1].IsEmpty()) TxtBuf->Delete(i); else i++;
				}
				else {
					TxtBuf->Strings[i++] = "<br>";
				}
			}
			else {
				i++;
			}
		}
		else {
			blkln = 0;
			i++;
		}
	}

	//文字列置換
	if (rep_lst && rep_lst->Count>0) {
		TRegExOptions opt;  opt<<roIgnoreCase;
		for (int i=topline; i<TxtBuf->Count; i++) {
			UnicodeString s = TxtBuf->Strings[i];
			for (int j=0; j<rep_lst->Count; j++) {
				UnicodeString lbuf = rep_lst->Strings[j];
				UnicodeString f_s = get_tkn(lbuf, "\t");
				UnicodeString t_s = get_tkn_r(lbuf, "\t");
				if (is_regex_slash(f_s)) {
					f_s = exclude_top_end(f_s);
					if (f_s.IsEmpty()) continue;
					s = TRegEx::Replace(s, f_s, t_s, opt);
				}
				else {
					if (f_s.IsEmpty()) continue;
					s = ReplaceStr(s, f_s, t_s);
				}
			}
			TxtBuf->Strings[i] = s;
		}
	}

	//ヘッダ・フッタの挿入
	TmpBuf->Clear();
	for (int lopn = 0; lopn<2; lopn++) {
		if (lopn==0)
			TmpBuf->Text = HeadStr;
		else
			TmpBuf->Text = FootStr;
		if (TmpBuf->Count==0) continue;

		for (int i=0; i<TmpBuf->Count; i++) {
			//書式文字列の展開
			UnicodeString s = TmpBuf->Strings[i];
			for (int j=0; j<9; j++) {
				UnicodeString swd, rwd;
				switch (j) {
				case  0: swd = "$TITLE"; rwd = TitleStr;				  break;
				case  1: swd = "$DTIME"; rwd = DateTimeToStr(Now());	  break;
				case  2:
					swd = "$FNAME";
					if (!UrlStr.IsEmpty())
						rwd = UrlStr;
					else
						rwd = ExtractFileName(FileName);
					break;
				case  3:
					swd = "$FTIME"; rwd = FileTime;	break;
				case  4:
					swd = "$LINE1";
					rwd = StringOfChar(_T('-'), LineWidth);
					break;
				case  5:
					swd = "$LINE2";
					rwd = EmptyStr;
					for (int k=0; k<LineWidth/2; k++) rwd.UCAT_TSTR("─");
					break;
				case  6:
					swd = "$LINE3";
					rwd = EmptyStr;
					for (int k=0; k<LineWidth/2; k++) rwd.UCAT_TSTR("━");
					break;
				case  7:
					swd = "$DESCR"; rwd = DescrStr; break;
				case  8:
					swd = "$KEYWD"; rwd = KeywdStr; break;
				}
				s = ReplaceStr(s, swd, rwd);
			}
			//テキストに挿入
			if (lopn==0)
				TxtBuf->Insert(topline + i, s);	//ヘッダ
			else
				TxtBuf->Add(s);	//フッタ
		}
	}
}
//---------------------------------------------------------------------------
//指定コードでファイルに保存
//---------------------------------------------------------------------------
bool HtmConv::SaveToFileCP(UnicodeString fnam)
{
	try {
		std::unique_ptr<TEncoding> enc(TEncoding::GetEncoding(OutCodePage));
		TxtBuf->SaveToFile(fnam, enc.get());
		return true;
	}
	catch (...) {
		return false;
	}
}
//---------------------------------------------------------------------------
