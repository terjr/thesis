#pragma once

#include <boost/lockfree/queue.hpp>
#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <string>
#include <vector>

#include "PowerModel.hpp"

class SimpleModel : public PowerModel {
    public:
        SimpleModel(
                boost::lockfree::queue<std::string*, boost::lockfree::fixed_sized<true> > *q,
                boost::atomic<bool> *done,
                std::map<unsigned long, std::string> *annotations,
                std::map<std::string, unsigned long> *weights,
                unsigned long bucket_size = 10000000, unsigned long long numTicks = 0L);
        virtual ~SimpleModel();
        virtual int calculate(const SimEvent *inst);
    private:
        boost::mutex m;
};
