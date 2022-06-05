#pragma once
#include <iostream>
#include <wx/wx.h>

namespace VQ {
  class Point {
    public: using T =             double;

    public: Point();
    public: Point(const Point& p);
    public: Point(const T& x, const T& y);
    public: Point(const std::pair<T, T>& pos);
    public: std::pair<T, T> getPos();
    public: const std::pair<T, T> getPos() const;

    public: void moveTo(const T& x, const T& y);
    public: void moveTo(const std::pair<T, T>& pos);
    public: void moveTo(const Point& point);
    public: void moveBy(const T& dx, const T& dy);
    public: void moveBy(const std::pair<T, T>& dpos);
    public: void moveBy(const Point& dpoint);

    public: Point operator*(const float scalar) const;
    public: Point operator*(const double scalar) const;
    public: Point operator-(const Point& other) const;
    public: Point operator+(const Point& other) const;
    public: Point& operator=(const Point&& other);
    public: Point& operator=(const std::pair<T, T>&& other);
    public: friend std::ostream& operator<<(std::ostream& os, const Point& dt);
    public: double abs() const;
    protected: std::pair<T, T> pos;

    public: void render(wxDC& dc, const wxBrush* brush) const;
  };

  Point randomPointInRange(const double& dx, const double& dy);
  std::ostream& operator<<(std::ostream& os, const Point& dt);
}