#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/msgdlg.h"
#include "src/automat.hpp"
#include "src/presets.hpp"

constexpr size_t CELL_WIDTH = 20;
constexpr size_t GRID_WIDTH = 30;

constexpr int BUTTON_NEXT_STEP = 101;
constexpr int BUTTON_SET_RULES = 102;
constexpr int BUTTON_CLEAR = 103;
constexpr int BUTTON_DISPLAY_HELP = 104;
constexpr int BUTTON_PRESET_GOL = 105;
constexpr int BUTTON_PRESET_WW = 106;

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

    wxCheckBox* checkOverFlow;

    wxButton* btnSet;
    wxButton* btnHelp;

    wxStaticText* presetsTitle;
    wxButton* btnPresetGOL;
    wxButton* btnPresetWW;

    wxStaticText* speedTxt;
    wxSlider* speedSlider;

    wxButton* btnStart;
    wxButton* btnOneStep;
    wxButton* btnClear;

    wxBoxSizer* sizer;
    wxBoxSizer* sizerCtrlBtns;
    wxBoxSizer* sizerSetHelp;
    wxBoxSizer* sizerPresets;
    wxFlexGridSizer* controlsSizer;

    unsigned int simulationSpeed;

public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    void oneStepBtnEvent(wxCommandEvent& event);
    void setRulesBtnEvent(wxCommandEvent& event);
    void displayHelpEvent(wxCommandEvent& event);
    void clearCells(wxCommandEvent& event);
    void loadPreset(wxCommandEvent& event);

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
    
    bool overflow = true;
    auto& def_defs = Presets::GOL_defs;
    auto& def_rules = Presets::GOL_rules;
    Automat* automat = new Automat(GRID_WIDTH, GRID_WIDTH, def_defs, def_rules, overflow);

    int gridWidth = (GRID_WIDTH * CELL_WIDTH) + GRID_WIDTH * 2;

    simulationSpeed = 1;
    
    drawPane = new DrawPane(this, wxSize(gridWidth, gridWidth), automat);

    SetBackgroundColour(*wxLIGHT_GREY);

    cellDefTitle = new wxStaticText(this, -1, wxT("CELL DEFINITIONS"));
    cellDefTxt = new wxTextCtrl(this, -1, def_defs, wxDefaultPosition, wxSize(300, 100), wxTE_MULTILINE | wxTE_LEFT);
    
    cellRulesTitle = new wxStaticText(this, -1, wxT("RULES"));
    cellRulesTxt = new wxTextCtrl(this, -1, def_rules, wxDefaultPosition, wxSize(300, 200), wxTE_MULTILINE | wxTE_LEFT);
    
    checkOverFlow = new wxCheckBox(this, -1, wxT("OVERFLOW BORDERS"), wxDefaultPosition);
    btnSet = new wxButton(this, BUTTON_SET_RULES, wxT("SET"));
    btnHelp = new wxButton(this, BUTTON_DISPLAY_HELP, wxT("HELP"));
    
    presetsTitle = new wxStaticText(this, -1, wxT("LOAD PRESET RULES"));
    btnPresetGOL = new wxButton(this, BUTTON_PRESET_GOL, wxT("GAME OF LIFE"));
    btnPresetWW = new wxButton(this, BUTTON_PRESET_WW, wxT("WIREWORLD"));

    speedTxt = new wxStaticText(this, -1, wxT("SIMULATION SPEED (SECONDS)"));
    speedSlider = new wxSlider(this, -1, 3, 1, 10, wxDefaultPosition, wxSize(200, -1), wxSL_HORIZONTAL | wxSL_MIN_MAX_LABELS | wxSL_VALUE_LABEL);
    
    btnStart = new wxButton(this, -1, wxT("START"));
    btnOneStep = new wxButton(this, BUTTON_NEXT_STEP, wxT("ONE STEP"));
    btnClear = new wxButton(this, BUTTON_CLEAR, wxT("CLEAR"));

    checkOverFlow->SetValue(overflow);

    sizer = new wxBoxSizer(wxHORIZONTAL);
    sizerCtrlBtns = new wxBoxSizer(wxHORIZONTAL);
    sizerSetHelp = new wxBoxSizer(wxHORIZONTAL);
    sizerPresets = new wxBoxSizer(wxHORIZONTAL);
    controlsSizer = new wxFlexGridSizer(11, 1, 10, 10);
    
    sizerSetHelp->Add(btnSet);
    sizerSetHelp->Add(btnHelp);

    sizerCtrlBtns->Add(btnStart);
    sizerCtrlBtns->Add(btnOneStep);
    sizerCtrlBtns->Add(btnClear);

    sizerPresets->Add(btnPresetGOL);
    sizerPresets->Add(btnPresetWW);

    controlsSizer->Add(cellDefTitle);
    controlsSizer->Add(cellDefTxt, wxEXPAND);
    controlsSizer->Add(cellRulesTitle);
    controlsSizer->Add(cellRulesTxt, wxEXPAND);
    controlsSizer->Add(checkOverFlow);
    controlsSizer->Add(sizerSetHelp);
    controlsSizer->Add(presetsTitle);
    controlsSizer->Add(sizerPresets);
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
    bool overflow = checkOverFlow->IsChecked();
    try {
        drawPane->automat = new Automat(GRID_WIDTH, GRID_WIDTH, newDefs, newRules, overflow);
        drawPane->paintNow();
    }
    catch (const Automat::InvalidFormatException& e) {
        auto error = e.what();
        wxMessageBox(wxString::FromUTF8(error), wxT("Format error"), wxICON_ERROR);
    }
}

void MainFrame::displayHelpEvent(wxCommandEvent& event) {
    const char* text =
        "CELL DEFINITION FORMAT: NAME,COLOR\n"
        "NAME - any string not containing whitespace\nCOLOR - colour in hexadecimal RGB format\n"
        "EXAMPLE: DEAD,FF0055\n\n"
        "RULE FORMAT: CURRENT_STATE,NEIGHBOUR_AMOUNT,NEIGHBOUR_STATE,NEW_STATE\n"
        "CURRENT_STATE - cell state the rule is applied to\n"
        "NEIGHBOUR_AMOUNT - amount of neighbours with NEIGHBOUR_STATE\n"
        "NEIGHBOUR_STATE - the state of neighbouring cells\n"
        "NEW_STATE - state the cell will transform into if all conditions are met\n"
        "EXAMPLE: ALIVE,01,ALIVE,DEAD\n"
        "NOTE: leave NEIGHBOUR_AMOUNT empty to always transfer\n"
        "NOTE: NEIGHBOUR_AMOUNT can contain multiple digits";
    wxMessageBox(wxString::FromUTF8(text), wxT("Help"), wxICON_INFORMATION);
}

void MainFrame::clearCells(wxCommandEvent& event) {
    drawPane->automat->clearCells();
    drawPane->paintNow();
}

void MainFrame::loadPreset(wxCommandEvent& event) {
    bool overflow = checkOverFlow->IsChecked();
    auto& new_defs = Presets::GOL_defs;
    auto& new_rules = Presets::GOL_rules;
    if (event.GetId() == BUTTON_PRESET_WW) {
        new_defs = Presets::WW_defs;
        new_rules = Presets::WW_rules;
    }
    drawPane->automat = new Automat(GRID_WIDTH, GRID_WIDTH, new_defs, new_rules, overflow);
    cellDefTxt->SetValue(new_defs);
    cellRulesTxt->SetValue(new_rules);
    drawPane->paintNow();
}

IMPLEMENT_APP(MainApp)

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(BUTTON_NEXT_STEP, MainFrame::oneStepBtnEvent)
EVT_BUTTON(BUTTON_SET_RULES, MainFrame::setRulesBtnEvent)
EVT_BUTTON(BUTTON_DISPLAY_HELP, MainFrame::displayHelpEvent)
EVT_BUTTON(BUTTON_PRESET_GOL, MainFrame::loadPreset)
EVT_BUTTON(BUTTON_PRESET_WW, MainFrame::loadPreset)
EVT_BUTTON(BUTTON_CLEAR, MainFrame::clearCells)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(DrawPane, wxPanel)
EVT_LEFT_DOWN(DrawPane::mouseDown)
EVT_PAINT(DrawPane::paintEvent)
END_EVENT_TABLE()