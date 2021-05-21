#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/msgdlg.h"
#include "src/automat.hpp"

constexpr size_t CELL_WIDTH = 20;
constexpr size_t GRID_WIDTH = 30;

constexpr int BUTTON_NEXT_STEP = 101;
constexpr int BUTTON_SET_RULES = 102;
constexpr int BUTTON_CLEAR = 103;

class DrawPane : public wxPanel
{
private:
    
public:
    Automat* automat;

    DrawPane(wxFrame* parent, wxSize size, Automat *automat) :
        wxPanel(parent, -1, wxDefaultPosition, size),
        automat(automat) {
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
    wxStaticText* cellDefTitle;
    wxTextCtrl* cellDefTxt;
    wxStaticText* cellRulesTitle;
    wxTextCtrl* cellRulesTxt;
    wxButton* btnSet;
    wxStaticText* speedTxt;
    wxSlider* speedSlider;
    wxButton* btnStart;
    wxButton* btnOneStep;
    wxButton* btnClear;

    wxBoxSizer* sizer;
    wxBoxSizer* sizerCtrlBtns;
    wxFlexGridSizer* controlsSizer;

    unsigned int simulationSpeed;

public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    void oneStepBtnEvent(wxCommandEvent& event);
    void setRulesBtnEvent(wxCommandEvent& event);
    void clearCells(wxCommandEvent& event);

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
    
    Automat* automat = new Automat(GRID_WIDTH, GRID_WIDTH, Automat::DEFAULT_DEFINITIONS, Automat::DEFAULT_RULES);

    int gridWidth = (GRID_WIDTH * CELL_WIDTH) + GRID_WIDTH * 2;

    simulationSpeed = 1;
    
    drawPane = new DrawPane(this, wxSize(gridWidth, gridWidth), automat);

    SetBackgroundColour(*wxLIGHT_GREY);

    cellDefTitle = new wxStaticText(this, -1, wxT("CELL DEFINITIONS\n(NAME,COLOUR)"));
    cellDefTxt = new wxTextCtrl(this, -1, Automat::DEFAULT_DEFINITIONS, wxDefaultPosition, wxSize(300, 100), wxTE_MULTILINE | wxTE_LEFT);
    cellRulesTitle = new wxStaticText(this, -1, wxT("RULES\n(OLD, NEIGHBOR_COUNT, NEIGHBOR_STATE, NEW)"));
    cellRulesTxt = new wxTextCtrl(this, -1, Automat::DEFAULT_RULES, wxDefaultPosition, wxSize(300, 200), wxTE_MULTILINE | wxTE_LEFT);
    btnSet = new wxButton(this, BUTTON_SET_RULES, wxT("SET"));
    speedTxt = new wxStaticText(this, -1, wxT("SIMULATION SPEED (SECONDS)"));
    speedSlider = new wxSlider(this, -1, 3, 1, 10, wxDefaultPosition, wxSize(200, -1), wxSL_HORIZONTAL | wxSL_MIN_MAX_LABELS | wxSL_VALUE_LABEL);
    btnStart = new wxButton(this, -1, wxT("START"));
    btnOneStep = new wxButton(this, BUTTON_NEXT_STEP, wxT("ONE STEP"));
    btnClear = new wxButton(this, BUTTON_CLEAR, wxT("CLEAR"));

    sizer = new wxBoxSizer(wxHORIZONTAL);
    sizerCtrlBtns = new wxBoxSizer(wxHORIZONTAL);
    controlsSizer = new wxFlexGridSizer(8, 1, 10, 10);
        
    sizerCtrlBtns->Add(btnStart);
    sizerCtrlBtns->Add(btnOneStep);
    sizerCtrlBtns->Add(btnClear);

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
    //automat is using opposite coordinate system
    int x = event.GetY();
    int y = event.GetX();
    int rowCell = x / CELL_WIDTH;
    int colCell = y / CELL_WIDTH;
    if (rowCell >= 0 && colCell >= 0 && rowCell < GRID_WIDTH && colCell < GRID_WIDTH) {
        automat->cellCycleType(rowCell, colCell);
        paintNow();
    }
}

void DrawPane::render(wxDC& dc) {
    dc.SetPen(*wxGREY_PEN);

    for (size_t i = 0; i < GRID_WIDTH; i++)
    {
        for (size_t j = 0; j < GRID_WIDTH; j++)
        {
            wxColor cellColour = wxColor(automat->getColourAt(j, i));
            dc.SetBrush(wxBrush(cellColour));
            dc.DrawRectangle(wxRect(wxPoint(i * CELL_WIDTH, j * CELL_WIDTH), wxSize(CELL_WIDTH, CELL_WIDTH)));
            //dc.DrawText(wxString(std::to_string(index)), i * CELL_WIDTH, j * CELL_WIDTH);
        }
    } 
}

void MainFrame::oneStepBtnEvent(wxCommandEvent& event) {
    drawPane->automat->doOneEvolution();
    drawPane->paintNow();
}

void MainFrame::setRulesBtnEvent(wxCommandEvent& event) {
    std::string newDefs = std::string(this->cellDefTxt->GetValue().mb_str());
    std::string newRules = std::string(this->cellRulesTxt->GetValue().mb_str());
    try
    {
        drawPane->automat = new Automat(GRID_WIDTH, GRID_WIDTH, newDefs, newRules);
        drawPane->paintNow();
    }
    catch (const Automat::InvalidFormatException& e)
    {
        auto error = e.what();
        wxMessageBox(wxString::FromUTF8(error), wxT("Format error"), wxICON_INFORMATION);
    }
}

void MainFrame::clearCells(wxCommandEvent& event) {
    drawPane->automat->clearCells();
    drawPane->paintNow();
}

IMPLEMENT_APP(MainApp)

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(BUTTON_NEXT_STEP, MainFrame::oneStepBtnEvent)
EVT_BUTTON(BUTTON_SET_RULES, MainFrame::setRulesBtnEvent)
EVT_BUTTON(BUTTON_CLEAR, MainFrame::clearCells)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(DrawPane, wxPanel)
EVT_LEFT_DOWN(DrawPane::mouseDown)
EVT_PAINT(DrawPane::paintEvent)
END_EVENT_TABLE()