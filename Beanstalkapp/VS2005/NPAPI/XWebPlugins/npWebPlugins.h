#pragma once
#include "../xulrunner-sdk/include/npapi.h"
#include "../xulrunner-sdk/include/npfunctions.h"
#include "../xulrunner-sdk/include/prtypes.h"
#include <string>
#include "XWebPlug.h"
#include "NPObject.h"

extern NPNetscapeFuncs*	gpnpf;

BOOL NPVariantToVARIANT(VARIANT* lpVal, const NPVariant var);