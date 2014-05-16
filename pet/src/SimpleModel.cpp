#include "SimpleModel.hpp"
#include "TraceLine.hpp"

#include "SimEvent.hpp"
#include "Instruction.hpp"
#include "Memory.hpp"

using namespace boost;

SimpleModel::SimpleModel(
        lfspscqueue *q,
        std::atomic<bool> *done,
        std::map<unsigned long, std::string> *annotations,
        std::map<std::string, unsigned long> *weights,
        unsigned long bucket_size,
        unsigned long long numTicks,
        bool stats)
    : PowerModel(q, done, annotations, weights, bucket_size, numTicks, stats), m()
{}

SimpleModel::~SimpleModel() {}

int SimpleModel::calculate(const SimEvent *se) {

    if (se->getType() == EventType::Unknown)
        return 0;

    // Increase bucket vector size if tick is too large
    setOutputSize(se->getTick() / bucket_size + 1);

    if (se->getType() == EventType::InstEvent) {
        const Instruction *instr = static_cast<const Instruction *>(se);

        output[instr->getTick()/bucket_size].sum += getWeight(instr->getInstrType());
        output[instr->getTick()/bucket_size].num++;
    } else if (se->getType() == EventType::MemEvent) {
        const Memory *mem = static_cast<const Memory *>(se);

        // Only consider non-Null memory events
        if (MemType::Null != mem->getMemType()) {
            output[mem->getTick()/bucket_size].sum += getWeight(mem->getMemType());
            output[mem->getTick()/bucket_size].num++;
        }
    }

    return 0;
}
