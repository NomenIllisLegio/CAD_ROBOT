#include ".\coredata.h"

CoreData::CoreData(void)
{
  SMotions tMTs[2]=
  {
	  {"Перемещение, м",-0.5f,0.5f, "Пост. скор., м/сек", -0.1f, 0.1f, "Пост. уск., м/сек^2", -0.02f, 0.02f},
    {"Вращение, град",-180,180, "Угл. скор., град/сек", -5, 5, "Угл. уск., град/сек^2", -1.0f, 1.0f,}
  };
  MTs[0] = tMTs[0];
  MTs[1] = tMTs[1];

  SLinkData tLDs[3]=
  {
	  {"Пост. кин. пара (V кл.)",/*IDI_ML5*/0,0},
	  {"Вращ. кин. пара (V кл.)",/*IDI_RL5*/0,1},
	  {"Схват",/*IDI_GRIPE1*/0,-1}
  };
  LDs[0] = tLDs[0];
  LDs[1] = tLDs[1];
  LDs[2] = tLDs[2];

  String tMLs[16]=
  {
	  "COS(X(i)^X(j))",
	  "COS(Y(i)^X(j))",
	  "COS(Z(i)^X(j))",
	  "X (i, j)",
	  "COS(Z(i)^Y(j))",
	  "COS(Y(i)^Y(j))",
	  "COS(Z(i)^Y(j))",
	  "Y (i, j)",
	  "COS(Z(i)^Z(j))",
	  "COS(Y(i)^Z(j))",
	  "COS(Z(i)^Z(j))",
	  "Z (i, j)",
    " --- ",
    " --- ",
    " --- ",
    " --- "
  };

  int tLOs[16] = 
  {
    3, 7, 11, 0, 1, 2, 4, 5, 6, 8, 9, 10, 12, 13, 14, 15
  };

  for (int n = 0; n < 16; ++ n)
  {
    MLs[n] = tMLs[n];
    LOs[n] = tLOs[n];
  }

  PTs[0] = "Положение";
  PTs[1] = "Скорость";
  PTs[2] = "Ускорение";
}

CoreData::~CoreData(void)
{
}
