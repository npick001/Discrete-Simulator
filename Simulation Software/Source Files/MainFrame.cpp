#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, "Dynamic GUI Application", wxDefaultPosition, wxSize(800, 600)) {
   
    //_mainSizer = new wxBoxSizer(wxHORIZONTAL);
    //
    ////Create menus
    //_fileMenu = new wxMenu();
    //_editMenu = new wxMenu();
    //_statMenu = new wxMenu();
    //
    //// FILE MENU
    //// mem leaks here
    //_fileMenu->Append(wxID_OPEN);
    //_fileMenu->Append(wxID_SAVE);
    //_fileMenu->Append(wxID_SAVEAS);
    //_fileMenu->Append(ID_Exit, "&Exit\tAlt-F4", "Exit the application");

    //// EDIT MENU
    //// mem leaks here
    //_editMenu->Append(wxID_UNDO);
    //_editMenu->Append(wxID_REDO);
    //_editMenu->AppendSeparator();
    //_editMenu->Append(wxID_CUT);
    //_editMenu->Append(wxID_COPY);
    //_editMenu->Append(wxID_PASTE);

    //// STAT MENU
    //// LOTS of mem leaks here
    ////_statMenu->Append(ID_Input_Analyzer, "&Input Analyzer", "Shows up in bottom left when clicked and hovered over");

    //// Use menus in menu bar
    //// mem leaks here
    //_menuBar = new wxMenuBar;
    //_menuBar->Append(_fileMenu, "&File");
    //_menuBar->Append(_editMenu, "&Edit");
    ////_menuBar->Append(_statMenu, "&Statistics");
    //SetMenuBar(_menuBar);

    //// Create splitter
    //_splitter = new wxSplitterWindow(this);
    //_mainSizer->Add(_splitter, 1, wxEXPAND);

    //// Create toolbar
    //_toolbar = new LeftToolbar(_splitter);

    //// Get the image file path
    //wxString imagePath = wxStandardPaths::Get().GetExecutablePath();
    //wxFileName fileName(imagePath);
    //fileName.RemoveLastDir();
    //fileName.RemoveLastDir();
    //fileName.AppendDir("Images");
    //fileName.SetFullName("KaiQuiSeth.png");
    //imagePath = fileName.GetFullPath();
    ////wxLogMessage(imagePath);

    //// add the node to the toolbar
    //for (int i = 0; i < 8; i++) {
    //    wxString nodeName = "test " + std::to_string(i);
    //    _toolbar->AddNode(new GenericNode(nodeName, imagePath));
    //}
   
    //// Create canvas
    //_canvas = new Canvas(_splitter);
    //int leftToolbarSize = 100;
    //_splitter->SplitVertically(_toolbar, _canvas, leftToolbarSize);
    //_splitter->SetMinimumPaneSize(leftToolbarSize);
    //_splitter->SetSashGravity(0.0); // left toolbar fixed size, canvas expand to size

    //// Create status bar
    //CreateStatusBar();
    //SetStatusText("Welcome to the Dynamic GUI Application!");

    //// Bind the events 
    //this->Bind(wxEVT_MENU, &MainFrame::OnOpen, this, wxID_OPEN);
    //this->Bind(wxEVT_MENU, &MainFrame::OnSave, this, wxID_SAVE);
    //this->Bind(wxEVT_MENU, &MainFrame::OnSaveAs, this, wxID_SAVEAS);
    //this->Bind(wxEVT_MENU, &MainFrame::OnExit, this, ID_Exit);
    //this->Bind(wxEVT_MENU, &MainFrame::OnClickAnalyzer, this, ID_Input_Analyzer);

    //SetSizer(_mainSizer);
    int minPaneSize = 150;
    wxSize window_size = this->GetClientSize();

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

    // add the node to the toolbar
    for (int i = 0; i < 8; i++) {
        wxString nodeName = "test " + std::to_string(i);
        left->AddNode(new GenericNode(nodeName, imagePath));
    }

    left->SetBackgroundColour(*wxCYAN);
    middle_top->SetBackgroundColour(*wxBLACK);
    middle_bottom->SetBackgroundColour(*wxRED);
    right->SetBackgroundColour(*wxBLUE);


    left_splitter->SetMinimumPaneSize(minPaneSize);
    middle_splitter->SetMinimumPaneSize(minPaneSize);
    right_splitter->SetMinimumPaneSize(minPaneSize);


    middle_splitter->SplitHorizontally(middle_top, middle_bottom);
    right_splitter->SplitVertically(middle_splitter, right);
    left_splitter->SplitVertically(left, right_splitter);
}

MainFrame::~MainFrame() {

}

void MainFrame::OnOpen(wxCommandEvent& event) {

    wxLogMessage("Inside OnOpen");

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