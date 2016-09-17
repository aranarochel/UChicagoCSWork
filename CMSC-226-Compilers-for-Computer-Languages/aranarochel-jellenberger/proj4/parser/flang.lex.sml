structure FLangLex  = struct

    datatype yystart_state = 
STRING | COMMENT | INITIAL
    local

    structure UserDeclarations = 
      struct



    structure T = FLangTokens

  (* some type lex_result is necessitated by ml-ulex *)
    type lex_result = T.token

  (* the depth int ref is used for keeping track of comment depth *)
    val depth = ref 0

  (* starting position of comments/string; used for error reporting *)
    val startPos : Error.pos ref = ref 0

  (* list of string fragments to concatenate *)
    val buf : string list ref = ref []

  (* add a string to the buffer *)
    fun addStr s = (buf := s :: !buf)

  (* make a string from buf *)
    fun mkString () = (T.STRING(String.concat(List.rev(!buf))) before buf := [])

  (* eof : unit -> lex_result *)
  (* ml-ulex requires this as well *)
    fun eof () = T.EOF

  (* keyword lookup table *)
    local
      val find =
	  let val tbl = AtomTable.mkTable (17, Fail "keywords")
	      fun ins (id, tok) = AtomTable.insert tbl (Atom.atom id, tok)
	  in
	      app ins [
		("case",	T.KW_case),
		("con",		T.KW_con),
		("data",	T.KW_data),
		("else",	T.KW_else),
		("end",		T.KW_end),
		("fun",		T.KW_fun),
		("if",		T.KW_if),
		("let",		T.KW_let),
		("of",		T.KW_of),
		("then",	T.KW_then),
		("type",	T.KW_type),
		("with",	T.KW_with)
	      ];
	      AtomTable.find tbl
	  end
    in
  (* return either a keyword token or a LID token *)
    fun idToken id =
	let val ida = Atom.atom id
	in
	    case find ida
	     of NONE => T.LID ida
	      | SOME kw => kw
	end
    end

      end

    datatype yymatch 
      = yyNO_MATCH
      | yyMATCH of ULexBuffer.stream * action * yymatch
    withtype action = ULexBuffer.stream * yymatch -> UserDeclarations.lex_result

    val yytable : ((UTF8.wchar * UTF8.wchar * int) list * int list) Vector.vector = 
Vector.fromList []
    fun yystreamify' p input = ULexBuffer.mkStream (p, input)

    fun yystreamifyReader' p readFn strm = let
          val s = ref strm
	  fun iter(strm, n, accum) = 
	        if n > 1024 then (String.implode (rev accum), strm)
		else (case readFn strm
		       of NONE => (String.implode (rev accum), strm)
			| SOME(c, strm') => iter (strm', n+1, c::accum))
          fun input() = let
	        val (data, strm) = iter(!s, 0, [])
	        in
	          s := strm;
		  data
	        end
          in
            yystreamify' p input
          end

    fun yystreamifyInstream' p strm = yystreamify' p (fn ()=>TextIO.input strm)

    fun innerLex 
(yyarg as  lexErr)(yystrm_, yyss_, yysm) = let
        (* current start state *)
          val yyss = ref yyss_
	  fun YYBEGIN ss = (yyss := ss)
	(* current input stream *)
          val yystrm = ref yystrm_
	  fun yysetStrm strm = yystrm := strm
	  fun yygetPos() = ULexBuffer.getpos (!yystrm)
	  fun yystreamify input = yystreamify' (yygetPos()) input
	  fun yystreamifyReader readFn strm = yystreamifyReader' (yygetPos()) readFn strm
	  fun yystreamifyInstream strm = yystreamifyInstream' (yygetPos()) strm
        (* start position of token -- can be updated via skip() *)
	  val yystartPos = ref (yygetPos())
	(* get one char of input *)
	  fun yygetc strm = (case ULexBuffer.getu strm
                of (SOME (0w10, s')) => 
		     (AntlrStreamPos.markNewLine yysm (ULexBuffer.getpos strm);
		      SOME (0w10, s'))
		 | x => x)
          fun yygetList getc strm = let
            val get1 = UTF8.getu getc
            fun iter (strm, accum) = 
	        (case get1 strm
	          of NONE => rev accum
	           | SOME (w, strm') => iter (strm', w::accum)
	         (* end case *))
          in
            iter (strm, [])
          end
	(* create yytext *)
	  fun yymksubstr(strm) = ULexBuffer.subtract (strm, !yystrm)
	  fun yymktext(strm) = Substring.string (yymksubstr strm)
	  fun yymkunicode(strm) = yygetList Substring.getc (yymksubstr strm)
          open UserDeclarations
          fun lex () = let
            fun yystuck (yyNO_MATCH) = raise Fail "lexer reached a stuck state"
	      | yystuck (yyMATCH (strm, action, old)) = 
		  action (strm, old)
	    val yypos = yygetPos()
	    fun yygetlineNo strm = AntlrStreamPos.lineNo yysm (ULexBuffer.getpos strm)
	    fun yygetcolNo  strm = AntlrStreamPos.colNo  yysm (ULexBuffer.getpos strm)
	    fun yyactsToMatches (strm, [],	  oldMatches) = oldMatches
	      | yyactsToMatches (strm, act::acts, oldMatches) = 
		  yyMATCH (strm, act, yyactsToMatches (strm, acts, oldMatches))
	    fun yygo actTable = 
		(fn (~1, _, oldMatches) => yystuck oldMatches
		  | (curState, strm, oldMatches) => let
		      val (transitions, finals') = Vector.sub (yytable, curState)
		      val finals = List.map (fn i => Vector.sub (actTable, i)) finals'
		      fun tryfinal() = 
		            yystuck (yyactsToMatches (strm, finals, oldMatches))
		      fun find (c, []) = NONE
			| find (c, (c1, c2, s)::ts) = 
		            if c1 <= c andalso c <= c2 then SOME s
			    else find (c, ts)
		      in case yygetc strm
			  of SOME(c, strm') => 
			       (case find (c, transitions)
				 of NONE => tryfinal()
				  | SOME n => 
				      yygo actTable
					(n, strm', 
					 yyactsToMatches (strm, finals, oldMatches)))
			   | NONE => tryfinal()
		      end)
	    val yylastwasnref = ref (ULexBuffer.lastWasNL (!yystrm))
	    fun continue() = let val yylastwasn = !yylastwasnref in
let
fun yyAction0 (strm, lastMatch : yymatch) = (yystrm := strm;  T.LP)
fun yyAction1 (strm, lastMatch : yymatch) = (yystrm := strm;  T.RP)
fun yyAction2 (strm, lastMatch : yymatch) = (yystrm := strm;  T.LB)
fun yyAction3 (strm, lastMatch : yymatch) = (yystrm := strm;  T.RB)
fun yyAction4 (strm, lastMatch : yymatch) = (yystrm := strm;  T.LCB)
fun yyAction5 (strm, lastMatch : yymatch) = (yystrm := strm;  T.RCB)
fun yyAction6 (strm, lastMatch : yymatch) = (yystrm := strm;  T.EQEQ)
fun yyAction7 (strm, lastMatch : yymatch) = (yystrm := strm;  T.LTEQ)
fun yyAction8 (strm, lastMatch : yymatch) = (yystrm := strm;  T.LT)
fun yyAction9 (strm, lastMatch : yymatch) = (yystrm := strm;  T.DCOLON)
fun yyAction10 (strm, lastMatch : yymatch) = (yystrm := strm;  T.AT)
fun yyAction11 (strm, lastMatch : yymatch) = (yystrm := strm;  T.PLUS)
fun yyAction12 (strm, lastMatch : yymatch) = (yystrm := strm;  T.MINUS)
fun yyAction13 (strm, lastMatch : yymatch) = (yystrm := strm;  T.TIMES)
fun yyAction14 (strm, lastMatch : yymatch) = (yystrm := strm;  T.DIV)
fun yyAction15 (strm, lastMatch : yymatch) = (yystrm := strm;  T.MOD)
fun yyAction16 (strm, lastMatch : yymatch) = (yystrm := strm;  T.EQ)
fun yyAction17 (strm, lastMatch : yymatch) = (yystrm := strm;  T.COMMA)
fun yyAction18 (strm, lastMatch : yymatch) = (yystrm := strm;  T.SEMI)
fun yyAction19 (strm, lastMatch : yymatch) = (yystrm := strm;  T.COLON)
fun yyAction20 (strm, lastMatch : yymatch) = (yystrm := strm;  T.ARROW)
fun yyAction21 (strm, lastMatch : yymatch) = (yystrm := strm;  T.DARROW)
fun yyAction22 (strm, lastMatch : yymatch) = (yystrm := strm;  T.WILD)
fun yyAction23 (strm, lastMatch : yymatch) = let
      val yytext = yymktext(strm)
      in
        yystrm := strm;  idToken yytext
      end
fun yyAction24 (strm, lastMatch : yymatch) = let
      val yytext = yymktext(strm)
      in
        yystrm := strm;  T.UID(Atom.atom yytext)
      end
fun yyAction25 (strm, lastMatch : yymatch) = let
      val yytext = yymktext(strm)
      in
        yystrm := strm;  T.NUMBER(valOf (IntInf.fromString yytext))
      end
fun yyAction26 (strm, lastMatch : yymatch) = (yystrm := strm;  skip ())
fun yyAction27 (strm, lastMatch : yymatch) = (yystrm := strm;
       YYBEGIN COMMENT; startPos := yypos; depth := 1; skip())
fun yyAction28 (strm, lastMatch : yymatch) = (yystrm := strm;
       YYBEGIN STRING; startPos := yypos; continue())
fun yyAction29 (strm, lastMatch : yymatch) = let
      val yytext = yymktext(strm)
      in
        yystrm := strm;
         lexErr((yypos, yypos), ["bad character `", String.toString yytext, "'"]);
			    continue()
      end
fun yyAction30 (strm, lastMatch : yymatch) = let
      val yytext = yymktext(strm)
      in
        yystrm := strm;  addStr(valOf(String.fromString yytext)); continue()
      end
fun yyAction31 (strm, lastMatch : yymatch) = let
      val yytext = yymktext(strm)
      in
        yystrm := strm;  addStr yytext; continue()
      end
fun yyAction32 (strm, lastMatch : yymatch) = (yystrm := strm;
       YYBEGIN INITIAL; mkString())
fun yyAction33 (strm, lastMatch : yymatch) = let
      val yytext = yymktext(strm)
      in
        yystrm := strm;
         lexErr((yypos, yypos), [
				"bad escape character `", String.toString yytext,
				"' in string literal"
			      ]);
			    continue()
      end
fun yyAction34 (strm, lastMatch : yymatch) = (yystrm := strm;
       lexErr((!startPos, yypos), ["unclosed string at end of file"]);
			    YYBEGIN INITIAL; mkString())
fun yyAction35 (strm, lastMatch : yymatch) = let
      val yytext = yymktext(strm)
      in
        yystrm := strm;
         lexErr((yypos, yypos), [
				"bad character `", String.toString yytext,
				"' in string literal"
			      ]);
			    continue()
      end
fun yyAction36 (strm, lastMatch : yymatch) = (yystrm := strm;
       depth := !depth + 1;
			    skip())
fun yyAction37 (strm, lastMatch : yymatch) = (yystrm := strm;
       depth := !depth - 1;
			    if (!depth = 0) then YYBEGIN INITIAL else ();
			    skip ())
fun yyAction38 (strm, lastMatch : yymatch) = (yystrm := strm;  skip ())
fun yyQ44 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction5(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction5(strm, yyNO_MATCH)
      (* end case *))
fun yyQ43 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction4(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction4(strm, yyNO_MATCH)
      (* end case *))
fun yyQ45 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction23(strm, yyNO_MATCH)
        | SOME(inp, strm') =>
            if inp = 0wx41
              then yyQ45(strm', yyMATCH(strm, yyAction23, yyNO_MATCH))
            else if inp < 0wx41
              then if inp = 0wx28
                  then yyAction23(strm, yyNO_MATCH)
                else if inp < 0wx28
                  then if inp = 0wx27
                      then yyQ45(strm', yyMATCH(strm, yyAction23, yyNO_MATCH))
                      else yyAction23(strm, yyNO_MATCH)
                else if inp = 0wx30
                  then yyQ45(strm', yyMATCH(strm, yyAction23, yyNO_MATCH))
                else if inp < 0wx30
                  then yyAction23(strm, yyNO_MATCH)
                else if inp <= 0wx39
                  then yyQ45(strm', yyMATCH(strm, yyAction23, yyNO_MATCH))
                  else yyAction23(strm, yyNO_MATCH)
            else if inp = 0wx60
              then yyAction23(strm, yyNO_MATCH)
            else if inp < 0wx60
              then if inp = 0wx5B
                  then yyAction23(strm, yyNO_MATCH)
                else if inp < 0wx5B
                  then yyQ45(strm', yyMATCH(strm, yyAction23, yyNO_MATCH))
                else if inp = 0wx5F
                  then yyQ45(strm', yyMATCH(strm, yyAction23, yyNO_MATCH))
                  else yyAction23(strm, yyNO_MATCH)
            else if inp <= 0wx7A
              then yyQ45(strm', yyMATCH(strm, yyAction23, yyNO_MATCH))
              else yyAction23(strm, yyNO_MATCH)
      (* end case *))
fun yyQ42 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction23(strm, yyNO_MATCH)
        | SOME(inp, strm') =>
            if inp = 0wx41
              then yyQ45(strm', yyMATCH(strm, yyAction23, yyNO_MATCH))
            else if inp < 0wx41
              then if inp = 0wx28
                  then yyAction23(strm, yyNO_MATCH)
                else if inp < 0wx28
                  then if inp = 0wx27
                      then yyQ45(strm', yyMATCH(strm, yyAction23, yyNO_MATCH))
                      else yyAction23(strm, yyNO_MATCH)
                else if inp = 0wx30
                  then yyQ45(strm', yyMATCH(strm, yyAction23, yyNO_MATCH))
                else if inp < 0wx30
                  then yyAction23(strm, yyNO_MATCH)
                else if inp <= 0wx39
                  then yyQ45(strm', yyMATCH(strm, yyAction23, yyNO_MATCH))
                  else yyAction23(strm, yyNO_MATCH)
            else if inp = 0wx60
              then yyAction23(strm, yyNO_MATCH)
            else if inp < 0wx60
              then if inp = 0wx5B
                  then yyAction23(strm, yyNO_MATCH)
                else if inp < 0wx5B
                  then yyQ45(strm', yyMATCH(strm, yyAction23, yyNO_MATCH))
                else if inp = 0wx5F
                  then yyQ45(strm', yyMATCH(strm, yyAction23, yyNO_MATCH))
                  else yyAction23(strm, yyNO_MATCH)
            else if inp <= 0wx7A
              then yyQ45(strm', yyMATCH(strm, yyAction23, yyNO_MATCH))
              else yyAction23(strm, yyNO_MATCH)
      (* end case *))
fun yyQ41 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction22(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction22(strm, yyNO_MATCH)
      (* end case *))
fun yyQ40 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction3(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction3(strm, yyNO_MATCH)
      (* end case *))
fun yyQ39 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction2(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction2(strm, yyNO_MATCH)
      (* end case *))
fun yyQ46 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction24(strm, yyNO_MATCH)
        | SOME(inp, strm') =>
            if inp = 0wx41
              then yyQ46(strm', yyMATCH(strm, yyAction24, yyNO_MATCH))
            else if inp < 0wx41
              then if inp = 0wx28
                  then yyAction24(strm, yyNO_MATCH)
                else if inp < 0wx28
                  then if inp = 0wx27
                      then yyQ46(strm', yyMATCH(strm, yyAction24, yyNO_MATCH))
                      else yyAction24(strm, yyNO_MATCH)
                else if inp = 0wx30
                  then yyQ46(strm', yyMATCH(strm, yyAction24, yyNO_MATCH))
                else if inp < 0wx30
                  then yyAction24(strm, yyNO_MATCH)
                else if inp <= 0wx39
                  then yyQ46(strm', yyMATCH(strm, yyAction24, yyNO_MATCH))
                  else yyAction24(strm, yyNO_MATCH)
            else if inp = 0wx60
              then yyAction24(strm, yyNO_MATCH)
            else if inp < 0wx60
              then if inp = 0wx5B
                  then yyAction24(strm, yyNO_MATCH)
                else if inp < 0wx5B
                  then yyQ46(strm', yyMATCH(strm, yyAction24, yyNO_MATCH))
                else if inp = 0wx5F
                  then yyQ46(strm', yyMATCH(strm, yyAction24, yyNO_MATCH))
                  else yyAction24(strm, yyNO_MATCH)
            else if inp <= 0wx7A
              then yyQ46(strm', yyMATCH(strm, yyAction24, yyNO_MATCH))
              else yyAction24(strm, yyNO_MATCH)
      (* end case *))
fun yyQ38 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction24(strm, yyNO_MATCH)
        | SOME(inp, strm') =>
            if inp = 0wx41
              then yyQ46(strm', yyMATCH(strm, yyAction24, yyNO_MATCH))
            else if inp < 0wx41
              then if inp = 0wx28
                  then yyAction24(strm, yyNO_MATCH)
                else if inp < 0wx28
                  then if inp = 0wx27
                      then yyQ46(strm', yyMATCH(strm, yyAction24, yyNO_MATCH))
                      else yyAction24(strm, yyNO_MATCH)
                else if inp = 0wx30
                  then yyQ46(strm', yyMATCH(strm, yyAction24, yyNO_MATCH))
                else if inp < 0wx30
                  then yyAction24(strm, yyNO_MATCH)
                else if inp <= 0wx39
                  then yyQ46(strm', yyMATCH(strm, yyAction24, yyNO_MATCH))
                  else yyAction24(strm, yyNO_MATCH)
            else if inp = 0wx60
              then yyAction24(strm, yyNO_MATCH)
            else if inp < 0wx60
              then if inp = 0wx5B
                  then yyAction24(strm, yyNO_MATCH)
                else if inp < 0wx5B
                  then yyQ46(strm', yyMATCH(strm, yyAction24, yyNO_MATCH))
                else if inp = 0wx5F
                  then yyQ46(strm', yyMATCH(strm, yyAction24, yyNO_MATCH))
                  else yyAction24(strm, yyNO_MATCH)
            else if inp <= 0wx7A
              then yyQ46(strm', yyMATCH(strm, yyAction24, yyNO_MATCH))
              else yyAction24(strm, yyNO_MATCH)
      (* end case *))
fun yyQ37 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction10(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction10(strm, yyNO_MATCH)
      (* end case *))
fun yyQ48 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction21(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction21(strm, yyNO_MATCH)
      (* end case *))
fun yyQ47 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction6(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction6(strm, yyNO_MATCH)
      (* end case *))
fun yyQ36 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction16(strm, yyNO_MATCH)
        | SOME(inp, strm') =>
            if inp = 0wx3E
              then yyQ48(strm', yyMATCH(strm, yyAction16, yyNO_MATCH))
            else if inp < 0wx3E
              then if inp = 0wx3D
                  then yyQ47(strm', yyMATCH(strm, yyAction16, yyNO_MATCH))
                  else yyAction16(strm, yyNO_MATCH)
              else yyAction16(strm, yyNO_MATCH)
      (* end case *))
fun yyQ49 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction7(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction7(strm, yyNO_MATCH)
      (* end case *))
fun yyQ35 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction8(strm, yyNO_MATCH)
        | SOME(inp, strm') =>
            if inp = 0wx3D
              then yyQ49(strm', yyMATCH(strm, yyAction8, yyNO_MATCH))
              else yyAction8(strm, yyNO_MATCH)
      (* end case *))
fun yyQ34 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction18(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction18(strm, yyNO_MATCH)
      (* end case *))
fun yyQ50 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction9(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction9(strm, yyNO_MATCH)
      (* end case *))
fun yyQ33 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction19(strm, yyNO_MATCH)
        | SOME(inp, strm') =>
            if inp = 0wx3A
              then yyQ50(strm', yyMATCH(strm, yyAction19, yyNO_MATCH))
              else yyAction19(strm, yyNO_MATCH)
      (* end case *))
fun yyQ51 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction25(strm, yyNO_MATCH)
        | SOME(inp, strm') =>
            if inp = 0wx30
              then yyQ51(strm', yyMATCH(strm, yyAction25, yyNO_MATCH))
            else if inp < 0wx30
              then yyAction25(strm, yyNO_MATCH)
            else if inp <= 0wx39
              then yyQ51(strm', yyMATCH(strm, yyAction25, yyNO_MATCH))
              else yyAction25(strm, yyNO_MATCH)
      (* end case *))
fun yyQ32 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction25(strm, yyNO_MATCH)
        | SOME(inp, strm') =>
            if inp = 0wx30
              then yyQ51(strm', yyMATCH(strm, yyAction25, yyNO_MATCH))
            else if inp < 0wx30
              then yyAction25(strm, yyNO_MATCH)
            else if inp <= 0wx39
              then yyQ51(strm', yyMATCH(strm, yyAction25, yyNO_MATCH))
              else yyAction25(strm, yyNO_MATCH)
      (* end case *))
fun yyQ31 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction14(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction14(strm, yyNO_MATCH)
      (* end case *))
fun yyQ52 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction20(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction20(strm, yyNO_MATCH)
      (* end case *))
fun yyQ30 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction12(strm, yyNO_MATCH)
        | SOME(inp, strm') =>
            if inp = 0wx3E
              then yyQ52(strm', yyMATCH(strm, yyAction12, yyNO_MATCH))
              else yyAction12(strm, yyNO_MATCH)
      (* end case *))
fun yyQ29 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction17(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction17(strm, yyNO_MATCH)
      (* end case *))
fun yyQ28 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction11(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction11(strm, yyNO_MATCH)
      (* end case *))
fun yyQ27 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction13(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction13(strm, yyNO_MATCH)
      (* end case *))
fun yyQ26 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction1(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction1(strm, yyNO_MATCH)
      (* end case *))
fun yyQ53 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction27(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction27(strm, yyNO_MATCH)
      (* end case *))
fun yyQ25 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction0(strm, yyNO_MATCH)
        | SOME(inp, strm') =>
            if inp = 0wx2A
              then yyQ53(strm', yyMATCH(strm, yyAction0, yyNO_MATCH))
              else yyAction0(strm, yyNO_MATCH)
      (* end case *))
fun yyQ24 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction15(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction15(strm, yyNO_MATCH)
      (* end case *))
fun yyQ23 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction28(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction28(strm, yyNO_MATCH)
      (* end case *))
fun yyQ22 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction26(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction26(strm, yyNO_MATCH)
      (* end case *))
fun yyQ21 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction29(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction29(strm, yyNO_MATCH)
      (* end case *))
fun yyQ2 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE =>
            if ULexBuffer.eof(!(yystrm))
              then let
                val yycolno = ref(yygetcolNo(!(yystrm)))
                val yylineno = ref(yygetlineNo(!(yystrm)))
                in
                  (case (!(yyss))
                   of COMMENT =>
                        ( lexErr((!startPos, yypos), ["unclosed comment at end of file"]); T.EOF)
                    | STRING =>
                        ( lexErr((!startPos, yypos), ["unclosed string at end of file"]); T.EOF)
                    | INITIAL => ( T.EOF)
                  (* end case *))
                end
              else yystuck(lastMatch)
        | SOME(inp, strm') =>
            if inp = 0wx3A
              then yyQ33(strm', lastMatch)
            else if inp < 0wx3A
              then if inp = 0wx28
                  then yyQ25(strm', lastMatch)
                else if inp < 0wx28
                  then if inp = 0wx21
                      then yyQ21(strm', lastMatch)
                    else if inp < 0wx21
                      then if inp = 0wxE
                          then yyQ21(strm', lastMatch)
                        else if inp < 0wxE
                          then if inp <= 0wx8
                              then yyQ21(strm', lastMatch)
                              else yyQ22(strm', lastMatch)
                        else if inp = 0wx20
                          then yyQ22(strm', lastMatch)
                          else yyQ21(strm', lastMatch)
                    else if inp = 0wx25
                      then yyQ24(strm', lastMatch)
                    else if inp < 0wx25
                      then if inp = 0wx22
                          then yyQ23(strm', lastMatch)
                          else yyQ21(strm', lastMatch)
                      else yyQ21(strm', lastMatch)
                else if inp = 0wx2D
                  then yyQ30(strm', lastMatch)
                else if inp < 0wx2D
                  then if inp = 0wx2B
                      then yyQ28(strm', lastMatch)
                    else if inp < 0wx2B
                      then if inp = 0wx29
                          then yyQ26(strm', lastMatch)
                          else yyQ27(strm', lastMatch)
                      else yyQ29(strm', lastMatch)
                else if inp = 0wx2F
                  then yyQ31(strm', lastMatch)
                else if inp = 0wx2E
                  then yyQ21(strm', lastMatch)
                  else yyQ32(strm', lastMatch)
            else if inp = 0wx5D
              then yyQ40(strm', lastMatch)
            else if inp < 0wx5D
              then if inp = 0wx40
                  then yyQ37(strm', lastMatch)
                else if inp < 0wx40
                  then if inp = 0wx3D
                      then yyQ36(strm', lastMatch)
                    else if inp < 0wx3D
                      then if inp = 0wx3B
                          then yyQ34(strm', lastMatch)
                          else yyQ35(strm', lastMatch)
                      else yyQ21(strm', lastMatch)
                else if inp = 0wx5B
                  then yyQ39(strm', lastMatch)
                else if inp = 0wx5C
                  then yyQ21(strm', lastMatch)
                  else yyQ38(strm', lastMatch)
            else if inp = 0wx7B
              then yyQ43(strm', lastMatch)
            else if inp < 0wx7B
              then if inp = 0wx60
                  then yyQ21(strm', lastMatch)
                else if inp < 0wx60
                  then if inp = 0wx5E
                      then yyQ21(strm', lastMatch)
                      else yyQ41(strm', lastMatch)
                  else yyQ42(strm', lastMatch)
            else if inp = 0wx7D
              then yyQ44(strm', lastMatch)
              else yyQ21(strm', lastMatch)
      (* end case *))
fun yyQ19 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction37(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction37(strm, yyNO_MATCH)
      (* end case *))
fun yyQ18 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction38(strm, yyNO_MATCH)
        | SOME(inp, strm') =>
            if inp = 0wx29
              then yyQ19(strm', yyMATCH(strm, yyAction38, yyNO_MATCH))
              else yyAction38(strm, yyNO_MATCH)
      (* end case *))
fun yyQ20 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction36(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction36(strm, yyNO_MATCH)
      (* end case *))
fun yyQ17 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction38(strm, yyNO_MATCH)
        | SOME(inp, strm') =>
            if inp = 0wx2A
              then yyQ20(strm', yyMATCH(strm, yyAction38, yyNO_MATCH))
              else yyAction38(strm, yyNO_MATCH)
      (* end case *))
fun yyQ16 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction38(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction38(strm, yyNO_MATCH)
      (* end case *))
fun yyQ1 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE =>
            if ULexBuffer.eof(!(yystrm))
              then let
                val yycolno = ref(yygetcolNo(!(yystrm)))
                val yylineno = ref(yygetlineNo(!(yystrm)))
                in
                  (case (!(yyss))
                   of COMMENT =>
                        ( lexErr((!startPos, yypos), ["unclosed comment at end of file"]); T.EOF)
                    | STRING =>
                        ( lexErr((!startPos, yypos), ["unclosed string at end of file"]); T.EOF)
                    | INITIAL => ( T.EOF)
                  (* end case *))
                end
              else yystuck(lastMatch)
        | SOME(inp, strm') =>
            if inp = 0wx29
              then yyQ16(strm', lastMatch)
            else if inp < 0wx29
              then if inp = 0wx28
                  then yyQ17(strm', lastMatch)
                  else yyQ16(strm', lastMatch)
            else if inp = 0wx2A
              then yyQ18(strm', lastMatch)
              else yyQ16(strm', lastMatch)
      (* end case *))
fun yyQ13 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction30(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction30(strm, yyNO_MATCH)
      (* end case *))
fun yyQ12 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yystuck(lastMatch)
        | SOME(inp, strm') =>
            if inp = 0wx30
              then yyQ13(strm', lastMatch)
            else if inp < 0wx30
              then yystuck(lastMatch)
            else if inp <= 0wx39
              then yyQ13(strm', lastMatch)
              else yystuck(lastMatch)
      (* end case *))
fun yyQ11 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction33(strm, yyNO_MATCH)
        | SOME(inp, strm') =>
            if inp = 0wx30
              then yyQ12(strm', yyMATCH(strm, yyAction33, yyNO_MATCH))
            else if inp < 0wx30
              then yyAction33(strm, yyNO_MATCH)
            else if inp <= 0wx39
              then yyQ12(strm', yyMATCH(strm, yyAction33, yyNO_MATCH))
              else yyAction33(strm, yyNO_MATCH)
      (* end case *))
fun yyQ10 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction30(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction30(strm, yyNO_MATCH)
      (* end case *))
fun yyQ9 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction33(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction33(strm, yyNO_MATCH)
      (* end case *))
fun yyQ8 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction35(strm, yyNO_MATCH)
        | SOME(inp, strm') =>
            if inp = 0wx66
              then yyQ10(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
            else if inp < 0wx66
              then if inp = 0wx3A
                  then yyQ9(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
                else if inp < 0wx3A
                  then if inp = 0wx23
                      then yyQ9(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
                    else if inp < 0wx23
                      then if inp = 0wx22
                          then yyQ10(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
                          else yyQ9(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
                    else if inp <= 0wx2F
                      then yyQ9(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
                      else yyQ11(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
                else if inp = 0wx5D
                  then yyQ9(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
                else if inp < 0wx5D
                  then if inp = 0wx5C
                      then yyQ10(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
                      else yyQ9(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
                else if inp = 0wx61
                  then yyQ10(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
                else if inp < 0wx61
                  then yyQ9(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
                else if inp <= 0wx62
                  then yyQ10(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
                  else yyQ9(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
            else if inp = 0wx73
              then yyQ9(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
            else if inp < 0wx73
              then if inp = 0wx6F
                  then yyQ9(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
                else if inp < 0wx6F
                  then if inp = 0wx6E
                      then yyQ10(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
                      else yyQ9(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
                else if inp = 0wx72
                  then yyQ10(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
                  else yyQ9(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
            else if inp = 0wx76
              then yyQ10(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
            else if inp < 0wx76
              then if inp = 0wx74
                  then yyQ10(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
                  else yyQ9(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
              else yyQ9(strm', yyMATCH(strm, yyAction35, yyNO_MATCH))
      (* end case *))
fun yyQ7 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction32(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction32(strm, yyNO_MATCH)
      (* end case *))
fun yyQ14 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction31(strm, yyNO_MATCH)
        | SOME(inp, strm') =>
            if inp = 0wx23
              then yyQ14(strm', yyMATCH(strm, yyAction31, yyNO_MATCH))
            else if inp < 0wx23
              then if inp = 0wx20
                  then yyQ14(strm', yyMATCH(strm, yyAction31, yyNO_MATCH))
                else if inp < 0wx20
                  then yyAction31(strm, yyNO_MATCH)
                else if inp = 0wx22
                  then yyAction31(strm, yyNO_MATCH)
                  else yyQ14(strm', yyMATCH(strm, yyAction31, yyNO_MATCH))
            else if inp = 0wx5D
              then yyQ14(strm', yyMATCH(strm, yyAction31, yyNO_MATCH))
            else if inp < 0wx5D
              then if inp = 0wx5C
                  then yyAction31(strm, yyNO_MATCH)
                  else yyQ14(strm', yyMATCH(strm, yyAction31, yyNO_MATCH))
            else if inp <= 0wx7E
              then yyQ14(strm', yyMATCH(strm, yyAction31, yyNO_MATCH))
              else yyAction31(strm, yyNO_MATCH)
      (* end case *))
fun yyQ6 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction31(strm, yyNO_MATCH)
        | SOME(inp, strm') =>
            if inp = 0wx23
              then yyQ14(strm', yyMATCH(strm, yyAction31, yyNO_MATCH))
            else if inp < 0wx23
              then if inp = 0wx20
                  then yyQ14(strm', yyMATCH(strm, yyAction31, yyNO_MATCH))
                else if inp < 0wx20
                  then yyAction31(strm, yyNO_MATCH)
                else if inp = 0wx22
                  then yyAction31(strm, yyNO_MATCH)
                  else yyQ14(strm', yyMATCH(strm, yyAction31, yyNO_MATCH))
            else if inp = 0wx5D
              then yyQ14(strm', yyMATCH(strm, yyAction31, yyNO_MATCH))
            else if inp < 0wx5D
              then if inp = 0wx5C
                  then yyAction31(strm, yyNO_MATCH)
                  else yyQ14(strm', yyMATCH(strm, yyAction31, yyNO_MATCH))
            else if inp <= 0wx7E
              then yyQ14(strm', yyMATCH(strm, yyAction31, yyNO_MATCH))
              else yyAction31(strm, yyNO_MATCH)
      (* end case *))
fun yyQ15 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction34(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction34(strm, yyNO_MATCH)
      (* end case *))
fun yyQ5 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction34(strm, yyNO_MATCH)
        | SOME(inp, strm') =>
            if inp = 0wxA
              then yyQ15(strm', yyMATCH(strm, yyAction34, yyNO_MATCH))
              else yyAction34(strm, yyNO_MATCH)
      (* end case *))
fun yyQ4 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction34(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction34(strm, yyNO_MATCH)
      (* end case *))
fun yyQ3 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE => yyAction35(strm, yyNO_MATCH)
        | SOME(inp, strm') => yyAction35(strm, yyNO_MATCH)
      (* end case *))
fun yyQ0 (strm, lastMatch : yymatch) = (case (yygetc(strm))
       of NONE =>
            if ULexBuffer.eof(!(yystrm))
              then let
                val yycolno = ref(yygetcolNo(!(yystrm)))
                val yylineno = ref(yygetlineNo(!(yystrm)))
                in
                  (case (!(yyss))
                   of COMMENT =>
                        ( lexErr((!startPos, yypos), ["unclosed comment at end of file"]); T.EOF)
                    | STRING =>
                        ( lexErr((!startPos, yypos), ["unclosed string at end of file"]); T.EOF)
                    | INITIAL => ( T.EOF)
                  (* end case *))
                end
              else yystuck(lastMatch)
        | SOME(inp, strm') =>
            if inp = 0wx20
              then yyQ6(strm', lastMatch)
            else if inp < 0wx20
              then if inp = 0wxB
                  then yyQ3(strm', lastMatch)
                else if inp < 0wxB
                  then if inp = 0wxA
                      then yyQ4(strm', lastMatch)
                      else yyQ3(strm', lastMatch)
                else if inp = 0wxD
                  then yyQ5(strm', lastMatch)
                  else yyQ3(strm', lastMatch)
            else if inp = 0wx5C
              then yyQ8(strm', lastMatch)
            else if inp < 0wx5C
              then if inp = 0wx22
                  then yyQ7(strm', lastMatch)
                  else yyQ6(strm', lastMatch)
            else if inp <= 0wx7E
              then yyQ6(strm', lastMatch)
              else yyQ3(strm', lastMatch)
      (* end case *))
in
  (case (!(yyss))
   of STRING => yyQ0(!(yystrm), yyNO_MATCH)
    | COMMENT => yyQ1(!(yystrm), yyNO_MATCH)
    | INITIAL => yyQ2(!(yystrm), yyNO_MATCH)
  (* end case *))
end
end
            and skip() = (yystartPos := yygetPos(); 
			  yylastwasnref := ULexBuffer.lastWasNL (!yystrm);
			  continue())
	    in (continue(), (!yystartPos, yygetPos()), !yystrm, !yyss) end
          in 
            lex()
          end
  in
    type pos = AntlrStreamPos.pos
    type span = AntlrStreamPos.span
    type tok = UserDeclarations.lex_result

    datatype prestrm = STRM of ULexBuffer.stream * 
		(yystart_state * tok * span * prestrm * yystart_state) option ref
    type strm = (prestrm * yystart_state)

    fun lex sm 
(yyarg as  lexErr)(STRM (yystrm, memo), ss) = (case !memo
	  of NONE => let
	     val (tok, span, yystrm', ss') = innerLex 
yyarg(yystrm, ss, sm)
	     val strm' = STRM (yystrm', ref NONE);
	     in 
	       memo := SOME (ss, tok, span, strm', ss');
	       (tok, span, (strm', ss'))
	     end
	   | SOME (ss', tok, span, strm', ss'') => 
	       if ss = ss' then
		 (tok, span, (strm', ss''))
	       else (
		 memo := NONE;
		 lex sm 
yyarg(STRM (yystrm, memo), ss))
         (* end case *))

    fun streamify input = (STRM (yystreamify' 0 input, ref NONE), INITIAL)
    fun streamifyReader readFn strm = (STRM (yystreamifyReader' 0 readFn strm, ref NONE), 
				       INITIAL)
    fun streamifyInstream strm = (STRM (yystreamifyInstream' 0 strm, ref NONE), 
				  INITIAL)

    fun getPos (STRM (strm, _), _) = ULexBuffer.getpos strm

  end
end
