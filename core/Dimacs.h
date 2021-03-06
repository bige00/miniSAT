/****************************************************************************************[Dimacs.h]
**************************************************************************************************/

#ifndef Minisat_Dimacs_h
#define Minisat_Dimacs_h

#include <stdio.h>

#include "../utils/ParseUtils.h"
#include "../core/SolverTypes.h"

namespace Minisat
{

//=================================================================================================
// DIMACS Parser:

template<class B, class Solver>
static void readClause(B& in, Solver& S, vec<Lit>& lits)
{
    int     parsed_lit, var;
    lits.clear();
    for (;;){
		parsed_lit = parseInt(in);// read an int
		if (parsed_lit == 0)
			break;  //construct a clause
		var = abs(parsed_lit) - 1;
		int tes=S.nVars();//current number of variable
		while (var >= S.nVars())
			S.newVar();
		lits.push((parsed_lit > 0) ? mkLit(var) : ~mkLit(var));
	}
}

//将in文件的内容写到Solver中
template<class B, class Solver>
static void parse_DIMACS_main(B& in, Solver& S)
{
    vec<Lit> lits;
    int vars    = 0;
	int clauses = 0;
	int cnt = 0;
	for (;;)
	{
		skipWhitespace(in);
		if (*in == EOF)
			break;
		else if (*in == 'p')
		{
			if (eagerMatch(in, "p cnf"))
			{
				vars = parseInt(in);
				clauses = parseInt(in);
				// SATRACE'06 hack
				// if (clauses > 4000000)
				//     S.eliminate(true);
			}
			else
			{
				printf("PARSE ERROR! Unexpected char: %c\n", *in), exit(3);
			}
		} else if (*in == 'c' || *in == 'p')
			skipLine(in);
		else {
			cnt++;//number of clauses
			readClause(in, S, lits);
			S.addClause_(lits);
		}
	}
	//int tes = S.nVars();
	if (S.nVars() != vars)
		fprintf(stderr,
				"WARNING! DIMACS header mismatch: wrong number of variables.\n");
	if (cnt != clauses)
		fprintf(stderr,
				"WARNING! DIMACS header mismatch: wrong number of clauses.\n");
}

// Inserts problem into solver.
//
template<class Solver>
static void parse_DIMACS(gzFile input_stream, Solver& S)
{
    StreamBuffer in(input_stream);
    parse_DIMACS_main(in, S); }

//=================================================================================================
}

#endif
