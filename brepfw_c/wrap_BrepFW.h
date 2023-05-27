#pragma once

#include <vessel.h>

namespace brepfwgraph
{

VesselForeignMethodFn BrepFWBindMethod(const char* signature);
void BrepFWBindClass(const char* class_name, VesselForeignClassMethods* methods);

}