(* parser.sml
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 *
 * CMSC 22610 Sample code (Winter 2015)
 *
 * Combine the parser and lexer to implement the front end.
 *)

structure Parser : sig

  (* parse an FLang program from an input stream using the given error stream to record errors *)
    val parse : TextIO.instream * Error.err_stream -> ParseTree.program option

  (* parse a file, returning an error stream and optional parse tree *)
    val parseFile : string -> Error.err_stream * ParseTree.program option

  end = struct

  (* glue together the lexer and parser *)
    structure FLangParser = FLangParseFn(FLangLex)

  (* error function for lexers *)
    fun lexErr errStrm (span, msg) = Error.errorAt(errStrm, span, msg)

  (* map tokens to strings *)
    fun tokToString (FLangTokens.UID x) = Atom.toString x
      | tokToString (FLangTokens.LID x) = Atom.toString x
      | tokToString (FLangTokens.NUMBER i) = IntInf.toString i
      | tokToString (FLangTokens.STRING s) = concat["\"", String.toString s, "\""]
      | tokToString tok = FLangTokens.toString tok

  (* error function for parsers *)
    val parseErr = Error.parseError tokToString

  (* parse a file, returning a parse tree *)
    fun parse (inStrm, errStrm) = let
	  fun get () = TextIO.input inStrm
	  val lexer = FLangLex.lex (Error.sourceMap errStrm) (lexErr errStrm)
	  val (yield, _, errs) = FLangParser.parse lexer (FLangLex.streamify get)
	  in
	    List.app (parseErr errStrm) errs;
	    yield
	  end

  (* parse a file, returning a parse tree *)
    fun parseFile filename = let
	  val errStrm = Error.mkErrStream filename
	  in
	    if OS.FileSys.access (filename, [OS.FileSys.A_READ])
	      then let
		val inStrm = TextIO.openIn filename
		val yield = parse (inStrm, errStrm)
		in
		  TextIO.closeIn inStrm;
		  (errStrm, yield)
		end
		  handle ex => (
		    Error.error (errStrm, ["Internal error: uncaught exception ", exnMessage ex]);
		    (errStrm, NONE))
	      else (
		Error.error (errStrm, ["source file \"", String.toString filename, "\" not found\n"]);
		(errStrm, NONE))
	  end

  end
