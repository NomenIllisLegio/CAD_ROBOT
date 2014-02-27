#include ".\posrecord.h"

#include "core.h"

PosRecord::PosRecord()
{
}

PosRecord::~PosRecord()
{
}

bool PosRecord::consists(const char* data) const
{
  return data == "positions";
}

void PosRecord::get_data()
{
  LinkPos link_pos;
  m_link_positions.clear();
  for (int l = 0; l < core().link_count(); ++ l)
  {
    const Link& link = core().link(l);
    link_pos.position = link.m_curd;
    link_pos.speed = link.m_cur_speed;
    m_link_positions.push_back(link_pos);
  }
}

void PosRecord::apply_data() const
{
  int max = min(int(m_link_positions.size()), core().link_count());
  for (int l = 0; l < max; ++ l)
  {
    Link& link = core().link(l);
    const LinkPos& link_pos = m_link_positions[l];
    link.m_curd = link_pos.position;
    link.m_cur_speed = link_pos.speed;
  }
}
