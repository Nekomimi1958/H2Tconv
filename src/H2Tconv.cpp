//----------------------------------------------------------------------//
// H2Tconv																//
//																		//
//----------------------------------------------------------------------//
USEFORM("Unit1.cpp", H2TconvForm);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	//�~���[�e�b�N�X�ɂ���d�N����h�~
	HANDLE hMutex = ::CreateMutex(NULL, FALSE, _T("H2Tconv_is_running"));
	if(::GetLastError()==ERROR_ALREADY_EXISTS) {
        //��d�N���Ȃ�A������H2Tconv���A�N�e�B�u��
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
		Application->ShowMainForm = false;		//��\���ŋN��
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
