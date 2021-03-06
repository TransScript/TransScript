    lexer grammar  MiniMLMetaLexer ;   
   MT_toplevel :        CRSX_META_CHAR      'toplevel'     [0-9]  *    ; 
   ET_toplevel :        CRSX_START_EMBED_CHAR      'toplevel:'    ->    pushMode (   CrsxEmbed )   ;     MT_var_TOK :        CRSX_META_CHAR      'VAR'     [0-9]  *    ; 
   ET_var_TOK :        CRSX_START_EMBED_CHAR      'VAR:'    ->    pushMode (   CrsxEmbed )   ;     MT_expr :        CRSX_META_CHAR      'expr'     [0-9]  *    ; 
   ET_expr :        CRSX_START_EMBED_CHAR      'expr:'    ->    pushMode (   CrsxEmbed )   ; 
   MT_timesExpr :        CRSX_META_CHAR      'timesExpr'     [0-9]  *    ; 
   ET_timesExpr :        CRSX_START_EMBED_CHAR      'timesExpr:'    ->    pushMode (   CrsxEmbed )   ; 
   MT_compExpr :        CRSX_META_CHAR      'compExpr'     [0-9]  *    ; 
   ET_compExpr :        CRSX_START_EMBED_CHAR      'compExpr:'    ->    pushMode (   CrsxEmbed )   ; 
   MT_primaryExpr :        CRSX_META_CHAR      'primaryExpr'     [0-9]  *    ; 
   ET_primaryExpr :        CRSX_START_EMBED_CHAR      'primaryExpr:'    ->    pushMode (   CrsxEmbed )   ;     MT_int_TOK :        CRSX_META_CHAR      'INT'     [0-9]  *    ; 
   ET_int_TOK :        CRSX_START_EMBED_CHAR      'INT:'    ->    pushMode (   CrsxEmbed )   ; 
   MT_app_expr :        CRSX_META_CHAR      'app_expr'     [0-9]  *    ; 
   ET_app_expr :        CRSX_START_EMBED_CHAR      'app_expr:'    ->    pushMode (   CrsxEmbed )   ; 
   MT_simple_expr :        CRSX_META_CHAR      'simple_expr'     [0-9]  *    ; 
   ET_simple_expr :        CRSX_START_EMBED_CHAR      'simple_expr:'    ->    pushMode (   CrsxEmbed )   ;     MT_ty :        CRSX_META_CHAR      'ty'     [0-9]  *    ; 
   ET_ty :        CRSX_START_EMBED_CHAR      'ty:'    ->    pushMode (   CrsxEmbed )   ; 
   MT_tyPrimary :        CRSX_META_CHAR      'tyPrimary'     [0-9]  *    ; 
   ET_tyPrimary :        CRSX_START_EMBED_CHAR      'tyPrimary:'    ->    pushMode (   CrsxEmbed )   ;     VAR :       ['a'-'z''A'-'Z']  +    ;     INT :       ['0'-'9']  +    ;     TINT :        'int'     ;     TBOOL :        'bool'     ;     TRUE :        'true'     ;     FALSE :        'false'     ;     FUN :        'fun'     ;     IS :        'is'     ;     IF :        'if'     ;     THEN :        'then'     ;     ELSE :        'else'     ;     LET :        'let'     ;     SEMISEMI :        ';;'     ;     EQUAL :        '='     ;     LESS :        '<'     ;     TARROW :        '->'     ;     COLON :        ':'     ;     LPAREN :        '('     ;     RPAREN :        ')'     ;     PLUS :        '+'     ;     MINUS :        '-'     ;     TIMES :        '*'     ;     WS :       [ \t\r\n\f]  +   ->    channel (   HIDDEN )   ;    fragment CRSX_META_CHAR :        '#'     ;    fragment CRSX_START_EMBED_CHAR :        '⟨'     ;    fragment CRSX_END_EMBED_CHAR :        '⟩'     ;  mode  CrsxEmbed ;    CRSX_EMBED_END :        CRSX_END_EMBED_CHAR    ->    popMode   ;    CRSX_EMBED_NESTED :        CRSX_START_EMBED_CHAR    ->    pushMode (   CrsxNestedEmbed )  ,    more  ;    CRSX_EMBEDDED :       .   ->    more   ;  mode  CrsxNestedEmbed ;    CRSX_NESTED_EMBED_END :        CRSX_END_EMBED_CHAR    ->    popMode  ,    more  ;    CRSX_NESTED_EMBED_NESTED :        CRSX_START_EMBED_CHAR    ->    pushMode (   CrsxNestedEmbed )  ,    more  ;    CRSX_NESTED_EMBEDDED :       .   ->    more   ;