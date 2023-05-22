#ifndef MAINFRAME_H
#define MAINFRAME_H

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
#include "Statistics.h"
#include "Canvas.h"
#include "LeftToolbar.h"

#ifdef __WXMSW__
    #include <wx/msw/msvcrt.h>      // redefines the new() operator 
#endif

enum {
    ID_Exit = 1,
    ID_Input_Analyzer,
    ID_Run_Sim,
    ID_Model_Settings,
    ID_CreateNotebook,
    ID_CreateTree,
    ID_CreateGrid,
    ID_DropDownToolbarItem,
    ID_CustomizeToolbar,

    ID_SampleItem,
};

class Canvas;

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);
    ~MainFrame();

    void DoUpdate();
private:
    wxAuiManager m_manager;
    wxAuiNotebook* m_mainCanvas;
    wxArrayString m_perspectives;
    long m_notebook_style;
    long m_notebook_theme;

    wxAuiNotebook* CreateNotebook();
    wxTreeCtrl* CreateTreeCtrl();
    void LoadDirectory(wxTreeCtrl* treeCtrl, const wxTreeItemId& parent, const wxString& directory);
    wxGrid* CreateGrid();
    wxPoint GetStartPosition();
 
    // Application Events
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnClickAnalyzer(wxCommandEvent& event);
    void OnCreateNotebook(wxCommandEvent& event);
    void OnCreateTree(wxCommandEvent& event);
    void OnCreateGrid(wxCommandEvent& event);  
};


#endif // MAINFRAME_H

