#include "MainApp.h"

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit() {
    wxImage::AddHandler(new wxPNGHandler);

    MainFrame* frame = new MainFrame("C++ GUI");
    frame->Show();
    return true;
}
