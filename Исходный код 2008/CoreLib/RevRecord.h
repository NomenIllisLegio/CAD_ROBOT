#pragma once

#include <vector>

#include "record.h"

struct OptData;

struct RevArg
{
public:
  //operations
  String string_mean() const;
  void apply_to_data(OptData& data) const;
  
  //data
  enum Type
  {
    rat_pos
  };

  Type  type;

  union
  {
    struct 
    {
      int mean; //0-pos, 1-vel, 2-acc
      int start; //start system
      int end; //end system
      int member; //mattrix memeber
      float penalty; //penalty for miss
      float value; //desired value
    };
  };
};

class RevRecord : public Record
{
public:
  RevRecord(void);
  virtual ~RevRecord(void);

  virtual void get_data();
  virtual void apply_data() const;
  virtual bool consists(const char* data) const;

  int add_arg(const RevArg& arg);
  RevArg& arg(int index);
  int arg_count()const;
  void del_arg(int index);
  void clear();

protected:
  typedef std::vector<RevArg> RevArgs;

  RevArgs m_args;

  friend class serializers;
};
