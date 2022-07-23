//----------------------------------------------------------------------//
// H2Tconv																//
//																		//
//----------------------------------------------------------------------//
USEFORM("Unit1.cpp", H2TconvForm);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	//ミューテックスにより二重起動を防止
	HANDLE hMutex = ::CreateMutex(NULL, FALSE, _T("H2Tconv_is_running"));
	if(::GetLastError()==ERROR_ALREADY_EXISTS) {
        //二重起動なら、既存のH2Tconvをアクティブに
		HWND hFrmWnd = ::FindWindow(_T("TH2TconvForm"),NULL);
		if (hFrmWnd) {
			HWND hApp = GetWindow(hFrmWnd, GW_OWNER);
			if (::IsIconic(hApp)) ::ShowWindow(hApp, SW_RESTORE);
			::SetForegroundWindow(::GetLastActivePopup(hApp));
		}
		::CloseHandle(hMutex);
		return -1;
	}

	try {
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->ShowMainForm = false;		//非表示で起動
		Application->Title = "H2Tconv";
		Application->CreateForm(__classid(TH2TconvForm), &H2TconvForm);
		Application->Run();
	}
	catch (Exception &exception) {
		Application->ShowException(&exception);
	}
	catch (...) {
		try {
			throw Exception("");
		}
		catch (Exception &exception) {
			Application->ShowException(&exception);
		}
	}

	::CloseHandle(hMutex);
	return 0;
}
//---------------------------------------------------------------------------
