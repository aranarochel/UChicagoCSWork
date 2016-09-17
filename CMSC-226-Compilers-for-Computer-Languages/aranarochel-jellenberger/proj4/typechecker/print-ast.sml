(* print-ast.sml
 *
 * CMSC22610 Winter 2015
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 *)

structure PrintAST : sig

    type strm

  (* create a new output stream *)
    val new : {
	    outS : TextIO.outstream,	(* output stream to print to *)
	    showStamps : bool		(* if true, the unique stamps on IDs will be included *)
	  } -> strm

  (* print functions for AST types *)
    val program	: strm * AST.program -> unit
    val bind	: strm * AST.bind -> unit
    val ty	: strm * AST.ty -> unit
    val param	: strm * AST.param -> unit
    val exp	: strm * AST.exp -> unit
    val rule	: strm * AST.rule -> unit
    val pat	: strm * AST.pat -> unit

  (* dump an AST term to the standard output for debugging purposes; this will show stamps *)
    val dump : ((strm * 'a) -> unit) -> 'a -> unit

  end = struct

    structure TV = TyVar
    structure TC = TyCon
    structure DC = DataCon

    datatype strm = S of {
	indent : int,			(* current indentation level *)
	info : strm_info
      }

    and strm_info = Info of {
	verbose : bool,			(* if true, print stamps info *)
	outS : TextIO.outstream		(* output I/O stream to print to *)
      }

    fun new {outS, showStamps} = S{
	    indent = 0,
	    info = Info{verbose = showStamps, outS = outS}
	  }

  (* print text *)
    fun pr (S{info=Info{outS, ...}, ...}, txt) = TextIO.output(outS, txt)

  (* print a newline *)
    fun nl (S{info=Info{outS, ...}, ...}) = TextIO.output1 (outS, #"\n")

  (* print whitespace to indent the text *)
    fun prIndent (S{indent=n, info=Info{outS, ...}, ...}) = let
	  fun lp 0 = ()
	    | lp i = (TextIO.output(outS, "  "); lp(i-1))
	  in
	    lp n
	  end

    fun showStamps (S{info=Info{verbose, ...}, ...}) = verbose

  (* increment indentation level *)
    fun inc (S{indent, info}) = S{indent=indent+1, info=info}

    fun nest strm f = f (inc strm)

    fun prTy (strm, ty) = pr (strm, TypeUtil.fmt (showStamps strm) ty)

    fun prStr (strm, s) = (prIndent strm; pr (strm, s); nl strm);

    fun prTV (strm, tv) = if showStamps strm
	  then prStr (strm, TV.toString tv)
	  else prStr (strm, TV.nameOf tv)
    fun prVar (strm, x) = if showStamps strm
	  then prStr (strm, Var.toString x)
	  else prStr (strm, Var.nameOf x)
    fun prVarBind (strm, x) = if showStamps strm
	  then prStr (strm, concat[Var.toString x, " : ", TypeUtil.fmt true (Var.typeOf x)])
	  else prStr (strm, concat[Var.nameOf x, " : ", TypeUtil.toString (Var.typeOf x)])
    fun prDCon (strm, dc) = if showStamps strm
	  then prStr (strm, DC.toString dc)
	  else prStr (strm, DC.nameOf dc)

    fun prNode (strm, name) = (prIndent strm; pr (strm, name); nl strm)

  (* print a list of items enclosed in "[" ... "]" *)
    fun prList prItem (strm, []) = (prIndent strm; pr (strm, "[ ]\n"))
      | prList prItem (strm, items) = (
	  prIndent strm; pr (strm, "[\n");
	  nest strm
	    (fn strm' => List.app (fn item => prItem (strm', item)) items);
	  prIndent strm; pr (strm, "]\n"))

    fun program (strm as S{info=Info{outS, ...}, ...}, AST.Program e) = (
	  prNode (strm, "Program");
	  exp (inc strm, e);
	  TextIO.flushOut outS)

    and bind (strm, b) = (case b
	   of AST.DataBind tyc => (
		prNode (strm, "DataBind");
		if (showStamps strm)
		  then prStr(strm, TC.toString tyc)
		  else prStr(strm, TC.nameOf tyc))
	    | AST.FunBind(f, params, e) => (
		prNode (strm, "FunBind");
		nest strm (fn strm => (
		  prVarBind (strm, f);
		  prList param (strm, params);
		  exp (strm, e))))
	    | AST.ValBind(x, e) => (
		prNode (strm, "ValBind");
		nest strm (fn strm => (
		  prVarBind (strm, x);
		  exp (strm, e))))
	  (* end case *))

    and ty (strm, t) = (prIndent strm; prTy (strm, t); nl strm)

    and param (strm, param) = (case param
	   of AST.TyParam tvs => (
		prNode (strm, "TyParam");
		prList prTV (inc strm, tvs))
	    | AST.ValParam x => (
		prNode (strm, "ValParam");
		prVarBind (inc strm, x))
	  (* end case *))

    and exp (strm, AST.Exp(e, t)) = let
	  fun prNode name = (
		prIndent strm; pr (strm, name); pr (strm, " : "); prTy (strm, t); nl strm)
	  in
	    case e
	     of AST.IfExp(e1, e2, e3) => (
		  prNode "IfExp";
		  nest strm (fn strm => (
		    exp (strm, e1);
		    exp (strm, e2);
		    exp (strm, e3))))
	      | AST.AppExp(e1, e2) => (
		  prNode "AppExp";
		  nest strm (fn strm => (
		    exp (strm, e1);
		    exp (strm, e2))))
	      | AST.TyAppExp(e, tys) => (
		  prNode "TyAppExp";
		  nest strm (fn strm => (
		    exp (strm, e);
		    prList ty (strm, tys))))
	      | AST.TupleExp es => (
		  prNode "TupleExp";
		  prList exp (inc strm, es))
	      | AST.LetExp(b, e) => (
		  prNode "LetExp";
		  nest strm (fn strm => (
		    bind (strm, b);
		    exp (strm, e))))
	      | AST.CaseExp(e, rules) => (
		  prNode "CaseExp";
		  nest strm (fn strm => (
		    exp (strm, e);
		    prList rule (strm, rules))))
	      | AST.VarExp x => (
		  prNode "VarExp";
		  prVar (inc strm, x))
	      | AST.ConExp dc => (
		  prNode "ConExp";
		  prDCon (inc strm, dc))
	      | AST.IntExp n => (
		  prNode "IntExp";
		  prStr (inc strm, IntInf.toString n))
	      | AST.StrExp s => (
		  prNode "StrExp";
		  prStr (inc strm, concat["\"", String.toString s, "\""]))
	    (* end case *)
	  end

    and rule (strm, AST.Rule(p, e)) = (
	  prNode (strm, "Rule");
	  nest strm (fn strm => (
	    pat (strm, p);
	    exp (strm, e))))

    and pat (strm, p) = (case p
	   of AST.ConAppPat(dc, tys, x) => (
		prNode (strm, "ConAppPat");
		nest strm (fn strm => (
		  prDCon (strm, dc);
		  prList ty (strm, tys);
		  prVarBind (strm, x))))
	    | AST.ConPat(dc, tys) => (
		prNode (strm, "ConPat");
		nest strm (fn strm => (
		  prDCon (strm, dc);
		  prList ty (strm, tys))))
	    | AST.ListPat(t, hd, tl) => (
		prNode (strm, "ListPat");
		nest strm (fn strm => (
		  ty (strm, t);
		  prVarBind (strm, hd);
		  prVarBind (strm, tl))))
	    | AST.TuplePat xs => (
		prNode (strm, "TuplePat");
		prList prVarBind (inc strm, xs))
	    | AST.VarPat x => (
		prNode (strm, "VarPat");
		prVarBind (inc strm, x))
	  (* end case *))

  (* dump an AST term to the standard output for debugging purposes *)
    fun dump printFn = let
	  val strm = new{outS = TextIO.stdOut, showStamps = true}
	  in
	    fn term => (printFn (strm, term); TextIO.flushOut TextIO.stdOut)
	  end

  end
