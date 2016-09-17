(* main.sml
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 *)

structure Main : sig

    type flags = {
	printPT : bool,		(* if true, print the parse tree with location info to a file *)
	printAST : bool,	(* if true, print the AST and simplified AST to a file *)
	printSIR : bool,	(* if true, print the Simple IR to a file *)
	printCode : bool	(* if true, print the bytecode to a file *)
      }

    val compileFile : flags -> string -> unit

  end = struct

    type flags = {
	printPT : bool,		(* if true, print the parse tree with location info to a file *)
	printAST : bool,	(* if true, print the AST and simplified AST to a file *)
	printSIR : bool,	(* if true, print the Simple IR to a file *)
	printCode : bool	(* if true, print the bytecode to a file *)
      }

    fun printIR (mkStrm, printFn, ext) = let
	  fun pr (false, _, _, prog) = prog
	    | pr (true, info, stem, prog) = let
		val outS = TextIO.openOut(OS.Path.joinBaseExt{base=stem, ext=SOME ext})
		val strm = mkStrm (outS, info)
		in
		  printFn (strm, prog) handle ex => (TextIO.closeOut outS; raise ex);
		  TextIO.closeOut outS;
		  prog
		end
	  in
	    pr
	  end

    val prParseTree = let
	  fun mkStrm (outS, errStrm) = PrintParseTree.new {
		  outS = outS,
		  errS = errStrm,
		  showMarks = true
		}
	  in
	    printIR (mkStrm, PrintParseTree.program, "pt")
	  end

    val prAST = let
	  fun mkStrm (outS, ()) = PrintAST.new {
		  outS = outS,
		  showStamps = true
		}
	  in
	    printIR (mkStrm, PrintAST.program, "ast")
	  end

  (* parse and type check the FLang program in the given file. *)
    fun compileFile {printPT, printAST, printSIR, printCode} file = let
	(* get the stem of the file *)
	  val stem = (case OS.Path.splitBaseExt file
		 of {base, ext=SOME"fl"} => if OS.Path.file base <> ""
		      then base
		      else raise Fail "bogus file name"
		  | _ => file
		(* end case *))
	  val (errStrm, yield) = Parser.parseFile file
	  in
	    Error.report (TextIO.stdOut, errStrm);
	    if not (Error.anyErrors errStrm)
	      then (case yield
		 of NONE => ()
		  | SOME prog => let
		      val prog = prParseTree (printPT, errStrm, stem, prog)
		      val ast = TypeChecker.chkProgram (errStrm, prog)
		      in
			Error.report (TextIO.stdOut, errStrm);
			if not(Error.anyErrors errStrm)
			  then let
			    val _ = ignore (prAST (printAST, (), stem, ast))
			    in
			      Backend.compile {
				  printAST = printAST,
				  printSIR = printSIR,
				  printCode = printCode
				} (stem, ast)
			    end
			  else ()
		      end
		(* end case *))
	      else ()
	  end

  end
