(* flang.lex
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 *
 * CMSC 22610 Winter 2015
 * 
 * ml-ulex specification for FLang
 *)

%name FLangLex;

%arg (lexErr);

%defs(

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
);

%states INITIAL STRING COMMENT;

%let ucLetter = [A-Z];
%let lcLetter = [a-z];
%let letter = {ucLetter}|{lcLetter};
%let dig = [0-9];
%let idchar = {letter}|{dig}|"_"|"'";
%let ucId = {ucLetter}{idchar}*;
%let lcId = {lcLetter}{idchar}*;
%let esc = "\\"[abfnrtv\\\"]|"\\"{dig}{dig}{dig};
%let sgood = [\032-\126]&[^\"\\]; (* sgood means "characters good inside strings" *)
%let ws = " "|[\t\n\v\f\r];

<INITIAL> "("		=> (T.LP);
<INITIAL> ")"		=> (T.RP);
<INITIAL> "["		=> (T.LB);
<INITIAL> "]"		=> (T.RB);
<INITIAL> "{"		=> (T.LCB);
<INITIAL> "}"		=> (T.RCB);
<INITIAL> "=="		=> (T.EQEQ);
<INITIAL> "<="		=> (T.LTEQ);
<INITIAL> "<"		=> (T.LT);
<INITIAL> "::"		=> (T.DCOLON);
<INITIAL> "@"		=> (T.AT);
<INITIAL> "+"		=> (T.PLUS);
<INITIAL> "-"		=> (T.MINUS);
<INITIAL> "*"		=> (T.TIMES);
<INITIAL> "/"		=> (T.DIV);
<INITIAL> "%"		=> (T.MOD);
<INITIAL> "="		=> (T.EQ);
<INITIAL> ","		=> (T.COMMA);
<INITIAL> ";"		=> (T.SEMI);
<INITIAL> ":"		=> (T.COLON);
<INITIAL> "->"		=> (T.ARROW);
<INITIAL> "=>"		=> (T.DARROW);
<INITIAL> "_"		=> (T.WILD);
<INITIAL> {lcId}	=> (idToken yytext);
<INITIAL> {ucId}	=> (T.UID(Atom.atom yytext));
<INITIAL> {dig}+	=> (T.NUMBER(valOf (IntInf.fromString yytext)));
<INITIAL> {ws}		=> (skip ());
<INITIAL> "(*"		=> (YYBEGIN COMMENT; startPos := yypos; depth := 1; skip());
<INITIAL> "\""		=> (YYBEGIN STRING; startPos := yypos; continue());
<INITIAL>.		=> (lexErr((yypos, yypos), ["bad character `", String.toString yytext, "'"]);
			    continue());
<INITIAL> <<EOF>>	=> (T.EOF);

<STRING> {esc}		=> (addStr(valOf(String.fromString yytext)); continue());
<STRING> {sgood}+	=> (addStr yytext; continue());
<STRING> "\""		=> (YYBEGIN INITIAL; mkString());
<STRING> "\\".		=> (lexErr((yypos, yypos), [
				"bad escape character `", String.toString yytext,
				"' in string literal"
			      ]);
			    continue());
<STRING> \n|\r|\r\n	=> (lexErr((!startPos, yypos), ["unclosed string at end of file"]);
			    YYBEGIN INITIAL; mkString());
<STRING> .		=> (lexErr((yypos, yypos), [
				"bad character `", String.toString yytext,
				"' in string literal"
			      ]);
			    continue());
<STRING> <<EOF>>	=> (lexErr((!startPos, yypos), ["unclosed string at end of file"]); T.EOF);

<COMMENT> "(*"		=> (depth := !depth + 1;
			    skip());
<COMMENT> "*)"		=> (depth := !depth - 1;
			    if (!depth = 0) then YYBEGIN INITIAL else ();
			    skip ());
<COMMENT> .|"\n" => (skip ());
<COMMENT> <<EOF>>	=> (lexErr((!startPos, yypos), ["unclosed comment at end of file"]); T.EOF);
