#include "MainApp.h"

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit() {
    wxImage::AddHandler(new wxPNGHandler);

    MainFrame* frame = MainFrame::GetInstance();
    frame->Show();
    return true;
}
