#ifndef MAINAPP_H
#define MAINAPP_H

#include <crtdbg.h>
#include <wx/wx.h>
#include <wx/app.h>

#include "Utility.h"
#include "MainFrame.h"

class MainApp : public wxApp {
public:
    virtual bool OnInit();
};

#endif // MAINAPP_H
