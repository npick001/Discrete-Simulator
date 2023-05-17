#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, "Dynamic GUI Application", wxDefaultPosition, wxSize(800, 600)) {
   
    //_mainSizer = new wxBoxSizer(wxHORIZONTAL);
    //
    //Create menus
    auto* file_menu = new wxMenu();
    auto* edit_menu = new wxMenu();
    auto* settings = new wxMenu();
    auto* stat_menu = new wxMenu();
    
    // FILE MENU
    file_menu->Append(wxID_OPEN);
    file_menu->Append(wxID_SAVE);
    file_menu->Append(wxID_SAVEAS);
    file_menu->Append(ID_Exit, "&Exit\tAlt-F4", "Exit the application");

    // EDIT MENU
    edit_menu->Append(wxID_UNDO);
    edit_menu->Append(wxID_REDO);
    edit_menu->AppendSeparator();
    edit_menu->Append(wxID_CUT);
    edit_menu->Append(wxID_COPY);
    edit_menu->Append(wxID_PASTE);

    // SETTINGS MENU
    settings->Append(ID_Model_Settings, "&Model Settings", "Change Model Settings");

    //// STAT MENU
    //// LOTS of mem leaks here
    ////_statMenu->Append(ID_Input_Analyzer, "&Input Analyzer", "Shows up in bottom left when clicked and hovered over");

    // Use menus in menu bar
    // mem leaks here
    auto* menu_bar = new wxMenuBar();
    menu_bar->Append(file_menu, "&File");
    menu_bar->Append(edit_menu, "&Edit");
    menu_bar->Append(settings, "&Settings");
    //_menuBar->Append(_statMenu, "&Statistics");
    SetMenuBar(menu_bar);

    // Create status bar
    CreateStatusBar();
    SetStatusText("Welcome to the Dynamic GUI Application!");

    int minPaneSize = 150;
    wxSize window_size = this->GetClientSize();

    /******************
    SPLITTERS FOR TEMPLATE
    ******************/
    auto left_splitter = new wxSplitterWindow(this);
    auto right_splitter = new wxSplitterWindow(left_splitter);
    auto middle_splitter = new wxSplitterWindow(right_splitter);

    auto* left = new LeftToolbar(left_splitter);
    auto* middle_top = new Canvas(middle_splitter);
    auto* middle_bottom = new wxTextCtrl(middle_splitter, wxID_ANY,
        wxEmptyString, wxDefaultPosition, wxDefaultSize,
        0, wxDefaultValidator, "Console");

    auto* right = new wxPanel(right_splitter);

    // Get the image file path
    wxString imagePath = wxStandardPaths::Get().GetExecutablePath();
    wxFileName fileName(imagePath);
    fileName.RemoveLastDir();
    fileName.RemoveLastDir();
    fileName.AppendDir("Images");
    fileName.SetFullName("KaiQuiSeth.png");
    imagePath = fileName.GetFullPath();
    //wxLogMessage(imagePath);

    // populating toolbar with dummy nodes
   /* for (int i = 0; i < 8; i++) {
        wxString nodeName = "test " + std::to_string(i);
        left->AddNode(new GenericNode(nodeName, imagePath));
    }*/

    // background colors
    left->SetBackgroundColour(*wxCYAN);
    middle_top->SetBackgroundColour(*wxBLACK);
    middle_bottom->SetBackgroundColour(*wxRED);
    right->SetBackgroundColour(*wxBLUE);

    // minimum pane sizes
    left_splitter->SetMinimumPaneSize(minPaneSize);
    middle_splitter->SetMinimumPaneSize(minPaneSize);
    right_splitter->SetMinimumPaneSize(minPaneSize);

    // splitting
    middle_splitter->SplitHorizontally(middle_top, middle_bottom);
    right_splitter->SplitVertically(middle_splitter, right);
    left_splitter->SplitVertically(left, right_splitter);

    //SetSizer(_mainSizer);
    
    // Bind the events 
    this->Bind(wxEVT_MENU, &MainFrame::OnOpen, this, wxID_OPEN);
    this->Bind(wxEVT_MENU, &MainFrame::OnSave, this, wxID_SAVE);
    this->Bind(wxEVT_MENU, &MainFrame::OnSaveAs, this, wxID_SAVEAS);
    this->Bind(wxEVT_MENU, &MainFrame::OnExit, this, ID_Exit);
    this->Bind(wxEVT_MENU, &MainFrame::OnClickAnalyzer, this, ID_Input_Analyzer);
}

MainFrame::~MainFrame() {

}

void MainFrame::OnOpen(wxCommandEvent& event) {

    //wxLogMessage("Inside OnOpen");

    try {
        // open the file explorer
        wxFileDialog openFileDialog(this, _("Open file"), "", "", "All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

        if (openFileDialog.ShowModal() == wxID_CANCEL) {
            return;
        }

        wxString path = openFileDialog.GetPath();
        wxMessageBox("Selected file: " + path, "Info", wxOK | wxICON_INFORMATION);
    }
    catch (const std::exception& e) {
        wxLogError("Exception caught in OnOpen: %s", e.what());
    }
    catch (...) {
        wxLogError("Unknown exception caught in OnOpen");
    }

}
void MainFrame::OnSave(wxCommandEvent& event) {
    // save the file 
    // will need to check if file has been saved already
}
void MainFrame::OnSaveAs(wxCommandEvent& event) {

    try {
        // save the file to the browsed location
        // will need to choose the file type
        wxFileDialog saveFileDialog(this, _("Save XYZ file"), "", "",
            "XYZ files (*.xyz)|*.xyz", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

        if (saveFileDialog.ShowModal() == wxID_CANCEL)
            return;     // the user changed idea...

        // save the current contents in the file;
        // this can be done with e.g. wxWidgets output streams:
        wxFileOutputStream output_stream(saveFileDialog.GetPath());
        if (!output_stream.IsOk())
        {
            wxLogError("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
            return;
        }
    }
    catch (const std::exception& e) {
        wxLogError("Exception caught in OnOpen: %s", e.what());
    }
    catch (...) {
        wxLogError("Unknown exception caught in OnOpen");
    }
}
void MainFrame::OnExit(wxCommandEvent& event) {

    try {
        Close(true);
    }
    catch (const std::exception& e) {
        wxLogError("Exception caught in OnOpen: %s", e.what());
    }
    catch (...) {
        wxLogError("Unknown exception caught in OnOpen");
    }
}
void MainFrame::OnClickAnalyzer(wxCommandEvent& event) {

    try {
        StatTesting();
    }
    catch (const std::exception& e) {
        wxLogError("Exception caught in OnOpen: %s", e.what());
    }
    catch (...) {
        wxLogError("Unknown exception caught in OnOpen");
    }
}