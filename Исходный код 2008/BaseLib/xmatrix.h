#pragma once

///////////////////////
//Class XMatrix

template <int iDy,int iDx, typename tType> class XMatrix 
{
public:
tType d[iDy][iDx];

//default constructor
XMatrix ()
{
}

XMatrix (tType* pMembers)
{
memcpy(d,pMembers,sizeof(d));
}

//data definition
XVector<iDx,tType>& operator [] (char y)
{
return (XVector<iDx,tType>&)d[y];
}

XMatrix& operator = (XVector<iDx,tType> mn)
{
xMember=0;
memcpy(d[xMember],&mn,sizeof(mn));
return *this;
}

XMatrix& operator , (XVector<iDx,tType> mn)
{
xMember++;
if (xMember<iDy) memcpy(d[xMember],&mn,sizeof(mn));
return *this;
}

tType* dptr ()
{
	return (tType*)d;
}

void LoadData(tType* pdata)
{
	for (int v=0;v<iDx*iDy;v++) ((tType*)d)[v]=pdata[v];
}

};