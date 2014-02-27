#pragma once

#include "BaseTypes.h"

#define DEG_K 57.2957795f

float mthRound(float x);
float mthStrToFloat(const char* s);
float mthGetAngleBySinAndCos(float sinv, float cosv);
void mthReplace(String& str, const String& old_str, const String& new_str);
