//----------------------------------------------------------------------//
// H2Tconv																//
//	メインフォーム														//
//----------------------------------------------------------------------//
#ifndef Unit1H
#define Unit1H

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.AppEvnts.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.Clipbrd.hpp>
#include <Vcl.Mask.hpp>
#include "usr_shell.h"
#include "htmconv.h"

//---------------------------------------------------------------------------
#define SUPPORT_URL	_T("http://nekomimi.la.coocan.jp/")

//---------------------------------------------------------------------------
typedef HWND (WINAPI *FUNC_HtmlHelp)(HWND, LPCWSTR, UINT, DWORD);

// 出力先(DestMode)
#define DSTMOD_CLIPBD  0	//クリップボード/
#define DSTMOD_ORGDIR  1	//元と同じ
#define DSTMOD_SELDIR  2	//場所指定

// 最小画面サイズ
#define NRM_MIN_WD	600
#define NRM_MIN_HI	580
#define CMP_MIN_WD	224
#define CMP_MIN_HI	110

//---------------------------------------------------------------------------
class TH2TconvForm : public TForm
{
__published:	// IDE 管理のコンポーネント
	TAction *AddRepAction;
	TAction *ChgRepAction;
	TAction *ConvertAction;
	TAction *CopyListAction;
	TAction *DelRepAction;
	TAction *DownRepAction;
	TAction *LoadListAction;
	TAction *PlaySoundAction;
	TAction *SaveIniAction;
	TAction *SaveListAction;
	TAction *SelectAllAction;
	TAction *UpRepAction;
	TActionList *ActionList1;
	TApplicationEvents *ApplicationEvents1;
	TBitBtn *ConvBtn;
	TBitBtn *ConvBtnC;
	TButton *AddBtn;
	TButton *AddItemBtn;
	TButton *ChgItemBtn;
	TButton *ClrBtn;
	TButton *DelBtn;
	TButton *DelItemBtn;
	TButton *DowItemBtn;
	TButton *DownRepBtn;
	TButton *InputBtn;
	TButton *PasteBtn;
	TButton *PlySndBtn;
	TButton *RefAppBtn;
	TButton *RefDstBtn;
	TButton *RefMac1Btn;
	TButton *RefMac2Btn;
	TButton *RefSndBtn;
	TButton *SortBtn;
	TButton *UpItemBtn;
	TButton *UpRepBtn;
	TCheckBox *AddNoCheck;
	TCheckBox *AltCheck;
	TCheckBox *DropSubCheck;
	TCheckBox *ExtLnkCheck;
	TCheckBox *FrcCrCheck;
	TCheckBox *ImgSrcCheck;
	TCheckBox *InsFtCheck;
	TCheckBox *InsHdCheck;
	TCheckBox *InsHrArtCheckBox;
	TCheckBox *InsHrNavCheckBox;
	TCheckBox *InsHrSctCheckBox;
	TCheckBox *JoinCheck;
	TCheckBox *KillCheck;
	TCheckBox *LinkCheck;
	TCheckBox *LinkCrCheck;
	TCheckBox *MarkdownCheck;
	TCheckBox *NaturalCheck;
	TCheckBox *OpenAppCheck;
	TCheckBox *PstHdrCheck;
	TCheckBox *SureKillCheck;
	TCheckBox *Tit2NamCheck;
	TCheckBox *TitCvExChCheck;
	TCheckBox *TitCvSpcCheck;
	TCheckBox *TopMostCheck;
	TCheckBox *UseTrashCheck;
	TCheckBox *ZenNoCheck;
	TCheckListBox *RepCheckListBox;
	TComboBox *CmpPosCombo;
	TComboBox *CodePageComboBox;
	TEdit *AltKetEdit;
	TEdit *AppNameEdit;
	TEdit *B_KetEdit;
	TEdit *DstDirEdit;
	TEdit *I_KetEdit;
	TEdit *LnkKetEdit;
	TEdit *U_KetEdit;
	TGroupBox *GrpBox1_1;
	TGroupBox *GrpBox1_2;
	TGroupBox *GrpBox1_3;
	TGroupBox *GrpBox2_1;
	TGroupBox *GrpBox2_2;
	TGroupBox *GrpBox2_3;
	TGroupBox *GrpBox2_4;
	TGroupBox *GrpBox2_5;
	TGroupBox *GrpBox2_6;
	TGroupBox *GrpBox2_7;
	TGroupBox *GrpBox2_8;
	TGroupBox *GrpBox2_9;
	TGroupBox *GrpBox2_10;
	TGroupBox *GrpBox2_11;
	TGroupBox *GrpBox2_12;
	TGroupBox *GrpBox3_1;
	TGroupBox *GrpBox3_2;
	TImage *ArrowImg;
	TLabel *Label21;
	TLabel *Label23;
	TLabeledEdit *AltBraEdit;
	TLabeledEdit *B_BraEdit;
	TLabeledEdit *BlkLmtEdit;
	TLabeledEdit *CelSepEdit;
	TLabeledEdit *DefAltEdit;
	TLabeledEdit *DelBlkClsEdit;
	TLabeledEdit *DelBlkIdEdit;
	TLabeledEdit *EndSoundEdit;
	TLabeledEdit *FilExtEdit;
	TLabeledEdit *FilNamEdit;
	TLabeledEdit *FromStrEdit;
	TLabeledEdit *H1Edit;
	TLabeledEdit *H2Edit;
	TLabeledEdit *H3Edit;
	TLabeledEdit *H4Edit;
	TLabeledEdit *H5Edit;
	TLabeledEdit *H6Edit;
	TLabeledEdit *HrStrEdit;
	TLabeledEdit *I_BraEdit;
	TLabeledEdit *IndentEdit;
	TLabeledEdit *InsHrClsEdit;
	TLabeledEdit *InsLineStrEdit;
	TLabeledEdit *LnkBraEdit;
	TLabeledEdit *LnWidEdit;
	TLabeledEdit *MarkEdit;
	TLabeledEdit *TitLmtEdit;
	TLabeledEdit *ToStrEdit;
	TLabeledEdit *U_BraEdit;
	TListBox *HtmFileList;
	TMemo *FootMemo;
	TMemo *HeadMemo;
	TMenuItem *AbautInf1Item;
	TMenuItem *AboutInf2Item;
	TMenuItem *AboutItem;
	TMenuItem *C1;
	TMenuItem *DefaultIniItem;
	TMenuItem *GoWebItem;
	TMenuItem *HelpItem;
	TMenuItem *L1;
	TMenuItem *LoadIniItem;
	TMenuItem *PopDatTimItem;
	TMenuItem *PopDescItem;
	TMenuItem *PopFnameItem;
	TMenuItem *PopFtimeItem;
	TMenuItem *PopKeywdItem;
	TMenuItem *PopLine1Item;
	TMenuItem *PopLine2Item;
	TMenuItem *PopLine3Item;
	TMenuItem *PopTitleItem;
	TMenuItem *SaveAsIniItem;
	TMenuItem *SaveIniItem;
	TMenuItem *SaveListItem;
	TMenuItem *SelectAllItem;
	TMenuItem *Sep_1;
	TMenuItem *Sep_m_1;
	TMenuItem *Sep_m_2;
	TMenuItem *Sep_p_1;
	TMenuItem *Sep_p_2;
	TOpenDialog *OpenDialog1;
	TPageControl *PageControl1;
	TPanel *CmpBtnPanel;
	TPanel *Panel1;
	TPanel *Panel1_1;
	TPanel *Panel1_1a;
	TPanel *Panel1_1b;
	TPanel *Panel1_2;
	TPanel *Panel2;
	TPanel *Panel3;
	TPopupMenu *PopupMenu1;
	TPopupMenu *PopupMenu2;
	TPopupMenu *ToolMenu;
	TRadioButton *ClipRadioBtn;
	TRadioButton *SamDirRadioBtn;
	TRadioButton *SetDirRadioBtn;
	TSaveDialog *SaveDialog1;
	TSpeedButton *CompactBtn;
	TSpeedButton *MenuBtn;
	TStatusBar *StatusBar1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TTabSheet *TabSheet3;
	TTabSheet *TabSheet4;
	TTabSheet *TabSheet5;
	TTimer *ScrollTimer;
	TUpDown *BlkLmtUpDown;
	TUpDown *LnWidUpDown;
	TUpDown *TitLmtUpDown;
	TAction *MenuAction;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall CompactBtnClick(TObject *Sender);
	void __fastcall ConvertExecute(TObject *Sender);
	void __fastcall AddBtnClick(TObject *Sender);
	void __fastcall InputBtnClick(TObject *Sender);
	void __fastcall PasteBtnClick(TObject *Sender);
	void __fastcall DelBtnClick(TObject *Sender);
	void __fastcall ClrBtnClick(TObject *Sender);
	void __fastcall SortBtnClick(TObject *Sender);
	void __fastcall NaturalCheckClick(TObject *Sender);
	void __fastcall UpItemBtnClick(TObject *Sender);
	void __fastcall DowItemBtnClick(TObject *Sender);
	void __fastcall HtmFileListKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RefAppBtnClick(TObject *Sender);
	void __fastcall LnWidEditChange(TObject *Sender);
	void __fastcall BlkLmtEditaChange(TObject *Sender);
	void __fastcall RefDstBtnClick(TObject *Sender);
	void __fastcall DestRadioBtnClick(TObject *Sender);
	void __fastcall JoinCheckClick(TObject *Sender);
	void __fastcall HtmFileListClick(TObject *Sender);
	void __fastcall FilNamEditChange(TObject *Sender);
	void __fastcall RefMac1BtnClick(TObject *Sender);
	void __fastcall PopRefItemClick(TObject *Sender);
	void __fastcall RefSndBtnClick(TObject *Sender);
	void __fastcall TopMostCheckClick(TObject *Sender);
	void __fastcall UrlLabelClick(TObject *Sender);
	void __fastcall ConvertActionUpdate(TObject *Sender);
	void __fastcall Tit2NamCheckClick(TObject *Sender);
	void __fastcall NumberEditExit(TObject *Sender);
	void __fastcall TitLmtEditChange(TObject *Sender);
	void __fastcall LoadIniItemClick(TObject *Sender);
	void __fastcall SaveAsIniItemClick(TObject *Sender);
	void __fastcall DefaultIniItemClick(TObject *Sender);
	void __fastcall HelpItemClick(TObject *Sender);
	void __fastcall AbautInfItemDrawItem(TObject *Sender, TCanvas *ACanvas, TRect &ARect, bool Selected);
	void __fastcall GoWebItemClick(TObject *Sender);
	void __fastcall AbautMeasureItem(TObject *Sender, TCanvas *ACanvas, int &Width, int &Height);
	void __fastcall PlaySoundActionExecute(TObject *Sender);
	void __fastcall PlaySoundActionUpdate(TObject *Sender);
	void __fastcall ApplicationEvents1Message(tagMSG &Msg, bool &Handled);
	void __fastcall RepCheckListBoxClick(TObject *Sender);
	void __fastcall RepCheckListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall AddRepActionExecute(TObject *Sender);
	void __fastcall AddRepActionUpdate(TObject *Sender);
	void __fastcall ChgRepActionExecute(TObject *Sender);
	void __fastcall ChgRepActionUpdate(TObject *Sender);
	void __fastcall DelRepActionExecute(TObject *Sender);
	void __fastcall DelRepActionUpdate(TObject *Sender);
	void __fastcall UpRepActionExecute(TObject *Sender);
	void __fastcall UpRepActionUpdate(TObject *Sender);
	void __fastcall DownRepActionExecute(TObject *Sender);
	void __fastcall DownRepActionUpdate(TObject *Sender);
	void __fastcall PageControl1Change(TObject *Sender);
	void __fastcall SaveListActionExecute(TObject *Sender);
	void __fastcall SaveListActionUpdate(TObject *Sender);
	void __fastcall LoadListActionExecute(TObject *Sender);
	void __fastcall CopyListActionExecute(TObject *Sender);
	void __fastcall CopyListActionUpdate(TObject *Sender);
	void __fastcall HtmFileListStartDrag(TObject *Sender, TDragObject *&DragObject);
	void __fastcall HtmFileListDragOver(TObject *Sender, TObject *Source, int X, int Y, TDragState State, bool &Accept);
	void __fastcall HtmFileListDragDrop(TObject *Sender, TObject *Source, int X, int Y);
	void __fastcall HtmFileListEndDrag(TObject *Sender, TObject *Target, int X, int Y);
	void __fastcall ScrollTimerTimer(TObject *Sender);
	void __fastcall SelectAllActionExecute(TObject *Sender);
	void __fastcall SelectAllActionUpdate(TObject *Sender);
	void __fastcall FilExtEditChange(TObject *Sender);
	void __fastcall MarkdownCheckClick(TObject *Sender);
	void __fastcall SaveIniActionExecute(TObject *Sender);
	void __fastcall SaveIniActionUpdate(TObject *Sender);
	void __fastcall DstDirEditChange(TObject *Sender);
	void __fastcall MenuActionUpdate(TObject *Sender);
	void __fastcall MenuActionExecute(TObject *Sender);

private:	// ユーザー宣言
	UnicodeString IniName;

	bool OptLocked;

	UnicodeString VersionStr;	//バージョン
	UnicodeString LastDir;		//参照ディレクトリ
	UnicodeString OrgDir;		//変換元のディレクトリ
	UnicodeString LastIniDir;	//設定ディレクトリ
	UnicodeString LastTxtDir;

	int  DestMode;				//出力先
	int  PopTag;
	bool Compact;
	bool ConvReady;
	bool Converting;
	bool AutoStart;
	bool InhUpdateInf;
	int  PrevListIdx;

	HtmConv *HC;

	void __fastcall WmDropped(TMessage &msg);
	void __fastcall WMMove(TMessage &Msg);
	void __fastcall AddStrToList(UnicodeString s);
	UnicodeString __fastcall UrlToName(UnicodeString s);
	UnicodeString __fastcall TitToName(UnicodeString s);
	UnicodeString __fastcall AddSerNo(UnicodeString s);
	void __fastcall UpdateInf(bool chgf);
	void __fastcall SetFrmStyle();
	void __fastcall LoadIniFile(UnicodeString fnam = EmptyStr);
	void __fastcall SaveIniFile(UnicodeString fnam = EmptyStr);
	void __fastcall ReplaceEx(UnicodeString &src);
	TColor __fastcall MdWinColor(UnicodeString text, UnicodeString s, bool i_sw = false);

public:		// ユーザー宣言
	__fastcall TH2TconvForm(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_DROPPED,	TMessage,	WmDropped)
		VCL_MESSAGE_HANDLER(WM_MOVE,			TMessage,	WMMove)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TH2TconvForm *H2TconvForm;
//---------------------------------------------------------------------------
#endif
