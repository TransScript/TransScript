// Copyright (c) 2016 IBM Corporation.

#ifndef NUM_EXTERN_SIGS_H_
#define NUM_EXTERN_SIGS_H_

#include "ts.h"
#include "core_types.h"

tosca::DoubleTerm& GetDefine_INTEGER_MAX_DBL_PRECISION(tosca::Context& ctx);
tosca::DoubleTerm& GetDefine_INTEGER_MIN_DBL_PRECISION(tosca::Context& ctx);
tosca::DoubleTerm& ClipToMaxOrMin(tosca::Context& ctx, tosca::DoubleTerm& dTerm);
Bool& IsWithinIntegerPrecision(tosca::Context& ctx, tosca::DoubleTerm& dTerm);

tosca::DoubleTerm& Plus(tosca::Context& ctx, tosca::DoubleTerm& left, tosca::DoubleTerm& right);
tosca::DoubleTerm& Minus(tosca::Context& ctx, tosca::DoubleTerm& left, tosca::DoubleTerm& right);
tosca::DoubleTerm& Times(tosca::Context& ctx, tosca::DoubleTerm& left, tosca::DoubleTerm& right);
tosca::DoubleTerm& Divide(tosca::Context& ctx, tosca::DoubleTerm& left, tosca::DoubleTerm& right);
tosca::DoubleTerm& Modulo(tosca::Context& ctx, tosca::DoubleTerm& left, tosca::DoubleTerm& right);

Bool& LessThan(tosca::Context& ctx, tosca::DoubleTerm& left, tosca::DoubleTerm& right);
Bool& GreaterThan(tosca::Context& ctx, tosca::DoubleTerm& left, tosca::DoubleTerm& right);
tosca::StringTerm& FormatNumber(tosca::Context& ctx, tosca::DoubleTerm& num);
tosca::StringTerm& FormatInteger(tosca::Context& ctx, tosca::DoubleTerm& num);
tosca::StringTerm& FormatDecimal(tosca::Context& ctx, tosca::DoubleTerm& num);
tosca::DoubleTerm& ToDecimal(tosca::Context& ctx, tosca::StringTerm& str);

tosca::DoubleTerm& Hex(tosca::Context& ctx, tosca::StringTerm& str);

#endif
