//----------------------------------------------------------------------//
// �t�@�C������	(UsrIniFile ���g�p)										//
// 																		//
//----------------------------------------------------------------------//
#ifndef FileHistoryH
#define FileHistoryH

//---------------------------------------------------------------------------
#include <System.IniFiles.hpp>
#include "UIniFile.h"

//---------------------------------------------------------------------------
#define MAX_FILEHISTORY   20	//�ő�t�@�C������

//---------------------------------------------------------------------------
class FileHistory
{
private:
	TMenuItem *HistMenu;

public:
	FileHistory(TMenuItem *menu=NULL, TNotifyEvent on_click=NULL);
	~FileHistory();

	TStringList *HistList;
	bool ShowDir;		//�f�B���N�g����\��
	bool ShowRel;		//���Ε\��

	void LoadHistory(UsrIniFile *fnam, UnicodeString sct=EmptyStr);
	void SaveHistory(UsrIniFile *fnam, UnicodeString sct=EmptyStr);
	void UpdateMenu();
	void AddHistory(UnicodeString fnam);
	UnicodeString GetHistory(int idx);
};
//---------------------------------------------------------------------------
#endif
