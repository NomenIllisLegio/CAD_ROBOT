#pragma once

#include "BaseTypes.h"

///////////////////
//class MVar

#define VAR_LEN 8

class MVar
{
public:
	MVar();
  MVar(const char* name);

  String GetName() const;
	void SetName(const char* name);
	bool operator != (const MVar& m) const;
	bool operator == (const MVar& m) const;
  bool operator < (const MVar& var) const;

private:
	unsigned char d[VAR_LEN];
};

//////////////////////
//class MGiven
class MGiven
{
public:
  typedef std::map<MVar, float> Values;

  Values  values;
};

//////////////////////
//Class MSymb

#define MS_MAX 64

#define MST_NONE	-1
#define	MST_NUMBER	0
#define MST_VAR		1
#define MST_OPER	2

#define MS_MAX_OPS	8

#define MST_DEF_PRIOR 30

#define MSO_ADD		0
#define MSO_MULT	1
#define MSO_DIV		2
#define MSO_POW		3
#define MSO_ABS		4
#define MSO_SIN		5
#define MSO_COS		6
#define MSO_TG		7

class MWorkspace;
class MSymb
{
public:
	//main
	MSymb();
	MSymb(const MSymb& e);
	MSymb(const MVar& v);
	MSymb(float f);
	MSymb(const String& s);
	MSymb(LPCSTR s);
	void Release();
	~MSymb();
	int AddSymbs(int sq);
	void DelSymb(int s);
	void Substate(MWorkspace * pWS);
  
  float solve(const MGiven& given) const;
  MSymb det(const MVar& var) const;
  MSymb* find_first(const MVar& var);
  void set_type(int);
  void set_value(float);
	
  bool IsNumber();
	float GetAsNumber();
	String GetAsStr() const;
	
  //operators
	MSymb& operator = (const MSymb& e);
	MSymb& operator = (float f);
	MSymb& operator = (const MVar& v);
	MSymb& operator = (const String& s);
	MSymb& operator = (LPCSTR s);
	MSymb  operator + (const MSymb& e) const;
	MSymb& operator +=(const MSymb& e);
	MSymb  operator - () const;
	MSymb  operator - (const MSymb& e) const;
	MSymb& operator -=(const MSymb& e);
	MSymb  operator * (const MSymb& e) const;
	MSymb& operator *=(const MSymb& e);
	MSymb  operator / (const MSymb& e) const;
	MSymb& operator /=(const MSymb& e);
	MSymb  operator ^ (const MSymb& e) const;
	MSymb& operator ^=(const MSymb& e);
  
  MSymb& absoluted();
  MSymb& cosed();
  MSymb& sined();
  
  //parsing
	struct SMorphem
	{
		char Type;
		String Data;
		MSymb * pS;
		SMorphem& operator == (SMorphem& m);
	};
protected:
	MSymb& ConvFromStr(const String& s);
	static MSymb * ConvMorphToSymb(SMorphem& m);
	static bool BreakOnMorphems(String s, SMorphem * pM, int& mc);
	static int GetMostIncMorph(SMorphem * pM,int mCount);
	static unsigned char GetCharCategory(unsigned char c);
	static int GetOperByName(String name);
	//math
	MSymb& Add(const MSymb& e);
	MSymb& Mult(const MSymb& e);
	MSymb& Div(const MSymb& e);
	MSymb& Pow(const MSymb& e);
	MSymb& Abs();
  MSymb& Cos();
  MSymb& Sin();

	int GetPrior() const;
	static float CalcOper(int oper,float* arg1,float* arg2);
	//post-processing
public:
	bool IsVarPresent(MVar & var);
	void PostProcess();
protected:	
	void sSubstate(MWorkspace * pWS);
	bool sExtend();
	bool sEval();
	bool sOpen();
	//data
	int Type;
	int Sq;
	MSymb * pSs[MS_MAX];
	//optional data
	//number (0)
	float Num;
	//var (1)
	MVar Var;
	//operation (2)
	int Oper;
	//DEBUG
#ifdef _DEBUG
	void dbgUpdate();
	int dbgCount;
	String dbgStr;
#endif
};

/////////////////////
//Class MWorkspace
#define WS_MAX 64

class MWorkspace
{
public:
	//main
	MWorkspace();
	~MWorkspace();
	void Release();
	void Assign(String name,MSymb val);
	int FindVar(String name);
	MSymb* GetVal(String name);
	struct SAssign
	{
		MVar var;
		MSymb val;
	};
	SAssign * pAs[WS_MAX];
	int Aq;
};

