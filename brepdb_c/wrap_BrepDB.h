#pragma once

#include <vessel.h>

namespace brepdbgraph
{

VesselForeignMethodFn BrepDBBindMethod(const char* signature);
void BrepDBBindClass(const char* class_name, VesselForeignClassMethods* methods);

}