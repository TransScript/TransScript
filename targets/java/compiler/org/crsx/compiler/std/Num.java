/* START MODULE /Users/villardl/Projects/crsx/crsx4/src/std/num.crs.java */
/** Generated File */package org.crsx.compiler.std;
import static org.crsx.runtime.ConstructionDescriptor.makeData;
import static org.crsx.runtime.ConstructionDescriptor.makeFunction;
import static org.crsx.runtime.Normalizer.thunk;
import static org.crsx.runtime.Normalizer.force;
import static org.crsx.runtime.Normalizer.forceSub;
import static org.crsx.runtime.Reference.safeRef;

import org.crsx.runtime.*;
import org.crsx.runtime.ConstructionDescriptor.*;
import static org.crsx.runtime.Primitives.*; import static org.crsx.compiler.std.Core.*;

public class Num
{
  final public static DynamicFunctionDescriptor _M_NumberMinus = makeFunction("NumberMinus", Num.class, "_M_NumberMinus");
  final public static DynamicFunctionDescriptor _M_NumberPlus = makeFunction("NumberPlus", Num.class, "_M_NumberPlus");
  final public static DynamicFunctionDescriptor _M_NumberEqual = makeFunction("NumberEqual", Num.class, "_M_NumberEqual");final public static boolean _M_NumberMinus(Sink sink, int shared, int depth, Term term, Term term_26) {
    if (depth < 2000) {
      sink.start(_M__s); sink.start(_M_Builtin_sMinus); sink.end(); sink.copy(term.ref()); sink.copy(term_26.ref());
      sink.end(); return true;
    }
    return thunk(sink, _M_NumberMinus, term, term_26);
  }  final public static boolean _M_NumberPlus(Sink sink, int shared, int depth, Term term_43, Term term_91) {
    if (depth < 2000) {
      sink.start(_M__s); sink.start(_M_Builtin_sPlus); sink.end(); sink.copy(term_43.ref()); sink.copy(term_91.ref());
      sink.end(); return true;
    }
    return thunk(sink, _M_NumberPlus, term_43, term_91);
  }  final public static boolean _M_NumberEqual(Sink sink, int shared, int depth, Term term_70, Term term_95) {
    if (depth < 2000) {
      sink.start(_M__s); sink.start(_M_Builtin_sIf); sink.end();
      sink.start(_M__s); sink.start(_M_Builtin_sEqual); sink.end(); sink.copy(term_70.ref()); sink.copy(term_95.ref());
      sink.end(); sink.start(_M_TRUE); sink.end(); sink.start(_M_FALSE); sink.end();
      sink.end(); return true;
    }
    return thunk(sink, _M_NumberEqual, term_70, term_95);
  }  
  private static boolean initialized = false;
  public static void init(Context context)
  {
    if (!initialized) 
    { org.crsx.compiler.std.Core.init(context); context.register(_M_NumberMinus); context.register(_M_NumberPlus); context.register(_M_NumberEqual);
      initialized = true;
    }
  }

/* END MODULE « "/Users/villardl/Projects/crsx/crsx4/src/std/num.crs.java" » */
}