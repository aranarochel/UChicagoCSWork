(* context.sml
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 *)

structure Context : sig

    datatype ty_def
      = TyDef of TyVar.t list * Type.ty
      | TyCon of TyCon.t

  (* the different kinds of environments used to check FLang programs *)
    type tyvar_env = TyVar.t AtomMap.map
    type tycon_env = ty_def AtomMap.map
    type dcon_env = DataCon.t AtomMap.map
    type var_env = Var.t AtomMap.map

  (* the contaxt captures both the syntactic and semantic context needed to type
   * check FLang programs.
   *)
    type context

  (* create a new context initialized to the FLang basis environments
   * an empty span from an error stream
   *)
    val new : Error.err_stream -> context

  (* report an error using the context's error stream and span *)
    val error : context * string list -> unit

  (* get the components of the context *)
    val errStrmOf : context -> Error.err_stream

  (* handle a marked tree by updating the context's span *)
    val mark : context * 'a Error.mark -> (context * 'a)

  (* return the span of the context *)
    val spanOf : context -> Error.span

  (* merge a type-variable environment into a context. The environment's bindings
   * override the context's bindings.
   *)
    val mergeTVEnv : context * tyvar_env -> context

    val findTyVar : context * Atom.atom -> TyVar.t option

  (* add a type-constructor binding to the context *)
    val bindTyCon : context * Atom.atom * ty_def -> context

    val findTyCon : context * Atom.atom -> ty_def option

  (* merge a data-constructor-variable environment into a context. The
   * environment's bindings override the context's bindings.
   *)
    val mergeDCEnv : context * dcon_env -> context

    val findDCon : context * Atom.atom -> DataCon.t option

  (* merge a value-variable environment into a context. The environment's bindings
   * override the context's bindings.
   *)
    val mergeVEnv : context * var_env -> context

    val findVar : context * Atom.atom -> Var.t option

  end = struct

    structure AMap = AtomMap

    datatype ty_def
      = TyDef of TyVar.t list * Type.ty
      | TyCon of TyCon.t

  (* the different kinds of environments used to check FLang programs *)
    type tyvar_env = TyVar.t AMap.map
    type tycon_env = ty_def AMap.map
    type dcon_env = DataCon.t AMap.map
    type var_env = Var.t AMap.map

    datatype context = Cxt of {
	errStrm : Error.err_stream,	(* error stream for reporting errors *)
	span : Error.span,		(* span containing the term being checked *)
	tvEnv : tyvar_env,		(* type-variable environment *)
	tcEnv : tycon_env,		(* type-constructor environment *)
	dcEnv : dcon_env,		(* data-constructor environment *)
	vEnv : var_env			(* value-variable environment *)
      }

    fun error (Cxt{errStrm, span, ...}, msg) = Error.errorAt(errStrm, span, msg)

    fun errStrmOf (Cxt{errStrm, ...}) = errStrm

  (* handle a marked tree by updating the context's span *)
    fun mark (Cxt{errStrm, tvEnv, tcEnv, dcEnv, vEnv, ...}, {span, tree}) =
	  (Cxt{
	      errStrm=errStrm, span=span,
	      tvEnv=tvEnv, tcEnv=tcEnv, dcEnv=dcEnv, vEnv=vEnv
	    }, tree)

    fun spanOf (Cxt{span, ...}) = span

    fun mergeTVEnv (Cxt stuff, tvEnv) = Cxt{
	    errStrm = #errStrm stuff,
	    span = #span stuff,
	    tvEnv = AMap.unionWith #2 (#tvEnv stuff, tvEnv),
	    tcEnv = #tcEnv stuff,
	    dcEnv = #dcEnv stuff,
	    vEnv = #vEnv stuff
	  }

    fun bindTyCon (Cxt stuff, tyc, def) = Cxt{
	    errStrm = #errStrm stuff,
	    span = #span stuff,
	    tvEnv = #tvEnv stuff,
	    tcEnv = AMap.insert (#tcEnv stuff, tyc, def),
	    dcEnv = #dcEnv stuff,
	    vEnv = #vEnv stuff
	  }

    fun mergeDCEnv (Cxt stuff, dcEnv) = Cxt{
	    errStrm = #errStrm stuff,
	    span = #span stuff,
	    tvEnv = #tvEnv stuff,
	    tcEnv = #tcEnv stuff,
	    dcEnv = AMap.unionWith #2 (#dcEnv stuff, dcEnv),
	    vEnv = #vEnv stuff
	  }

    fun mergeVEnv (Cxt stuff, vEnv) = Cxt{
	    errStrm = #errStrm stuff,
	    span = #span stuff,
	    tvEnv = #tvEnv stuff,
	    tcEnv = #tcEnv stuff,
	    dcEnv = #dcEnv stuff,
	    vEnv = AMap.unionWith #2 (#vEnv stuff, vEnv)
	  }

    fun findTyVar (Cxt{tvEnv, ...}, id) = AMap.find(tvEnv, id)
    fun findTyCon (Cxt{tcEnv, ...}, id) = AMap.find(tcEnv, id)
    fun findDCon (Cxt{dcEnv, ...}, id) = AMap.find(dcEnv, id)
    fun findVar (Cxt{vEnv, ...}, id) = AMap.find(vEnv, id)

    structure BN = BasisNames
    structure B = Basis

    fun new errStrm = Cxt{
	    errStrm = errStrm,
	    span = (0, 0),
	    tvEnv = AMap.empty,
	    tcEnv = List.foldl AMap.insert' AMap.empty [
		(BN.bool,	TyCon B.boolTyc),
		(BN.int,	TyCon B.intTyc),
		(BN.list,	TyCon B.listTyc),
		(BN.string,	TyCon B.stringTyc),
		(BN.unit,	TyCon B.unitTyc)
	      ],
	    dcEnv =  List.foldl AMap.insert' AMap.empty [
		(BN.boolTrue,	B.boolTrue),
		(BN.boolFalse,	B.boolFalse),
		(BN.listCons,	B.listCons),
		(BN.listNil,	B.listNil),
		(BN.unitUnit,	B.unitUnit)
	      ],
	    vEnv =  List.foldl AMap.insert' AMap.empty [
	      (* primitive operators *)
		(BN.eql,	B.eql),
		(BN.lte,	B.lte),
		(BN.lt,		B.lt),
		(BN.concat,	B.concat),
		(BN.add,	B.add),
		(BN.subtract,	B.subtract),
		(BN.multiply,	B.multiply),
		(BN.divide,	B.divide),
		(BN.modulo,	B.modulo),
	      (* predefine functions *)
		(BN.argc,	B.argc),
		(BN.arg,	B.arg),
		(BN.fail,	B.fail),
		(BN.ignore,	B.ignore),
		(BN.neg,	B.neg),
		(BN.not,	B.not),
		(BN.print,	B.print),
		(BN.size,	B.size),
		(BN.sub,	B.sub)
	      ]
	  }

  end
