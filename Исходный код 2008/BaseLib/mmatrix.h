#pragma once

#include <math.h>

static int iPos;

template <typename tType> class MMatrix
{
public:
///data
	tType * pData;
	int Xs, Ys;
#ifdef _DEBUG
	tType dbgData[16];
#endif
//main
void Release()
{
#ifndef _DEBUG
	if (pData) delete[] pData;
	pData=NULL;
#endif
}
	
void Init(int sy, int sx=1)
{
	Release();
	Xs=sx;
	Ys=sy;
#ifdef _DEBUG
	pData=dbgData;
#else
	pData=new tType[sy*sx];
#endif
}

MMatrix()
{
	pData=NULL;
	Xs=0;Ys=0;
	//it(4,4);
}

MMatrix(const MMatrix& m)
{
	pData=NULL;
	Init(m.Ys,m.Xs);
	for (int v=0;v<Ys*Xs;v++) pData[v]=m.pData[v];
}

MMatrix(int sy,int sx=1)
{
	pData=NULL;
	Init(sy,sx);
}

~MMatrix()
{
	Release();
}

static MMatrix* MakeArray(int size, int sy, int sx=1)
{
	MMatrix * pD=new MMatrix[size];
	for (int m=0;m<size;m++) pD[m].Init(sy,sx);
	return pD;
}

///data access
tType& operator () (int y, int x=0)
{
	return pData[x+y*Xs];
}

const tType& operator () (int y, int x=0) const
{
	return pData[x+y*Xs];
}

tType& operator [] (int v)
{
	return pData[v];
}

const tType& operator [] (int v) const
{
	return pData[v];
}

///operations
MMatrix& LoadData(tType * data)
{
	for (int v=0;v<Xs*Ys;v++) pData[v]=data[v];
	return *this;
}

MMatrix& operator = (const MMatrix& m)
{
	Init(m.Ys,m.Xs);
	LoadData(m.pData);
	return *this;
}

MMatrix SubMatrix(int yp,int xp,int sy=1,int sx=1)
{
	MMatrix rv;
	rv.Init(sy,sx);
	for (int y=0;y<sy;y++)
		for (int x=0;x<sx;x++)
			rv.pData[x+y*sx]=pData[(x+xp)+(y+yp)*Xs];
	return rv;
}

MMatrix SubState(MMatrix& m,int yp=0,int xp=0)
{
	MMatrix rv=*this;
	for (int y=0;y<m.Ys;y++)
		for (int x=0;x<m.Xs;x++)
			rv.pData[(x+xp)+(y+yp)*Xs]=m.pData[x+y*m.Xs];
	return rv;
}

MMatrix& SubStateThis(MMatrix& m,int yp=0,int xp=0)
{
	for (int y=0;y<m.Ys;y++)
		for (int x=0;x<m.Xs;x++)
			pData[(x+xp)+(y+yp)*Xs]=m.pData[x+y*m.Xs];
	return *this;
}

MMatrix CutOff(int yc=-1,int xc=-1)
{
	MMatrix rv=*this;
	if (yc!=-1)
	{
		for (int y=yc+1;y<rv.Ys;y++)
			for (int x=0;x<rv.Xs;x++)
				rv(y-1,x)=rv(y,x);
		rv=rv.SubMatrix(0,0,rv.Ys-1,rv.Xs);
	}
	if (xc!=-1)
	{
		for (int x=xc+1;x<rv.Xs;x++)
			for (int y=0;y<rv.Ys;y++)
				rv(y,x-1)=rv(y,x);
		rv=rv.SubMatrix(0,0,rv.Ys,rv.Xs-1);
	}
	return rv;
}

MMatrix& operator << (tType v)
{
	pData[0]=v;
	iPos=0;
	return *this;
}

MMatrix& operator = (tType v)
{
	pData[0]=v;
	iPos=0;
	return *this;
}

MMatrix& operator , (tType v)
{
	iPos++;
	pData[iPos]=v;
	return *this;
}

MMatrix t()
{
	MMatrix<tType> rv(Xs,Ys);
	for (int y=0;y<Ys;y++)
		for (int x=0;x<Xs;x++)
			rv.pData[y+x*Ys]=pData[x+y*Xs];
	return rv;
}

MMatrix norm()
{
	_ASSERTE (Xs==1 || Ys==1);
	tType sum=pData[0]*pData[0];
	for (int v=1;v<Xs*Ys;v++)
		sum+=pData[v]*pData[v];
	if (sum==0) return *this;
	sum=(tType)sqrt(sum);
	MMatrix rv=*this;
	for (int v=0;v<Xs*Ys;v++)
		rv.pData[v]=pData[v]/sum;
	return rv;
}

tType tr() const
{
  _ASSERTE (Xs == Ys);
  tType rv = pData[0];
  for (int m = 1; m < Xs; ++ m)
    rv += (*this)(m, m);
  return rv;
}

MMatrix& IdentityThis()
{
	_ASSERTE (Xs == Ys);
	for (int y=0;y<Ys;y++)
		for (int x=0;x<Xs;x++)
			(*this)(y,x) = (y == x)? tType(1.0f) : tType(0.0f);
	return *this;
}

MMatrix& ZeroThis()
{
	for (int p = 0; p < Xs * Ys; ++p)
		pData[p] = tType(0.0f);
	return *this;
}


///logic
bool operator == (MMatrix& m)
{
	_ASSERTE(Xs==m.Xs && Ys==m.Ys);
	for (int v=0;v<Xs*Ys;v++)
		if (pData[v]!=m.pData[v]) return false;
	return true;
}

bool operator != (MMatrix& m)
{
	_ASSERTE(Xs==m.Xs && Ys==m.Ys);
	for (int v=0;v<Xs*Ys;v++)
		if (pData[v]!=m.pData[v]) return true;
	return false;
}

///math
MMatrix operator + (MMatrix& m)
{
	MMatrix rv(Ys,Xs);
	for (int v=0;v<Xs*Ys;v++) rv.pData[v]=pData[v]+m.pData[v];
	return rv;
}
MMatrix& operator += (MMatrix& m)
{
	for (int v=0;v<Xs*Ys;v++) pData[v]+=m.pData[v];
	return *this;
}

MMatrix operator - (MMatrix& m)
{
	MMatrix rv(Ys,Xs);
	for (int v=0;v<Xs*Ys;v++) rv.pData[v]=pData[v]-m.pData[v];
	return rv;
}
MMatrix& operator -= (MMatrix& m)
{
	for (int v=0;v<Xs*Ys;v++) pData[v]-=m.pData[v];
	return *this;
}


MMatrix operator * (tType t)
{
	MMatrix rv(Ys,Xs);
	for (int v=0;v<Xs*Ys;v++) rv.pData[v]=pData[v]*t;
	return rv;
}
MMatrix& operator *= (tType t)
{
	for (int v=0;v<Xs*Ys;v++) pData[v]*=t;
	return *this;
}

MMatrix operator / (tType t)
{
	MMatrix rv(Ys,Xs);
	for (int v=0;v<Xs*Ys;v++) rv.pData[v]=pData[v]/t;
	return rv;
}
MMatrix& operator /= (tType t)
{
	for (int v=0;v<Xs*Ys;v++) pData[v]/=t;
	return *this;
}

MMatrix operator % (MMatrix m)
{
	MMatrix rv(3);
	rv.pData[0]=pData[1]*m.pData[2]-pData[2]*m.pData[1];
	rv.pData[1]=pData[2]*m.pData[0]-pData[0]*m.pData[2];
	rv.pData[2]=pData[0]*m.pData[1]-pData[1]*m.pData[0];
	return rv;
}

MMatrix operator * (MMatrix& m)
{
	_ASSERTE(Xs==m.Ys);
	MMatrix rv(Ys,m.Xs);
	for (int y=0;y<Ys;y++)
		for (int x=0;x<m.Xs;x++)
		{
			float zero=0;
			tType sum=zero;
			for (int p=0;p<Xs;p++)
				sum+=pData[p+Xs*y]*m.pData[x+m.Xs*p];
			rv.pData[x+rv.Xs*y]=sum;
		}
	return rv;
}

MMatrix operator *= (MMatrix m)
{
	*this=(*this)*m;
	return *this;
}

MMatrix operator - ()
{
	_ASSERTE(Xs>0 && Ys>0);
	MMatrix rv(Ys,Xs);
	for (int v=0;v<Xs*Ys;v++) rv.pData[v]=-pData[v];
	return rv;
}

tType det()
{
	_ASSERTE(Xs>0 && Ys>0);
	_ASSERTE(Xs==Ys);
	if (Xs==1)
		return pData[0];
	else
	{
		float zero=0;
		tType it=1;
		tType sum=zero;
		for (int x=0;x<Xs;x++)
		{
			sum+=CutOff(0,x).det()*it*pData[x];
			it=-it;
		}
		return sum;

	}
}

MMatrix GetXForZ()
{
	MMatrix x(3),y(3);
	y=0,1,0;
	if (*this==y || *this==-y) y=1,0,0;
	x=y%(*this);
	return x;
}

MMatrix rev()
{
	tType dt=det();
//	_ASSERTE(dt!=0);
	MMatrix rv(Ys,Xs);
	for (int y=0;y<Ys;y++)
		for (int x=0;x<Xs;x++)
			rv(y,x)=CutOff(y,x).det()*(tType)pow((float)-1,(float)x+y)/dt;
	return rv.t();
}

};