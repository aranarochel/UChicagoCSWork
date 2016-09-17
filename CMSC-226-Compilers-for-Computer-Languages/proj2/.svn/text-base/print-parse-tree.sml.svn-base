(* print-parse-tree.sml
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 *)

structure PrintParseTree : sig

    type strm

  (* create a new output stream *)
    val new : {
	    outS : TextIO.outstream,	(* output stream to print to *)
	    errS : Error.err_stream,	(* error stream from parse; used to report locations *)
	    showMarks : bool		(* if true, node position information will be included *)
	  } -> strm

    val program    : strm * ParseTree.program -> unit
    val definition : strm * ParseTree.definition -> unit
    val typ        : strm * ParseTree.typ -> unit
    val con        : strm * ParseTree.con -> unit
    val bind       : strm * ParseTree.bind -> unit
    val param      : strm * ParseTree.param -> unit
    val exp        : strm * ParseTree.exp -> unit
    val scope      : strm * ParseTree.scope -> unit
    val case_match : strm * ParseTree.case_match -> unit
    val pat        : strm * ParseTree.pat -> unit

  end = struct

    structure PT = ParseTree
    structure E = Error

    datatype strm = S of {
	indent : int,			(* current indentation level *)
	span : E.span,
	info : strm_info
      }

    and strm_info = Info of {
	mark : bool,			(* if true, print mark info *)
	errS : Error.err_stream,	(* for interpreting spans *)
	outS : TextIO.outstream		(* output I/O stream to print to *)
      }

    fun new {errS, outS, showMarks} = S{
	    indent = 0, span = (0, 0),
	    info = Info{mark = showMarks, errS = errS, outS = outS}
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

  (* increment indentation level *)
    fun inc (S{indent, span, info}) = S{indent=indent+1, span=span, info=info}

    fun nest strm f = f (inc strm)

  (* print location information *)
    fun prLoc (S{span, info, ...}) = (case info
	   of Info{mark=true, errS, outS, ...} =>
		TextIO.output (outS, Error.locToString (Error.location (errS, span)))
	    | _ => ()
	  (* end case *))

  (* update the current span *)
    fun mark (S{indent, info, ...}, {span, tree}) = (S{span=span, indent=indent, info=info}, tree)

    fun prStr (strm, s) = (prIndent strm; pr (strm, s); nl strm);
    fun prId (strm, id) = prStr (strm, Atom.toString id)

    fun prNode (strm, name) = (prIndent strm; pr (strm, name); prLoc strm; nl strm)

  (* print a list of items encolsed in "[" ... "]" *)
    fun prList prItem (strm, []) = (prIndent strm; pr (strm, "[ ]\n"))
      | prList prItem (strm, items) = (
	  prIndent strm; pr (strm, "[\n");
	  nest strm
	    (fn strm' => List.app (fn item => prItem (strm', item)) items);
	  prIndent strm; pr (strm, "]\n"))

    fun program (strm, PT.MarkProgram m) = program (mark (strm, m))
      | program (strm, PT.Program(defs, e)) = (
	  prNode (strm, "Program");
	  nest strm (fn strm => (
	    prList definition (strm, defs);
	    exp (strm, e))))

    and definition (strm, obj) = (case obj
	   of PT.MarkDef m => definition (mark (strm, m))
	    | PT.TypeDef(id, tvs, ty) => (
		prNode (strm, "TypeDef");
		nest strm (fn strm => (
		  prId (strm, id); prList prId (strm, tvs); typ (strm, ty))))
	    | PT.DataDef(id, tvs, dcs) => (
 		prNode (strm, "DataDef");
		nest strm (fn strm => (
		  prId (strm, id); prList prId (strm, tvs); prList con (strm, dcs))))
	    | PT.ValDef vb => (
		prNode (strm, "ValDef");
		bind (inc strm, vb))
	  (* end case *))

    and typ (strm, obj) = (case obj
	   of PT.MarkTyp m => typ (mark (strm, m))
	    | PT.PolyTyp(tvs, ty) => (
		prNode (strm, "PolyTyp");
		nest strm (fn strm => (
		  prList prId (strm, tvs);
		  typ (strm, ty))))
	    | PT.VarTyp id => (
		prNode (strm, "VarTyp");
		prId (inc strm, id))
	    | PT.ConTyp(id, tys) => (
		prNode (strm, "ConTyp");
		nest strm (fn strm => (
		  prId (strm, id);
		  prList typ (strm, tys))))
	    | PT.FunTyp(ty1, ty2) => (
		prNode (strm, "FunTyp");
		nest strm (fn strm => (
		  typ (strm, ty1);
		  typ (strm, ty2))))
	    | PT.TupleTyp tys => (
		prNode (strm, "TupleTyp");
		prList typ (inc strm, tys))
	  (* end case *))

    and con (strm, obj) = (case obj
	   of PT.MarkCon m => con (mark (strm, m))
	    | PT.Con(id, NONE) => (
		prNode (strm, "Con");
		nest strm (fn strm => (
		  prId (strm, id);
		  prNode (strm, "<NONE>"))))
	    | PT.Con(id, SOME ty) => (
		prNode (strm, "Con");
		nest strm (fn strm => (
		  prId (strm, id);
		  typ (strm, ty))))
	  (* end case *))

    and bind (strm, obj) = (case obj
	   of PT.MarkBind m => bind (mark (strm, m))
	    | PT.FunBind(id, params, ty, e) => (
		prNode (strm, "FunBind");
		nest strm (fn strm => (
		  prId (strm, id);
		  prList param (strm, params);
		  typ (strm, ty);
		  exp (strm, e))))
	    | PT.ValBind(p, NONE, e) => (
		prNode (strm, "ValBind");
		nest strm (fn strm => (
		  pat (strm, p);
		  prNode (strm, "<NONE>");
		  exp (strm, e))))
	    | PT.ValBind(p, SOME ty, e) => (
		prNode (strm, "ValBind");
		nest strm (fn strm => (
		  pat (strm, p);
		  typ (strm, ty);
		  exp (strm, e))))
	    | PT.ExpBind e => (
		prNode (strm, "ExpBind");
		exp (inc strm, e))
	  (* end case *))

  (* print a function parameter *)
    and param (strm, obj) = (case obj
	   of PT.MarkParam m => param (mark (strm, m))
	    | PT.TyParam tvs => (
		prNode (strm, "TyParam");
		prList prId (inc strm, tvs))
	    | PT.ValParam(id, ty) => (
		prNode (strm, "ValParam");
		nest strm (fn strm => (
		  prId (strm, id);
		  typ (strm, ty))))
	  (* end case *))

  (* print an expression *)
    and exp (strm, obj) = (case obj
	   of PT.MarkExp m => exp (mark (strm, m))
	    | PT.IfExp(e1, e2, e3) => (
		prNode (strm, "IfExp");
		nest strm (fn strm => (
		  exp (strm, e1);
		  exp (strm, e2);
		  exp (strm, e3))))
	    | PT.BinExp(e1, rator, e2) => (
		prNode (strm, "BinExp");
		nest strm (fn strm => (
		  exp (strm, e1);
		  prId (strm, rator);
		  exp (strm, e2))))
	    | PT.AppExp(e1, e2) => (
		prNode (strm, "AppExp");
		nest strm (fn strm => (
		  exp (strm, e1);
		  exp (strm, e2))))
	    | PT.TyAppExp(e, tys) => (
		prNode (strm, "TyAppExp");
		nest strm (fn strm => (
		  exp (strm, e);
		  prList typ (strm, tys))))
	    | PT.VarExp id => (
		prNode (strm, "VarExp");
		prId (inc strm, id))
	    | PT.ConExp id => (
		prNode (strm, "ConExp");
		prId (inc strm, id))
	    | PT.IntExp n => (
		prNode (strm, "IntExp");
		prStr (inc strm, IntInf.toString n))
	    | PT.StrExp s => (
		prNode (strm, "StrExp");
		prStr (inc strm, concat["\"", String.toString s, "\""]))
	    | PT.TupleExp es => (
		prNode (strm, "TupleExp");
		prList exp (inc strm, es))
	    | PT.CaseExp(e, matches) => (
		prNode (strm, "CaseExp");
		nest strm (fn strm => (
		  exp (strm, e);
		  prList case_match (strm, matches))))
	    | PT.LetExp body => (
		prNode (strm, "LetExp");
		scope (inc strm, body))
	  (* end case *))

  (* print a local scope *)
    and scope (strm, obj) = (case obj
	   of PT.MarkScope m => scope (mark (strm, m))
	    | PT.Scope(binds, e) => (
		prNode (strm, "Scope");
		nest strm (fn strm => (
		  prList bind (strm, binds);
		  exp (strm, e))))
	  (* end case *))

  (* print a pattern-match case rule *)
    and case_match (strm, obj) = (case obj
	   of PT.MarkCaseMatch m => case_match (mark (strm, m))
	    | PT.CaseMatch(p, body) => (
		prNode (strm, "CaseMatch");
		nest strm (fn strm => (
		  pat (strm, p);
		  scope (strm, body))))
	  (* end case *))

  (* print a pattern *)
    and pat (strm, obj) = (case obj
	   of PT.MarkPat m => pat (mark (strm, m))
	    | PT.VarPat id => (
		prNode (strm, "VarPat");
		prId (inc strm, id))
	    | PT.ConPat(id, NONE) => (
		prNode (strm, "ConPat");
		nest strm (fn strm => (
		  prId (strm, id);
		  prNode (strm, "<NONE>"))))
	    | PT.ConPat(id, SOME p) => (
		prNode (strm, "ConPat");
		nest strm (fn strm => (
		  prId (strm, id);
		  pat (strm, p))))
	    | PT.TuplePat pats => (
		prNode (strm, "TuplePat");
		prList pat (inc strm, pats))
	    | PT.WildPat => prNode (strm, "WildPat")
	  (* end case *))

  end


