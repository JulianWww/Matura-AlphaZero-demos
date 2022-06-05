#include <point.hpp>
#include <utils.hpp>
#include <config.hpp>

VQ::Point::Point(): pos({0,0}) {}
VQ::Point::Point(const VQ::Point& p): pos(p.getPos()) {}
VQ::Point::Point(const VQ::Point::T& x, const VQ::Point::T& y): pos({x, y}) {}
VQ::Point::Point(const std::pair<VQ::Point::T, VQ::Point::T>& _pos): pos(_pos) {}
std::pair<VQ::Point::T, VQ::Point::T> VQ::Point::getPos() {return pos;}
const std::pair<VQ::Point::T, VQ::Point::T> VQ::Point::getPos() const {return pos;}

void VQ::Point::moveTo(const VQ::Point::T& x, const VQ::Point::T& y) {
  this->moveTo(std::pair<double, double>(x, y));
}
void VQ::Point::moveTo(const std::pair<VQ::Point::T, VQ::Point::T>& _pos) {this->pos = _pos;}
void VQ::Point::moveTo(const Point& point) {
  this->moveTo(point.getPos());
}
void VQ::Point::moveBy(const VQ::Point::T& dx, const VQ::Point::T& dy) {
  this->pos = {
    dx + this->pos.first,
    dy + this->pos.second
  };
}
void VQ::Point::moveBy(const std::pair<VQ::Point::T, VQ::Point::T>& _pos){
  this->moveBy(_pos.first, _pos.second);
}
void VQ::Point::moveBy(const Point& point) {
  this->moveBy(point.getPos());
}

VQ::Point VQ::Point::operator*(const float scalar) const {
  return Point(
    scalar * this->pos.first,
    scalar * this->pos.second
  );
}
VQ::Point VQ::Point::operator*(const double scalar) const {
  return Point(
    scalar * this->pos.first,
    scalar * this->pos.second
  );
}
VQ::Point VQ::Point::operator+(const VQ::Point& point) const {
  return Point(
    this->pos.first + point.getPos().first,
    this->pos.second + point.getPos().second
  );
}
VQ::Point VQ::Point::operator-(const VQ::Point& point) const {
  return Point(
    this->pos.first  - point.getPos().first,
    this->pos.second - point.getPos().second
  );
}

VQ::Point& VQ::Point::operator=(const Point&& other) {
  return (*this = other.getPos());
}
VQ::Point& VQ::Point::operator=(const std::pair<VQ::Point::T, VQ::Point::T>&& other) {
  this->pos = other;
  return *this;
}
double VQ::Point::abs() const {
  return std::sqrt(
      this->pos.first * this->pos.first
    + this->pos.second * this->pos.second
  );
}

void VQ::Point::render(wxDC& dc, const wxBrush* brush) const {
  dc.SetBrush(*brush);
  dc.DrawCircle(this->pos.first, this->pos.second, POINT_RADIUS);
}

VQ::Point VQ::randomPointInRange(const double& x, const double& y) {
  return Point(
    std::rand(x),
    std::rand(y)
  );
}
std::ostream& VQ::operator<<(std::ostream& stream,  const VQ::Point& p) {
  stream << "<" << p.getPos().first << "< " << p.getPos().second << ">";
  return stream;
}