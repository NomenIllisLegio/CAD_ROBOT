#include <crtdbg.h>
#include <math.h>
#include <sstream>

#include "mathmain.h"
#include "msymb.h"

/////AdvMath
static MSymb::SMorphem Ms[128];

//////////////////////////
//Class MVar

MVar::MVar()
{
	memset(d,0,VAR_LEN);
}

MVar::MVar(const char* name)
{
  memset(d,0,VAR_LEN);
  SetName(name);
}

void MVar::SetName(const char* name)
{
	String s(name);
  _ASSERTE(s.size()<VAR_LEN);
	memset(d,0,VAR_LEN);
	memcpy(d,s.c_str(),s.size());
}

String MVar::GetName() const
{
	return (const char*)d;
}

bool MVar::operator == (const MVar& m) const
{
	return memcmp(d,m.d,VAR_LEN)==0;
}

bool MVar::operator != (const MVar& m) const
{
	return memcmp(d,m.d,VAR_LEN)!=0;
}

bool MVar::operator < (const MVar& var) const
{
  return memcmp(d, var.d, VAR_LEN) < 0;
}

///////////
//Class MSymb

struct
{
	LPCSTR Prefix,Postfix,Div;
	int Prior;
} static OPs[MS_MAX_OPS] =
{
	{"","","+",10},
	{"","","*",20},
	{"","","/",20},
	{"","","^",30},
	{"ABS","","",50},//EMPTY
	{"SIN","","",50},
	{"COS","","",50},
	{"TG","","",50},
};

//MAIN

MSymb::MSymb()
{
	Sq=0;
	Type=MST_NUMBER;
	Num=0;
}

MSymb::MSymb(const MSymb& e)
{
	Sq=0;
	Type=MST_NUMBER;
	Num=0;
	*this=e;
#ifdef _DEBUG
	dbgUpdate();
#endif
}

MSymb::MSymb(float f)
{
	Sq=0;
	Type=MST_NUMBER;
	Num=f;
}

MSymb::MSymb(const MVar& v)
{
	Sq=0;
	Type=MST_VAR;
	Var=v;
}

MSymb::MSymb(const String& s)
{
	Sq=0;
	ConvFromStr(s);
}

MSymb::MSymb(LPCSTR s)
{
	Sq=0;
	ConvFromStr(s);
}

void MSymb::Release()
{
	if (Type==MST_OPER)
	{
		for (int s=0;s<Sq;s++)
			delete pSs[s];
		Sq=0;
	}
	Type=MST_NUMBER;
	Num=0;
}

MSymb::~MSymb()
{
	Release();
}

int MSymb::AddSymbs(int sq=1)
{
	_ASSERTE (sq+Sq<=MS_MAX);
	for (int s=0;s<sq;s++)
		pSs[s+Sq]=new MSymb;
	Sq+=sq;
	return Sq-sq;
}

void MSymb::DelSymb(int s)
{
	if (s>=Sq) return;
	if (!pSs[s]) return;
	delete pSs[s];
	if (s<Sq-1)
		pSs[s]=pSs[Sq-1];
	Sq--;
}

bool MSymb::IsNumber()
{
	return (Type==MST_NUMBER);
}

float MSymb::GetAsNumber()
{
	_ASSERTE(Type==MST_NUMBER);
	return Num;
}
//OPERATORS

MSymb& MSymb::operator = (const MSymb& e)
{
	Release();
	Type=e.Type;
	switch (Type)
	{
	case MST_NUMBER:
		Num=e.Num;
		break;
	case MST_VAR:
		Var=e.Var;
		break;
	case MST_OPER:
		Oper=e.Oper;
		AddSymbs(e.Sq);
		for (int s=0;s<Sq;s++)
			*pSs[s]=*e.pSs[s];
		break;
	}
#ifdef _DEBUG
	dbgUpdate();
#endif
	return *this;
}

MSymb& MSymb::operator = (float f)
{
	Release();
	Num=f;
	return *this;
}

MSymb& MSymb::operator = (const MVar& v)
{
	Release();
	Type=MST_VAR;
	Var=v;
	return *this;
}

MSymb& MSymb::operator = (const String& s)
{
	ConvFromStr(s);
	return *this;
}

MSymb& MSymb::operator = (LPCSTR s)
{
	ConvFromStr(s);
	return *this;
}

MSymb MSymb::operator + (const MSymb& e) const
{
	MSymb rv=*this;
	rv.Add(e);
	return rv;
}

MSymb& MSymb::operator += (const MSymb& e)
{
	Add(e);
	return *this;
}

MSymb MSymb::operator - () const
{
	MSymb rv;
	rv.Type=MST_OPER;
	rv.Oper=MSO_MULT;
	rv.AddSymbs(2);
	*rv.pSs[0]=-1;
	*rv.pSs[1]=*this;
	return rv;
}

MSymb MSymb::operator - (const MSymb& e) const
{
	MSymb rv=*this;
	rv.Add(-e);
	return rv;
}

MSymb& MSymb::operator -= (const MSymb& e)
{
	Add(-e);
	return *this;
}

MSymb MSymb::operator * (const MSymb& e) const
{
	MSymb rv = *this;
	rv.Mult(e);
	return rv;
}

MSymb& MSymb::operator *= (const MSymb& e)
{
	Mult(e);
	return *this;
}

MSymb MSymb::operator / (const MSymb& e) const
{
	MSymb rv=*this;
	rv.Div(e);
	return rv;
}

MSymb& MSymb::operator /= (const MSymb& e)
{
	Div(e);
	return *this;
}

MSymb MSymb::operator ^ (const MSymb& e) const
{
	MSymb rv=*this;
	rv.Pow(e);
	return rv;
}

MSymb& MSymb::operator ^= (const MSymb& e)
{
	Pow(e);
	return *this;
}

MSymb& MSymb::absoluted()
{
  Abs();
  return *this;
}

MSymb& MSymb::cosed()
{
  Cos();
  return *this;
}

MSymb& MSymb::sined()
{
  Sin();
  return *this;
}

////PARSING
String MSymb::GetAsStr() const
{
	String rv;
	switch (Type)
	{
	case MST_NUMBER:
    {
      std::stringstream ss;
      ss << mthRound(Num);
      rv = ss.str();
    }
		break;
	case MST_VAR:
		rv=Var.GetName();
		break;
	case MST_OPER:
		bool brac;
		rv+=OPs[Oper].Prefix;
		for (int s=0;s<Sq;s++)
		{
			if (s!=0) rv+=OPs[Oper].Div;
			brac=GetPrior()>pSs[s]->GetPrior();
			if (brac) rv+="(";
			rv+=pSs[s]->GetAsStr();
			if (brac) rv+=")";
		}
		rv+=OPs[Oper].Postfix;
		break;
	}
	
  mthReplace(rv, "+-", "-");
  mthReplace(rv, "+1*", "+");
  mthReplace(rv, "-1*", "-");
  mthReplace(rv, "(1*", "(");
	return rv;
}

int MSymb::GetMostIncMorph(SMorphem * pM,int mCount)
{
	int bi=0;
	int is=0;
	int ie=mCount-1;
	int i=0;

	bool bOpen=false;
	for (int m=0;m<mCount;m++)
	{
		if (pM[m].Type=='(')
		{
			i++;
			if (i>bi)
			{
				bi=i;
				is=m+1;
				bOpen=true;
			}
		}
		if (pM[m].Type==')')
		{
			if (bOpen)
			{
				ie=m-1;
				bOpen=false;
			}
			i--;
		}
	}
	_ASSERTE(i==0);
	_ASSERTE(ie-is>=0);
	if (ie-is==0) return ie;
	int bp=0;
	int bm=is;
	for (int m=is;m<=ie;m++)
	{
		if (pM[m].Type=='l' || pM[m].Type=='n' || pM[m].Type=='S') continue;
		for (int o=0;o<MS_MAX_OPS;o++)
			if (OPs[o].Prefix==pM[m].Data || OPs[o].Div==pM[m].Data)
			{
				int pr=OPs[o].Prior;
				if (pr>bp)
				{
					bp=pr;
					bm=m;
				}
			}
	}
	return bm;
}

MSymb * MSymb::ConvMorphToSymb(SMorphem& m)
{
	MSymb * pS;
	switch (m.Type)
	{
	case 'S':
		return m.pS;
	case 'n':
		pS=new MSymb((float)atof(m.Data.c_str()));
		m.pS=pS;
		m.Type='S';
		return pS;
	case 'l':
		MVar v;
		v.SetName(m.Data.c_str());
		pS=new MSymb(v);
		m.pS=pS;
		m.Type='S';
		return pS;
	}
	_ASSERTE(false);
	return NULL;
}

MSymb& MSymb::ConvFromStr(const String& s)
{
	int mCount=0;
	bool sSuccess=BreakOnMorphems(s,Ms,mCount);
	_ASSERTE(sSuccess); 
	if (mCount==0)
	{
		Release();
		return *this;
	}
	while (true)
	{
		int bm=GetMostIncMorph(Ms,mCount);
		String& dat=Ms[bm].Data;
		unsigned char mt=Ms[bm].Type;
		MSymb *pL,*pR,*pS;
		bool CBRC=false;
		switch (mt)
		{
		case 'n':
			ConvMorphToSymb(Ms[bm]);
			CBRC=true;
			break;
		case 'l':
			ConvMorphToSymb(Ms[bm]);
			CBRC=true;
			break;
		case '+':
		case '*':
		case '/':
		case '^':
			pL=ConvMorphToSymb(Ms[bm-1]);
			pR=ConvMorphToSymb(Ms[bm+1]);
			pS=new MSymb();
			pS->Type=MST_OPER;
			switch (mt)
			{
			case '+':
				pS->Oper=MSO_ADD;
				break;
			case '*':
				pS->Oper=MSO_MULT;
				break;
			case '/':
				pS->Oper=MSO_DIV;
				break;
			case '^':
				pS->Oper=MSO_POW;
				break;
			}
			pS->Sq=2;
			pS->pSs[0]=pL;
			pS->pSs[1]=pR;
			Ms[bm-1].Type='0';
			Ms[bm+1].Type='0';
			Ms[bm].Type='S';
			Ms[bm].pS=pS;
			break;
		case 'o':
			pR=ConvMorphToSymb(Ms[bm+1]);
			pS=new MSymb();
			pS->Type=MST_OPER;
			pS->Sq=1;
			pS->pSs[0]=pR;
			pS->Oper=GetOperByName(dat);
			_ASSERTE(pS->Oper>=0);
			Ms[bm+1].Type='0';
			Ms[bm].Type='S';
			Ms[bm].pS=pS;
			break;
		case 'S':
			CBRC=true;
			break;
		default:
			_ASSERTE(false);
		}
		
		if (CBRC)
		{
			if (bm>0 && Ms[bm-1].Type=='(')
				Ms[bm-1].Type='0';
			if (bm<mCount-1 && Ms[bm+1].Type==')')
				Ms[bm+1].Type='0';
		}

		////PACKING
		int d=0;
		for (int m=0;m<mCount;m++)
		{
			if (Ms[m].Type=='0')
			{
				d++;
				continue;
			}
			if (d!=0)
				Ms[m-d]=Ms[m];
		}
		mCount-=d;
		if (mCount==1 && Ms[0].Type=='S') break;
	}
	Release();
	memcpy(this,Ms[0].pS,sizeof(MSymb));
	return *this;
}

bool MSymb::BreakOnMorphems(String s, SMorphem * pM, int& mc)
{
	int mp=0;
	s+="@";
	String acc;
	unsigned char m='r';

	for (size_t p=0;p<s.size();p++)
	{
		/*unsigned*/ char c=s[p];
		unsigned char t=GetCharCategory(c);

		////CONTINING && ENDING
		switch (m)
		{
		case 'l':
			if (t=='l' || t=='n')
				acc+=c;
			else
			{
				pM[mp].Type='l';
				pM[mp].Data=acc;
				mp++;
				m='r';
			}
			break;
		case 'n':
			if (t=='n')
				acc+=c;
			else
			{
				pM[mp].Type='n';
				pM[mp].Data=acc;
				mp++;
				m='r';
			}
			break;
		case '-':
			if (t=='n'|| t=='@')
			{
				acc+=c;
				m='n';
			}
			else
			{
				pM[mp].Type='n';
				pM[mp].Data="-1";
				pM[mp+1].Type='*';
				pM[mp+1].Data="*";
				mp+=2;
				m='r';
			}
			break;
		};
		////STARTING
		if (m=='r')
		{
			switch (t)
			{
			case '*':
			case '/':
			case '^':
			case '|':
			case '(':
			case ')':
			case '+':
				pM[mp].Type=t;
				pM[mp].Data=c;
				mp++;
				break;
			case 'l':
				//acc=c;
				acc=c;
				m='l';
				break;
			case 'n':
				acc=c;
				m='n';
				break;
			case '-':
				acc=c;
				m='-';
				unsigned char pt=(mp>0)?pM[mp-1].Type:'@';
				if (pt!='(' && pt!='@')
				{
					pM[mp].Type='+';
					pM[mp].Data='+';
					mp++;
				}
				break;
			}
		}
	}
	mc=mp;
	for (int u=0;u<mc;u++)
	{
		if (pM[u].Type!='l') continue;
		for (int o=0;o<MS_MAX_OPS;o++)
			if (pM[u].Data==OPs[o].Prefix)
			{
				pM[u].Type='o';
				break;
			}
	}
	return true;
};

unsigned char MSymb::GetCharCategory(unsigned char c)
{
	if (c=='*')						return '*';	
	if (c=='/')						return '/';	
	if (c=='^')						return '^';	
	if (c=='|')						return '|';	
	if (c=='(')						return '(';	
	if (c==')')						return ')';	
	if (c=='@' || c==0)				return '@';
	if (c=='+')						return '+';
	if (c=='-')						return '-';
	if (c>='0' && c<='9' || c=='.') return 'n';
	if (c>='A' && c<='z')			return 'l';
	return '0';
}

int MSymb::GetOperByName(String name)
{
	for (int o=0;o<MS_MAX_OPS;o++)
		if (name==OPs[o].Prefix)
			return o;
	return -1;
}

////Math
MSymb& MSymb::Add(const MSymb& e)
{
	MSymb temp=*this;
	Release();
	Type=MST_OPER;
	Oper=MSO_ADD;
	AddSymbs(2);
	*pSs[0]=temp;
	*pSs[1]=e;
	PostProcess();
	return *this;
}

MSymb& MSymb::Mult(const MSymb& e)
{
	MSymb temp=*this;
	Release();
	Type=MST_OPER;
	Oper=MSO_MULT;
	AddSymbs(2);
	*pSs[0]=temp;
	*pSs[1]=e;
	PostProcess();
	return *this;
}

MSymb& MSymb::Div(const MSymb& e)
{
	MSymb temp=*this;
	Release();
	Type=MST_OPER;
	Oper=MSO_DIV;
	AddSymbs(2);
	*pSs[0]=temp;
	*pSs[1]=e;
	PostProcess();
	return *this;
}

MSymb& MSymb::Pow(const MSymb& e)
{
	MSymb temp=*this;
	Release();
	Type=MST_OPER;
	Oper=MSO_POW;
	AddSymbs(2);
	*pSs[0]=temp;
	*pSs[1]=e;
	PostProcess();
	return *this;
}

MSymb& MSymb::Abs()
{
	MSymb temp=*this;
	Release();
	Type=MST_OPER;
	Oper=MSO_ABS;
	AddSymbs(1);
	*pSs[0]=temp;
	PostProcess();
	return *this;
}

MSymb& MSymb::Cos()
{
	MSymb temp=*this;
	Release();
	Type=MST_OPER;
	Oper=MSO_COS;
	AddSymbs(1);
	*pSs[0]=temp;
	PostProcess();
	return *this;
}

MSymb& MSymb::Sin()
{
	MSymb temp=*this;
	Release();
	Type=MST_OPER;
	Oper=MSO_SIN;
	AddSymbs(1);
	*pSs[0]=temp;
	PostProcess();
	return *this;
}

int MSymb::GetPrior() const
{
	if (Type!=MST_OPER) return MST_DEF_PRIOR;
	return (OPs[Oper].Prior);
};

float MSymb::CalcOper(int oper,float* arg1=NULL,float* arg2=NULL)
{
	switch (oper)
	{
	case MSO_DIV:
		return *arg1/(*arg2);
	case MSO_POW:
		return powf(*arg1,*arg2);
	case MSO_ABS:
		return fabsf(*arg1);
	case MSO_SIN:
		return sinf(*arg1/DEG_K);
	case MSO_COS:
		return cosf(*arg1/DEG_K);
	case MSO_TG:
		return tanf(*arg1/DEG_K); 
	default:
		_ASSERTE(false);
	}
	return 0;
}

//post-processing
void MSymb::PostProcess()
{
	//simplify
	while (true)
	{
		bool bWas=false;
		bWas|=sExtend();
		bWas|=sEval();
		bWas|=sOpen();

		if (!bWas) break;
	};

#ifdef _DEBUG
	dbgUpdate();
#endif
};

void MSymb::Substate(MWorkspace * pWS)
{
	sSubstate(pWS);
	PostProcess();
}


void MSymb::sSubstate(MWorkspace * pWS)
{
	if (!pWS) return;
	switch (Type)
	{
	case MST_NUMBER:
		break;
	case MST_VAR:
		{
			MSymb * pS=pWS->GetVal(Var.GetName());
			if (pS)
			{
				*this=*pS;
			}
			break;
		}
	case MST_OPER:
		for (int s=0;s<Sq;s++)
			pSs[s]->Substate(pWS);
		break;
	}
}
	
bool MSymb::sExtend()
{
	if (Type!=MST_OPER) return false;
	bool rv=false;
	for (int s=0;s<Sq;s++)
	{
		rv|=pSs[s]->sExtend();
	}
	if (Oper!=MSO_ADD && Oper!=MSO_MULT) return false;
	
	MSymb * npSs[MS_MAX];
	int nSq=0;
	for (int s=0;s<Sq;s++)
	{
		if (Type!=pSs[s]->Type || Oper!=pSs[s]->Oper)
		{
			npSs[nSq]=pSs[s];
			nSq++;
		}
		else
		{
			MSymb * old=pSs[s];
			for (int p=0;p<old->Sq;p++)
				npSs[p+nSq]=old->pSs[p];
			nSq+=old->Sq;
			free(old);
			rv=true;
		}
	}
	memcpy(pSs,npSs,4*nSq);
	Sq=nSq;
	return rv;
}

bool MSymb::sEval()
{
	if (Type!=MST_OPER) return false;
	bool rv=false;
	bool allnum=true;
	for (int s=0;s<Sq;s++)
	{
		rv|=pSs[s]->sEval();
		allnum&=(pSs[s]->Type==MST_NUMBER);
	}
	
	if (Oper>=MSO_DIV)
	{
		if (!allnum) return rv;
		float res=mthRound(CalcOper(Oper,&pSs[0]->Num,&pSs[1]->Num));
		Release();
		Num=res;
		rv=true;
	}
	else
	{
		int fi=-1;
		for (int m=0;m<Sq;m++)
			if (pSs[m]->Type==MST_NUMBER) 
			{
				fi=m;
				break;
			}
		if (fi==-1) return rv;
		float acc=pSs[fi]->Num;
		for (int m=fi+1;m<Sq;m++)
		{
			MSymb& ms=*pSs[m];
			if (ms.Type!=MST_NUMBER) continue;
			acc=(Oper==MSO_ADD)?acc+ms.Num:acc*ms.Num;
			delete pSs[m];
			pSs[m]=NULL;
			rv=true;
		}
		pSs[fi]->Num=acc;
		if (fi!=0 && Oper==MSO_MULT)
		{
			MSymb * pS=pSs[fi];
			pSs[fi]=pSs[0];
			pSs[0]=pS;
			fi=0;
		}
		///Exceptions			
		if (Oper==MSO_ADD && acc==0)
		{
			delete pSs[fi];
			pSs[fi]=NULL;
		}
		if (Oper==MSO_MULT && acc==0)
		{
			Release();
			return rv;
		}
		if (Oper==MSO_MULT && acc==1)
		{
			delete pSs[fi];
			pSs[fi]=NULL;
		}

		///PACKING
		int d=0;
		for (int m=fi;m<Sq;m++)
		{
			if (!pSs[m])
			{
				d++;
				continue;
			}
			if (d)
				pSs[m-d]=pSs[m];
		}
		Sq-=d;
		////Check on Sq=0
		if (Sq==0)
		{
			Type=MST_NUMBER;
			Num=(Oper==MSO_ADD)?0:1.0f;
			return rv;
		}
		////Check on sq==1;
		if (Sq==1)
		{
			memcpy(this,pSs[0],sizeof(MSymb));
			rv=true;
		}
	}
	return rv;
}

bool MSymb::sOpen()
{
	if (Type!=MST_OPER) return false;
	bool rv=false;
	bool bWasSum=false;
	for (int s=0;s<Sq;s++)
	{
		rv|=pSs[s]->sOpen();
		bWasSum|=(pSs[s]->Type==MST_OPER && pSs[s]->Oper==MSO_ADD);
	}
	if (Oper!=MSO_MULT) return rv;
	//We need open?
	if (!bWasSum) return rv;
	//Init
	int SNs[MS_MAX];
	memset(SNs,0,sizeof(SNs));
	MSymb * npSs[MS_MAX];
	int nSq=0;
	//Perebor
	bool bCont=true;
	while (bCont)
	{
		//creating new member
		npSs[nSq]=new MSymb;
		npSs[nSq]->Type=MST_OPER;
		npSs[nSq]->Oper=MSO_MULT;
		npSs[nSq]->AddSymbs(Sq);
		for (int s=0;s<Sq;s++)
		{
			MSymb* pS=(pSs[s]->Type==MST_OPER && pSs[s]->Oper==MSO_ADD)?pSs[s]->pSs[SNs[s]]:pSs[s];
			*(npSs[nSq]->pSs[s])=*pS;
		}
		nSq++;
		//
		SNs[Sq-1]++;
		for (int s=Sq-1;s>=0;s--)
		{
			
			if (SNs[s]<pSs[s]->Sq)
				break;
			///overload
			if (s>0)
			{
				SNs[s]=0;
				SNs[s-1]++;
				continue;
			}
			///end
			bCont=false;
			break;
		}
		
	}
	Release();
	Type=MST_OPER;
	Oper=MSO_ADD;
	Sq=nSq;
	memcpy(pSs,npSs,sizeof(pSs));
	return true;
}

bool MSymb::IsVarPresent(MVar &var)
{
	switch (Type)
	{
	case MST_NUMBER:
		return false;
	case MST_VAR:
		return Var == var;
	case MST_OPER:
		{
			for (int s=0;s<Sq;s++)
				if (pSs[s]->IsVarPresent(var))
					return true;
			return false;
		}
	}
	_ASSERTE(false);
	return false;
}

float MSymb::solve(const MGiven& given) const
{
  switch (Type)
  {
  case MST_NUMBER:
    return Num;
  case MST_VAR:
    {
      MGiven::Values::const_iterator it = given.values.find(Var);
      if (it != given.values.end())
        return it->second;
      else
        return 0;
    }
  case MST_OPER:
    switch (Oper)
    {
    case MSO_ADD:
      {
        float rv = 0;
        for (int s = 0; s < Sq; ++ s)
          rv += pSs[s]->solve(given);
        return rv;
      }
    case MSO_MULT:
      {
        float rv = 1;
        for (int s = 0; s < Sq; ++ s)
          rv *= pSs[s]->solve(given);
        return rv;
      }
	  case MSO_DIV:
		  return pSs[0]->solve(given) / pSs[1]->solve(given);
    case MSO_POW:
		  return powf(pSs[0]->solve(given), pSs[1]->solve(given));
	  case MSO_ABS:
		  return fabsf(pSs[0]->solve(given));
	  case MSO_SIN:
		  return sinf(pSs[0]->solve(given)/DEG_K);
	  case MSO_COS:
		  return cosf(pSs[0]->solve(given)/DEG_K);
	  case MSO_TG:
		  return tanf(pSs[0]->solve(given)/DEG_K); 
    }
    return 0;
  };
  return 0;
}

MSymb MSymb::det(const MVar& var) const
{
  switch (Type)
  {
  case MST_NUMBER:
    return 0.0f;
  case MST_VAR:
    {
      if (Var == var)
        return 1.0f;
      else if (Var == MVar(("d" + var.GetName()).c_str()))
        return MVar(("dd" + var.GetName()).c_str());
      else
        return 0.0f;
    }
  case MST_OPER:
    switch (Oper)
    {
    case MSO_ADD:
      {
        MSymb rv = 0.0f;
        for (int s = 0; s < Sq; ++ s)
          rv += pSs[s]->det(var);
        return rv;
      }
    case MSO_MULT:
      {
        if (Sq == 2)
        {
          return (pSs[0]->det(var)) * *(pSs[1]) + (pSs[1]->det(var)) * (*pSs[0]);
        }
        else
        {
          MSymb part = *this;
          part.DelSymb(Sq - 1);
          return (part.det(var)) * *(pSs[Sq - 1]) + (pSs[Sq - 1]->det(var)) * (part);
        }
      }
    case MSO_DIV:
		  _ASSERT(false);
      return 0.0f;
    case MSO_POW:
		  _ASSERT(false);
      return 0.0f;
    case MSO_ABS:
		  _ASSERT(false);
      return 0.0f;
	  case MSO_SIN:
      {
        MSymb temp = *pSs[0];
        temp.cosed();
        return temp * pSs[0]->det(var);
      }
	  case MSO_COS:
      {
        MSymb temp = *pSs[0];
        temp.sined();
        return -temp * pSs[0]->det(var);
      }
		  _ASSERT(false);
      return 0.0f;
    }
    return 0.0f;
  };
  return 0.0f;
}

MSymb* MSymb::find_first(const MVar& var)
{
  switch(Type)
  {
  case MST_NUMBER:
    return NULL;
  case MST_VAR:
    if (Var == var)
      return this;
    else
      return NULL;
  case MST_OPER:
    for (int s = 0; s < Sq; ++ s)
    {
      MSymb* ms = pSs[s]->find_first(var);
      if (ms)
        return ms;
    }
    return NULL;
  }
  return NULL;
}

void MSymb::set_type(int type)
{
  Type = type;
}

void MSymb::set_value(float val)
{
  Num = val;
}

//DEBUG
#ifdef _DEBUG
////////////////////
void MSymb::dbgUpdate()
{
	///Checking
	if (Type==MST_OPER) _ASSERTE(Sq!=0);
	if (Type!=MST_OPER) _ASSERTE(Sq==0);

	///Sub-Ss
	for (int s=0;s<Sq;s++)
		pSs[s]->dbgUpdate();
	///Count;
	int sum=0;
	for (int s=0;s<Sq;s++)
		sum+=pSs[s]->dbgCount;
	dbgCount=sum+1;
	///Value
	dbgStr=GetAsStr();
}
////////////////////
#endif


///Struct SMorphem

MSymb::SMorphem& MSymb::SMorphem::operator == (MSymb::SMorphem& m)
{
	Data=m.Data;
	pS=m.pS;
	Type=m.Type;
	return *this;
}


/////////////////////
//Class MWorkspace
MWorkspace::MWorkspace()
{
	Aq=0;
}

MWorkspace::~MWorkspace()
{
	Release();
}

void MWorkspace::Release()
{
	for (int a=0;a<Aq;a++)
		if (pAs[a])
		{
			delete pAs[a];
			pAs[a]=NULL;
		}
	Aq=0;
}

void MWorkspace::Assign(String name,MSymb val)
{
	int p=FindVar(name);
	if (p==-1)
	{
		pAs[Aq]=new SAssign;
		pAs[Aq]->var.SetName(name.c_str());
		pAs[Aq]->val=val;
		Aq++;
	}
	else
	{
		pAs[p]->val=val;
	}
};

int MWorkspace::FindVar(String name)
{
	for (int a=0;a<Aq;a++)
		if (pAs[a]->var.GetName()==name)
			return a;
	return -1;
}

MSymb* MWorkspace::GetVal(String name)
{
	int p=FindVar(name);
	return (p==-1)?NULL:&(pAs[p]->val);
}