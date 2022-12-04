#pragma once

#include <vessel.h>

namespace brepvmgraph
{

VesselForeignMethodFn BrepVMBindMethod(const char* signature);
void BrepVMBindClass(const char* class_name, VesselForeignClassMethods* methods);

}