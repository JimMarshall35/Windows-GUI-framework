#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "View.h"
#include "ViewModel.h"
#include "CommonWidgets.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

    CWPF::RegisterCommonWidgets();

    CWPF::View v(CWPF::ViewInitArgs{ L"MainWindow.xml" });

    std::shared_ptr<CWPF::ViewModel> vm = std::make_shared<CWPF::TestViewModelMain>();

    v.SetDataContext(vm);
    v.Show();

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
