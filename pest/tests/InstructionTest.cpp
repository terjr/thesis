#include <boost/test/unit_test.hpp>
#include <Instruction.hpp>

using namespace std;

#define BASIC_MOV "  47000: system.cpu T0 : 0x89ec    :   mov   fp, #0             : IntAlu :  D=0x0000000000000000  "
#define SPARSE_ADD " 252000: system.cpu T0 : 0x8e40    :   add   r0, r3, #1         : IntAlu :  D=0x0000000000000002 "
#define BASIC_MEM " 154000: system.cpu T0 : 0x8e04    :   ldr   r12, [pc, #624]    : MemRead :  D=0x0000000000000000 A=0x907c "

struct Basic {
    Basic() :
        instr_mov(Instruction(BASIC_MOV)),
        instr_add(Instruction(SPARSE_ADD)),
        instr_mem(Instruction(BASIC_MEM))
    { }
    virtual ~Basic() { }

    Instruction instr_mov, instr_add, instr_mem;
};


BOOST_FIXTURE_TEST_SUITE( InstructionParsing, Basic )

BOOST_AUTO_TEST_CASE ( InstructionParseMnemonic )
{
    BOOST_CHECK_EQUAL(instr_mov.getMnemonic(), string("mov"));
    BOOST_CHECK_EQUAL(instr_add.getMnemonic(), string("add"));
    BOOST_CHECK_EQUAL(instr_mem.getMnemonic(), string("ldr"));
}

BOOST_AUTO_TEST_CASE ( InstructionToString )
{
    BOOST_CHECK_EQUAL (instr_mov.toString(), string("mov fp,#0"));
    BOOST_CHECK_EQUAL (instr_add.toString(), string("add r0,r3,#1"));
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
    BOOST_CHECK_EQUAL(instr_add.getOp(0), string("r0"));
    BOOST_CHECK_EQUAL(instr_add.getOp(1), string("r3"));
    BOOST_CHECK_EQUAL(instr_add.getOp(2), string("#1"));

    BOOST_CHECK_EQUAL(instr_mem.getOp(0), string("r12"));
}

BOOST_AUTO_TEST_CASE ( InstructionEqualTest )
{
    BOOST_CHECK_EQUAL(instr_mov, instr_mov);
    BOOST_CHECK_EQUAL(instr_add, instr_add);
    BOOST_CHECK(instr_mov != instr_add);
}

BOOST_AUTO_TEST_CASE ( InstructionAltConstr )
{
    Instruction instr_mov_local;
    instr_mov_local.parseAssembly("   mov   fp, #0             ");
    instr_mov_local.setInstrType("IntAlu");
    instr_mov_local.setTick(47000);
    instr_mov_local.setPC(0x89ec);
    BOOST_CHECK_EQUAL(instr_mov, instr_mov_local);
}

BOOST_AUTO_TEST_SUITE_END()
