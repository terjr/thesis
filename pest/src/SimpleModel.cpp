#include "SimpleModel.hpp"
#include "TraceLine.hpp"

using namespace boost;

#define TICKS 100000

SimpleModel::SimpleModel(lockfree::queue<std::string*> *q, atomic<bool> *done) : PowerModel(q, done), output(), m() { }

SimpleModel::~SimpleModel()
{
    for (unsigned long i = 0; i < output.size(); ++i)
    {
        if (output[i])
        {
            delete output[i];
            output[i] = 0;
        }
    }
}

OutputVector SimpleModel::getOutput() const
{
    return output;
}

int SimpleModel::calculate(TraceLine tr)
{
    while (output.size() <= (tr.getTick()/TICKS)) {
        atomic<unsigned long> *ul = new atomic<unsigned long>(0L);
        m.lock();
        output.push_back(ul);
        m.unlock();
    }
    switch (tr.getInstr().getInstrType())
    {
        case IntAlu:
            *(output[tr.getTick()/TICKS]) += 28;
            break;
        case IntMult:
            *(output[tr.getTick()/TICKS]) += 23;
            break;
        case MemRead:
            *(output[tr.getTick()/TICKS]) += 18;
            break;
        case MemWrite:
            *(output[tr.getTick()/TICKS]) += 13;
            break;
        case SimdFloatMisc:
            *(output[tr.getTick()/TICKS]) += 43;
            break;
        case ErrorType:
            break;
    }
    return 0;
}
