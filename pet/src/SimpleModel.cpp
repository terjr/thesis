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
        unsigned long long numTicks)
    : PowerModel(q, done, annotations, weights, bucket_size, numTicks), m(){ }

SimpleModel::~SimpleModel() {}

int SimpleModel::calculate(const SimEvent *se) {

    if (se->getType() == EventType::Unknown)
        return 0;

    while (output.size() <= (se->getTick() / bucket_size)) {
        output.push_back(0L);
    }

    if (se->getType() == EventType::InstEvent) {
        const Instruction *inst = static_cast<const Instruction *>(se);
        output[inst->getTick()/bucket_size] += getWeight(inst->getInstrType());
    } else if (se->getType() == EventType::MemEvent) {
        const Memory *mem = static_cast<const Memory *>(se);
        // Only consider non-Null memory events
        if (MemType::Null != mem->getMemType()) {
            output[mem->getTick()/bucket_size] += getWeight(mem->getMemType());
        }
    }
    return 0;
}
