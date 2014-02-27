#include ".\coredata.h"

CoreData::CoreData(void)
{
  SMotions tMTs[2]=
  {
	  {"�����������, �",-0.5f,0.5f, "����. ����., �/���", -0.1f, 0.1f, "����. ���., �/���^2", -0.02f, 0.02f},
    {"��������, ����",-180,180, "���. ����., ����/���", -5, 5, "���. ���., ����/���^2", -1.0f, 1.0f,}
  };
  MTs[0] = tMTs[0];
  MTs[1] = tMTs[1];

  SLinkData tLDs[3]=
  {
	  {"����. ���. ���� (V ��.)",/*IDI_ML5*/0,0},
	  {"����. ���. ���� (V ��.)",/*IDI_RL5*/0,1},
	  {"�����",/*IDI_GRIPE1*/0,-1}
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

  PTs[0] = "���������";
  PTs[1] = "��������";
  PTs[2] = "���������";
}

CoreData::~CoreData(void)
{
}
