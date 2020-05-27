datatype pattern = Wildcard | Variable of string | UnitP
			     | ConstP of int | TupleP of pattern list
				 | ConstructorP of string * pattern
				 
datatype valu = Const of int | Unit | Tuple of valu list
			  | Constructor of string * valu

fun check_pat pat =
let fun to_list pat =
		case pat of
			Variable s => [s]
		  | TupleP pl => List.foldl(fn (p, append) => (to_list p)@append) [] pl
		  | ConstructorP (s, p) => to_list p
		  | _ => []
	  
	fun no_repeats slist =
		case slist of
			[] => true
		  | s::ss => if List.exists(fn (x) => x = s) ss
					 then false
					 else no_repeats ss
in
	no_repeats(to_list pat)
end


fun match (valu, pat) =
	case (valu, pat) of
		(_, Wildcard)         => SOME []
	  | (v, Variable s)       => SOME [(s, v)]
	  | (Unit, UnitP)         => SOME []
	  | (Const n1, ConstP n2) => if n1 = n2 then SOME [] else NONE
	  | (Tuple vs, TupleP ps) =>
            if List.length vs = List.length ps
            then if List.length vs = List.length 
                    (List.filter(fn (vp) => isSome(match(#1 vp, #2 vp))) 
					(ListPair.zip(vs, ps)))
                 then SOME (List.foldl(fn (x, app) => case match x of
                                                        SOME lst => lst @ app) 
                            [] (ListPair.zip(vs, ps)))
                 else NONE
            else NONE
	  | (Constructor (s1, v), ConstructorP (s2, p)) => 
			if s1 = s2 then match (v, p) else NONE
	  | _ => NONE


type name = string
datatype RSP = ROCK
			 | SCISSORS
			 | PAPER
			 
datatype 'a strategy = Cons of 'a * (unit -> 'a strategy)
datatype tournament = PLAYER of name * (RSP strategy ref)
					| MATCH of tournament * tournament

fun onlyOne(one:RSP) = Cons(one, fn() => onlyOne(one))

fun alterTwo(one:RSP, two:RSP) = Cons(one, fn() => alterTwo(two, one))

fun alterThree(one:RSP, two:RSP, three:RSP) = 
	Cons(one,fn() => alterThree(two, three, one))

val r = onlyOne(ROCK)
val s = onlyOne(SCISSORS)
val p = onlyOne(PAPER)
val rp = alterTwo(ROCK, PAPER)
val sr = alterTwo(SCISSORS, ROCK)
val ps = alterTwo(PAPER, SCISSORS)
val srp = alterThree(SCISSORS, ROCK, PAPER)

(*
val r = onlyOne(ROCK)
val s = onlyOne(SCISSORS)
val p = onlyOne(PAPER)

val rp = alterTwo(ROCK, PAPER)
val pr = alterTwo(PAPER, ROCK)

val sr = alterTwo(SCISSORS, ROCK)
val rs = alterTwo(ROCK, SCISSORS)

val ps = alterTwo(PAPER, SCISSORS)
val sp = alterTwo(SCISSORS, PAPER)

val srp = alterThree(SCISSORS, ROCK, PAPER)
val rps = alterThree(ROCK, PAPER, SCISSORS)
val psr = alterThree(PAPER, SCISSORS, ROCK)

val rsp = alterThree(ROCK, SCISSORS, PAPER)
val prs = alterThree(PAPER, ROCK, SCISSORS)
val spr = alterThree(SCISSORS, PAPER, ROCK)
*)

fun next(strategyRef) =
	let val Cons(rsp, func) = !strategyRef in
		strategyRef := func();
		rsp
	end

fun check_strategy (PLAYER p) =
	PLAYER p	

fun whosWinner(t) = 
    let fun leftWin(strategy1, strategy2) =
        let val s1 = next(strategy1)
            val s2 = next(strategy2)
        in
            case (s1, s2) of
                (ROCK, SCISSORS)  => true
              | (ROCK, PAPER)     => false
              | (SCISSORS, PAPER) => true
              | (SCISSORS, ROCK)  => false
              | (PAPER, ROCK)     => true
              | (PAPER, SCISSORS) => false
              | _                 => leftWin(strategy1, strategy2)
        end
    in
        case t of
            PLAYER p1 => PLAYER p1
          | MATCH(PLAYER(n1, s1), PLAYER(n2, s2)) =>
                if leftWin(s1, s2)
                then PLAYER(n1, s1)
                else PLAYER(n2, s2)
          | MATCH(t1, t2) => 
                whosWinner(MATCH(whosWinner(t1), whosWinner(t2)))
    end