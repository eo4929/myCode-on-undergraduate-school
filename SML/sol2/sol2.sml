(* Problem 1. Simple Eval *)

datatype expr = NUM of int
			  | PLUS of expr * expr
			  | MINUS of expr * expr

datatype formula = TRUE
				 | FALSE
				 | NOT of formula
				 | ANDALSO of formula * formula
				 | ORELSE of formula * formula
				 | IMPLY of formula * formula
				 | LESS of expr * expr

fun eval f =
let fun eval2 e =
	case e of
		NUM i 			=> i
	  | PLUS(e1,e2) 	=> (eval2 e1) + (eval2 e2)
	  | MINUS(e1,e2) 	=> (eval2 e1) - (eval2 e2)
in
	case f of
		TRUE 			 => true
	  | FALSE 			 => false
	  | NOT f1 			 => not (eval f1)
	  | ANDALSO(f1,f2)	 => (eval f1) andalso (eval f2)
	  | ORELSE(f1,f2)	 => (eval f1) orelse (eval f2)
	  | IMPLY(f1,f2) 	 => (not (eval f1)) orelse (eval f2)
	  | LESS(e1,e2) 	 => (eval2 e1) < (eval2 e2)
end



(* Problem 2. Check MetroMap *)
	
type name = string

datatype metro = STATION of name
	   | AREA of name * metro
	   | CONNECT of metro * metro

fun checkMetro met =
let fun check (m : metro, names : string list) =
	case m of
		STATION n 		=> List.exists(fn (x) => x = n) names
	  | AREA(n,m) 	 	=> check(m, n::names)
	  | CONNECT(m1,m2)	=> check(m1, names) andalso check(m2, names)
in
	check(met, [])
end



(* Problem 3. Lazy List *)

(* i *)

datatype 'a lazyList = nullList
					 | cons of 'a * (unit -> 'a lazyList)
					 
fun seq (first, last) =
	if first > last
	then nullList
	else cons(first, fn () => seq(first + 1, last))
	
fun infSeq (first) =
	cons(first, fn () => infSeq(first + 1))
	
fun firstN (lazyListVal, n) =
	if n < 1
	then []
	else
		case lazyListVal of
			nullList 	=> []
		  | cons(a, f)  => a::firstN(f(), n - 1)

fun Nth (lazyListVal, n) =
	if n < 1
	then NONE
	else 
		case (lazyListVal, n) of
			(nullList, _) 	=> NONE
		  | (cons(a, f), 1) => SOME (a)
		  | (cons(a, f), m) => Nth(f(), m - 1)

fun filterMultiples (lazyListVal, n) =
	case lazyListVal of
		nullList => nullList
	  | cons(a, f) => if a mod n <> 0
					  then cons(a, fn () => filterMultiples(f(), n))
					  else filterMultiples(f(), n)
  
(* ii *)

fun sieve (lazyListVal) =
	case lazyListVal of
		nullList => nullList
		| cons(a, f) => cons(a, fn () => sieve(filterMultiples(f(), a)))

fun primes () =
let val sequence = infSeq(2)
in
	sieve(sequence)	
end
