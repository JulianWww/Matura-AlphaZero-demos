#include <model.hpp>
#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/timer.h>
#include <config.hpp>

class BasicDrawPane;

class RenderTimer : public wxTimer
{
    BasicDrawPane* pane;
public:
    RenderTimer(BasicDrawPane* pane);
    void Notify();
    void start();
};

class MyFrame;
class BasicDrawPane : public wxPanel
{
  private: MyFrame* parent;
  private: SL::Model* model;
public:
    BasicDrawPane(MyFrame* parent);
    ~BasicDrawPane();
	
    void paintEvent(wxPaintEvent& evt);
    void paintNow();
    void render( wxDC& dc );
    
    DECLARE_EVENT_TABLE()
};

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
    wxBoxSizer* sizer;
    MyFrame() : wxFrame((wxFrame *)NULL, -1,  wxT("Hello wxDC"), wxPoint(50,50), wxSize(WIDTH,HEIGHT))
    {
        sizer = new wxBoxSizer(wxHORIZONTAL);
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



BasicDrawPane::BasicDrawPane(MyFrame* _parent) :
wxPanel(_parent), parent(_parent)
{
  model = new SL::Model;
}
BasicDrawPane::~BasicDrawPane(){
  delete model;
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
  //std::cout << this->model->forward(torch::ones({1,2})) << std::endl;
  this->model->train();
  auto size = this->parent->sizer->GetSize();
  dc.DrawBitmap(this->model->getMap().Rescale(size.GetX(), size.GetY()), wxPoint(0,0), false);
  dc.SetBrush(*wxTRANSPARENT_BRUSH);
  dc.DrawEllipse(
    size.GetX()/2 - size.GetX() * (double)CIRC_RADIUS/((double)WIDTH), 
    size.GetY()/2 - size.GetY() * (double)CIRC_RADIUS/((double)HEIGHT), 
    2 * size.GetX() * (double)CIRC_RADIUS/((double)WIDTH),
    2 * size.GetY() * (double)CIRC_RADIUS/((double)HEIGHT)
  );
}
