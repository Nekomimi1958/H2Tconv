//----------------------------------------------------------------------//
// ファイル履歴	(UsrIniFile を使用)										//
// 																		//
//----------------------------------------------------------------------//
#ifndef FileHistoryH
#define FileHistoryH

//---------------------------------------------------------------------------
#include <System.IniFiles.hpp>
#include "UIniFile.h"

//---------------------------------------------------------------------------
#define MAX_FILEHISTORY   20	//最大ファイル履歴数

//---------------------------------------------------------------------------
class FileHistory
{
private:
	TMenuItem *HistMenu;

public:
	FileHistory(TMenuItem *menu=NULL, TNotifyEvent on_click=NULL);
	~FileHistory();

	TStringList *HistList;
	bool ShowDir;		//ディレクトリを表示
	bool ShowRel;		//相対表示

	void LoadHistory(UsrIniFile *fnam, UnicodeString sct=EmptyStr);
	void SaveHistory(UsrIniFile *fnam, UnicodeString sct=EmptyStr);
	void UpdateMenu();
	void AddHistory(UnicodeString fnam);
	UnicodeString GetHistory(int idx);
};
//---------------------------------------------------------------------------
#endif
