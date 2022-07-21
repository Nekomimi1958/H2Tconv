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
class TDropSource : public IDropSource
{
public:
	TDropSource() : FRefCount(0) { }
	HRESULT __stdcall QueryInterface(REFIID riid, void **ppv);
	ULONG   __stdcall AddRef();
	ULONG   __stdcall Release();
	HRESULT __stdcall QueryContinueDrag(BOOL fEsc, DWORD grfKeyState);
	HRESULT __stdcall GiveFeedback(DWORD dwEffect);

private:
	LONG FRefCount;
};

//---------------------------------------------------------------------------
class TDropTargetBase : public IDropTarget
{
protected:
	unsigned int refCount;

	TDropTargetBase();
	virtual ~TDropTargetBase() { }

	virtual HRESULT __stdcall QueryInterface(const IID& iid, void **ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();
};

//---------------------------------------------------------------------------
class TDropTarget : public TDropTargetBase
{
public:
	static void CreateInstance(IDropTarget **pp)
	{
		if (pp) {
			TDropTarget *p = new TDropTarget();
			p->QueryInterface(IID_IDropTarget, (void **)pp);
		}
	}

private:
	virtual HRESULT __stdcall DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
	virtual HRESULT __stdcall DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
	virtual HRESULT __stdcall DragLeave();
	virtual HRESULT __stdcall Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
};

//---------------------------------------------------------------------------
void DropInitialize(HWND hWnd, TWinControl *wp);
void DropUninitialize();

//---------------------------------------------------------------------------
#endif
