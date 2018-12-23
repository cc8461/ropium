#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Expression.hpp"
#include "Condition.hpp"
#include "Gadget.hpp"
#include <vector>

using std::vector;
using std::pair; 

enum SubConstraintType: int {  CONSTR_RETURN=0, CONSTR_BAD_BYTES, CONSTR_KEEP_REGS, 
                            CONSTR_VALID_READ, CONSTR_VALID_WRITE, CONSTR_SP_INC, NB_CONSTR};
enum ConstrEval {EVAL_VALID, EVAL_INVALID, EVAL_MAYBE};

class SubConstraint{
    SubConstraintType _type; 
    public: 
        SubConstraint(SubConstraintType t);
        SubConstraintType type();
        // Functions of child classes
        virtual SubConstraint* copy(){throw "Should not be called here";}
        virtual void merge(SubConstraint* c){throw "Should not be called here";}
};

class ConstrReturn: public SubConstraint{
    bool _ret, _jmp, _call; 
    public: 
        ConstrReturn(bool r, bool j, bool c);
        bool ret(); 
        bool jmp();
        bool call();
        pair<ConstrEval,CondObjectPtr> verify(Gadget* g);
        virtual SubConstraint* copy();
        virtual void merge(SubConstraint* c, bool del);
};

class ConstrBadBytes: public SubConstraint{
    vector<unsigned char> _bad_bytes; 
    public:
        ConstrBadBytes();
        ConstrBadBytes(vector<unsigned char> bb);
        bool verify_address(addr_t a);
        pair<ConstrEval,CondObjectPtr> verify(Gadget* g);
        virtual SubConstraint* copy();
        virtual void merge(SubConstraint* c, bool del);
};

class ConstrKeepRegs: public SubConstraint{
    bool _regs[NB_REGS_MAX];
    public:
        ConstrKeepRegs();
        bool get(int num);
        void add_reg(int num);
        void remove_reg(int num);
        pair<ConstrEval,CondObjectPtr> verify(Gadget* g);
        virtual SubConstraint* copy();
        virtual void merge(SubConstraint* c, bool del);
};

class ConstrValidRead: public SubConstraint{
    vector<ExprObjectPtr> _addresses;
    public:
        ConstrValidRead();
        void add_addr( ExprObjectPtr a);
        pair<ConstrEval,CondObjectPtr> verify(Gadget* g);
        virtual SubConstraint* copy();
        virtual void merge(SubConstraint* c, bool del);
};

class ConstrValidWrite: public SubConstraint{
    vector<ExprObjectPtr> _addresses;
    public:
        ConstrValidWrite();
        void add_addr( ExprObjectPtr a);
        pair<ConstrEval,CondObjectPtr> verify(Gadget* g);
        virtual SubConstraint* copy();
        virtual void merge(SubConstraint* c, bool del);
};

class ConstrSpInc: public SubConstraint{
    cst_t _inc;
    public:
        ConstrSpInc(cst_t i);
        pair<ConstrEval,CondObjectPtr> verify(Gadget* g);
        virtual SubConstraint* copy();
};

// Constraint class (collection of subconstraints)
class Constraint{
    ConstrReturn* constr_return; 
    ConstrKeepRegs* constr_keep_regs;
    ConstrBadBytes* constr_bad_bytes;
    ConstrValidRead * constr_valid_read;
    ConstrValidWrite * constr_valid_write;
    ConstrSpInc * constr_sp_inc; 
    public:
        Constraint();
        // Accessors 
        SubConstraint* get(SubConstraintType t);
        // Modifiers
        void add(SubConstraint* c, bool del);
        void update(SubConstraint* c);
        void remove(SubConstraintType t);
        // Copy
        Constraint* copy();
        // Destructor 
        ~Constraint();
};

#endif
