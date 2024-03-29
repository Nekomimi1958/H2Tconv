//----------------------------------------------------------------------//
// Shell 関連															//
// 最低限のドロップ処理のみ												//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop

#include "usr_shell.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
HWND TargetHandle = NULL;
int  DroppedMode  = 0;				//ドロップのモード

TStringList *DroppedList   = NULL;	//ドロップされたファイルリスト
TWinControl *TargetWndCtrl = NULL;	//ドロップを受け入れるウィンドウ

//---------------------------------------------------------------------------
//ドロップのモードを取得 (コピーのみ)
//---------------------------------------------------------------------------
DWORD get_DropMode(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt)
{
	DWORD d_mode = DROPEFFECT_NONE;
	const POINT p = {pt.x, pt.y};
	HWND hWnd = ::WindowFromPoint(p);
	if (TargetWndCtrl && hWnd==TargetWndCtrl->Handle) {
		DroppedMode = d_mode = DROPEFFECT_COPY;
	}
	return d_mode;
}

//---------------------------------------------------------------------------
// TDropSource クラス
//---------------------------------------------------------------------------
HRESULT __stdcall TDropSource::QueryInterface(REFIID riid, void **ppv)
{
	IUnknown *punk = NULL;
	if		(riid == IID_IUnknown)		punk = static_cast<IUnknown*>(this);
	else if (riid == IID_IDropSource)	punk = static_cast<IDropSource*>(this);

	*ppv = punk;
	if (punk) {
		punk->AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}
//---------------------------------------------------------------------------
ULONG __stdcall TDropSource::AddRef()
{
	InterlockedIncrement(&FRefCount);
	return (ULONG)FRefCount;
}
//---------------------------------------------------------------------------
ULONG __stdcall TDropSource::Release()
{
	ULONG ret = (ULONG)InterlockedDecrement(&FRefCount);
	if (ret==0) delete this;
	return ret;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TDropSource::QueryContinueDrag(BOOL fEsc, DWORD grfKeyState)
{
	//ESC、または両方のボタンが押されたら中止
	if (fEsc || (grfKeyState & (MK_LBUTTON|MK_RBUTTON))==(MK_LBUTTON|MK_RBUTTON))
		return DRAGDROP_S_CANCEL;
	//マウスボタンが離された場合はドロップ処理へ
	if ((grfKeyState & (MK_LBUTTON|MK_RBUTTON))==0)
		return DRAGDROP_S_DROP;
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TDropSource::GiveFeedback(DWORD dwEffect)
{
	return DRAGDROP_S_USEDEFAULTCURSORS;
}

//---------------------------------------------------------------------------
// TDropTarget クラス
//---------------------------------------------------------------------------
TDropTargetBase::TDropTargetBase()
{
	refCount = 0;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TDropTargetBase::QueryInterface(const IID& iid, void **ppv)
{
	if (IsEqualIID(iid, IID_IUnknown) || IsEqualIID(iid, IID_IDropTarget)) {
		*ppv = static_cast<IDropTarget *>(this);
	}
	else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	AddRef();
	return S_OK;
}
//---------------------------------------------------------------------------
ULONG __stdcall TDropTargetBase::AddRef()
{
	refCount++;
	return refCount;
}
//---------------------------------------------------------------------------
ULONG __stdcall TDropTargetBase::Release()
{
	refCount--;
	if (refCount>0) return refCount;
	delete this;
	return 0;
}

//---------------------------------------------------------------------------
// TDropTarget クラス
//---------------------------------------------------------------------------
HRESULT __stdcall TDropTarget::DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
	*pdwEffect = get_DropMode(pDataObj, grfKeyState, pt);
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TDropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
	*pdwEffect = get_DropMode(NULL, grfKeyState, pt);
	return (*pdwEffect==DROPEFFECT_NONE)? DRAGDROP_S_CANCEL : S_OK;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TDropTarget::DragLeave()
{
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TDropTarget::Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
	if (!DroppedList) return E_FAIL;
	DroppedList->Clear();

	FORMATETC formatetc;
	formatetc.cfFormat = CF_HDROP;
	formatetc.ptd	   = NULL;
	formatetc.dwAspect = DVASPECT_CONTENT;
	formatetc.lindex   = -1;
	formatetc.tymed    = TYMED_HGLOBAL;
	STGMEDIUM medium;
	HRESULT hr;

	//ドロップされたファイルのリストを作成
	if (pDataObj->QueryGetData(&formatetc)==S_OK) {
		hr = pDataObj->GetData(&formatetc, &medium);	if (FAILED(hr)) return hr;
		HDROP dp = (HDROP)::GlobalLock(medium.hGlobal);
		UINT f_cnt = ::DragQueryFile(dp, 0xFFFFFFFF, NULL, 0);
		UnicodeString fnam;
		for (UINT i=0; i<f_cnt; i++) {
			fnam.SetLength(MAX_PATH);
			int len = ::DragQueryFile(dp, i, fnam.c_str(),fnam.Length());
			fnam.SetLength(len);
			if (faDirectory & FileGetAttr(fnam)) fnam += "\\";
			DroppedList->Add(fnam);
		}
		::GlobalUnlock(medium.hGlobal);
		ReleaseStgMedium(&medium);
	}
	//リンク項目
	else {
		UnicodeString url;
		formatetc.cfFormat = ::RegisterClipboardFormat(CFSTR_INETURL);
		hr = pDataObj->QueryGetData(&formatetc);		if (FAILED(hr)) return hr;
		hr = pDataObj->GetData(&formatetc, &medium);	if (FAILED(hr)) return hr;
		url = (LPTSTR)::GlobalLock(medium.hGlobal);
		::GlobalUnlock(medium.hGlobal);
		ReleaseStgMedium(&medium);
		if (!url.IsEmpty()) {
			DroppedList->Add(url);
			DroppedMode = DROPEFFECT_LINK;
		}
	}

	//メインフォームにドロップ処理を要求
	if (DroppedList && DroppedList->Count>0) ::SendMessage(TargetHandle, WM_FORM_DROPPED, 0, 0);

	return S_OK;
}

//---------------------------------------------------------------------------
//ドロップ処理の初期化
//---------------------------------------------------------------------------
void DropInitialize(HWND hWnd, TWinControl *wp)
{
	TargetHandle  = hWnd;
	TargetWndCtrl = wp;

	::OleInitialize(NULL);

	DroppedList = new TStringList();

	//ドロップターゲットを登録
	IDropTarget *pDropTarget = NULL;
	TDropTarget::CreateInstance(&pDropTarget);
	if (pDropTarget) {
		RegisterDragDrop(TargetHandle, pDropTarget);
		pDropTarget->Release();
	}
}
//---------------------------------------------------------------------------
//ドロップの終了時処理
//---------------------------------------------------------------------------
void DropUninitialize()
{
	RevokeDragDrop(TargetHandle);

	delete DroppedList;

	::OleUninitialize();
}
//---------------------------------------------------------------------------
