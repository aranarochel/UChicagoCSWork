(* flang-tokens.sml
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 *
 * Signature and implementation of Flang tokens.
 *)

signature FLANG_TOKENS =
  sig

    datatype token
      = KW_case		        (* "case" *)
      | KW_con		        (* "con" *)
      | KW_data		        (* "data" *)
      | KW_else		        (* "else" *)
      | KW_end		        (* "end" *)
      | KW_fun		        (* "fun" *)
      | KW_if		        (* "if" *)
      | KW_let		        (* "let" *)
      | KW_of		        (* "of" *)
      | KW_then		        (* "then" *)
      | KW_type		        (* "type" *)
      | KW_with		        (* "with" *)
      | LP		        (* "(" *)
      | RP		        (* ")" *)
      | LB		        (* "[" *)
      | RB		        (* "]" *)
      | LCB                     (* "{" *)
      | RCB                     (* "}" *)
      | EQ                      (* "=" *)
      | EQEQ                    (* "==" *)
      | LTEQ                    (* "<=" *)
      | LT                      (* "<" *)
      | COLON                   (* ":" *)
      | DCOLON                  (* "::" *)
      | AT                      (* "@" *)
      | PLUS                    (* "+" *)
      | MINUS                   (* "-" *)
      | TIMES                   (* "*" *)
      | DIV                     (* "/" *)
      | COMMA                   (* "," *)
      | SEMI                    (* ";" *)
      | ARROW                   (* "->" *)
      | DARROW                  (* "=>" *)
      | WILD                    (* "_" *)
      | UID of Atom.atom	(* upper-case identifiers *)
      | LID of Atom.atom        (* lower-case identifiers *)
      | NUMBER of IntInf.int    (* integer literals *)
      | STRING of string        (* string literals; argument does not
                                 * include enclosing quotes *)

    val toString : token -> string

  end;

structure FLangTokens :> FLANG_TOKENS =
  struct

    datatype token
      = KW_case	 | KW_con | KW_data | KW_else | KW_end | KW_fun | KW_if	| KW_let | KW_of | KW_then | KW_type | KW_with	
      | LP | RP | LB | RB | LCB | RCB
      | EQ | EQEQ | LTEQ | LT 
      | COLON | DCOLON| AT| PLUS| MINUS| TIMES | DIV | COMMA| SEMI | ARROW | DARROW  | WILD
      | UID of Atom.atom
      | LID of Atom.atom
      | NUMBER of IntInf.int
      | STRING of string

    fun toString tok = (case tok
	   of KW_case => "case"
	    | KW_con => "con"
	    | KW_data => "data"
	    | KW_else => "else"
	    | KW_end => "end"
	    | KW_fun => "fun"
	    | KW_if => "if"
	    | KW_let => "let"
	    | KW_of => "of"
	    | KW_then => "then"
	    | KW_type => "type"
	    | KW_with => "with"
	    | LP => "("
	    | RP => ")"
	    | LB => "["
	    | RB => "]"
	    | LCB => "{"
	    | RCB => "}"
	    | EQ => "="
	    | EQEQ => "=="
	    | LTEQ => "<="
	    | LT => "<"
	    | COLON => ":"
	    | DCOLON => "::"
	    | AT => "@"
	    | PLUS => "+"
	    | MINUS => "-"
	    | TIMES => "*"
	    | DIV => "/"
	    | COMMA => ","
	    | SEMI => ";"
	    | ARROW => "->"
	    | DARROW => "=>"
	    | WILD => "_"
	    | UID id => Atom.toString id
	    | LID id => Atom.toString id
	    | NUMBER n => IntInf.toString n
	    | STRING s => String.concat ["\"", String.toString s, "\""]
	  (* end case *))

  end (* FLangTokens *)
