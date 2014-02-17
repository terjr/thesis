#pragma once

#include <string>
#include <vector>

#include <boost/lockfree/queue.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/thread.hpp>

#include "SimEvent.hpp"

typedef std::vector<unsigned long> OutputVector;

class PowerModel {
    public:
        PowerModel(
                boost::lockfree::queue<std::string*, boost::lockfree::fixed_sized<true>> *q,
                boost::atomic<bool> *done,
                std::map<unsigned long, std::string> *annotations,
                unsigned long bucket_size = 10000,
                unsigned long long numTicks = 0);
        virtual ~PowerModel();
        virtual int calculate(const SimEvent *se) = 0;
        OutputVector getOutput() const;
        std::map<unsigned long, std::string> getAnnotations() const;
        int run();
    protected:
        void annotate(SimEvent *se);
        boost::lockfree::queue<std::string*, boost::lockfree::fixed_sized<true>> *q;
        boost::atomic<bool> *done;
        OutputVector output;
        unsigned long bucket_size;
        unsigned long long numTicks;
        std::map<unsigned long, std::string> *annotations;
        std::map<unsigned long, std::string> annotation_map;
};
