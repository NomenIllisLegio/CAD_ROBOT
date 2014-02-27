#include ".\record.h"

Record::Record()
{
}

Record::~Record()
{
}

void Record::set_name(const char* name)
{
  m_name = name;
}

const String& Record::name() const
{
  return m_name;
}
