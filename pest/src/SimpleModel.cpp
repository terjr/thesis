#include "SimpleModel.hpp"
#include "TraceLine.hpp"

using namespace boost;

SimpleModel::SimpleModel(lockfree::queue<std::string*> *q, atomic<bool> *done, unsigned long buckets) : PowerModel(q, done, buckets), m(){ }

SimpleModel::~SimpleModel() {}

int SimpleModel::calculate(const TraceLine &tr)
{
    while (output.size() <= (tr.getTick()/bucket_size)) {
        atomic<unsigned long> *ul = new atomic<unsigned long>(0L);
        m.lock();
        output.push_back(ul);
        if (!(output.size() % 10000)) printf("Output vector is %lu long.", output.size());
        m.unlock();
    }
    switch (tr.getInstr().getInstrType())
    {
        case IntAlu:
            (output[tr.getTick()/bucket_size])->fetch_add(28, memory_order_relaxed);
            break;
        case IntMult:
            (output[tr.getTick()/bucket_size])->fetch_add(23, memory_order_relaxed);
            break;
        case MemRead:
            (output[tr.getTick()/bucket_size])->fetch_add(18, memory_order_relaxed);
            break;
        case MemWrite:
            (output[tr.getTick()/bucket_size])->fetch_add(13, memory_order_relaxed);
            break;
        case SimdFloatMisc:
            (output[tr.getTick()/bucket_size])->fetch_add(43, memory_order_relaxed);
            break;
        case ErrorType:
            break;
    }
    return 0;
}
