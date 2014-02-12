#include <boost/test/unit_test.hpp>
#include <Memory.hpp>

using namespace std;


struct BasicMem {
    BasicMem() :
        mem_read_l1_hit(" 202500: system.cpu.icache: ReadReq (ifetch) bc0 hit state: 5 (S) valid: 1 writable: 0 readable: 1 dirty: 0 tag: 0 data: 4a"),
        mem_write_l1_hit(" 374500: system.cpu.dcache: WriteReq 80dcc hit state: f (M) valid: 1 writable: 1 readable: 1 dirty: 1 tag: 10 data: 0"),
        mem_read_l1_miss(" 202500: system.cpu.icache: ReadReq (ifetch) bc0 miss"),
        mem_write_l1_miss(" 374500: system.cpu.dcache: WriteReq 80dcc miss"),

        mem_read_l2_hit(" 556500: system.l2: ReadReq dd40 hit state: 7 (E) valid: 1 writable: 1 readable: 1 dirty: 0 tag: 0 data: 3"),
        mem_write_l2_hit("7380104000: system.l2: Block for addr 81900 being updated in Cache"),
        mem_read_l2_miss(" 556500: system.l2: ReadReq dd40 miss"),
        mem_write_l2_miss(" 180000: system.l2: ReadExReq 80ec0 miss "),

        mem_write("1305500: system.physmem: Write of size 64 on address 0x80c40 "),
        mem_read("   73172: system.physmem: Read of size 64 on address 0x80f00      "),
        mem_null("   0: system.physmem: Write of size 4 on address 0x80f00      "),
        mem_iread(" 1330000: system.physmem: IFetch of size 64 on address 0xec00  ")

    { }
    virtual ~BasicMem() { }

    Memory  mem_read_l1_hit, mem_write_l1_hit, mem_read_l1_miss, mem_write_l1_miss,  mem_read_l2_hit, mem_write_l2_hit,mem_read_l2_miss, mem_write_l2_miss, mem_write, mem_read, mem_null, mem_iread;
};


BOOST_FIXTURE_TEST_SUITE( MemoryParsing, BasicMem )

BOOST_AUTO_TEST_CASE ( MemoryTypes )
{
    BOOST_CHECK_EQUAL (mem_read_l1_hit.getMemType(), MemType::L1I);
    BOOST_CHECK_EQUAL (mem_write_l1_hit.getMemType(), MemType::L1D);
    BOOST_CHECK_EQUAL (mem_read_l1_miss.getMemType(), MemType::L1I);
    BOOST_CHECK_EQUAL (mem_write_l1_miss.getMemType(), MemType::L1D);

    BOOST_CHECK_EQUAL (mem_read_l2_hit.getMemType(), MemType::L2);
    BOOST_CHECK_EQUAL (mem_write_l2_hit.getMemType(), MemType::L2);
    BOOST_CHECK_EQUAL (mem_read_l2_miss.getMemType(), MemType::L2);
    BOOST_CHECK_EQUAL (mem_write_l2_miss.getMemType(), MemType::L2);

    BOOST_CHECK_EQUAL (mem_read.getMemType(), MemType::Phys);
    BOOST_CHECK_EQUAL (mem_write.getMemType(), MemType::Phys);
    BOOST_CHECK_EQUAL (mem_null.getMemType(), MemType::Null);
    BOOST_CHECK_EQUAL (mem_iread.getMemType(), MemType::Phys);
}

BOOST_AUTO_TEST_CASE ( MemoryIsHit )
{
    BOOST_CHECK (mem_read_l1_hit.isHit());
    BOOST_CHECK (mem_write_l1_hit.isHit());
    BOOST_CHECK (!mem_read_l1_miss.isHit());
    BOOST_CHECK (!mem_write_l1_miss.isHit());

    BOOST_CHECK (mem_read_l2_hit.isHit());
    BOOST_CHECK (mem_write_l2_hit.isHit());
    BOOST_CHECK (!mem_read_l2_miss.isHit());
    BOOST_CHECK (!mem_write_l2_miss.isHit());

    BOOST_CHECK (mem_read.isHit());
    BOOST_CHECK (mem_write.isHit());
    BOOST_CHECK (mem_null.isHit());
    BOOST_CHECK (mem_iread.isHit());
}


BOOST_AUTO_TEST_SUITE_END()
