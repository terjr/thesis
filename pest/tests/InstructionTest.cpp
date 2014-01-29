#include <boost/test/unit_test.hpp>
#include <Instruction.hpp>

using namespace std;

#define BASIC_MOV "mov fp,#0"
#define SPARSE_ADD "   add    r1,      r2, r3  "
#define BASIC_MEM "ldr   r3, [pc, #12]"
struct Basic {
    Basic() :
        instr_mov(Instruction(BASIC_MOV,  "IntAlu")),
        instr_add(Instruction(SPARSE_ADD, "IntAlu")),
        instr_mem(Instruction(BASIC_MEM, "MemRead"))
    { }
    virtual ~Basic() { }

    Instruction instr_mov, instr_add, instr_mem;
};


BOOST_FIXTURE_TEST_SUITE( InstructionParsing, Basic )

BOOST_AUTO_TEST_CASE ( InstructionParseMnemonic )
{
    BOOST_CHECK_EQUAL(instr_mov.getMnemonic(), string("mov"));
    BOOST_CHECK_EQUAL(instr_add.getMnemonic(), string("add"));
}

BOOST_AUTO_TEST_CASE ( InstructionToString )
{
    BOOST_CHECK_EQUAL (instr_mov.toString(), string(BASIC_MOV));
    BOOST_CHECK_EQUAL (instr_add.toString(), string("add r1,r2,r3"));
}

BOOST_AUTO_TEST_CASE ( InstructionTypes )
{
    BOOST_CHECK_EQUAL (instr_mov.getInstrType(), InstrType::IntAlu);
    BOOST_CHECK_EQUAL (instr_add.getInstrType(), InstrType::IntAlu);
    BOOST_CHECK_EQUAL (instr_mem.getInstrType(), InstrType::MemRead);
}

BOOST_AUTO_TEST_CASE ( InstructionParseOperands )
{
    BOOST_CHECK_EQUAL(instr_mov.getNumOp(), 2);
    BOOST_CHECK_EQUAL(instr_mov.getOp(0), "fp");
    BOOST_CHECK_EQUAL(instr_mov.getOp(1), string("#0"));

    BOOST_CHECK_EQUAL(instr_add.getNumOp(), 3);
    BOOST_CHECK_EQUAL(instr_add.getOp(0), string("r1"));
    BOOST_CHECK_EQUAL(instr_add.getOp(1), string("r2"));
    BOOST_CHECK_EQUAL(instr_add.getOp(2), string("r3"));

    BOOST_CHECK_EQUAL(instr_mem.getOp(0), string("r3"));
}

BOOST_AUTO_TEST_CASE ( InstructionEqualTest )
{
    BOOST_CHECK_EQUAL(instr_mov, instr_mov);
    BOOST_CHECK_EQUAL(instr_add, instr_add);
    BOOST_CHECK(instr_mov != instr_add);
}

BOOST_AUTO_TEST_CASE ( InstructionAltConstr )
{
    Instruction instr_add;
    instr_add.parseAssembly(string(BASIC_MOV));
    instr_add.setInstrType("IntAlu");
    BOOST_CHECK_EQUAL(instr_mov, instr_add);
}

BOOST_AUTO_TEST_SUITE_END()
