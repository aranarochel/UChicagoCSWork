(* emit.sml
 *
 * CMSC22610 Winter 2015 Sample code.
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 *)

signature EMIT =
  sig
    type code_stream

  (* create a new code stream; the string is the name of the object file *)
    val openCode : string -> code_stream

  (* dump the contents of the code stream to the specified output stream *)
    val dump : (TextIO.outstream * code_stream) -> unit

  (* emit an instruction to the stream *)
    val emit : code_stream -> Instructions.t -> unit

  (* bind the label to the current location *)
    val defineLabel : code_stream -> Labels.t -> unit

  (* assemble the code and write the object file *)
    val finish : code_stream -> unit

  (* map a C function's name to an index in the C-function table *)
    val c_function : (code_stream * string) -> int

  (* map a string literal to an index in the literal table *)
    val string : (code_stream * string) -> int

  end

structure Emit :> EMIT =
  struct

    structure Rep = CodeRep
    structure W = Word
    structure F = Format

    structure StringTbl = HashTableFn (
      struct
	type hash_key = string
	val hashVal = HashString.hashString
	val sameKey : (string * string) -> bool = (op =)
      end)

    datatype code = datatype Rep.code

    datatype code_stream = CS of {
	file : string,
	addr : int ref,
	code : code list ref,
	nLits : int ref,
	lits : int StringTbl.hash_table,
	nCfuns : int ref,
	cfuns : int StringTbl.hash_table
      }

    fun tableToArray (n : int, tbl : int StringTbl.hash_table) = let
	  val arr = Array.array(n, "")
	  fun ins (s, i) = Array.update(arr, i, s)
	  in
	    StringTbl.appi ins tbl;
	    arr
	  end

  (* create a new code stream; the string is the name of the object file *)
    fun openCode outFile = CS{
	    file = outFile,
	    addr = ref 0,
	    code = ref [],
	    nLits = ref 0,
	    lits = StringTbl.mkTable (32, Fail "lits"),
	    nCfuns = ref 0,
	    cfuns = StringTbl.mkTable (16, Fail "cfuns")
	  }

  (* dump the contents of the code stream to the specified output stream *)
    fun dump (outS, CS{file, code, nLits, lits, nCfuns, cfuns, ...}) = let
	  fun pr s = TextIO.output(outS, s)
	  fun prf (fmt, items) = pr(F.format fmt items)
	  fun prStr (id, s) =
		prf("  [%2d] \"%s\"\n", [F.INT id, F.STR(String.toString s)])
	  fun prIns (Rep.Loc lab, adr) = (
		prf("[%04d]  %s:\n", [F.INT adr, F.STR(Labels.toString lab)]);
		adr)
	    | prIns (Rep.Instr{i, sz}, adr) = (
		prf("[%04d]    %s\n", [F.INT adr, F.STR(Instructions.toString i)]);
		adr+sz)
	    | prIns (Rep.SDI{i, sz}, adr) = (
		prf("[%04d]    %s  /* sz = %d */\n",
		  [F.INT adr, F.STR(Instructions.toString i), F.INT sz]);
		adr+sz)
	  in
	    pr (concat["DUMP: file = ", file, "\n"]);
	    prf ("%d Literals\n", [F.INT(!nLits)]);
	    Array.appi prStr (tableToArray(!nLits, lits));
	    prf ("%d C functions\n", [F.INT(!nCfuns)]);
	    Array.appi prStr (tableToArray(!nCfuns, cfuns));
	    pr "Code\n";
	    ignore (List.foldr prIns 0 (!code))
	  end

  (* emit an instruction to the stream *)
    fun emit (CS{addr, code, ...}) = let
	  fun emit' (i as Rep.I{info, param=Rep.Label _}) = let
		val sz = Rep.sizeOf(!addr + #minSz info, i)
		in
		  addr := !addr + sz;
		  if (sz = #maxSz info)
		    then code := Rep.Instr{i=i, sz=sz} :: !code
		    else code := Rep.SDI{i=i, sz=sz} :: !code
		end
	    | emit' i =  let
		val sz = Rep.sizeOf(!addr, i)
		in
		  addr := !addr + sz;
		  code := Rep.Instr{i=i, sz=sz} :: !code
		end
	  in
	    emit'
	  end

  (* bind the label to the current location *)
    fun defineLabel (CS{addr, code, ...}) (lab as Rep.L{loc, name}) = (
	  if (!loc >= 0)
	    then raise Fail(name ^ " already defined")
	    else ();
	  loc := !addr;
	  code := Loc lab :: !code)

  (* map a C function's name to an index in the C-function table *)
    fun c_function (CS{nCfuns, cfuns, ...}, name) = (case StringTbl.find cfuns name
	   of NONE => let
		val id = !nCfuns
		in
		  StringTbl.insert cfuns (name, id);
		  nCfuns := id+1;
		  id
		end
	    | SOME id => id
	  (* end case *))

  (* map a string literal to an index in the literal table *)
    fun string (CS{nLits, lits, ...}, name) = (case StringTbl.find lits name
	   of NONE => let
		val id = !nLits
		in
		  StringTbl.insert lits (name, id);
		  nLits := id+1;
		  id
		end
	    | SOME id => id
	  (* end case *))

  (* resolve span-dependent instructions in the code stream *)
    fun assemble instrs = let
	(* update the addresses of locations, while reversing the list *)
	  fun updateLocs (adr, [], instrs) = instrs
	    | updateLocs (adr, i::r, instrs) = (case i
		 of Loc(Rep.L{loc, ...}) => (loc := adr; updateLocs(adr, r, i::instrs))
		  | Instr{sz, ...} => updateLocs(adr-sz, r, i::instrs)
		  | SDI{sz, ...} => updateLocs(adr-sz, r, i::instrs)
		(* end case *))
	(* fix the sizes of SDIs for when we have reached a fixed point *)
	  fun fixSizes ([], instrs) = instrs
	    | fixSizes (i::r, instrs) = (case i
		 of SDI{sz, i} => fixSizes (r, Instr{sz=sz, i=i}::instrs)
		  | _ => fixSizes (r, i::instrs)
		(* end case *))
	  fun resolve (false, true, adr, [], instrs) =
		{sz = adr, code = updateLocs(adr, instrs, [])}
	    | resolve (true, true, adr, [], instrs) =
		resolve (false, false, 0, updateLocs(adr, instrs, []), [])
	    | resolve (false, false, adr, [], instrs) =
		{sz = adr, code = updateLocs(adr, instrs, [])}
	    | resolve (true, false, adr, [], instrs) =
		{sz = adr, code = fixSizes (instrs, [])}
	    | resolve (anySDIs, changed, adr, instr::r, instrs) = (
		case instr
		 of Loc _ => resolve(anySDIs, changed, adr, r, instr::instrs)
		  | Instr{sz, ...} => resolve(anySDIs, changed, adr+sz, r, instr::instrs)
		  | SDI{sz, i as Rep.I{info, ...}} => let
		      val sz' = Rep.sizeOf(adr+sz, i)
		      val changed = changed orelse (sz <> sz')
		      in
			if (sz' <> sz)
			  then if sz' = #maxSz info
			    then resolve(anySDIs, true, adr+sz', r, Instr{sz=sz', i=i}::instrs)
			    else resolve(true, true, adr+sz', r, SDI{sz=sz', i=i}::instrs)
			  else resolve(true, changed, adr+sz, r, instr::instrs)
		      end
		(* end case *))
	  in
	    resolve (false, false, 0, instrs, [])
	  end

  (* assemble the code and write the object file *)
    fun finish (CS{file, addr, code, nLits, lits, nCfuns, cfuns}) = let
	  val {sz, code} = assemble (List.rev (!code)) before code := []
	  val litTbl = tableToArray (!nLits, lits)
	  val cfunTbl = tableToArray (!nCfuns, cfuns)
	  val outS = BinIO.openOut file
	(* emit an unsigned byte to the object file *)
	  fun emitByte b = BinIO.output1(outS, Word8.fromLargeWord(Word.toLargeWord b))
	(* emit a signed byte to the object file *)
	  fun emitS8 i = emitByte(W.fromInt i)
	(* emit an unsigned 16-bit quantity *)
	  fun emitU16 w = (emitByte (W.>>(w, 0w8)); emitByte w)
	(* emit a signed 16-bit quantity *)
	  fun emitS16 i = emitU16(W.fromInt i)
	(* emit a signed 32-bit quantity *)
	  fun emitS32 i = let
		val w = W.fromInt i
		in
		  emitU16 (W.>>(w, 0w16));
		  emitU16 w
		end
	(* emit a string with its length *)
	  fun emitS s = let
		val n = size s
		in
		  if (65535 < n)
		    then raise Fail "" else ();
		  emitU16(W.fromInt n);
		  BinIO.output (outS, Byte.stringToBytes s)
		end
	(* emit an instruction *)
	  fun emitI (Rep.Loc _, adr) = adr
	    | emitI (Rep.Instr{i=Rep.I{info as {opc, ...}, param}, sz}, adr) = let
		val len = if (sz = 5) then 0w3 else W.fromInt(sz-1)
		fun emitParam (1, _) = ()
		  | emitParam (2, Rep.SignedInt i) = emitS8 i
		  | emitParam (3, Rep.SignedInt i) = emitS16 i
		  | emitParam (5, Rep.SignedInt i) = emitS32 i
		  | emitParam (2, Rep.UnsignedInt n) = emitByte n
		  | emitParam (3, Rep.UnsignedInt n) = emitU16 n
		  | emitParam (sz, Rep.Label(Rep.L{name, loc})) = (
		      if (!loc < 0)
			then raise Fail(concat[
			    "undefined label: ", #name info, "(", name, ")"
			  ])
			else ();
		      emitParam (sz, Rep.SignedInt(!loc - (adr+sz))))
		  | emitParam _ = raise Fail "bogus instruction format"
		in
		  emitByte (W.orb(W.<<(len, 0w6), opc));
		  emitParam (sz, param);
		  adr + sz
		end
	    | emitI _ = raise Fail "unexpected SDI"
	(* object file magic header; this string must agree with vm/load.c
	 * and vm/version.h, and be padded out to 12 characters.
	 *)
	  val magic = StringCvt.padRight #" " 12 "vm 2.2"
	  in
	  (* output header *)
	    BinIO.output (outS, Byte.stringToBytes magic);
	    emitU16 (W.fromInt (!nLits));
	    emitU16 (W.fromInt (!nCfuns));
	    emitU16 (W.fromInt sz);
	  (* output literals *)
	    Array.app emitS litTbl;
	  (* output C functions *)
	    Array.app emitS cfunTbl;
	  (* output code *)
	    List.foldl emitI 0 code;
	    BinIO.closeOut outS
	  end

  end
