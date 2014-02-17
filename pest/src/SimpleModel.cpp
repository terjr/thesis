#include "SimpleModel.hpp"
#include "TraceLine.hpp"

#include "SimEvent.hpp"
#include "Instruction.hpp"
#include "Memory.hpp"

using namespace boost;

SimpleModel::SimpleModel(lockfree::queue<std::string*, boost::lockfree::fixed_sized<true>> *q, atomic<bool> *done, unsigned long bucket_size, unsigned long long numTicks) : PowerModel(q, done, bucket_size, numTicks), m(){ }

SimpleModel::~SimpleModel() {}

int SimpleModel::calculate(const SimEvent *se) {

    if (se->getType() == EventType::Unknown)
        return 0;

    while (output.size() <= (se->getTick() / bucket_size)) {
        output.push_back(0L);
        if (!(output.size() % 10000)) {
            printf("Output vector is %lu long.", output.size());
        }
    }

    // TODO: Use dynamic_cast instead
    if (se->getType() == EventType::InstEvent) {
        const Instruction *inst = (Instruction *) se;

        switch (inst->getInstrType()) {
            case IntAlu:
                output[inst->getTick()/bucket_size] += 28;
                break;
            case IntMult:
                output[inst->getTick()/bucket_size] += 23;
                break;
            case MemRead:
                output[inst->getTick()/bucket_size] += 18;
                break;
            case MemWrite:
                output[inst->getTick()/bucket_size] += 13;
                break;
            case SimdFloatMisc:
                output[inst->getTick()/bucket_size] += 43;
                break;
            case ErrorType:
                break;
        }
    } else if (se->getType() == EventType::MemEvent) {
        const Memory *mem = (Memory *) se;
        switch (mem->getMemType()) {
            case L1I:
            case L1D:
                {
                    output[mem->getTick()/bucket_size] += 3;
                    break;
                }
            case L2:
                {
                    output[mem->getTick()/bucket_size] += 30;
                    break;
                }
            case Phys:
                {
                    output[mem->getTick()/bucket_size] += 30000;
                    break;
                }
            default:
                break;

        }
    }
    return 0;
}
