
class Ex_Monitor_Wrapper 
    : public Monitor_Wrapper
{
private:
    Ex_Monitor_Wrapper(Self const & other);
    Self & operator=(Self const & other);

public:
    explicit Ex_Monitor_Wrapper(Fsm * const fsm);
    virtual ~Ex_Monitor_Wrapper();


protected:
    bool p_translate(Record_t const & record_t,
		     Bit_Vector_t & bit_vector);
};

Ex_Monitor_Wrapper::Ex_Monitor_Wrapper(Fsm * const fsm)
    : Monitor_Wrapper(fsm,
		      10) // we have 10 propostions to check for
{
}


Ex_Monitor_Wrapper::~Ex_Monitor_Wrapper()
{
}

bool Ex_Monitor_Wrapper::p_translate(Record_t const & record,
				     Bit_Vector_t & bit_vector)
{
    if(record.type() == ... ) 
	// irrelevant
	return false;

    // bit_vector[x] can be 0 or 1 initially

    if(....)
	bit_vector[0]=1;
    
    if(....)
	bit_vector[1]=0;


    return true;
}
