//----------------------------------------------------------------------//
// HTML���e�L�X�g�ϊ������N���X											//
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

	TStringList *HtmBuf;		//HTML�������̓o�b�t�@
	TStringList *TxtBuf;		//�e�L�X�g�o�̓o�b�t�@
	UnicodeString  TempDir;
	int OutCodePage;			//�t�@�C���ۑ����̃R�[�h�y�[�W

	bool IsMarkdown;			//Markdown �L��

	UnicodeString  HeadStr;		//�w�b�_
	UnicodeString  FootStr;		//�t�b�^
	UnicodeString  FileName;	//���̓t�@�C����
	UnicodeString  FileTime;	//�t�@�C���̃^�C���X�^���v
	UnicodeString  UrlStr;		//URL
	UnicodeString  BaseStr;		//<base href=�c>
	UnicodeString  TitleStr;	//�^�C�g��
	UnicodeString  StatMsg;		//�X�e�[�^�X���b�Z�[�W

	int  LineWidth;				//����
	bool ForceCr;				//�������s
	int  BlankLnLimit;			//�A����s����
	bool NoPstHdr;				//���o������󂯂Ȃ�
	UnicodeString HeaderStr[6];	//���o��
	UnicodeString HrLineStr;	//HR�r��
	UnicodeString IndentStr;	//�C���f���g
	UnicodeString UlMarkStr;	//�}�[�N
	bool ZenNumber;

	bool UseAlt;				//��֕�����ŕ\��
	bool AddImgSrc;				//�摜�̃t�@�C������t��
	UnicodeString DefAltStr;	//�f�t�H���g
	UnicodeString AltBraStr;	//�摜 �͂�
	UnicodeString AltKetStr;
	UnicodeString B_BraStr;		//���� �͂�
	UnicodeString B_KetStr;
	UnicodeString I_BraStr;		//�Α� �͂�
	UnicodeString I_KetStr;
	UnicodeString U_BraStr;		//���� �͂�
	UnicodeString U_KetStr;
	UnicodeString CellSepStr;	//�Z����؂�

	bool InsLink;				//�����N�}��
	bool OnlyExLink;			//�O�������N�̂�
	bool AddCrLink;				//�O��ɉ��s
	UnicodeString LnkBraStr;	//�����N �͂�
	UnicodeString LnkKetStr;

	UnicodeString InsLineStr;	//��؂�r��
	UnicodeString InsHrClass;	//�w��N���X��DIV�̑O�Ɍr���}��
	bool InsHrSct;				//<section>�̑O�Ɍr���}��
	bool InsHrArt;				//<article>�̑O�Ɍr���}��
	bool InsHrNav;				//<nav>�̑O�Ɍr���}��

	UnicodeString DelBlkCls;	//�폜������ class ���X�g
	UnicodeString DelBlkId;		//�폜������ id ���X�g

	UnicodeString RefEntity(UnicodeString s);
	UnicodeString GetTitle(UnicodeString fnam);
	bool LoadFile(UnicodeString fnam);
	void Convert(TStringList *rep_lst = NULL);
	bool SaveToFileCP(UnicodeString fnam);
};
//---------------------------------------------------------------------------
#endif
