#include <wx/sizer.h>
#include <wx/timer.h>
#include <config.hpp>
#include <cluster.hpp>

class BasicDrawPane;

class RenderTimer : public wxTimer
{
    BasicDrawPane* pane;
public:
    RenderTimer(BasicDrawPane* pane);
    void Notify();
    void start();
};


class BasicDrawPane : public wxPanel
{
    VQ::Cluster* cluster;
public:
    BasicDrawPane(wxFrame* parent);
    ~BasicDrawPane();
	
    void paintEvent(wxPaintEvent& evt);
    void paintNow();
    void render( wxDC& dc );
    
    DECLARE_EVENT_TABLE()
};

class MyFrame;

class MyApp: public wxApp
{
    bool OnInit();
    
    MyFrame* frame;
public:
        
};


RenderTimer::RenderTimer(BasicDrawPane* pane) : wxTimer()
{
    RenderTimer::pane = pane;
}

void RenderTimer::Notify()
{
    pane->Refresh();
}

void RenderTimer::start()
{
    wxTimer::Start(10);
}

IMPLEMENT_APP(MyApp)

class MyFrame : public wxFrame
{
    RenderTimer* timer;
    BasicDrawPane* drawPane;
    
public:
    MyFrame() : wxFrame((wxFrame *)NULL, -1,  wxT("Hello wxDC"), wxPoint(50,50), wxSize(WIDTH,HEIGHT))
    {
        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        drawPane = new BasicDrawPane( this );
        sizer->Add(drawPane, 1, wxEXPAND);
        SetSizer(sizer);
        
        timer = new RenderTimer(drawPane);
        Show();
        timer->start();
    }
    ~MyFrame()
    {
        delete timer;
    }
    void onClose(wxCloseEvent& evt)
    {
        timer->Stop();
        evt.Skip();
    }
    DECLARE_EVENT_TABLE()
};


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_CLOSE(MyFrame::onClose)
END_EVENT_TABLE()

bool MyApp::OnInit()
{
    frame = new MyFrame();
    frame->Show();

    return true;
} 


BEGIN_EVENT_TABLE(BasicDrawPane, wxPanel)
EVT_PAINT(BasicDrawPane::paintEvent)
END_EVENT_TABLE()



BasicDrawPane::BasicDrawPane(wxFrame* parent) :
wxPanel(parent)
{
  auto a = wxGREEN_BRUSH;
  std::vector<const wxBrush*> brushes;
  brushes.push_back(wxGREEN_BRUSH);
  brushes.push_back(wxRED_BRUSH);
  brushes.push_back(wxBLUE_BRUSH);
  this->cluster = new VQ::Cluster(POINT_COUNT, (double)WIDTH, (double)HEIGHT, brushes);
}
BasicDrawPane::~BasicDrawPane(){
  delete this->cluster;
}


void BasicDrawPane::paintEvent(wxPaintEvent& evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void BasicDrawPane::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void BasicDrawPane::render( wxDC& dc )
{
    this->cluster->update();
    
    dc.SetBackground( *wxWHITE_BRUSH );
    dc.SetBrush( *wxBLUE_BRUSH);
    dc.Clear();
    this->cluster->render(dc);
}
