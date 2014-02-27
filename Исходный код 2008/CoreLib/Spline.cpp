#include <algorithm>

#include "../BaseLib/base.h"

#include ".\spline.h"

//////////////////////////////////////////////////////////////////////////

bool less(const Point& p1, const Point& p2)
{
  return p1.x < p2.x;
}

//////////////////////////////////////////////////////////////////////////

Spline::Spline()
{
  m_bias = 0.0f;
  m_tens = 0.0f;
  m_cont = 0.0f;
  
  m_min_y = 10000000.0f;
  m_max_y = -10000000.0f;
}

Spline::~Spline()
{
}


void Spline::clear()
{
  m_points.clear();

  m_min_y = 10000000.0f;
  m_max_y = -10000000.0f;
}

void Spline::add_point(float x, float y)
{
  Point point;
  point.x = x;
  point.y = y;
  m_points.push_back(point);

  m_min_y = min(m_min_y, y);
  m_max_y = max(m_max_y, y);
}

Size Spline::size() const
{
  return m_points.size();
}

Point& Spline::point(Size index)
{
  return m_points[index];
}

void Spline::interpolate()
{
  std::sort(m_points.begin(), m_points.end(), less);
  for (Size p = 0; p < m_points.size(); ++p)
  {
    Point& cur = m_points[p];
    if (p == 0)
    {
      Point& next = m_points[p + 1];
      cur.ra = next.y - cur.y;
    }
    else if (p == m_points.size() - 1)
    {
      Point& prev = m_points[p - 1];            
      cur.rb= cur.y - prev.y;
    }
    else
    {
      Point& next = m_points[p + 1];
      Point& prev = m_points[p - 1];
      
      float g1 = (cur.y - prev.y) * (1.0f + m_bias);
      float g2 = (next.y - cur.y) * (1.0f - m_bias);
      float g3 = g2 - g1;

      cur.ra = (1.0f - m_tens) * (g1 + 0.5f * g3 * (1.0f + m_cont));
      cur.rb = (1.0f - m_tens) * (g1 + 0.5f * g3 * (1.0f - m_cont));
    }
  }
}

float Spline::min_x() const
{
  return m_points.front().x;
}

float Spline::max_x() const
{
  return m_points.back().x;
}

float Spline::min_y() const
{
  return m_min_y;
}

float Spline::max_y() const
{
  return m_max_y;
}

bool Spline::empty() const
{
  return m_points.empty();
}

float Spline::operator () (float x) const
{
  if (x < m_points.front().x)
    return m_points.front().y;
  for (Size p = 1; p < m_points.size(); ++ p)
  {
    const Point& cur = m_points[p];
    if (x < cur.x)
    {
      const Point& prev = m_points[p - 1];
      float t = (x - prev.x) / (cur.x - prev.x);
      float t2 = t * t;
      float t3 = t2 * t;
      return prev.y * (2.0f * t3 - 3.0f * t2 + 1.0f) +
             prev.ra * (t3 - 2.0f * t2 + t) +
             cur.y * (- 2.0f * t3 + 3.0f * t2) +
             cur.rb * (t3 - t2);
    }
  }
  return m_points.back().y;
}