#pragma once

#include <vector>

#include "Record.h"
#include "Spline.h"
#include "reports.h"

struct TrajPoint
{
  float time;
  float x;
  float y;
  float z;
};

class TrajRecord : public Record
{
public:
  TrajRecord();
  virtual ~TrajRecord();

  virtual void get_data();
  virtual void apply_data() const;
  virtual bool consists(const char* data) const;

  void add_point(const TrajPoint&);
  Size size();
  TrajPoint& point(Size index);
  void del_point(Size index);
  void update_data();

  Spline& spline(const MVar& var);
  void clear_splines();
  void interpolate_splines();
  float start_time() const;
  float finish_time() const;
  void vars_in_time(float t, Vars& vars) const;
  bool traj_ready() const;

  void clear();

protected:
  typedef std::vector<TrajPoint> TrajPoints;
  typedef std::map<MVar, Spline> TrajSplines;

  TrajPoints  m_points;
  TrajSplines m_splines;

  friend class serializers;
};
