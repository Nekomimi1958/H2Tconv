//----------------------------------------------------------------------//
// Shell 関連															//
// 最低限のドロップ処理のみ												//
//----------------------------------------------------------------------//
#ifndef UserShellH
#define UserShellH

//---------------------------------------------------------------------------
#include <shlobj.h>

//---------------------------------------------------------------------------
#define WM_FORM_DROPPED  (WM_APP + 10)

//---------------------------------------------------------------------------
extern HWND TargetHandle;
extern int  DroppedMode;
extern TStringList *DroppedList;
extern TWinControl *TargetWndCtrl;

//---------------------------------------------------------------------------
void DropInitialize(HWND hWnd, TWinControl *wp);
void DropUninitialize();

//---------------------------------------------------------------------------
#endif
