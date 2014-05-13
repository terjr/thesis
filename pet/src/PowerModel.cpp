#include "PowerModel.hpp"

#include "TraceLine.hpp"
#include "Instruction.hpp"

using namespace boost;

#define DELETE_STACK_SIZE 1024
PowerModel::PowerModel(lfspscqueue *q,
        std::atomic<bool> *done,
        std::map<unsigned long, std::string> *annotations,
        std::map<std::string, unsigned long> *weights,
        unsigned long bucket_size,
        unsigned long long numTicks,
        bool stats) : q(q), done(done), output(), bucket_size(bucket_size), numTicks(numTicks), annotations(annotations), weights(weights), annotation_map(), eventStats(), stats(stats) { }

PowerModel::~PowerModel() { };

OutputVector PowerModel::getOutput() const {
    return output;
}

void PowerModel::setOutputSize(unsigned long size) {
    while (output.size() < size) {
        output.push_back({0L,0L});
        eventStats.push_back(std::map<const std::string, unsigned long>());
    }
}

std::map<unsigned long, std::string> PowerModel::getAnnotations() const {
    return annotation_map;
}

int inline cleanStack(std::string *delete_stack[DELETE_STACK_SIZE], int ds) {
    for (int i = 0; i < ds; i++)
        delete delete_stack[i];
    return 0;
}

unsigned long PowerModel::getWeight(MemType type) const {
    return getWeight(memTypeToString(type));
}
unsigned long PowerModel::getWeight(InstrType type) const {
    return getWeight(instrTypeToString(type));
}
unsigned long PowerModel::getWeight(const std::string &name) const {
    if (weights == 0) return 0;
    auto it = weights->find(name);
    if (it != weights->end())
        return it->second;
    else
        return 0;
}

const std::vector<std::map<const std::string, unsigned long>> PowerModel::getStats() const {
    return eventStats;
}

void PowerModel::annotate(SimEvent *se) {
    if (se->getType() != InstEvent)
        return;

    Instruction *instr = static_cast<Instruction*>(se);
    instr->getPC();
    auto it = annotations->find(instr->getPC());
    if (it != annotations->end())
        annotation_map.insert(std::pair<unsigned long, std::string>(instr->getTick()/bucket_size, it->second));
}

void PowerModel::updateStats(SimEvent *se) {
    switch (se->getType()) {
        case InstEvent:
            {
                Instruction *in = static_cast<Instruction*>(se);
                eventStats[in->getTick()/bucket_size][instrTypeToString(in->getInstrType())]++;
            }
            break;
        case MemEvent:
            {
                Memory *mem = static_cast<Memory*>(se);
                eventStats[mem->getTick()/bucket_size][memTypeToString(mem->getMemType())]++;
            }
            break;
        default:
            break;
    }
}

int PowerModel::run() {
    std::string *s = 0;
    std::string *delete_stack[DELETE_STACK_SIZE];
    int ds = 0;
    short cnt = 0;
    while (!(*done && q->empty())) {
        while (q->pop(s)) {
            TraceLine tr = TraceLine(*s);

            calculate(tr.getSimEvent());
            if (annotations)
                annotate(tr.getSimEvent());
            if (stats)
                updateStats(tr.getSimEvent());
            if (numTicks > 0 && cnt++ % 2048 == 0) {
                printf("\rTicks prosessed: \t%ld/%llu (%.3f%%)", tr.getSimEvent()->getTick(), numTicks, (((double)tr.getSimEvent()->getTick())/numTicks)*100);
            }
            if (ds < DELETE_STACK_SIZE) {
                delete_stack[ds++] = s;
            } else {
                ds = cleanStack(delete_stack, ds);
                delete s;
            }
        }
        ds = cleanStack(delete_stack, ds);
        this_thread::yield();
    }
    ds = cleanStack(delete_stack, ds);
    //    std::cout << "Thread " << this_thread::get_id() << std::endl;
    return 0;
}
