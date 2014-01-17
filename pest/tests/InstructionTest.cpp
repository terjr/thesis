#include <Instruction.hpp>
#define BOOST_TEST_MODULE InstrTypeTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE ( InstrTypeTest )
{
    std::string input_1("mov fp,#0");
    Instruction instr_1(input_1, Instruction::InstrType::IntAlu);

    BOOST_CHECK_EQUAL(std::strcmp( instr_1.getInstr(), "mov"), 0);
    BOOST_CHECK_EQUAL(std::strcmp( instr_1.getOp1(), "fp"), 0);
    BOOST_CHECK_EQUAL(std::strcmp( instr_1.getOp2(), "#0"), 0);
    BOOST_CHECK_EQUAL(std::strcmp( instr_1.getOp3(), NULL), 0);
    BOOST_CHECK_EQUAL(instr_1.getNumOp(), 2);
    BOOST_CHECK_EQUAL(instr_1.getExecType(), Instruction::InstrType::IntAlu);

}
