#include "MainFrame.h"
#include "Canvas.h"

MainFrame::MainFrame(const wxString& title) 
    : wxFrame(nullptr, wxID_ANY, "Dynamic GUI Application", wxDefaultPosition, wxSize(800, 600))
{   
    m_manager.SetManagedWindow(this);

    // set up default notebook style
    m_notebook_style = wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER;
    m_notebook_theme = 0;

    //Create menus
    auto* menu_bar = new wxMenuBar();
    auto* file_menu = new wxMenu();
    auto* edit_menu = new wxMenu();
    auto* view_menu = new wxMenu();
    auto* settings_menu = new wxMenu();
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

    // VIEW MENU
    view_menu->Append(ID_CreateTree, _("Create Tree"));
    view_menu->Append(ID_CreateNotebook, _("Create Notebook"));
    view_menu->Append(ID_CreateGrid, _("Create Grid"));
    view_menu->Append(ID_CreateCanvas, _("Create Canvas"));
    view_menu->Append(ID_CreateLeftToolbar, _("Create Left Toolbar"));

    // SETTINGS MENU
    settings_menu->Append(ID_Model_Settings, "&Model Settings", "Change Model Settings");

    // STAT MENU
    stat_menu->Append(ID_Input_Analyzer, "&Input Analyzer", "Shows up in bottom left when clicked and hovered over");

    // Use menus in menu bar
    menu_bar->Append(file_menu, _("&File"));
    menu_bar->Append(edit_menu, _("&Edit"));
    menu_bar->Append(view_menu, _("&View"));
    menu_bar->Append(settings_menu, _("&Settings"));
    menu_bar->Append(stat_menu, _("&Statistics"));
    SetMenuBar(menu_bar);

    // Create status bar
    CreateStatusBar();
    SetStatusText("Welcome to the Dynamic GUI Application!");

    // Prepare a few custom overflow elements for the toolbars overflow buttons.
    wxAuiToolBarItemArray prepend_items;
    wxAuiToolBarItemArray append_items;
    wxAuiToolBarItem item;
    item.SetKind(wxITEM_SEPARATOR);
    append_items.Add(item);
    item.SetKind(wxITEM_NORMAL);
    item.SetId(ID_CustomizeToolbar);
    item.SetLabel(_("Customize..."));
    append_items.Add(item);

    // Generate first toolbar with all question marks
    wxAuiToolBar* tb1 = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_HORIZONTAL);

    // get the art provider for each tool in the toolbar
    wxBitmapBundle tb1_bmp1 = wxArtProvider::GetBitmapBundle(wxART_QUESTION, wxART_OTHER, wxSize(16, 16));

    // add some random placeholder tools
    tb1->AddTool(ID_SampleItem + 6, "Disabled", tb1_bmp1);
    tb1->AddTool(ID_SampleItem + 7, "Test", tb1_bmp1);
    tb1->AddTool(ID_SampleItem + 8, "Test", tb1_bmp1);
    tb1->AddTool(ID_SampleItem + 9, "Test", tb1_bmp1);
    tb1->AddSeparator();
    tb1->AddTool(ID_SampleItem + 10, "Test", tb1_bmp1);
    tb1->AddTool(ID_SampleItem + 11, "Test", tb1_bmp1);
    tb1->AddSeparator();
    tb1->AddTool(ID_SampleItem + 12, "Test", tb1_bmp1);
    tb1->AddTool(ID_SampleItem + 13, "Test", tb1_bmp1);
    tb1->AddTool(ID_SampleItem + 14, "Test", tb1_bmp1);
    tb1->AddTool(ID_SampleItem + 15, "Test", tb1_bmp1);
    tb1->SetCustomOverflowItems(prepend_items, append_items);
    tb1->EnableTool(ID_SampleItem + 6, false);
    tb1->Realize();

    // make the default notebook on startup
    m_mainCanvas = CreateNotebook();
    wxBitmapBundle page_bmp = wxArtProvider::GetBitmapBundle(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16, 16));

    // Add a second page to the canvas
    m_mainCanvas->AddPage(new Canvas(this, GetStatusBar()), "Canvas notebook", true, page_bmp);

    // display the toolbar 
    m_manager.AddPane(tb1, wxAuiPaneInfo().Name("Tb1").
        Caption("Main Toolbar").ToolbarPane().Top());

    // Add the basic panes to the AUI manager for display
    m_manager.AddPane(CreateTreeCtrl(), wxAuiPaneInfo().Name("Current Directory").
        Dockable(true).Left());
    m_manager.AddPane(m_mainCanvas, wxAuiPaneInfo().Name("Current Model").
        Dockable(true).CenterPane());

    auto propWidth = GetSize().x * 0.2;
    auto propSize = new wxSize(propWidth, GetSize().y);
    m_properties = new PropertiesViewer(this);
    m_properties->ShowProperties();
    //m_properties->SetSize(*propSize);

    // PLUG IN PROPERTIES VIEWER HERE
    m_manager.AddPane(m_properties, wxAuiPaneInfo().Name("Test Property Panel").
        Dockable(true).Right());

    // Commit the changes with the AUI manager
    m_manager.Update();
    this->Layout();

    // File menu events 
    this->Bind(wxEVT_MENU, &MainFrame::OnOpen, this, wxID_OPEN);
    this->Bind(wxEVT_MENU, &MainFrame::OnSave, this, wxID_SAVE);
    this->Bind(wxEVT_MENU, &MainFrame::OnSaveAs, this, wxID_SAVEAS);
    this->Bind(wxEVT_MENU, &MainFrame::OnExit, this, ID_Exit);

    // Edit menu events

    // View menu Events
    this->Bind(wxEVT_MENU, &MainFrame::OnCreateNotebook, this, ID_CreateNotebook);
    this->Bind(wxEVT_MENU, &MainFrame::OnCreateTree, this, ID_CreateTree);
    this->Bind(wxEVT_MENU, &MainFrame::OnCreateGrid, this, ID_CreateGrid);
    this->Bind(wxEVT_MENU, &MainFrame::OnCreateCanvas, this, ID_CreateCanvas);
    this->Bind(wxEVT_MENU, &MainFrame::OnCreateLeftToolbar, this, ID_CreateLeftToolbar);

    // Statistics menu Events
    this->Bind(wxEVT_MENU, &MainFrame::OnClickAnalyzer, this, ID_Input_Analyzer);
}
MainFrame::~MainFrame() {
    Destroy();
}
void MainFrame::DoUpdate()
{
    m_manager.Update();
}
wxAuiNotebook* MainFrame::CreateNotebook()
{
    wxSize client_size = GetClientSize();

    wxAuiNotebook* notebook = new wxAuiNotebook(this, wxID_ANY,
                                        wxPoint(client_size.x, client_size.y),
                                        FromDIP(wxSize(430, 200)));
    notebook->Freeze();

    wxBitmapBundle page_bmp = wxArtProvider::GetBitmapBundle(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16, 16));

    // Write the code for adding models in here.
    notebook->AddPage(new Canvas(this, GetStatusBar()), "Canvas notebook", true, page_bmp);

    notebook->Thaw();

    return notebook;
}
wxTreeCtrl* MainFrame::CreateTreeCtrl()
{
    // Create the file tree
    wxTreeCtrl* files = new wxTreeCtrl(this, wxID_ANY,
        wxPoint(0, 0), FromDIP(wxSize(160, 250)),
        wxTR_DEFAULT_STYLE | wxNO_BORDER);

    // set the images for the files in the tree
    wxSize size(16, 16);
    wxVector<wxBitmapBundle> images;
    images.push_back(wxArtProvider::GetBitmapBundle(wxART_FOLDER, wxART_OTHER, size));
    images.push_back(wxArtProvider::GetBitmapBundle(wxART_NORMAL_FILE, wxART_OTHER, size));
    files->SetImages(images);

    // get the names of the parent directory
    wxFileName exeFile;
    exeFile.MakeRelativeTo();
    const wxString& parentDir = exeFile.GetPath();
    
    //exeFile.RemoveLastDir();
    //exeFile.AppendDir("Simulation Software");
    exeFile.AppendDir("Source Files");

    // get the names of the children
    const wxString& dir = exeFile.GetPath();

    // Create the tree
    wxTreeItemId root = files->AddRoot(dir, 0);
    files->Expand(root);
    LoadDirectory(files, root, dir);

    return files;
}
wxGrid* MainFrame::CreateGrid()
{
    wxGrid* grid = new wxGrid(this, wxID_ANY,
        wxPoint(0, 0),
        FromDIP(wxSize(150, 250)),
        wxNO_BORDER | wxWANTS_CHARS);
    grid->CreateGrid(50, 20);
    return grid;
}

Canvas* MainFrame::CreateCanvas()
{
    Canvas* newCanvas = new Canvas(this, GetStatusBar());    
    return newCanvas;
}

SimObjectLibrary* MainFrame::CreateLeftToolbar()
{
    SimObjectLibrary* simLibrary = new SimObjectLibrary(this);
    return simLibrary;
}

void MainFrame::LoadDirectory(wxTreeCtrl* treeCtrl, const wxTreeItemId& parent, const wxString& directory)
{
    // open the passed directory
    wxDir dir(directory);

    wxString dirName = dir.GetName();

    // make sure its open
    if (!dir.IsOpened()) {
        // Error handling
        return;
    }

    // get the first filename from the directory
    wxString filename;
    bool keepGoing = dir.GetFirst(&filename);
    while (keepGoing) {

        // creating a filename object to access the members
        wxFileName fn(directory, filename);
        if (fn.DirExists()) {
            wxTreeItemId id = treeCtrl->AppendItem(parent, filename);

            // recursively populate the tree with children
            LoadDirectory(treeCtrl, id, fn.GetFullPath());
        }
        else {
            treeCtrl->AppendItem(parent, filename);
        }

        // decide if we should keep going
        keepGoing = dir.GetNext(&filename);
    }
}

wxPoint MainFrame::GetStartPosition()
{
    return wxPoint();
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
        wxFileDialog saveFileDialog(this, _("Save PNG file"), "", "",
            "PNG files (*.png)|*.xyz", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

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
void MainFrame::OnCreateNotebook(wxCommandEvent& event)
{
    m_manager.AddPane(CreateNotebook(), wxAuiPaneInfo().Caption("Notebook").
        CenterPane().Dockable(true));
    m_manager.Update();
}
void MainFrame::OnCreateTree(wxCommandEvent& event)
{
    m_manager.AddPane(CreateTreeCtrl(), wxAuiPaneInfo().
        Caption("Tree Control").
        Float().FloatingPosition(GetStartPosition()).
        FloatingSize(FromDIP(wxSize(150, 300))));
    m_manager.Update();
}
void MainFrame::OnCreateGrid(wxCommandEvent& event)
{
    m_manager.AddPane(CreateGrid(), wxAuiPaneInfo().Dockable(true).
        Caption("Grid").Float().FloatingPosition(GetStartPosition()).
        FloatingSize(FromDIP(wxSize(150, 300))));
    m_manager.Update();
}

void MainFrame::OnCreateCanvas(wxCommandEvent& event)
{
    m_mainCanvas->AddPage(CreateCanvas(), "Model", true, wxID_ANY);
    m_mainCanvas->Update();
}

void MainFrame::OnCreateLeftToolbar(wxCommandEvent& event)
{
    m_manager.AddPane(CreateLeftToolbar(), wxAuiPaneInfo().Dockable(true).
        Caption("Simulation Library").Left());
    m_manager.Update();
}

void MainFrame::OnClickAnalyzer(wxCommandEvent& event) {

    try {
       // StatTesting();
    }
    catch (const std::exception& e) {
        wxLogError("Exception caught in OnOpen: %s", e.what());
    }
    catch (...) {
        wxLogError("Unknown exception caught in OnOpen");
    }
}
