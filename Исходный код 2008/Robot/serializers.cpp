#include "Stdafx.h"

#include ".\serializers.h"

serializers::serializers(void)
{
}

serializers::~serializers(void)
{
}

//CORE
void serializers::store(CArchive& a, Core& core)
{
  a.Write(&core.options(), sizeof(Core::Options));
  a << core.link_count();
  for (int l = 0; l < core.link_count(); ++ l)
  {
    //Сериализация одного линка
    Link& link = core.link(l);
    store(a, link);
  }
  store(a, core.m_rev_record);
  store(a, core.m_traj_record);

  a << core.m_records.size();
  for (Size n = 0; n < core.m_records.size(); ++ n)
  {
    store(a, (PosRecord&)(*core.m_records[n]));
  }
}

void serializers::load(CArchive& a, Core& core)
{
  a.Read(&core.options(), sizeof(Core::Options));
  Size l_count;
  a >> l_count;
  for (Size l = 0; l < l_count; ++ l)
  {
    //Сериализация одного линка
    Link link;
    load(a, link);
    core.insert_link(link);
  }
  load(a, core.m_rev_record);
  load(a, core.m_traj_record);

  core.m_records.clear();
  a >> l_count;
  for (Size n = 0; n < l_count; ++ n)
  {
    PosRecord* rec = new PosRecord;
    load(a, *rec);
    core.m_records.push_back(rec);
  }
}

//LINK
void serializers::store(CArchive& a, Link& link)
{
	a << CString(link.Name.c_str());

  a << link.Type;
  a.Write(&link.Orient, sizeof(link.Orient));
	a.Write(&link.Geo, sizeof(link.Geo));
	a << link.m_min;
	a << link.m_max;
	a << link.m_curd;
	
  a << link.m_max_speed;
	a << link.m_min_speed;
	a << link.m_cur_speed;
  
  a << link.m_cur_acc;
  a << link.m_max_acc;
  a << link.m_min_acc;
  
  a << link.mass;
  a << link.length;
  a << link.radius;
}

void serializers::load(CArchive& a, Link& link)
{
	CString name;
	a >> name;
  link.Name = name;
  
  a >> link.Type;
  a.Read(&link.Orient, sizeof(link.Orient));
	a.Read(&link.Geo, sizeof(link.Geo));
	a >> link.m_min;
	a >> link.m_max;
	a >> link.m_curd;
	a >> link.m_max_speed;
	a >> link.m_min_speed;
	a >> link.m_cur_speed;
  a >> link.m_cur_acc;
  a >> link.m_max_acc;
  a >> link.m_min_acc;

  a >> link.mass;
  a >> link.length;
  a >> link.radius;
}

//Record
void serializers::store(CArchive& a, Record& rec)
{
  CString str = rec.m_name.c_str();
  a << str;
}

void serializers::load(CArchive& a, Record& rec)
{
  CString str;
  a >> str;
  rec.m_name = str;
}

//RevRecord
void serializers::store(CArchive& a, RevRecord& rev_r)
{
  store(a, static_cast<Record&>(rev_r));
  a << rev_r.m_args.size();
  for (Size q = 0; q < rev_r.m_args.size(); ++ q)
  {
    RevArg& arg = rev_r.m_args[q];
    a << arg.end;
    a << arg.mean;
    a << arg.member;
    a << arg.penalty;
    a << arg.start;
    a << (int)arg.type;
    a << arg.value;
  }
}

void serializers::load(CArchive& a, RevRecord& rev_r)
{
  load(a, static_cast<Record&>(rev_r));
  Size s;
  a >> s;
  rev_r.m_args.clear();
  for (Size q = 0; q < s; ++ q)
  {
    RevArg arg;
    int type;
    a >> arg.end;
    a >> arg.mean;
    a >> arg.member;
    a >> arg.penalty;
    a >> arg.start;
    
    a >> type;
    arg.type = static_cast<RevArg::Type>(type);
    
    a >> arg.value;
    rev_r.m_args.push_back(arg);
  }
}
//TrajRecord
void serializers::store(CArchive& a, TrajRecord& rev_r)
{
  store(a, static_cast<Record&>(rev_r));
  a << rev_r.m_points.size();
  for (Size q = 0; q < rev_r.m_points.size(); ++ q)
  {
    TrajPoint& arg = rev_r.m_points[q];
    a << arg.time;
    a << arg.x;
    a << arg.y;
    a << arg.z;
  }
}

void serializers::load(CArchive& a, TrajRecord& rev_r)
{
  load(a, static_cast<Record&>(rev_r));
  Size s;
  a >> s;
  rev_r.m_points.clear();
  for (Size q = 0; q < s; ++ q)
  {
    TrajPoint arg;
    a >> arg.time;
    a >> arg.x;
    a >> arg.y;
    a >> arg.z;
    rev_r.m_points.push_back(arg);
  }
}

void serializers::store(CArchive& a, PosRecord& rev_r)
{
  store(a, static_cast<Record&>(rev_r));
  a << rev_r.m_link_positions.size();
  for (Size q = 0; q < rev_r.m_link_positions.size(); ++ q)
  {
    PosRecord::LinkPos& pos = rev_r.m_link_positions[q];
    a << pos.position;
    a << pos.speed;
    a << pos.acceleration;
  }
}

void serializers::load(CArchive& a, PosRecord& rev_r)
{
  load(a, static_cast<Record&>(rev_r));
  Size s;
  a >> s;
  rev_r.m_link_positions.clear();
  for (Size q = 0; q < s; ++ q)
  {
    PosRecord::LinkPos pos;
    a >> pos.position;
    a >> pos.speed;
    a >> pos.acceleration;
    rev_r.m_link_positions.push_back(pos);
  }
}

