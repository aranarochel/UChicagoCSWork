(* code-gen.sml
 *
 * CMSC22610 Winter 2015 Sample code.
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 *)

signature CODE_GEN =
  sig

  (* given an output code stream and a simplified program, generate code for the VM *)
    val gen : (Emit.code_stream * Simple.program) -> unit

  end

structure CodeGen :> CODE_GEN =
  struct

  	fun isPrim(prim) = (case prim 
  		of Simple.Add  => true
  		 | Simple.Sub => true
			 | Simple.Mul => true
			 | Simple.Div => true
			 | Simple.Mod => true
			 | Simple.Neg => true
			 | Simple.Equ => true
			 | Simple.Less => true
			 | Simple.LessEq => true
			 | Simple.Not => true
			 | Simple.Size => true
			 | Simple.SUB => true
			 | _ => false
			 (*end case*))

  	fun primCode(prim) = (case prim 
  		of Simple.Add  => Instructions.add
  		 | Simple.Sub => Instructions.sub
			 | Simple.Mul => Instructions.mul 
			 | Simple.Div => Instructions.div 
			 | Simple.Mod => Instructions.mod 
			 | Simple.Neg => Instructions.neg 
			 | Simple.Equ => Instructions.equ 
			 | Simple.Less => Instructions.less
			 | Simple.LessEq => Instructions.lesseq
			 | Simple.Not => Instructions.not 
			 | Simple.Size => Instructions.size
			 | Simple.SUB => Instructions.subscript 
			 | _ => raise Fail "never gonna get here"
			 (* end case *))

		fun builtinCode (bIn) = (case bIn
				of Simple.ConC => RuntimeAPI.concat
				 | Simple.Argc => RuntimeAPI.argc
				 | Simple.Arg => RuntimeAPI.arg
				 | Simple.Fail => RuntimeAPI.fail
				 | Simple.Print => RuntimeAPI.print
				 | _ => raise Fail "nope"
				 (* end case *))


  	fun bootstrap codeS = (Emit.emit codeS (Instructions.label(Labels.new "main"));
					  						   Emit.emit codeS (Instructions.alloc 1);
					  						   Emit.emit codeS (Instructions.label(Labels.new "main"));
					  						   Emit.emit codeS Instructions.call;
					  						   Emit.emit codeS Instructions.halt)

  	fun countlocs exp = (case exp
  			of Simple.LetExp(_,_,exp') => 1 + (countlocs exp')
  			 | _ => 0
  			(* end case *))

  	fun indexVars (vars, i) = (case vars
				of [] => []
		 		| v::rest => (v,i)::(indexVars (vars, (i+1)))
		 (* end case *))

		fun getIndex (x, xs) = (case xs
				of [] => 0
				 | (x',i)::xs' => if x = x' then i else getIndex(x,xs')
				(* end case *))


  	fun generateExpCode (ps,gs,exp, l, codeS) = (case exp
  			of Simple.LetExp (x,e1,e2) => let 
  																				val l' = generateExpCode (ps,gs,e1,l, codeS)
  																				val _ = Emit.emit codeS (Instructions.storelocal(~1))
  																				val l'' = generateExpCode (ps,gs,e2,l', codeS)
  																		in
  																				l''
  																		end
  			| Simple.IfExp (e1,e2,e3) => let 
  																				val l' = generateExpCode (ps,gs,e1,l, codeS)
  																				val _ = Emit.emit codeS (Instructions.jmpif(Labels.new (Int.toString l')))
  																				val l'' = (generateExpCode (ps,gs,e3,l', codeS))
  																				val l'' = l'' + 1
  																				val _ = Emit.emit codeS (Instructions.jmp(Labels.new (Int.toString l'')))
  																				val _ = Emit.defineLabel codeS (Labels.new (Int.toString l'))
  																				val l''' = (generateExpCode (ps,gs,e2,l'',codeS))
  																				val l''' = l''' + 1
  																				val _ = Emit.defineLabel codeS (Labels.new (Int.toString l''))
  																	 in
  																	 			l'''
  																	 end
  			| Simple.FAppExp(f, exps, self) 
  					=> (case self
									of Simple.SelfExp(self) =>
										 let 
										 fun loop exps = (case (exps, f)
										 		of ([], Simple.StringExp s) => let 
										 					val _ = Emit.defineLabel codeS (Labels.new s)
										 					val _ = Emit.emit codeS (Instructions.call)
										 				in 
										 					l
										 				end
										 		 | (e::es, _) => let 
										 		 			val _ = generateExpCode(ps,gs,e,l,codeS)
										 		 		in 
										 		 			loop(es)
										 		 		end
										 		 | ([], _) => raise Fail "nu-uh"
										 		 (* end case *))
										 	in 
										 		loop(exps)
										 	end
									 | _ => 
										 let 
											 fun loop exps = (case exps
											 		of [] => let 
											 			  	val l' = generateExpCode (ps,gs,self,l, codeS)
											 			  	val l'' = generateExpCode (ps,gs,f,l', codeS)
											 					val _ = Emit.emit codeS (Instructions.call)
											  			  val _ = Emit.emit codeS (Instructions.loadlocal 2)
							  								val _ = Emit.emit codeS Instructions.popep
									 				in 
											 					l
											 				end
											 		 | e::es => let 
											 		 			val _ = generateExpCode(ps,gs,e,l,codeS)
											 		 		in 
											 		 			loop(es)
											 		 		end
											 		 (* end case *))
											 	in 
											 		loop(exps)
											 	end
								(*end case*))
				| Simple.OpAppExp(prim, exps) => 
						if isPrim(prim)
						then let 
							fun loop exps = (case exps
							 		of [] => let 
							 				val _ = Emit.emit codeS (primCode(prim))
					 				in 
							 					l
							 				end
							 		 | e::es => let 
							 		 			val _ = generateExpCode(ps,gs,e,l,codeS)
							 		 		in 
							 		 			loop(es)
							 		 		end
							 		 (* end case *))
							in
								 loop (exps)
							end
						else let
							fun loop exps = (case exps
							 		of [] => let 
							 				val _ = RuntimeAPI.emitCCall (codeS, (builtinCode(prim)))
					 				in 
							 					l
							 				end
							 		 | e::es => let 
							 		 			val _ = generateExpCode(ps,gs,e,l,codeS)
							 		 		in 
							 		 			loop(es)
							 		 		end
							 		 (* end case *))
							in
									loop (exps)
							end
				| Simple.ComponentExp(exp, i) => let
							val l' = generateExpCode(ps,gs,exp,l,codeS)
							val _ = Emit.emit codeS (Instructions.select(i))
						in l' 
						end
				| Simple.FunExp(f) => let 
							val _ = Emit.defineLabel codeS (Labels.new (f))
						in l 
						end
				| Simple.GlobalVarExp(str, ind) => let
							val index = getIndex(Simple.GlobalVarExp(str,ind), indexVars(gs,1))
							val _ = Emit.emit codeS (Instructions.loadglobal index)
						in l 
						end
				| Simple.LocalVarExp(str, ind) => let 
							val _ = Emit.emit codeS (Instructions.loadlocal(ind))
						in l 
						end
				| Simple.ParameterExp(str) => 
						let val index = getIndex(Simple.ParameterExp(str), indexVars(ps,1))
							  val _ = Emit.emit codeS (Instructions.loadlocal index)
						in 
							 l
						end
				| Simple.SelfExp(exp) => raise Fail "cannot happen, since self only occurs in applications"
				| Simple.NumExp(num) => let 
							val _ = Emit.emit codeS (Instructions.int(num))
						in l end
				| Simple.StringExp(str) => let 
							val _ = Emit.emit codeS (Instructions.literal(Emit.string(codeS,str)))
						in l end
				| Simple.TupleExp(exps) =>
						let 
							  fun loop exps = (case exps
							 		  of [] => let 
							 				          val _ = Emit.emit codeS (Instructions.alloc(List.length exps))
					 				           in 
							 					        l
							 				       end
							 		   | e::es => let 
							 		 			           val _ = generateExpCode(ps,gs,e,l,codeS)
							 		 		          in 
							 		 			           loop(es)
							 		 		          end
							 		 (* end case *))
							in
								 loop (exps)
							end
			  (*end case *))


    fun gen (codeS, prog) = (bootstrap codeS;
    	(case prog 
    	   of Simple.Program defs => 
    	   		let 
    	   				fun lp (defs,l) = (case defs
    	   						of [Simple.Function(f, ps, gs, exp)] => let
    	   											 val _ = Emit.defineLabel codeS (Labels.new f)
    	   									  	 val _ = Emit.emit codeS (Instructions.entry (countlocs exp))
    	   									  	 val _ = Emit.emit codeS (Instructions.loadlocal 2)
    	   										   val _ = Emit.emit codeS Instructions.popep
    	   										   val l' = generateExpCode (ps, gs, exp, l, codeS) 
    	   										   val _ = Emit.emit codeS Instructions.swap
    	   										   val _ = Emit.emit codeS Instructions.pop
    	   										   val _ = Emit.emit codeS (Instructions.storelocal 2)
    	   										in 
    	   												Emit.emit codeS Instructions.ret
    	   										end
    	   						 | Simple.Function(f, ps, gs, exp)::rest => 
    	   									let val _ = Emit.defineLabel codeS (Labels.new f)
    	   									  	val _ = Emit.emit codeS (Instructions.entry (countlocs exp))
    	   									  	val _ = Emit.emit codeS (Instructions.loadlocal 2)
    	   										  val _ = Emit.emit codeS Instructions.popep
    	   										  val l' = generateExpCode (ps, gs, exp, l, codeS)
    	   										  val _ = Emit.emit codeS (Instructions.storelocal 2)
    	   										  val _ = Emit.emit codeS Instructions.ret
    	   									in 
    	   											lp (rest,l'+1)
    	   									end
    	   							| [] => raise Fail "uh-oh"
    	   						(* end case *))
						in
								lp (defs,0) 
						end
				(* end case *))
	)



  end
