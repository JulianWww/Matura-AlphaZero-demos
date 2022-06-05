#pragma once
#include <point.hpp>

namespace VQ {
  class Group: public Point {
    protected: const wxBrush* brush;

    public: Group();
    public: Group(const T& x, const T& y, const wxBrush* brush);
    public: Group(const std::pair<T, T>& pos, const wxBrush* brush);
    //public: ~Group();
    public: void render(wxDC& dc) const;
    public: const wxBrush* getBrush() const;
  };

  Group randomGroupInRange(const double& x, const double& y, const wxBrush* brush);
}