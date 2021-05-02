#include "wx/wx.h"
#include "wx/sizer.h"
#include "src/automat.hpp"

constexpr size_t CELL_WIDTH = 25;
constexpr size_t GRID_WIDTH = 25;

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


class MyApp : public wxApp
{
private:
    bool OnInit();

    wxFrame* frame;
    DrawPane* drawPane;
    //automat automat;
public:

};

IMPLEMENT_APP(MyApp)


bool MyApp::OnInit()
{
    int gridWidth = (GRID_WIDTH * CELL_WIDTH) + GRID_WIDTH * 2;

    frame = new wxFrame((wxFrame*)NULL, -1, wxT("CELAT"), wxPoint(50, 50), wxSize(1000, gridWidth));
    
    drawPane = new DrawPane(frame, wxSize(gridWidth, gridWidth));

    frame->SetBackgroundColour(*wxLIGHT_GREY);

    wxStaticText* cellDefTitle = new wxStaticText(frame, -1, wxT("CELL DEFINITIONS"));
    wxTextCtrl* cellDefTxt = new wxTextCtrl(frame, -1, wxT("CELLS"), wxDefaultPosition, wxSize(300, 200), wxTE_MULTILINE | wxTE_LEFT);
    wxStaticText* cellRulesTitle = new wxStaticText(frame, -1, wxT("RULES"));
    wxTextCtrl* cellRulesTxt = new wxTextCtrl(frame, -1, wxT("RULES"), wxDefaultPosition, wxSize(300, 200), wxTE_MULTILINE | wxTE_LEFT);
    wxButton* btnSet = new wxButton(frame, -1, wxT("SET"));
    wxStaticText* speedTxt = new wxStaticText(frame, -1, wxT("SIMULATION SPEED (SECONDS)"));
    wxSlider* speedSlider = new wxSlider(frame, -1, 3, 1, 10, wxDefaultPosition, wxSize(200, -1), wxSL_HORIZONTAL | wxSL_MIN_MAX_LABELS | wxSL_VALUE_LABEL);
    wxButton* btnStart = new wxButton(frame, -1, wxT("START"));
    wxButton* btnOneStep = new wxButton(frame, -1, wxT("ONE STEP"));

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

    //auto button1 = new wxButton((wxFrame*)frame, 1, "Test Button");
    //sizer->Add(button1, 2);

    frame->SetSizer(sizer);
    frame->SetAutoLayout(true);

    frame->Show();
    return true;
}

BEGIN_EVENT_TABLE(DrawPane, wxPanel)
// some useful events
/*
 EVT_MOTION(DrawPane::mouseMoved)
 EVT_LEFT_DOWN(DrawPane::mouseDown)
 EVT_LEFT_UP(DrawPane::mouseReleased)
 EVT_RIGHT_DOWN(DrawPane::rightClick)
 EVT_LEAVE_WINDOW(DrawPane::mouseLeftWindow)
 EVT_KEY_DOWN(DrawPane::keyPressed)
 EVT_KEY_UP(DrawPane::keyReleased)
 EVT_MOUSEWHEEL(DrawPane::mouseWheelMoved)
 */
    EVT_LEFT_DOWN(DrawPane::mouseDown)
 // catch paint events
    EVT_PAINT(DrawPane::paintEvent)

    END_EVENT_TABLE()


    // some useful events
    /*
     void DrawPane::mouseMoved(wxMouseEvent& event) {}
     void DrawPane::mouseDown(wxMouseEvent& event) {}
     void DrawPane::mouseWheelMoved(wxMouseEvent& event) {}
     void DrawPane::mouseReleased(wxMouseEvent& event) {}
     void DrawPane::rightClick(wxMouseEvent& event) {}
     void DrawPane::mouseLeftWindow(wxMouseEvent& event) {}
     void DrawPane::keyPressed(wxKeyEvent& event) {}
     void DrawPane::keyReleased(wxKeyEvent& event) {}
     */

void DrawPane::mouseDown(wxMouseEvent& event)
{
    wxClientDC dc(this);
    int x = event.GetX();
    int y = event.GetY();
    int row_cell = x / CELL_WIDTH;
    int col_cell = y / CELL_WIDTH;
    dc.DrawText(wxString(std::to_string(row_cell) + " " + std::to_string(col_cell)), event.GetX(), event.GetY());
}

void DrawPane::render(wxDC& dc)
{
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