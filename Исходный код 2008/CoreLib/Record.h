#pragma once

#include "../BaseLib/base.h"

class Record
{
public:
  Record();
  virtual ~Record();

  void set_name(const char* name);
  const String& name() const;

  virtual void get_data() = 0;
  virtual void apply_data() const = 0;
  virtual bool consists(const char* data) const = 0;

protected:  
  String m_name;

  friend class serializers;
};
