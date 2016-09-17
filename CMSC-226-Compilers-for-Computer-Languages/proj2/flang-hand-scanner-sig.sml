(* flang-hand-scanner-sig.sml
 *
 * CMSC 22610 Sample code (Winter 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 *
 * The signature of a modified Project-1 scanner.
 *)

signature FLANG_HAND_SCANNER =
  sig

    val lexer : {
            getPos : 'strm -> Error.pos,
            forward : Error.pos * int -> Error.pos,
            reportErrorAt : Error.pos * string -> unit
          } -> (char, 'strm) StringCvt.reader
            -> (FLangTokens.token * Error.span, 'strm) StringCvt.reader

  end
