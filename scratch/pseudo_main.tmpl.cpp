
template<typename REP>
class Automaton
{
public:
	typedef REP Representation_t;
	typedef typename Representation_t::vertex_descriptor Vertex_Descriptor_t;
public:
	virtual ~Automaton();
	
	virtual Vertex_Descriptor_t initial_state() =0;
};


template<typename BASE>
class Standard_Automonton_Aspect
	: public BASE
{
public:
	typedef typename BASE::Representation_t Representation_t;
protected:
	Standard_Automonton_Aspect()
		: m_representation(new BASE)
	{
	}
	

	inline Representation_t * p_representation()
	{
		return m_representation;
	}
private:
	Representation_t * m_representation;
};



template<typename REP>
class Nondet_Buechi_Automonton
	: public Standard_Automonton_Aspect<Automaton<REP> >
// I want a Automaton<REP> with a standard implemenation of some common mehtods
{
};


int main()
{
	XYZ_NBA_Generator nba_generator;
	NFA_Generator<XYZ_NBA_Generator::Representation_t> nfa_generator;
	CPP_Code_Generator<XYZ_NBA_Generator::Representation_t> code_generator;
	

	//XYZ_NBA_Generator::Representation_t == Graphviz_Representation.

	Nondet_Buechi_Automonton<XYZ_NBA_Generator::Representation_t> * nba(nba_generator.translate("[]<>p"));
	Nondet_Finite_Automonton<XYZ_NBA_Generator::Representation_t> * nfa(nfa_generator.translate(nba));
	
	code_generator->generate(nfa, ::std::cout);
	
	delete nba;
	delete nfa;

	return 0;
}
