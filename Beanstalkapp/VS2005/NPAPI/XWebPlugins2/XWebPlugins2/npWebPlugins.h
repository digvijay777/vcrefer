#pragma once
#include "../../xulrunner-sdk/include/npapi.h"
#include "../../xulrunner-sdk/include/npfunctions.h"
#include "../../xulrunner-sdk/include/prtypes.h"
#include <string>
#include "XWebPlugins2.h"

extern NPNetscapeFuncs*	gpnpf;

BOOL NPVariantToVARIANT(VARIANT* lpVal, const NPVariant var);