#include <group.hpp>
#include <config.hpp>
#include <utils.hpp>

VQ::Group::Group(): Group(0,0,nullptr) {}
VQ::Group::Group(const VQ::Group::T& x, const VQ::Group::T& y, const wxBrush* _brush): VQ::Point(x, y), brush(_brush) {}
VQ::Group::Group(const std::pair<VQ::Group::T, VQ::Group::T>& pos, const wxBrush* _brush): VQ::Point(pos), brush(_brush) {}
//VQ::Group::~Group() { delete brush; }

VQ::Group VQ::randomGroupInRange(const double& x, const double& y, const wxBrush* brush) {
  return Group(
    std::rand(x),
    std::rand(y),
    brush
  );
}
void VQ::Group::render(wxDC& dc) const {
  dc.SetBrush(*(this->brush));
  dc.DrawRectangle(wxPoint(this->pos.first, this->pos.second), wxSize(GRPOUT_SIZE, GRPOUT_SIZE));
}
const wxBrush* VQ::Group::getBrush() const { return this->brush; }