(* runtime-api.sml
 *
 * CMSC22610 Winter 2015 Sample code.
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 *
 * The CCall instructions to call the runtime functions.
 *)

structure RuntimeAPI : sig

  (* run time function ID *)
    type t

  (* emit a call to a C function to the code stream.  This function does both
   * the C function lookup and instruction emission; i.e., the call
   *
   *	emitCCall (cs, f)
   *
   * is the same as
   *
   *	Emit.emit cs (Instructions.ccall(Emit.c_function(cs, f)))
   *)
    val emitCCall : Emit.code_stream * t -> unit

  (* available run-time system functions *)
    val arg : t
    val argc : t
    val concat : t
    val fail : t
    val print : t

  end = struct

    type t = string

    fun emitCCall (codeS, t) = Emit.emit codeS (Instructions.ccall(Emit.c_function(codeS, t)))

    val arg = "VM_arg"
    val argc = "VM_argc"
    val concat = "VM_concat"
    val fail = "VM_fail"
    val print = "VM_print"

  end
