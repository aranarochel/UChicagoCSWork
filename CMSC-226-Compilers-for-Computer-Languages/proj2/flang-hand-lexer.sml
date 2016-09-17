(* flang-hand-lexer.sml
 *
 * CMSC 22610 Sample code (Winter 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 *
 * This module wraps a modified hand-written scanner so that it will
 * work with the ML-Antlr parser infrastructure.
 *)

structure FLangLex : sig

    type pos = Error.pos

    type strm

    val streamify : (unit -> string) -> strm

    val getPos : strm -> pos

    val lex : AntlrStreamPos.sourcemap
	  -> (Error.span * string list -> unit)
	     -> strm
		-> FLangTokens.token * Error.span * strm

  end = struct

    type pos = Error.pos

  (* input stream with position tracking *)
    datatype buf
      = B of {
	    pos : pos,		(* start position of buffer *)
	    data : string,	(* data in buffer *)
	    next : buf ref	(* next buffer in chain *)
	  }
      | MORE of (unit -> string)
      | EOF of pos

    datatype strm = S of pos * buf

  (* get the current position of the stream *)
    fun getPos (S(i, B{pos, ...})) = i + pos
      | getPos _ = raise Fail "bogus stream state"

  (* get the next character from the stream *)
    fun getc (S(i, buf as B{pos, data, next})) = let
	  val c = String.sub(data, i)
	  val i' = i+1
	  val strm' = if (i' < size data)
		then S(i', buf)
		else (case !next
		   of MORE getData => let
			val pos' = pos + i' (* start position of next buffer *)
			val next' = (case getData()
			       of "" => EOF pos'
				| s => B{pos=pos', data=s, next=ref(MORE getData)}
			      (* end case *))
			in
			  next := next';
			  S(0, next')
			end
		    | b => S(0, b)
		  (* end case *))
	  in
	    SOME(c, strm')
	  end
      | getc (S(0, EOF _)) = NONE
      | getc _ = raise Fail "bogus stream state"

  (* convert a function for getting blocks of text to a lexer stream *)
    fun streamify getData = (case getData()
	   of "" => S(0, EOF 0)
	    | s => S(0, B{pos=0, data=s, next=ref(MORE getData)})
	  (* end case *))

    fun lex srcMap lexErr = let
	(* reader that tracks newlines *)
	  fun getc' strm = (case getc strm
		 of result as SOME(#"\n", strm') => (
		      AntlrStreamPos.markNewLine srcMap (getPos strm');
		      result)
		  | result => result
		(* end case *))
	  val lexer = FLangHandScanner.lexer {
		  getPos = getPos,
		  forward = AntlrStreamPos.forward,
		  reportErrorAt = fn (pos, msg) => lexErr((pos, pos), [msg])
		} getc'
	  in
	    fn strm => (case lexer strm
		 of NONE => let
		      val p = getPos strm
		      in
			(FLangTokens.EOF, (p, p), strm)
		      end
		  | SOME((tok, span), strm) => (tok, span, strm)
		(* end case *))
	  end

  end
