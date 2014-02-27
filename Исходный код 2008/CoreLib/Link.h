#pragma once

#include "../BaseLib/base.h"

class Link
{
public:
  Link(void);
  ~Link(void);
  
  String Name;
	int Type;	  //link type
	V3f Orient;	//link orientation
	V3f Geo;	  //link geometry
	
	float m_min, m_max;             //motions
	float m_min_speed, m_max_speed; //speeds
  float m_min_acc, m_max_acc; //speeds

  float m_curd;
  float m_cur_speed;
  float m_cur_acc;

  float mass;
  float length;
  float radius;
};
