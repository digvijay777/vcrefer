#pragma once

bool StringToNPVariantV(NPVariant& npvar, char* pfmt, va_list arr);
bool StringToNPVariant(NPVariant& npvar, char* pfmt, ...);