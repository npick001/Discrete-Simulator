#include "MainApp.h"
#include <string>
#include <iostream>
#include <fstream>

// Callback function to handle debug reports
int DebugReport(int reportType, char* message, int* returnValue)
{
    if (reportType == _CRT_ERROR || reportType == _CRT_ASSERT || reportType == _CRT_WARN)
    {
        // Output the error message
        if (message == nullptr) {
            throw std::runtime_error("MESSAGE IS NULLPTR");
        }

        //std::cerr << std::string(message) << std::endl;
    }

    // Return 1 to break into the debugger
    *returnValue = 1;
    return true;
}

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit() {
    
    // Enable memory leak detection
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // Set the debug report mode to include file names and line numbers
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);

    HANDLE hLogFile;
    LPCWSTR path = L"..\\Output Files\\MemLeaks.txt";
    hLogFile = CreateFile(path, GENERIC_WRITE,
        FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, hLogFile);

    // Set the debug report callback function
    //_CrtSetReportHook(DebugReport);

    wxImage::AddHandler(new wxPNGHandler);

    MainFrame* frame = MainFrame::GetInstance();
    frame->Show();
    return true;
}
