#pragma once

#include "mmatrix.h"

template <typename tType> class MLine
{
public:
//data
	MMatrix<tType> abc;
	MMatrix<tType> xyz;
//main
MLine()
{
	abc.Init(3);
	xyz.Init(3);
}
MLine(MLine& l)
{
	abc=l.abc;
	xyz=l.xyz;
}
~MLine()
{
	abc.Release();
	xyz.Release();
}
//operators
MLine& operator = (MLine& l)
{
	abc=l.abc;
	xyz=l.xyz;
	return *this;
}

//math
MLine GetCrossingCoNormal(MLine& l2)
{
	MLine l3;
	
	if (abc==l2.abc)
		l3.abc=(abc%((l2.xyz-xyz).norm()%abc)).norm();
	else
		l3.abc=abc%l2.abc;
	MMatrix<tType> A(3,3),B(3);
	A.SubStateThis(abc,0,0);
	A.SubStateThis(-l2.abc,0,1);
	A.SubStateThis(l3.abc,0,2);
	
	XMatrix<3,3,float> m;
	m.LoadData(A.pData);

	B=l2.xyz-xyz;

	tType d,d1;
	d=A.det();
	tType t1;
	if (d!=0)
	{
		A.SubStateThis(B,0,0);
		d1=A.det();
		t1=d1/d;
	}
	else
		t1=0;

	l3.xyz=abc*t1+xyz;
	return l3;
}

bool IsLineCollinearTo(MLine& l2)
{
	MMatrix<float> zero(3);zero=0,0,0;
	if (abc%l2.abc!=zero) return false;
	if (abc%(l2.xyz-xyz)!=zero) return false;
	return true;
}
};