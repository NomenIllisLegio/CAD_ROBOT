#pragma once

#include <vector>

#include "Link.h"
#include "CoreData.h"
#include "reports.h"
#include "Record.h"
#include "RevRecord.h"
#include "TrajRecord.h"
#include "PosRecord.h"
#include "Spline.h"

class Core
{
public:
  struct Options
	{
    ////analys
		bool aUseNumbers;//исп. числа вместо A и S.
		bool aBorders;	 //граничные значения.
		////control
		float cMotStp;
		float cRotStp;
		////kinrev
		float drE;		//точность в обр. кинематике
		int drProbes;	//кол-во проб.
		bool drUse;		//исп. пробы.
    ////grav
    float grV[3];
  };

  Core(void);
  ~Core(void);

  Link& link(int n);
  int link_count() const;
  int insert_link(const Link& link);
  void remove_link(int n);
  void remove_all_links();
  
  Options& options();
  CoreData& data();

  void drGetMatrixT(int s, int e, MMatrix<MSymb> & m);
	void drGetMatrixV(int s, int e, MMatrix<MSymb> & m);
	void drGetMatrixC(int s, int e, MMatrix<MSymb> & m);
  
  bool drOptimize(OptData& data);
	void drProbes(OptData& data);
	
	void drMatriceUij(MMatrix<MSymb>& matrix, int i, int j);
  void drMatriceVijk(MMatrix<MSymb>& matrix, int i, int j, int k);
  bool drTrajectory(TrajData& data);
  void drRevDyn(const Vars& vars, MMatrix<float>& Q);
  void drKinEnerg(const Vars& vars, MMatrix<float>& Q);
  void drPotEnerg(const Vars& vars, MMatrix<float>& Q);
  void drVectorR(int link, MMatrix<float>& out);

  SRStaticTMs*			    rStaticTMs(bool ForLinks=false);
	SRLinkPositions*		  rLinkPositions();
	SRCoordinateSystems*	rCoordinateSystems(bool ForLinks=false);
	SRParamsTable*			  rParamsTable();
	SRMatricesAi* 			  rMatricesAi();
  SRMatricesHi*         rMatricesHi();
  SRMatricesTi*         rMatricesTi();

  void OnBigModify();
	void BlankReports();

  V3f GetAutoOrientFor(int Index);
	V3f GetAutoGeoFor(int Index);

  void Release();
	int AddLink(int LinkTo, int LinkType);
	void Init();
	void DelLink(int Link);

	//OTHER
  Record* find_record(const char* name);
  void delete_record(const char* name);
  void apply_record(const char* name);
  void record_positions(const char* name);
  int records_count() const;
  Record& record(int record);

  RevRecord& rev_record();
  TrajRecord& traj_record();

  String link_label(int x);
  String link_det_unit(int x) const;

  void enum_all_vars(Vars& vars) const;
  void apply_vars(const Vars& vars);
  static const SOVar* find_var(const Vars& vars, const MVar& var);
  MVar var_by_number(int number, int det = 0) const;

  bool manipulator_ready() const;
  bool attach_avaiable() const;

protected:
  typedef std::vector<Link> Links;
	struct SReports
	{
		SRStaticTMs*			    pSTMs,*pSTMs_fl;
		SRLinkPositions*		  pLPs;
		SRCoordinateSystems*	pCSs,*pCSs_fl;
		SRParamsTable*			  pPTs;
		SRMatricesAi *			  pMAi;
    SRMatricesHi *			  pMHi;
    SRMatricesTi *			  pMTi;
	};
  typedef std::vector<Record*> Records;

  SReports  Rs;
  Links     m_links;
  Options   m_options;
  CoreData  m_core_data;
  Records   m_records;
  
  RevRecord   m_rev_record;
  TrajRecord  m_traj_record;

  friend class serializers;
};

void solve(MMatrix<float>& out, const MMatrix<MSymb>& in, const MGiven& given);

Core& core();