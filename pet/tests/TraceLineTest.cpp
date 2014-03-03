#include <boost/test/unit_test.hpp>
#include <TraceLine.hpp>
#include <Instruction.hpp>

BOOST_AUTO_TEST_CASE ( TraceLineTestParsing )
{
    std::string input_1 = "      0: system.cpu T0 : 0x89cc    :   mov   fp, #0             : IntAlu :  D=0x0000000000000000";
    TraceLine tl_1(input_1);
    BOOST_CHECK_EQUAL(tl_1.getSimEvent()->getTick(), 0);
    BOOST_CHECK_EQUAL(((Instruction*)tl_1.getSimEvent())->getCPU(), 0);
    BOOST_CHECK_EQUAL(((Instruction*)tl_1.getSimEvent())->getPC(), 0x89cc);
    BOOST_CHECK_EQUAL(*((Instruction*)tl_1.getSimEvent()), Instruction(input_1));
}
