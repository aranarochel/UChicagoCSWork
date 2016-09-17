structure Derivation : DERIVATION = struct

  fun draw (sourceCode, optFilename) = let
    (* note: you are free to restructure this function however 
     * you like, but continue to respect the signature 
     *)
    val toks = Lex.lex sourceCode
    val ast  = Parse.parse toks
    val optOutfile = 
     (case optFilename
       of NONE   => NONE
	| SOME f => SOME (TextIO.openOut f))
    val println =
     (case optOutfile
       of NONE     => (fn s => (TextIO.print s; 
				TextIO.print "\n"))
	| SOME out => (fn s => (TextIO.output (out, s);
				TextIO.output (out, "\n")))
     (* end case *))
    val _ = List.app println 
		     ["<html>",
		      "<head>",
		      "<style>",
		      "th { text-align : right; }",
		      "</style>",
		      "</head>",
		      "<body>",
		      "<p>[your derivation tree here]</p>",
		      "<p>",
		      "<table>",
		      "<tr><th>code</th><td><code>" ^ sourceCode ^ "</code></td></tr>",
		      "<tr><th>parsed code</th><td><code>" ^ AST.tos ast ^ "</code></td></tr>",
		      "<tr><th>unparsed</th><td><code>" ^ AST.unparse ast ^ "</code></td></tr>",
		      "</table>",
		      "</p>",
		      "</body>",
		      "</html>"]

    in
      case optOutfile
       of SOME out => TextIO.closeOut out
	| NONE     => ()
    end

end
