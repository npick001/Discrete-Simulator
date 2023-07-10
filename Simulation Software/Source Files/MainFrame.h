#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <crtdbg.h>
#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/statusbr.h>
#include <wx/artprov.h>
#include <wx/wfstream.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/grid.h>
#include <wx/dir.h>

#include "SimulationExecutive.h"
#include "Statistics.h"
#include "GraphicalNode.h"
#include "Canvas.h"
#include "SimProject.h"
#include "SimObjectLibrary.h"
#include "PropertiesViewer.h"
#include "XMLSerialization.h"


enum {
    ID_Exit = 1,
    ID_Input_Analyzer,
    ID_Run_Sim,
    ID_Build_SimCode,
    ID_Build_And_Run,
    ID_Model_Settings,
    ID_CreateNotebook,
    ID_CreateTree,
    ID_CreateGrid,
    ID_CreateCanvas,
    ID_CreateSimLibrary,
    ID_DropDownToolbarItem,
    ID_CustomizeToolbar,

    ID_SampleItem,
};

class GraphicalNode;
class Canvas;
struct Selection;
class SimProject;
class SimObjectLibrary;

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);
    ~MainFrame();

    void DoUpdate();

    void RegisterNewSelection(GraphicalNode* selection);

    static MainFrame* GetInstance();

private:
    static MainFrame* m_instance;
    SimProject* m_simProject;

    wxAuiManager m_manager;
    PropertiesViewer* m_properties;
    wxArrayString m_perspectives; // not used yet
    wxAuiNotebook* m_mainCanvas;

    long m_notebook_style;
    long m_notebook_theme;

    wxAuiNotebook* CreateNotebook();
    wxTreeCtrl* CreateTreeCtrl();
    wxGrid* CreateGrid();
    Canvas* CreateCanvas();
    SimObjectLibrary* CreateSimLibrary();

    // Loads the .\Source Files directory into the passed tree.
    void LoadDirectory(wxTreeCtrl* treeCtrl, const wxTreeItemId& parent, const wxString& directory);
    
    wxPoint GetStartPosition();
 
    // File menu events
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);

    // Edit menu events
    
    // View menu events    
    void OnCreateNotebook(wxCommandEvent& event);
    void OnCreateTree(wxCommandEvent& event);
    void OnCreateGrid(wxCommandEvent& event);  
    void OnCreateCanvas(wxCommandEvent& event);
    void OnCreateSimLibrary(wxCommandEvent& event);

    // Statistics menu events
    void OnClickAnalyzer(wxCommandEvent& event);

    // Project Menu Events
    void OnBuild(wxCommandEvent& event);
    void OnRun(wxCommandEvent& event);
    void OnBuildAndRun(wxCommandEvent& event);

    void OnResize(wxSizeEvent& event);
};


#endif // MAINFRAME_H

