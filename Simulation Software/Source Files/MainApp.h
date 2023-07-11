#pragma once
#include "Utility.h"

#include <wx/wx.h>
#include <wx/app.h>

#include "Utility.h"
#include "MainFrame.h"

class MainApp : public wxApp {
public:
    virtual bool OnInit();
};