#include <TraceLine.hpp>
#define BOOST_TEST_MODULE TraceLineTest
#include <boost/test/unit_test.hpp>

#include <InstrType.cpp>

BOOST_AUTO_TEST_CASE ( TraceLine )
{
    std::string input_1 = "      0: system.cpu T0 : 0x89cc    :   mov   fp, #0             : IntAlu :  D=0x0000000000000000";
    TraceLine tl_1(input1);
    BOOST_CHECK_EQUAL(tl_1.getTick(), 0);
    BOOST_CHECK_EQUAL(tl_1.getCPU(), 0);
    BOOST_CHECK_EQUAL(tl_1.getPC(), 0x89cc);
    BOOST_CHECK_EQUAL(t1_1.getInstr(), Instruction("mov fp,#0", Instruction::IntAlu))
}
