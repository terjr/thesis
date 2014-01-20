#include <boost/test/unit_test.hpp>
#include <Instruction.hpp>

using namespace std;

struct Basic {
    Basic() : instr_1(Instruction("mov fp,#0", IntAlu)) { }
    virtual ~Basic() { }

    Instruction instr_1;
};


BOOST_FIXTURE_TEST_SUITE( InstructionParsing, Basic )

BOOST_AUTO_TEST_CASE ( InstructionStoreType )
{
    BOOST_CHECK_EQUAL(instr_1.getExecType(), IntAlu);
}

BOOST_AUTO_TEST_CASE ( InstructionParseMnemonic )
{
    BOOST_CHECK_EQUAL(instr_1.getMnemonic(), string("mov"));
}

BOOST_AUTO_TEST_CASE ( InstructionParseOperands )
{
    BOOST_CHECK_EQUAL(instr_1.getNumOp(), 2);
    BOOST_CHECK_EQUAL(instr_1.getOp(0), string("fp"));
    BOOST_CHECK_EQUAL(instr_1.getOp(1), string("#0"));
}


BOOST_AUTO_TEST_CASE ( InstructionAltConstr )
{
    Instruction instr_2;
    instr_2.parseAssembly(string("mov fp,#0"));
//    BOOST_CHECK_EQUAL(instr_1, instr_2);
}

BOOST_AUTO_TEST_SUITE_END()
