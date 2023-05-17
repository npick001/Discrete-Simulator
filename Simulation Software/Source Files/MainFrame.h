#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/statusbr.h>
#include <wx/artprov.h>
#include <wx/wfstream.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/splitter.h>
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
    ID_Model_Settings
};

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);
    ~MainFrame();
private:
 
    // Application Events
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnClickAnalyzer(wxCommandEvent& event);

    //wxBoxSizer* _mainSizer;
    //wxMenu* _fileMenu;
    //wxMenu* _editMenu;
    //wxMenu* _statMenu;
    //wxMenuBar* _menuBar;
    //wxSplitterWindow* _splitter;
    //Canvas* _canvas;
    //LeftToolbar* _toolbar;
};


#endif // MAINFRAME_H

