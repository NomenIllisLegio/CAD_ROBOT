#include ".\trajrecord.h"

#include <algorithm>

TrajRecord::TrajRecord()
{
}

TrajRecord::~TrajRecord()
{
}

void TrajRecord::get_data()
{
}

void TrajRecord::apply_data() const
{
}

bool TrajRecord::consists(const char* data) const
{
  return false;
}

bool less(const TrajPoint& p1, const TrajPoint& p2)
{
  return p1.time < p2.time;
}

void TrajRecord::add_point(const TrajPoint& p)
{
  m_points.push_back(p);
  update_data();
}

void TrajRecord::update_data()
{
  std::sort(m_points.begin(), m_points.end(), less);
}

Size TrajRecord::size()
{
  return m_points.size();
}

TrajPoint& TrajRecord::point(Size index)
{
  return m_points[index];
}

void TrajRecord::del_point(Size index)
{
  TrajPoints::iterator it = m_points.begin();
  std::advance(it, index);
  m_points.erase(it);
  update_data();
}

Spline& TrajRecord::spline(const MVar& var)
{
  return m_splines[var];
}

void TrajRecord::clear_splines()
{
  m_splines.clear();
}

void TrajRecord::interpolate_splines()
{
  for (TrajSplines::iterator it = m_splines.begin();
        it != m_splines.end();
        ++ it)
  {
    it->second.interpolate();
  }
}

bool TrajRecord::traj_ready() const
{
  return !m_splines.empty();
}

float TrajRecord::start_time() const
{
  if (m_points.empty())
    return 0.0f;
  else
    return m_points.front().time;
}

float TrajRecord::finish_time() const
{
  if (m_points.empty())
    return 0.0f;
  else
    return m_points.back().time;
}

void TrajRecord::vars_in_time(float t, Vars& vars) const
{
  vars.clear();
  for (TrajSplines::const_iterator it = m_splines.begin();
       it != m_splines.end();
       ++ it)
  {
    SOVar var;
    var.var = it->first;
    var.end = it->second(t);
    vars.push_back(var);
  }
}

void TrajRecord::clear()
{
  m_points.clear();
  m_splines.clear();
}