#include "mathmain.h"

#include <stdlib.h>
#include <math.h>

float mthStrToFloat(const char* s)
{
	if (s=="" || s=="+") return 1;
	if (s=="-") return -1;
	return (float)atof(s);
}

float mthGetAngleBySinAndCos(float sinv, float cosv)
{
	float cv=acosf(cosv);
	float sv=asinf(sinv);
	return (sv < 0) ? - cv * DEG_K : cv * DEG_K;
}

float mthRound(float x)
{
  int v = int(x * 100.0f + ((x > 0.0f) ? 0.5f : -0.5f));
	return float(v) / 100.0f;
}

void mthReplace(String& str, const String& old_str, const String& new_str)
{
  if (old_str == new_str)
    return;
  Size pos;
  while ((pos = str.find(old_str)) != String::npos)
  {
    str.replace(pos, old_str.length(), new_str); 
  }
}