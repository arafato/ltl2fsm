
class NBA
	: public Automaton
{
public:
	NBA(Representation * const rep);
	
};



int main()
{
	Nba_Generator * const nba_generator(new XYZ_NBA_Generator);
	Nfa_Generator * const nfa_generator(new XYZ_NFA_Generator);
	Code_Generator * const code_generator(new CPP_Code_Generator);

	// NBA is a nondeterministic buechi automaton -- which is derived
	// from automaton and which contains as a field the Representation
	Nondet_Buechi_Automonton * const nba(nba_generator->translate("[]<>p"));
	Nondet_Finite_Automonton * const nfa(nfa_generator->translate(nba));
	
	code_generator->generate(nfa, ::std::cout);
	
	delete ... ;
	

	return 0;
}
