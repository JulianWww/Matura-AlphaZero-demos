#include <cluster.hpp>
#include <climits>
#include <float.h>
#include <utils.hpp>
#include <chrono>

void set_seed() {
  size_t ms = std::chrono::duration_cast< std::chrono::milliseconds >(
    std::chrono::system_clock::now().time_since_epoch()
  ).count();
  std::cout << "seed: " << std::hex << ms << std::endl;
  std::srand(ms);
}

VQ::Cluster::Cluster(const size_t& count, const double& x, const double& y, const std::vector<const wxBrush*>& brushes) { 
  set_seed();
  this->generatePoints(count, x, y);
  this->generateGroups(x, y, brushes);
}

void VQ::Cluster::update() {
  Point& point = jce::randElement(this->points);
  Group* group = this->getClosestGroup(point);
  Point delta = (point - (*group)) * 0.01;
  group->moveBy(delta);
}
void VQ::Cluster::render(wxDC& dc) {
  for (auto const point : this->points) {
    point.render(dc, this->getPointBrush(point));
  }
  for (auto const group : this->groups) {
    group.render(dc);
  }
}
VQ::Group* VQ::Cluster::getClosestGroup(const VQ::Point& point) {
  double distance = DBL_MAX;
  Group* out = nullptr;
  for (auto& group : this->groups) {
    double other = (group - point).abs();
    if (other < distance) {
      distance = other;
      out = &group;
    }
  }
  return out;
}
const VQ::Group* VQ::Cluster::getClosestGroup(const VQ::Point& point) const {
  double distance = DBL_MAX;
  const Group* out = nullptr;
  for (auto const& group : this->groups) {
    double other = (group - point).abs();
    if (other < distance) {
      distance = other;
      out = &group;
    }
    else {
      int a = 1;
    }
  }
  return out;
}
const wxBrush* VQ::Cluster::getPointBrush(const Point& point) const {
  
  return this->getClosestGroup(point)->getBrush();
}
void VQ::Cluster::generatePoints(const size_t& count, const double& x, const double& y) { 
  this->points.clear();
  this->points.resize(count);
  for (auto iter = this->points.begin(); iter != this->points.end(); iter++) {
    *iter = randomPointInRange(x, y);
  }
}
void VQ::Cluster::generateGroups(const double& x, const double& y, const std::vector<const wxBrush*>& brushes) {
  this->groups.clear();
  this->groups.resize(brushes.size());
  for (size_t idx = 0; idx < brushes.size(); idx++) {
    groups[idx] = randomGroupInRange(x, y, brushes[idx]);
  }
}