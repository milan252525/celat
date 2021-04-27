#include "wx/wx.h"
#include "wx/sizer.h"

constexpr size_t CELL_WIDTH = 25;
constexpr size_t GRID_WIDTH = 20;

class DrawPane : public wxPanel
{

public:
    DrawPane(wxFrame* parent);

    void paintEvent(wxPaintEvent& evt);
    void paintNow();

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
public:

};

IMPLEMENT_APP(MyApp)


bool MyApp::OnInit()
{
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    frame = new wxFrame((wxFrame*)NULL, -1, wxT("Hello wxDC"), wxPoint(50, 50), wxSize((GRID_WIDTH * CELL_WIDTH) + GRID_WIDTH * 2, (GRID_WIDTH*CELL_WIDTH)+GRID_WIDTH*2));

    drawPane = new DrawPane((wxFrame*)frame);
    sizer->Add(drawPane, 1, wxEXPAND);

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

DrawPane::DrawPane(wxFrame* parent) :
    wxPanel(parent)
{
}

void DrawPane::paintEvent(wxPaintEvent& evt)
{
    wxPaintDC dc(this);
    render(dc);
}


void DrawPane::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void DrawPane::mouseDown(wxMouseEvent& event)
{
    wxClientDC dc(this);
    dc.DrawText(wxString(std::to_string(event.GetX()) + " " + std::to_string(event.GetY())), event.GetX(), event.GetY());
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