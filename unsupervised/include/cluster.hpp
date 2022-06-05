#pragma once

#include <group.hpp>
#include <vector>

namespace VQ {
  class Cluster {
    private: std::vector<Point> points;
    private: std::vector<Group> groups;
    public:  Cluster(const size_t& points, const double& x, const double& y, const std::vector<const wxBrush*>& brushes);
    public:  void update();
    public:  void render(wxDC& dc);
    private: Group* getClosestGroup(const Point& point);
    private: const Group* getClosestGroup(const Point& point) const;
    private: const wxBrush* getPointBrush(const Point& point) const;
    private: void generatePoints(const size_t& count, const double& x, const double& y);
    private: void generateGroups(const double& x, const double& y, const std::vector<const wxBrush*>& brushes);
  };
}