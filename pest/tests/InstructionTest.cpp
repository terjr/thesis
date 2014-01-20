#include <boost/test/unit_test.hpp>
#include <Instruction.hpp>

using namespace std;

BOOST_AUTO_TEST_CASE ( InstructionBasicParseTest )
{
    std::string input_1("mov fp,#0");
    Instruction instr_1(input_1, IntAlu);

    BOOST_CHECK_EQUAL(*instr_1.getInstr(), string("mov"));
    BOOST_CHECK_EQUAL(instr_1.getNumOp(), 2);
    BOOST_CHECK_EQUAL(*instr_1.getOp(1), string("fp"));
    BOOST_CHECK_EQUAL(*instr_1.getOp(2), string("#0"));
    BOOST_CHECK(!instr_1.getOp(3));
    BOOST_CHECK_EQUAL(instr_1.getExecType(), IntAlu);
}
