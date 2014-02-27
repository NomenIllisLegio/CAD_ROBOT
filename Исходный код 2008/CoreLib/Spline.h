#pragma once

#include <vector>

struct Point
{
  float x;
  float y;
  
  float ra;
  float rb;
};

class Spline
{
public:
  Spline();
  virtual ~Spline();

  void clear();
  void add_point(float x, float y);
  Size size() const;
  Point& point(Size index);

  void interpolate();

  float min_x() const;
  float max_x() const;
  float min_y() const;
  float max_y() const;
  bool empty() const;


  virtual float operator () (float x) const;

protected:
  typedef std::vector<Point> Points;

  Points  m_points;
  float   m_bias;
  float   m_tens;
  float   m_cont;

  float m_min_y;
  float m_max_y;
};

/*
Spline spline;
spline.add_point(3, 5);
spline.add_point(2, 1);

spline.interpolate();

float res = spline(3.4f);

spline.clear();
*/