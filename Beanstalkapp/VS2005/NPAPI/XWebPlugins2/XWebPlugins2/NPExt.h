#pragma once

bool StringToNPVariantV(NPVariant& npvar, const char* pfmt, va_list arr);
bool StringToNPVariant(NPVariant& npvar, const char* pfmt, ...);
bool ClearStringNPVariant(NPVariant& npvar);