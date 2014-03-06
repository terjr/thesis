#pragma once

#include <string>
#include <vector>
#include <atomic>

#include <boost/lockfree/spsc_queue.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/thread.hpp>

#include "SimEvent.hpp"
#include "Instruction.hpp"
#include "Memory.hpp"

typedef std::vector<unsigned long> OutputVector;

class PowerModel {
    public:
        PowerModel(
                boost::lockfree::spsc_queue<std::string*> *q,
                std::atomic<bool> *done,
                std::map<unsigned long, std::string> *annotations,
                std::map<std::string, unsigned long> *weights,
                unsigned long bucket_size = 10000,
                unsigned long long numTicks = 0);
        virtual ~PowerModel();
        virtual int calculate(const SimEvent *se) = 0;
        unsigned long getWeight(const std::string &name) const;
        unsigned long getWeight(InstrType type) const;
        unsigned long getWeight(MemType type) const;
        OutputVector getOutput() const;
        std::map<unsigned long, std::string> getAnnotations() const;
        int run();
    protected:
        void annotate(SimEvent *se);
        boost::lockfree::spsc_queue<std::string*> *q;
        std::atomic<bool> *done;
        OutputVector output;
        unsigned long bucket_size;
        unsigned long long numTicks;
        std::map<unsigned long, std::string> *annotations;
        std::map<std::string, unsigned long> *weights;
        std::map<unsigned long, std::string> annotation_map;
};
