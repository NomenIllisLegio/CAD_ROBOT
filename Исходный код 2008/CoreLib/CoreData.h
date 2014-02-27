#pragma once

#include "../BaseLib/base.h"

struct SMotions
{
	String Capt;
	float def_min, def_max;

	String Speed_Capt;
	float def_speed_min, def_speed_max;

	String acc_capt;
	float def_acc_min, def_acc_max;
};

struct SLinkData
{
	String Name;	//name
	UINT Icon;		//link icon
	int Motion;		//type of motion -1-none
};

class CoreData
{
public:
  CoreData(void);
  ~CoreData(void);

  SMotions  MTs[2];
  SLinkData LDs[3];
  String    MLs[16];
  int       LOs[16];
  String    PTs[3];
};
