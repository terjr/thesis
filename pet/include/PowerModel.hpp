#pragma once
#include "QueueType.hpp"

#include <string>
#include <vector>
#include <atomic>

#include <boost/thread.hpp>

#include "SimEvent.hpp"
#include "Instruction.hpp"
#include "Memory.hpp"

typedef struct {
    unsigned long sum;
    unsigned long num;
} powerSum;

typedef std::vector<powerSum> OutputVector;

class PowerModel {
    public:
        PowerModel(
                lfspscqueue *q,
                std::atomic<bool> *done,
                std::map<unsigned long, std::string> *annotations,
                std::map<std::string, unsigned long> *weights,
                unsigned long bucket_size = 10000,
                unsigned long long numTicks = 0,
                bool stats = false);
        virtual ~PowerModel();
        virtual int calculate(const SimEvent *se) = 0;
        unsigned long getWeight(const std::string &name) const;
        unsigned long getWeight(InstrType type) const;
        unsigned long getWeight(MemType type) const;
        const std::vector<std::map<const std::string, unsigned long>> getStats() const;
        OutputVector getOutput() const;
        void setOutputSize(unsigned long size);
        std::map<unsigned long, std::string> getAnnotations() const;
        int run();
    protected:
        void annotate(SimEvent *se);
        void updateStats(SimEvent *se);
        lfspscqueue *q;
        std::atomic<bool> *done;
        OutputVector output;
        unsigned long bucket_size;
        unsigned long long numTicks;
        std::map<unsigned long, std::string> *annotations;
        std::map<std::string, unsigned long> *weights;
        std::map<unsigned long, std::string> annotation_map;
        std::vector<std::map<const std::string, unsigned long>> eventStats;
        bool stats;
};
