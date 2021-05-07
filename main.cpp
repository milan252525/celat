#include "wx/wx.h"
#include "wx/sizer.h"
#include "src/automat.hpp"

constexpr size_t CELL_WIDTH = 25;
constexpr size_t GRID_WIDTH = 25;

constexpr int BUTTON_NEXT_STEP = 101;

class DrawPane : public wxPanel
{
public:
    DrawPane(wxFrame* parent, wxSize size) :
        wxPanel(parent, -1, wxDefaultPosition, size) {
    }

    void paintEvent(wxPaintEvent& evt) {
        wxPaintDC dc(this);
        render(dc);
    }

    void paintNow() {
        wxClientDC dc(this);
        render(dc);
    }

    void render(wxDC& dc);

    void mouseDown(wxMouseEvent& event);
 
    DECLARE_EVENT_TABLE()
};

class MainApp : public wxApp {
public:
    virtual bool OnInit();
};

class MainFrame : public wxFrame {
private:
    DrawPane* drawPane;
    Automat* automaton;
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    void oneStepBtnEvent(wxMouseEvent& event);

    DECLARE_EVENT_TABLE()
};

bool MainApp::OnInit() {
    int gridWidth = (GRID_WIDTH * CELL_WIDTH) + GRID_WIDTH * 2;
    MainFrame* mainWin = new MainFrame(wxT("CELAT"), wxPoint(50, 50), wxSize(1000, gridWidth));
    mainWin->Show(TRUE);
    SetTopWindow(mainWin);
    return TRUE;
}

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame((wxFrame*)NULL, -1, title, pos, size) {
    
    automaton = new Automat(GRID_WIDTH, GRID_WIDTH, "", "");
    int gridWidth = (GRID_WIDTH * CELL_WIDTH) + GRID_WIDTH * 2;
    
    drawPane = new DrawPane(this, wxSize(gridWidth, gridWidth));

    SetBackgroundColour(*wxLIGHT_GREY);

    wxStaticText* cellDefTitle = new wxStaticText(this, -1, wxT("CELL DEFINITIONS"));
    wxTextCtrl* cellDefTxt = new wxTextCtrl(this, -1, DEFAULT_DEFINITIONS, wxDefaultPosition, wxSize(300, 200), wxTE_MULTILINE | wxTE_LEFT);
    wxStaticText* cellRulesTitle = new wxStaticText(this, -1, wxT("RULES"));
    wxTextCtrl* cellRulesTxt = new wxTextCtrl(this, -1, DEFAULT_RULES, wxDefaultPosition, wxSize(300, 200), wxTE_MULTILINE | wxTE_LEFT);
    wxButton* btnSet = new wxButton(this, -1, wxT("SET"));
    wxStaticText* speedTxt = new wxStaticText(this, -1, wxT("SIMULATION SPEED (SECONDS)"));
    wxSlider* speedSlider = new wxSlider(this, -1, 3, 1, 10, wxDefaultPosition, wxSize(200, -1), wxSL_HORIZONTAL | wxSL_MIN_MAX_LABELS | wxSL_VALUE_LABEL);
    wxButton* btnStart = new wxButton(this, -1, wxT("START"));
    wxButton* btnOneStep = new wxButton(this, BUTTON_NEXT_STEP, wxT("ONE STEP"));

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizerCtrlBtns = new wxBoxSizer(wxHORIZONTAL);
    wxFlexGridSizer* controlsSizer = new wxFlexGridSizer(8, 1, 10, 10);
        
    sizerCtrlBtns->Add(btnStart);
    sizerCtrlBtns->Add(btnOneStep);

    controlsSizer->Add(cellDefTitle);
    controlsSizer->Add(cellDefTxt, wxEXPAND);
    controlsSizer->Add(cellRulesTitle);
    controlsSizer->Add(cellRulesTxt, wxEXPAND);
    controlsSizer->Add(btnSet);
    controlsSizer->Add(speedTxt);
    controlsSizer->Add(speedSlider);
    controlsSizer->Add(sizerCtrlBtns);
    
    sizer->Add(drawPane, 1);
    sizer->Add(controlsSizer, 1);

    SetSizer(sizer);
    SetAutoLayout(true);
}

void DrawPane::mouseDown(wxMouseEvent& event) {
    wxClientDC dc(this);
    int x = event.GetX();
    int y = event.GetY();
    int row_cell = x / CELL_WIDTH;
    int col_cell = y / CELL_WIDTH;
    dc.DrawText(wxString(std::to_string(row_cell) + " " + std::to_string(col_cell)), event.GetX(), event.GetY());
}

void DrawPane::render(wxDC& dc) {
    dc.SetPen(wxPen(wxColor(0, 0, 0), 1));
    dc.SetBrush(*wxWHITE_BRUSH);

    for (size_t i = 0; i < GRID_WIDTH; i++)
    {
        for (size_t j = 0; j < GRID_WIDTH; j++)
        {
            dc.DrawRectangle(wxRect(wxPoint(i * CELL_WIDTH, j * CELL_WIDTH), wxSize(CELL_WIDTH, CELL_WIDTH)));
        }
    }    
}

void MainFrame::oneStepBtnEvent(wxMouseEvent& event) {
    MainFrame::automaton->doOneEvolution();
}

IMPLEMENT_APP(MainApp)

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(DrawPane, wxPanel)
EVT_LEFT_DOWN(DrawPane::mouseDown)
EVT_PAINT(DrawPane::paintEvent)
END_EVENT_TABLE()