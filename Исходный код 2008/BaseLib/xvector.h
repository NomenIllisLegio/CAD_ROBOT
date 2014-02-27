#pragma once

static int xMember;

///////////////////////
//Class XVector

template <int iDms, typename tType> class XVector  
{
public:
tType d[iDms];

//data definition
tType& operator [] (int m)
{
return d[m];	
}

XVector& operator = (tType mn)
{
xMember=0;
d[xMember]=mn;
return *this;
}

XVector& operator , (tType mn)
{
xMember++;
if (xMember<iDms) d[xMember]=mn;
return *this;
}

XVector& LoadData(tType * pdata)
{
	for (int v=0;v<iDms;v++) ((tType*)d)[v]=pdata[v];
	return *this;
}

//mathematics
XVector& operator +(XVector& v)
{
XVector rv;	
for (int m=0;m<iDms;m++) rv.d[m]=d[m]+v.d[m];
return rv;
}

XVector& operator +=(XVector& v)
{
for (int m=0;m<iDms;m++) d[m]+=v.d[m];
return *this;
}

XVector& operator -(XVector& v)
{
XVector rv;	
for (int m=0;m<iDms;m++) rv.d[m]=d[m]-v.d[m];
return rv;
}

XVector& operator -=(XVector& v)
{
for (int m=0;m<iDms;m++) d[m]-=v.d[m];
return *this;
}

XVector operator * (tType a)
{
XVector rv;	
for (int m=0;m<iDms;m++) rv.d[m]=d[m]*a;
return rv;
}

XVector operator * (XVector& v)
{
XVector rv;	
for (int m=0;m<iDms;m++) rv[m]=d[m]*v[m];
return rv;
}

XVector operator % (XVector& v)
{
XVector rv;	
rv.d[0]=d[1]*v.d[2]-d[2]*v.d[1];
rv.d[1]=d[2]*v.d[0]-d[0]*v.d[2];
rv.d[2]=d[0]*v.d[1]-d[1]*v.d[0];
return rv;
}

XVector& operator *= (tType& a)
{
for (int m=0;m<iDms;m++) d[m]*=a;
return *this;
}

XVector& operator / (tType a)
{
XVector rv;	
for (int m=0;m<iDms;m++) rv.d[m]=d[m]/a;
return rv;
}

XVector& operator /= (const tType& a)
{
for (int m=0;m<iDms;m++) d[m]/=a;
return *this;
}


tType Length()
{
tType sum=d[0]*d[0];
for (int m=1;m<iDms;m++) sum+=d[m]*d[m];
return (tType)sqrtf((float)sum);
}

tType LengthSq()
{
tType sum=d[0]*d[0];
for (int m=1;m<iDms;m++) sum+=d[m]*d[m];
return sum;
}

//logic
bool operator == (XVector& v)
{
//return (memcmp(d,v.d,sizeof(XVector))==0);
for (int m=0;m<iDms;m++) if (d[m]!=v.d[m]) return false;
return true;
}

bool operator != (XVector& v)
{
//return (memcmp(d,v.d,sizeof(XVector))!=0);
for (int m=0;m<iDms;m++) if (d[m]==v.d[m]) return false;
return true;
}


bool operator > (XVector& v)
{
return Length()>v.Length();
}

bool operator < (XVector& v)
{
return Length()<v.Length();
}

XVector Module()
{
XVector rv;	
for (int m=0;m<iDms;m++) rv[m]=(tType)fabs(d[m]);
return rv;
}

};

typedef XVector<4,float> V4f;
typedef XVector<3,float> V3f;
typedef XVector<2,float> V2f;