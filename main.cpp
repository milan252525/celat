#include "wx/wx.h"

#include "src/automat.hpp"
#include "src/presets.hpp"

constexpr size_t CELL_WIDTH = 20;
constexpr size_t GRID_WIDTH = 30;

enum class IDs {
    next_step = 101,
    set_rules,
    clear,
    display_help,
    preset_gol,
    preset_ww,
    timer,
    start,
    preset_bb,
    randomize
};

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
    wxButton* btnPresetBB;

    wxStaticText* speedTxt;
    wxSlider* speedSlider;

    wxButton* btnStart;
    wxButton* btnOneStep;

    wxButton* btnClear;
    wxButton* btnRandom;

    wxBoxSizer* sizer;
    wxBoxSizer* sizerCtrlBtns;
    wxBoxSizer* sizerSetHelp;
    wxBoxSizer* sizerPresets;
    wxBoxSizer* sizerBoardControl;
    wxFlexGridSizer* controlsSizer;

    wxTimer* timer;

public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    void oneStepBtnEvent(wxCommandEvent& event);
    void setRulesBtnEvent(wxCommandEvent& event);
    void displayHelpEvent(wxCommandEvent& event);
    void clearCells(wxCommandEvent& event);
    void loadPreset(wxCommandEvent& event);
    void onTimer(wxTimerEvent& event);
    void timerStartStop(wxCommandEvent& event);
    void randomizeCells(wxCommandEvent& event);

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
    
    drawPane = new DrawPane(this, wxSize(gridWidth, gridWidth), automat);

    SetBackgroundColour(*wxLIGHT_GREY);

    cellDefTitle = new wxStaticText(this, -1, wxT("CELL DEFINITIONS"));
    cellDefTxt = new wxTextCtrl(this, -1, def_defs, wxDefaultPosition, wxSize(300, 100), wxTE_MULTILINE | wxTE_LEFT);
    
    cellRulesTitle = new wxStaticText(this, -1, wxT("RULES"));
    cellRulesTxt = new wxTextCtrl(this, -1, def_rules, wxDefaultPosition, wxSize(300, 150), wxTE_MULTILINE | wxTE_LEFT);
    
    checkOverFlow = new wxCheckBox(this, -1, wxT("WRAP AROUND BORDERS"), wxDefaultPosition);
    btnSet = new wxButton(this, IDs::set_rules, wxT("SET"));
    btnHelp = new wxButton(this, IDs::display_help, wxT("HELP"));
    
    presetsTitle = new wxStaticText(this, -1, wxT("LOAD PRESET RULES"));
    btnPresetGOL = new wxButton(this, IDs::preset_gol, wxT("GAME OF LIFE"));
    btnPresetWW = new wxButton(this, IDs::preset_ww, wxT("WIREWORLD"));
    btnPresetBB = new wxButton(this, IDs::preset_bb, wxT("BRIAN'S BRAIN"));

    speedTxt = new wxStaticText(this, -1, wxT("SIMULATION SPEED"));
    speedSlider = new wxSlider(this, -1, 500, 100, 1000, wxDefaultPosition, wxSize(200, -1), wxSL_HORIZONTAL);
    
    btnStart = new wxButton(this, IDs::start, wxT("START"));
    btnOneStep = new wxButton(this, IDs::next_step, wxT("ONE STEP"));
    
    btnRandom = new wxButton(this, IDs::randomize, wxT("RANDOMIZE BOARD"));
    btnClear = new wxButton(this, IDs::clear, wxT("CLEAR"));

    checkOverFlow->SetValue(overflow);

    sizer = new wxBoxSizer(wxHORIZONTAL);
    sizerCtrlBtns = new wxBoxSizer(wxHORIZONTAL);
    sizerSetHelp = new wxBoxSizer(wxHORIZONTAL);
    sizerPresets = new wxBoxSizer(wxHORIZONTAL);
    sizerBoardControl = new wxBoxSizer(wxHORIZONTAL);
    controlsSizer = new wxFlexGridSizer(12, 1, 10, 10);
    
    sizerSetHelp->Add(btnSet);
    sizerSetHelp->Add(btnHelp);

    sizerCtrlBtns->Add(btnStart);
    sizerCtrlBtns->Add(btnOneStep);

    sizerBoardControl->Add(btnRandom);
    sizerBoardControl->Add(btnClear);

    sizerPresets->Add(btnPresetGOL);
    sizerPresets->Add(btnPresetWW);
    sizerPresets->Add(btnPresetBB);

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
    controlsSizer->Add(sizerBoardControl);
    
    sizer->Add(drawPane, 1);
    sizer->Add(controlsSizer, 1);

    SetSizer(sizer);
    SetAutoLayout(true);

    timer = new wxTimer(this, IDs::timer);
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
    if (timer->IsRunning()) timerStartStop(event);
    drawPane->automat->doOneEvolution();
    drawPane->paintNow();
}

void MainFrame::setRulesBtnEvent(wxCommandEvent& event) {
    if (timer->IsRunning()) timerStartStop(event);
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
    if (timer->IsRunning()) timerStartStop(event);
    bool overflow = checkOverFlow->IsChecked();
    std::string new_defs;
    std::string new_rules;
    if (event.GetId() == IDs::preset_gol) {
        new_defs = Presets::GOL_defs;
        new_rules = Presets::GOL_rules;
    }
    else if (event.GetId() == IDs::preset_ww) {
        new_defs = Presets::WW_defs;
        new_rules = Presets::WW_rules;
    }
    else if (event.GetId() == IDs::preset_bb) {
        new_defs = Presets::BB_defs;
        new_rules = Presets::BB_rules;
    }
    drawPane->automat = new Automat(GRID_WIDTH, GRID_WIDTH, new_defs, new_rules, overflow);
    cellDefTxt->SetValue(new_defs);
    cellRulesTxt->SetValue(new_rules);
    drawPane->paintNow();
}

void MainFrame::onTimer(wxTimerEvent& event) {
    drawPane->automat->doOneEvolution();
    drawPane->paintNow();
    if (speedSlider->GetValue() != timer->GetInterval())  timer->Start(speedSlider->GetValue());
}

void MainFrame::timerStartStop(wxCommandEvent& event) {
    if (timer->IsRunning()) {
        timer->Stop();
        btnStart->SetLabel("START");
    }
    else {
        timer->Start(speedSlider->GetValue());
        btnStart->SetLabel("STOP");
    }
}

void MainFrame::randomizeCells(wxCommandEvent& event) {
    drawPane->automat->randomizeCells();
    drawPane->paintNow();
}

IMPLEMENT_APP(MainApp)

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(IDs::next_step, MainFrame::oneStepBtnEvent)
EVT_BUTTON(IDs::set_rules, MainFrame::setRulesBtnEvent)
EVT_BUTTON(IDs::display_help, MainFrame::displayHelpEvent)
EVT_BUTTON(IDs::preset_gol, MainFrame::loadPreset)
EVT_BUTTON(IDs::preset_ww, MainFrame::loadPreset)
EVT_BUTTON(IDs::preset_bb, MainFrame::loadPreset)
EVT_BUTTON(IDs::clear, MainFrame::clearCells)
EVT_BUTTON(IDs::randomize, MainFrame::randomizeCells)
EVT_TIMER(IDs::timer, MainFrame::onTimer)
EVT_BUTTON(IDs::start, MainFrame::timerStartStop)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(DrawPane, wxPanel)
EVT_LEFT_DOWN(DrawPane::mouseDown)
EVT_PAINT(DrawPane::paintEvent)
END_EVENT_TABLE()