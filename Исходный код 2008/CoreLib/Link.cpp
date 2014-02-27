#include "link.h"

Link::Link(void)
:Name("Link")
,m_curd(0.0f)
,m_cur_speed(0.0f)
,m_cur_acc(0.0f)
,mass(10.0f)
,length(0.2f)
,radius(0.05f)
,m_min_acc(-1.0f)
,m_max_acc(1.0f)
{
}

Link::~Link(void)
{
}

