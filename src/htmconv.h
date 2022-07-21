//----------------------------------------------------------------------//
// HTML→テキスト変換処理クラス											//
//																		//
//----------------------------------------------------------------------//
#ifndef htmconvH
#define htmconvH

//---------------------------------------------------------------------------
#define DEF_LINE_WIDTH		80
#define DEF_BLANK_LN_LIMIT	5

//---------------------------------------------------------------------------
int check_UTF8(BYTE *bp, int size);
int get_MemoryCodePage(TMemoryStream *ms);

//---------------------------------------------------------------------------
#define NO_CRSPC_PTN	"^(#{1,6}|\\s*[\\*\\+\\-]|\\s*\\d+\\.)\\s.+"

//---------------------------------------------------------------------------
class HtmConv {
private:
	UnicodeString TxtLineBuf;
	TStringList *AlignList;
	bool Skip;
	bool fPRE, fXMP;
	int  BQ_level;

	void DelTagBlock(TStringList *lst, UnicodeString start_wd, UnicodeString end_wd);
	void DelAtrBlock(TStringList *lst, UnicodeString anam, UnicodeString aval);
	UnicodeString GetTagAtr(UnicodeString s, UnicodeString t, UnicodeString a);
	UnicodeString GetTag(UnicodeString s);
	void AddText(UnicodeString s);
	void FlushText();
	void AddLine(UnicodeString hrstr = EmptyStr, UnicodeString wdstr = EmptyStr, UnicodeString alstr = EmptyStr);

	void DecLevel(int &n)
	{
		n = std::max(n - 1, 0);
	}

public:
	HtmConv();
	~HtmConv();

	TStringList *HtmBuf;		//HTML文書入力バッファ
	TStringList *TxtBuf;		//テキスト出力バッファ
	UnicodeString  TempDir;
	int OutCodePage;			//ファイル保存時のコードページ

	bool IsMarkdown;			//Markdown 有効

	UnicodeString  HeadStr;		//ヘッダ
	UnicodeString  FootStr;		//フッタ
	UnicodeString  FileName;	//入力ファイル名
	UnicodeString  FileTime;	//ファイルのタイムスタンプ
	UnicodeString  UrlStr;		//URL
	UnicodeString  BaseStr;		//<base href=…>
	UnicodeString  TitleStr;	//タイトル
	UnicodeString  StatMsg;		//ステータスメッセージ

	int  LineWidth;				//桁数
	bool ForceCr;				//強制改行
	int  BlankLnLimit;			//連続空行制限
	bool NoPstHdr;				//見出し後を空けない
	UnicodeString HeaderStr[6];	//見出し
	UnicodeString HrLineStr;	//HR罫線
	UnicodeString IndentStr;	//インデント
	UnicodeString UlMarkStr;	//マーク
	bool ZenNumber;

	bool UseAlt;				//代替文字列で表示
	bool AddImgSrc;				//画像のファイル名を付加
	UnicodeString DefAltStr;	//デフォルト
	UnicodeString AltBraStr;	//画像 囲み
	UnicodeString AltKetStr;
	UnicodeString B_BraStr;		//太字 囲み
	UnicodeString B_KetStr;
	UnicodeString I_BraStr;		//斜体 囲み
	UnicodeString I_KetStr;
	UnicodeString U_BraStr;		//下線 囲み
	UnicodeString U_KetStr;
	UnicodeString CellSepStr;	//セル区切り

	bool InsLink;				//リンク挿入
	bool OnlyExLink;			//外部リンクのみ
	bool AddCrLink;				//前後に改行
	UnicodeString LnkBraStr;	//リンク 囲み
	UnicodeString LnkKetStr;

	UnicodeString InsLineStr;	//区切り罫線
	UnicodeString InsHrClass;	//指定クラスのDIVの前に罫線挿入
	bool InsHrSct;				//<section>の前に罫線挿入
	bool InsHrArt;				//<article>の前に罫線挿入
	bool InsHrNav;				//<nav>の前に罫線挿入

	UnicodeString DelBlkCls;	//削除部分の class リスト
	UnicodeString DelBlkId;		//削除部分の id リスト

	UnicodeString RefEntity(UnicodeString s);
	UnicodeString GetTitle(UnicodeString fnam);
	bool LoadFile(UnicodeString fnam);
	void Convert(TStringList *rep_lst = NULL);
	bool SaveToFileCP(UnicodeString fnam);
};
//---------------------------------------------------------------------------
#endif
