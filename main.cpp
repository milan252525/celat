#include <wx/wx.h>

class Application : public wxApp {
    bool OnInit() override {
        (new wxFrame(nullptr, wxID_ANY, "CELAT"))->Show();
        return true;
    }
};

wxIMPLEMENT_APP(Application);