#include <sstream>

#include "core.h"
#include "PosRecord.h"

Core::Core(void)
{
}

Core::~Core(void)
{
}

int Core::link_count() const
{
  return int(m_links.size());
}

Link& Core::link(int n)
{
  _ASSERTE(n < link_count());
  return m_links[n];
}

int Core::insert_link(const Link& link)
{
  m_links.push_back(link);
  return int(m_links.size() - 1);
}

void Core::remove_link(int n)
{
  _ASSERTE(n < link_count());
  Links::iterator it = m_links.begin();
  std::advance(it, n);
  m_links.erase(it);
}

void Core::remove_all_links()
{
  m_links.clear();
}

CoreData& Core::data()
{
  return m_core_data;
}

Core::Options& Core::options()
{
  return m_options;
}

void Core::Init()
{
	Release();
  m_options.aUseNumbers = true;
	m_options.aBorders = true;
	//Control
	m_options.cMotStp = 0.1f;
	m_options.cRotStp = 5;
	//KinRev
	m_options.drE = 0.0001f;
	m_options.drProbes = 3;
	m_options.drUse = true;
  //GR
  m_options.grV[0] = 0.0f;
  m_options.grV[1] = 0.0f;
  m_options.grV[2] = -9.8f;
}

int Core::AddLink(int LinkTo, int LinkType)
{
  std::stringstream ss;
  int ln = insert_link(Link());
  ss << "Звено " << unsigned int(ln);
  link(ln).Name = ss.str();
	link(ln).Type=LinkType;
	link(ln).Orient=GetAutoOrientFor(ln);
	link(ln).Geo=GetAutoGeoFor(ln);	
	link(ln).m_min = data().MTs[data().LDs[LinkType].Motion].def_min;
	link(ln).m_max = data().MTs[data().LDs[LinkType].Motion].def_max;
	link(ln).m_min_speed = data().MTs[data().LDs[LinkType].Motion].def_speed_min;
	link(ln).m_max_speed = data().MTs[data().LDs[LinkType].Motion].def_speed_max;
	link(ln).m_min_acc = data().MTs[data().LDs[LinkType].Motion].def_acc_min;
	link(ln).m_max_acc = data().MTs[data().LDs[LinkType].Motion].def_acc_max;
  return ln;
}


V3f Core::GetAutoGeoFor(int Index)
{
	V3f rv;
	if (Index==0)
	{
		rv=0,0,0;
		return rv;
	}
	else
	{
		rv=link(Index-1).Orient * 0.8f;
		return rv;
	}
}

void Core::Release()
{
	remove_all_links();
  BlankReports();
  for (Size r = 0; r < m_records.size(); ++ r)
    delete m_records[r];
  m_records.clear();
  m_rev_record.clear();
  m_traj_record.clear();
}

V3f Core::GetAutoOrientFor(int Index)
{
	V3f rv;
	if (Index==0)
	{
		rv=0,1,0;
		return rv;
	}
	else
	{
		rv=link(Index-1).Orient;
		return rv;
	}
}

SRCoordinateSystems* Core::rCoordinateSystems(bool ForLinks)
{
	if (ForLinks)
	{
		if (Rs.pCSs_fl) return Rs.pCSs_fl;
	}
	else
	{
		if (Rs.pCSs) return Rs.pCSs;
	}
	SRCoordinateSystems * pCS=new SRCoordinateSystems;
  pCS->pCSs.resize(link_count(), MMatrix<float>(4, 4));
	SRLinkPositions * pLP=rLinkPositions();
//Zs
	MMatrix<float> x1(3),z0(3);
	for (int l=0;l<link_count();l++)
	{
		MMatrix<float> z(3);
		z.LoadData(link(l).Orient.d);
		pCS->pCSs[l].SubStateThis(z,0,2);
		if (l==0) z0=z;
	}
//Xs, Ys and Positions
	MMatrix<float> btm(1,4);
	btm=0,0,0,1;
	for (int l=1;l<link_count();l++)
	{
		MMatrix<float> z(3),zp(3),y(3);
		MLine<float> zl,zlp,xl;
		z.LoadData(link(l).Orient.d);
		zp.LoadData(link(l - 1).Orient.d);
		zl.abc=z;zl.xyz=pLP->pLPs[l];
		zlp.abc=zp;zlp.xyz=pLP->pLPs[l-1];
		if (!zl.IsLineCollinearTo(zlp))
			xl=zl.GetCrossingCoNormal(zlp);
		else
		{
			xl.xyz=zl.xyz;
			if (l>1)
				xl.abc=pCS->pCSs[l-1].SubMatrix(0,0,3,1);
			else
				xl.abc=zl.abc.GetXForZ();
		}

		pCS->pCSs[l].SubStateThis(xl.abc,0,0);
		pCS->pCSs[l].SubStateThis(xl.xyz,0,3);
		y=z%xl.abc;
		pCS->pCSs[l].SubStateThis(y,0,1);
		pCS->pCSs[l].SubStateThis(btm,3,0);
		if (l==1) x1=xl.abc;
	}
	//Zero Link (if exists)
	if (link_count()>0)
	{
		if (link_count()==1 || (x1==z0)) x1=z0.GetXForZ();
		//x1=z0.GetXForZ();
		
		pCS->pCSs[0].SubStateThis(x1,0,0);
		pCS->pCSs[0].SubStateThis(z0%x1,0,1);
		
		pCS->pCSs[0].SubStateThis(btm,3,0);
		pCS->pCSs[0].SubStateThis(btm.t(),0,3);
	}
	//If ForLinks
	if (ForLinks)
		for (int l=0;l<link_count();l++)
			pCS->pCSs[l].SubStateThis(pLP->pLPs[l],0,3);

	//delete pLP;
	if (ForLinks)
		Rs.pCSs_fl=pCS;
	else	
		Rs.pCSs=pCS;
	return pCS;
}

SRStaticTMs * Core::rStaticTMs(bool ForLinks)
{
	if (ForLinks)
	{
		if (Rs.pSTMs_fl) return Rs.pSTMs_fl; 
	}
	else
	{
		if (Rs.pSTMs) return Rs.pSTMs; 
	}
	
	SRStaticTMs* pTM = new SRStaticTMs;
	SRCoordinateSystems* pCS = rCoordinateSystems(ForLinks);
  pTM->pTMs.resize(link_count(), MMatrix<float>(4, 4));

  if (link_count() > 0)
  {
	  //Creating
	  pTM->pTMs[0]=pCS->pCSs[0];
	  for (int m=1;m<link_count();m++)
	  {
		  MMatrix<float>& mi=pCS->pCSs[m];
		  MMatrix<float>& mip=pCS->pCSs[m-1];
		  pTM->pTMs[m]=mip.rev()*mi;
	  }
  }

	//delete pCS;
	if (ForLinks)
		Rs.pSTMs_fl=pTM;
	else
		Rs.pSTMs=pTM;
	return pTM;
}


SRLinkPositions * Core::rLinkPositions()
{
	if (Rs.pLPs) return Rs.pLPs;
	SRLinkPositions * pLP=new SRLinkPositions;
  pLP->pLPs.resize(link_count(), MMatrix<float>(4, 4));

	MMatrix<float> v(3),v2(3);
	v=0,0,0;
	for (int l=0;l<link_count();l++)
	{
		v2.LoadData(link(l).Geo.d);	
		v+=v2;
		pLP->pLPs[l]=v;
	}
	
	Rs.pLPs=pLP;
	return pLP;
}

SRParamsTable * Core::rParamsTable()
{
	if (Rs.pPTs) return Rs.pPTs;
	SRStaticTMs * pTM = rStaticTMs();
	SRParamsTable * pPT=new SRParamsTable;

  pPT->pPLs.resize(link_count());
  pPT->pVs.resize(link_count()>0?link_count()-1:0);

	String l;
	BOOL bUse=m_options.aUseNumbers;
	BOOL bBord=m_options.aBorders;
  for (int i=1;i<link_count();i++)
	{
		MMatrix<float>& A=pTM->pTMs[i];
		int pmot=data().LDs[link(i-1).Type].Motion;
		//getting S
		float s=A(2,3);
		if ((!bUse&&s!=0)||pmot==0)
		{
      std::stringstream ss;
      ss << "S" << unsigned int(i);
      l = ss.str();
			pPT->pPLs[i].S=l;
		}
		else
			pPT->pPLs[i].S=s;
		//getting Th
		float th=mthGetAngleBySinAndCos(A(1,0),A(0,0));
		if (pmot==1)
		{
      std::stringstream ss;
      ss << "Th" << unsigned int(i);
      l = ss.str();
			pPT->pPLs[i].Th=l;
		}
		else
			pPT->pPLs[i].Th=th;
		//getting Al
		pPT->pPLs[i].Al=mthGetAngleBySinAndCos(A(2,1),A(2,2));
		//getting A
		float a;
		if (A(0,0)!=0)
			a=A(0,3)/A(0,0);
		else
			a=A(1,3)/A(1,0);
		if (!bUse&&a!=0)
		{
      std::stringstream ss;
      ss << "A" << unsigned int(i);
      l = ss.str();
			pPT->pPLs[i].A=l;
		}
		else
			pPT->pPLs[i].A=a;
		//Adding Variable
		float inc,def;
		switch (pmot)
		{
		case -1:
			continue;
		case 0:
      {
        std::stringstream ss;
        ss << "S" << unsigned int(i);
        l = ss.str();
			  pPT->pVs[i-1].var.SetName(l.c_str());
			  def=s;
      }
			break;
		case 1:
      {
        std::stringstream ss;
        ss << "Th" << unsigned int(i);
        l = ss.str();
			  pPT->pVs[i-1].var.SetName(l.c_str());
			  def=th;
      }
			break;
		}
		inc=(bBord)?def:0;
		pPT->pVs[i-1].min=link(i-1).m_min+inc;
		pPT->pVs[i-1].max=link(i-1).m_max+inc;
		pPT->pVs[i-1].def=def;
	}
	
	Rs.pPTs=pPT;
	return pPT;
}

SRMatricesAi * Core::rMatricesAi()
{
	if (Rs.pMAi) return Rs.pMAi;
	SRMatricesAi * pAi=new SRMatricesAi;
	pAi->pAis.resize(link_count(), MMatrix<MSymb>(4, 4));
	if (link_count()==0) return pAi;
	
	SRCoordinateSystems * pCS = rCoordinateSystems();	
	SRParamsTable * pPT = rParamsTable();
	
	////Copy A0
	for (int m=0;m<16;m++) pAi->pAis[0][m]=pCS->pCSs[0][m];
	////Making other
	MMatrix<MSymb> prod(4,4);
	prod=	"COS(Th)",	"-SIN(Th)*COS(Al)",	"SIN(Th)*SIN(Al)",	"A*COS(Th)",
			"SIN(Th)",	"COS(Th)*COS(Al)",	"-COS(Th)*SIN(Al)",	"A*SIN(Th)",
			"0",		"SIN(Al)",			"COS(Al)",			"S",
			"0",		"0",				"0",				"1";
	
	MWorkspace WS;
  for (int a=1;a < int(pAi->pAis.size());a++)
	{
		WS.Assign("Th",pPT->pPLs[a].Th);
		WS.Assign("S",pPT->pPLs[a].S);
		WS.Assign("Al",pPT->pPLs[a].Al);
		WS.Assign("A",pPT->pPLs[a].A);
		pAi->pAis[a]=prod;
		//pAi->pAis[a].SubState(&WS);
		for (int m=0;m<16;m++)
			pAi->pAis[a][m].Substate(&WS);
	}
	WS.Release();
	Rs.pMAi=pAi;
	return pAi;
}

SRMatricesTi * Core::rMatricesTi()
{
	if (Rs.pMTi) return Rs.pMTi;
	SRMatricesTi* pTi=new SRMatricesTi;
	pTi->Ti.resize(link_count(), MMatrix<MSymb>(4, 4));
	if (link_count()==0) return pTi;
	
  SRMatricesAi* pAi = rMatricesAi();
  MMatrix<MSymb> m(4, 4);
  m.IdentityThis();

  for (int i = 0; i < link_count(); ++ i)
  {
    m *= pAi->pAis[i];
    pTi->Ti[i] = m;
  }

	Rs.pMTi=pTi;
	return pTi;
}

SRMatricesHi * Core::rMatricesHi()
{
	if (Rs.pMHi) return Rs.pMHi;
	SRMatricesHi* pHi=new SRMatricesHi;
	pHi->Hi.resize(link_count(), MMatrix<float>(4, 4));
	if (link_count()==0) return pHi;
	
  MMatrix<float> v_r;

  for (int i = 0; i < link_count(); ++ i)
  {
    MMatrix<float>& m = pHi->Hi[i];
    const Link& lnk = link(i);
    drVectorR(i, v_r);

    float Ix = lnk.mass * (lnk.length * lnk.length / 12.0f + lnk.radius * lnk.radius / 4.0f) + lnk.mass * v_r[2] * v_r[2];
    float Iy = Ix;
    float Iz = lnk.mass * lnk.radius * lnk.radius / 2.0f;

    m.ZeroThis();
    m(0, 0) = (Iy + Iz - Ix) / 2.0f;
    m(1, 1) = (Ix + Iz - Iy) / 2.0f;
    m(2, 2) = (Ix + Iy - Iz) / 2.0f;
    
    m(3, 3) = lnk.mass;
    m(2, 3) = v_r[2] * lnk.mass;
    m(3, 2) = v_r[2] * lnk.mass;
  }
	Rs.pMHi=pHi;
	return pHi;
}

void Core::drMatriceUij(MMatrix<MSymb>& matrix, int i, int j)
{
	matrix.Init(4, 4);
	if (j > i)
	{
		matrix.ZeroThis();
		return;
	}

  MVar var = var_by_number(j);
  MMatrix<MSymb>& Ti = rMatricesTi()->Ti[i];
  for (int p = 0; p < 16; ++ p)
    matrix[p] = Ti[p].det(var);
}

void Core::drMatriceVijk(MMatrix<MSymb>& matrix, int i, int j, int k)
{
  drMatriceUij(matrix, i, j);
  MVar var = var_by_number(k);
  for (int p = 0; p < 16; ++ p)
    matrix[p] = matrix[p].det(var);
}

void Core::DelLink(int Link)
{
	if (Link==-1 || Link>=link_count()) return;
  remove_link(Link);
  //OnBigModify(); !!!
}

void Core::BlankReports()
{
	if (Rs.pCSs) delete Rs.pCSs;
	if (Rs.pCSs_fl) delete Rs.pCSs_fl;
	if (Rs.pLPs) delete Rs.pLPs;
	if (Rs.pMAi) delete Rs.pMAi;
  if (Rs.pMHi) delete Rs.pMHi;
  if (Rs.pMTi) delete Rs.pMTi;
	if (Rs.pPTs) delete Rs.pPTs;
	if (Rs.pSTMs) delete Rs.pSTMs;
	if (Rs.pSTMs_fl) delete Rs.pSTMs_fl;
	memset(&Rs,0,sizeof(Rs));
}

void Core::OnBigModify()
{
	BlankReports();
  m_rev_record.clear();
}

void Core::drGetMatrixT(int s, int e, MMatrix<MSymb> &m)
{
	_ASSERTE(e > s);
	SRMatricesAi * pMAi = rMatricesAi();
	m=pMAi->pAis[s + 1];
	for (int x=s + 2; x < e + 1; x ++)
		m *= pMAi->pAis[x];
}

void Core::drGetMatrixV(int s, int e, MMatrix<MSymb> &m)
{
  MMatrix<MSymb> T(4, 4), dT(4, 4);
  drGetMatrixT(s, e, T);
  m.ZeroThis();
  SRParamsTable* par_table = rParamsTable();
  for (int v = 0; v < int(par_table->pVs.size()); ++ v)
  {
    for (int q = 0; q < 16; ++ q)
      dT[q] = T[q].det(par_table->pVs[v].var);
    m += dT * MVar(("d" + par_table->pVs[v].var.GetName()).c_str());
  }
}

void Core::drGetMatrixC(int s, int e, MMatrix<MSymb> &m)
{
  MMatrix<MSymb> V(4, 4), dV(4, 4);
  drGetMatrixV(s, e, V);
  m.ZeroThis();
  SRParamsTable* par_table = rParamsTable();
  for (int v = 0; v < int(par_table->pVs.size()); ++ v)
  {
    for (int q = 0; q < 16; ++ q)
      dV[q] = V[q].det(par_table->pVs[v].var);
    m += dV;
  }
}

void Core::drProbes(OptData& data)
{
	if (data.probes_done)
    return;
  float best_val = 9999999999999.0f;
  MGiven best_vals;
  MGiven cur_vals;
  for (Size v = 0; v < data.vars.size(); ++ v)
  {
    const SOVar& sovar = data.vars[v];
    cur_vals.values[sovar.var] = sovar.min + (sovar.max - sovar.min) / (m_options.drProbes + 0.99f);
  }
  
  for (;;)
  {
//testing...
    float val = data.func.solve(cur_vals);
    if (val < best_val)
    {
      best_vals = cur_vals;
      best_val = val;
    }
  
//increment
    bool end = true;
    for (Size v = 0; v < data.vars.size(); ++ v)
    {
      const SOVar& sovar = data.vars[v];
      float& val = cur_vals.values[sovar.var];
      val += (sovar.max - sovar.min) / (m_options.drProbes + 0.99f);
      if (val < sovar.max)
      {
        end = false;
        break;
      }
      val = sovar.min + (sovar.max - sovar.min) / (m_options.drProbes + 0.99f);
    }

    if (end) //ending...
      break;
  }

  data.given = best_vals;
  data.probes_done = true;
}

bool Core::drOptimize(OptData& data)
{
  if (m_options.drUse)
    drProbes(data);

  //cycle by percition	
  int vars_count = int(data.vars.size());
  while (true)
	{
		bool again = false;
    for (int v = vars_count - 1; v >= 0; -- v)
		{
			SOVar & rv = data.vars[v];
      float & val = data.given.values[rv.var];
      float ove = data.func.solve(data.given);
			float left_e = ove;
      float d;
      float temp;
			//left
			temp = val;
      if (val - data.step >= rv.min)
			{
				val -= data.step;
				float lve = data.func.solve(data.given);
				if (lve < ove)
				{
					d = -data.step;
					left_e = lve;
          again = true;
				}
				//val += data.step;
			}
      val = temp;
			//right
			if (val + data.step <= rv.max)
			{
				val += data.step;
        float rve = data.func.solve(data.given);
				if (rve < ove && rve < left_e)
				{
					d = data.step;
          again = true;
				}
				//val -= data.step;
			}
      val = temp;
			if (!again) continue;
			
			//stepping down!
			float ovv = val;
			float nvv = ovv;
      float nve = ove;

			while (true)
			{
				nvv += d;
				if (nvv > rv.max || nvv < rv.min) break;
				val = nvv;
				nve = data.func.solve(data.given);
				if (nve >= ove)
					break;
				ove = nve;
				ovv = nvv;
			}
			val = ovv;
			//break;
		}
    if (again) continue;

    //exiting
    data.e = data.func.solve(data.given);
		if (data.step < m_options.drE)
    {
      for (int v = 0; v < vars_count; ++ v)
      {
			  SOVar & rv = data.vars[v];        
        rv.end = data.given.values[rv.var];
      }
      return true;
    }
    else
    {
	  	data.step /= 2;
      return false;
    }
	}
}

/*
float Core::drGetVal(MSymb & func, SOVar * pVars, int Vq)
{
  static MWorkspace WS;
  for (int v=0;v<Vq;v++)
		WS.Assign(pVars[v].var.GetName().c_str(),pVars[v].end);
	MSymb ms = func;
	ms.Substate(&WS);
	_ASSERTE(ms.IsNumber());
	return ms.GetAsNumber();
}
*/

Record* Core::find_record(const char* name)
{
  for (Size r = 0; r < m_records.size(); ++ r)
  {
    if (m_records[r]->name() == name)
      return m_records[r];
  }
  return NULL;
}

void Core::delete_record(const char* name)
{
  for (Records::iterator it = m_records.begin();
       it != m_records.end();
       ++ it)
  {
    if ((*it)->name() == name)
    {
      delete *it;
      m_records.erase(it);
      return;
    }
  }
}

void Core::apply_record(const char* name)
{
  Record* record = find_record(name);
  if (record)
    record->apply_data();
}

void Core::record_positions(const char* name)
{
  delete_record(name);
  PosRecord* pos_record = new PosRecord;
  pos_record->set_name(name);
  pos_record->get_data();
  m_records.push_back(pos_record);
}

int Core::records_count() const
{
  return int(m_records.size());
}

Record& Core::record(int record)
{
  return *m_records[record];
}

RevRecord& Core::rev_record()
{
  return m_rev_record;
}

TrajRecord& Core::traj_record()
{
  return m_traj_record;
}

String Core::link_label(int x)
{
  switch (x)
  {
  case -1:
    return "Статическая";
  case 0:
    return "Инерциальная";
  default:
    return link(x).Name;
  }
}

String Core::link_det_unit(int x) const
{
  const Link& lnk = core().link(x);
  int mot = m_core_data.LDs[lnk.Type].Motion;
  if (mot == 0)
    return "мм";
  else
    return "°";
}

void Core::enum_all_vars(Vars& vars) const
{
  vars.clear();
  SRParamsTable* pt = core().rParamsTable();
  SOVar sovar;
  for (int v = 0; v < int(pt->pVs.size()); ++v)
  {
    const SRParamsTable::SVars& p_var = pt->pVs[v];
    const Link& lnk = core().link(v);

    sovar.var = p_var.var;
    sovar.min = p_var.min;
    sovar.max = p_var.max;
    sovar.pen = 0.0f;
    sovar.start = lnk.m_curd + p_var.def;
    vars.push_back(sovar);

    sovar.var = MVar(("d" + sovar.var.GetName()).c_str());
    sovar.max = lnk.m_max_speed;
    sovar.min = lnk.m_min_speed;
    sovar.pen = 0.0f;
    sovar.start = lnk.m_cur_speed;
    vars.push_back(sovar);

    sovar.var = MVar(("d" + sovar.var.GetName()).c_str());
    sovar.max = lnk.m_max_speed;;
    sovar.min = lnk.m_min_speed;
    sovar.pen = 0.0f;
    sovar.start = lnk.m_cur_acc;
    vars.push_back(sovar);
  }
}

const SOVar* Core::find_var(const Vars& vars, const MVar& var)
{
  for (Vars::const_iterator it = vars.begin();
       it != vars.end();
       ++ it)
  {
    if (it->var == var)
      return &(*it);
  }
  return NULL;
}

void Core::apply_vars(const Vars& vars)
{
  SRParamsTable* pt = core().rParamsTable();
  MVar m_var;
  for (int v = 0; v < int(pt->pVs.size()); ++v)
  {
    const SRParamsTable::SVars& p_var = pt->pVs[v];
    Link& lnk = core().link(v);

    m_var = p_var.var;
    const SOVar* var = find_var(vars, m_var);
    if (var)
      lnk.m_curd = var->end - p_var.def;
    
    m_var = MVar(("d" + m_var.GetName()).c_str());
    var = find_var(vars, m_var);
    if (var)
      lnk.m_cur_speed = var->end;

    m_var = MVar(("d" + m_var.GetName()).c_str());
    var = find_var(vars, m_var);
    if (var)
      lnk.m_cur_acc = var->end;
  }
}

MVar Core::var_by_number(int number, int det) const
{
  SRParamsTable* pPT = core().rParamsTable();
  String s = pPT->pVs[number].var.GetName();
  for (int x = 0; x < det; ++ x)
    s = "d" + s;
  return s.c_str();
}

bool Core::drTrajectory(TrajData& data)
{
  if (!data.start_ready) //getting start for optimization
  {
    //creating func...
    m_traj_record.clear_splines();
    
    MMatrix<MSymb> matr(4, 4);
    drGetMatrixT(-1, link_count() - 1, matr);

    data.opt.func += ((matr[3] - "x").absoluted()) * 1.0f;
    data.opt.func += ((matr[7] - "y").absoluted()) * 1.0f;
    data.opt.func += ((matr[11] - "z").absoluted()) * 1.0f;

    Vars vars; //getting necessary vars...
    enum_all_vars(vars);
    for (Vars::iterator it = vars.begin();
        it != vars.end();
        ++ it)
    {
      if (data.opt.func.IsVarPresent(it->var))
        data.opt.vars.push_back(*it);
    }

    data.values[0] = data.opt.func.find_first("x");
    data.values[0]->set_type(MST_NUMBER);
    data.values[1] = data.opt.func.find_first("y");
    data.values[1]->set_type(MST_NUMBER);
    data.values[2] = data.opt.func.find_first("z");
    data.values[2]->set_type(MST_NUMBER);

    data.start_ready = true;
    data.point = 0;
    return false;
  }

  if (!data.traj_ready) //creating trajectory
  {
    //preparing func...
    const TrajPoint& pt = m_traj_record.point(data.point);
    
    data.values[0]->set_value(pt.x);
    data.values[1]->set_value(pt.y);
    data.values[2]->set_value(pt.z);
    
    m_traj_record.spline("xt").add_point(pt.time, pt.x);
    m_traj_record.spline("yt").add_point(pt.time, pt.y);
    m_traj_record.spline("zt").add_point(pt.time, pt.z);

    data.opt.init();
    while (!drOptimize(data.opt));
    
    for (Vars::const_iterator it = data.opt.vars.begin();
         it != data.opt.vars.end();
         ++ it)
    {
      const SOVar& var = *it;
      m_traj_record.spline(var.var).add_point(pt.time, var.end);
    }

    MMatrix<MSymb> matr(4, 4);
    drGetMatrixT(-1, link_count() - 1, matr);

    m_traj_record.spline("xr").add_point(pt.time, matr[3].solve(data.opt.given));
    m_traj_record.spline("yr").add_point(pt.time, matr[7].solve(data.opt.given));
    m_traj_record.spline("zr").add_point(pt.time, matr[11].solve(data.opt.given));

    ++ data.point;
    data.progress = int(data.point * 100 / m_traj_record.size());

    if (data.point >= m_traj_record.size())
    {
      data.traj_ready = true;
      m_traj_record.interpolate_splines();
    }
    
    return false;
  }

  return true;
}

//Reverse Dynamics

void Core::drVectorR(int link, MMatrix<float>& out)
{
  SRCoordinateSystems* l_pos_cs = rCoordinateSystems(false);  
  SRCoordinateSystems* l_pos_fl = rCoordinateSystems(true);

  MMatrix<float> abs_det = (l_pos_fl->pCSs[link] - l_pos_cs->pCSs[link]).SubMatrix(0, 3, 3, 1);
  
  out = l_pos_fl->pCSs[link].SubMatrix(0, 0, 3, 3).t() * abs_det;
}

void Core::drRevDyn(const Vars& vars, MMatrix<float>& Q)
{
  int N = link_count() - 1;
  SRMatricesHi* pMHi = rMatricesHi();

  Q.Init(N);
  Q.ZeroThis();

  MGiven given;
  for (Size x = 0; x < vars.size(); ++ x)
    given.values[vars[x].var] = vars[x].start;

  MMatrix<float> g(4);
  g = m_options.grV[0], m_options.grV[1], m_options.grV[2], 0.0f;
  g = g.t();

  MMatrix<float> r(4);
  r = 0.0f, 0.0f, 0.0f, 1.0f;

  MMatrix<float> f, acc;
  MMatrix<MSymb> s;

  for (int i = 0; i < N; ++ i) //i [0; q_links - 2] номер переменной в векторе Q
  {
    float& Qi = Q[i];
    
    //Part I
    for (int j = i; j < N; ++ j) //j [0; q_links - 2] - номер переменной (с нуля)
    {
      for (int k = 0; k <= j; ++ k) //k [0; q_links - 2] - номер переменной (с нуля)
      {
        float qk = given.values[var_by_number(k, 2)];
        if (qk == 0.0f)
          continue;
        
        drMatriceUij(s, j + 1, k); //матрица U. Производная j-ой системы по к-й переменной
        solve(f, s, given);

        acc = f;
        acc *= pMHi->Hi[j + 1]; //матрица инерции (надо индексировать с 1)

        drMatriceUij(s, j + 1, i);
        solve(f, s, given);

        acc *= f.t();

        Qi += acc.tr() * qk;
      }
    }

    //Part II
    for (int j = i; j < N; ++ j) //j [0; q_links - 2] - номер переменной (с нуля)
    {
      for (int k = 0; k <= j; ++ k) //k [0; q_links - 2] - номер переменной (с нуля)
      {
        float qk = given.values[var_by_number(k, 1)];
        if (qk == 0.0f)
          continue;

        for (int m = 0; m <= j; ++ m) //m [0; q_links - 2] - номер переменной (с нуля)                
        {
          float qm = given.values[var_by_number(m, 1)];
          if (qm == 0.0f)
            continue;

          drMatriceVijk(s, j + 1, k, m); //матрица V. производная U по m-ой переменной
          solve(f, s, given);

          acc = f;
          acc *= pMHi->Hi[j + 1]; //матрица инерции (надо индексировать с 1)

          drMatriceUij(s, j + 1, i);
          solve(f, s, given);

          acc *= f.t();

          Qi += acc.tr() * qk * qm;
        }
      }
    }

    //Part III
    for (int j = i; j < N; ++ j) //j [0; q_links - 2] - номер переменной (с нуля)
    {
      acc.Init(1);
      acc = -link(j + 1).mass;
      acc *= g;

      drMatriceUij(s, j + 1, i);
      solve(f, s, given);
      
      acc *= f;

      acc *= r;

      Qi += acc(0, 0);
    }
  }
}

void Core::drKinEnerg(const Vars& vars, MMatrix<float>& Q)
{
  int N = link_count() - 1;
  SRMatricesHi* pMHi = rMatricesHi();

  Q.Init(N);
  Q.ZeroThis();

  MGiven given;
  for (Size x = 0; x < vars.size(); ++ x)
    given.values[vars[x].var] = vars[x].start;

  MMatrix<float> f, acc, sum;
  MMatrix<MSymb> s;

  for (int i = 0; i < N; ++ i) //i [0; q_links - 2] номер переменной в векторе Q
  {
    float& Qi = Q[i];
    sum.Init(4, 4);
    sum.ZeroThis();
    
    for (int p = 0; p <= i; ++ p) //p [0; i] - номер переменной (с нуля)
    {
      float qp = given.values[var_by_number(p, 1)];
      if (qp == 0.0f)
        continue;

      for (int r = 0; r <= i; ++ r) //r [0; i] - номер переменной (с нуля)                
      {
        float qr = given.values[var_by_number(r, 1)];
        if (qr == 0.0f)
          continue;

        drMatriceUij(s, i + 1, p);
        solve(f, s, given);

        acc = f;
        acc *= pMHi->Hi[i + 1]; //матрица инерции (надо индексировать с 1)

        drMatriceUij(s, i + 1, r);
        solve(f, s, given);

        acc *= f.t();

        sum += acc * (qp * qr);
      }
    }
    Qi = sum.tr() * 0.5f;
  }
}

void Core::drPotEnerg(const Vars& vars, MMatrix<float>& Q)
{
  int N = link_count() - 1;
  SRMatricesHi* pMHi = rMatricesHi();
  SRMatricesTi* pMTi = rMatricesTi();

  Q.Init(N);
  Q.ZeroThis();

  MGiven given;
  for (Size x = 0; x < vars.size(); ++ x)
    given.values[vars[x].var] = vars[x].start;

  MMatrix<float> g(4);
  g = m_options.grV[0], m_options.grV[1], m_options.grV[2], 0.0f;
  g = g.t();

  MMatrix<float> r(4);
  r = 0.0f, 0.0f, 0.0f, 1.0f;

  MMatrix<float> f, acc, sum;
  MMatrix<MSymb> s;

  for (int i = 0; i < N; ++ i) //i [0; q_links - 2] номер переменной в векторе Q
  {
    float& Qi = Q[i];
    solve(f, pMTi->Ti[i + 1], given);
    Qi = -(g * f * r * link(i + 1).mass)[0];
  }
}


bool Core::manipulator_ready() const
{
  if (m_links.size() < 2)
    return false;

  if (m_links.back().Type != 2)
    return false;
  
  return true;
}

bool Core::attach_avaiable() const
{
  if (m_links.empty())
    return true;

  if (m_links.back().Type == 2)
    return false;

  return true;
}

//
//////////////////////////////////////////////////////////////////////////

void solve(MMatrix<float>& out, const MMatrix<MSymb>& in, const MGiven& given)
{
  out.Init(in.Ys, in.Ys);
  for (int m = 0; m < in.Ys * in.Xs; ++ m)
  {
    out[m] = in[m].solve(given);
  }
}

//////////////////////////////////////////////////////////////////////////

Core& core()
{
  static Core s_core;
  return s_core;
}