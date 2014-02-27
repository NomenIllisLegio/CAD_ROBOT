#pragma once

#include <vector>

#include "Record.h"

class PosRecord : public Record
{
public:
  PosRecord();
  virtual ~PosRecord();

  virtual void get_data();
  virtual void apply_data() const;
  virtual bool consists(const char* data) const;

protected:
  struct LinkPos
  {
    float position;
    float speed;
    float acceleration;
  };
  typedef std::vector<LinkPos> LinkPositions;

  LinkPositions m_link_positions;

  friend class serializers;
};
