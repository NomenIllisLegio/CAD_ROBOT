#pragma once

#include "Spline.h"

struct SRCoordinateSystems
{
  std::vector<MMatrix<float> > pCSs;
};

struct SRStaticTMs
{
	std::vector<MMatrix<float> > pTMs;
};

struct SRLinkPositions
{
	std::vector<MMatrix<float> > pLPs;
};

struct SRParamsTable
{
	struct SParamsLine
	{
    SParamsLine()
    {
    }
    SParamsLine(const SParamsLine& pl):
    Th(pl.Th),
    S(pl.S),
    A(pl.A),
    Al(pl.Al) {}
    MSymb Th,S,A,Al;
	};
	struct SVars
	{
		MVar var;
		float min,max,def;
	};
  std::vector<SParamsLine> pPLs;
  std::vector<SVars> pVs;
};

struct SRMatricesAi
{
	std::vector<MMatrix<MSymb> > pAis;
};

struct SRMatricesHi
{
	std::vector<MMatrix<float> > Hi;
};

struct SRMatricesTi
{
	std::vector<MMatrix<MSymb> > Ti;
};

struct SOVar
{
	MVar var;
	float start, end, min, max, pen;
};

typedef std::vector<SOVar> Vars;

struct OptData
{
  void init()
  {
    step = 10.0f;
    probes_done = false;
    
    for (Vars::iterator it = vars.begin();
         it != vars.end();
         ++ it)
    {
      SOVar & var = *it;
			float val = var.start;
      if (val > var.max) val = var.max;
			if (val < var.min) val = var.min;	
      given.values[var.var] = val;      
    }
  }

  void clear()
  {
    vars.clear();  
    func = 0.0f;
  }

  float e;
  float step;
  Vars  vars;
  MSymb func;
  MGiven given;

  bool probes_done;
};

typedef std::vector<Vars> VarsVector;

struct TrajData
{
  void clear()
  {
    opt.clear();
    //splines.clear();
    //vars_vector.clear();
    //splines[0].clear();
    //splines[1].clear();
    //splines[2].clear();

    //spline_ready = false;
    start_ready = false;
    traj_ready = false;

    progress = 0;
  }
  
  //bool    spline_ready;
  bool    start_ready;
  bool    traj_ready;
  
  MSymb*      values[3];

  OptData     opt;
  float       time;
  float       max_time;
  float       min_time;
  //VarsVector  vars_vector;
  Size        point;

  int progress; //in percents
};