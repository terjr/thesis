#pragma once

#include <string>
#include <vector>

#include <boost/lockfree/queue.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/thread.hpp>

#include "TraceLine.hpp"

typedef std::vector<boost::atomic<unsigned long>*> OutputVector;

class PowerModel {
    public:
        PowerModel(boost::lockfree::queue<std::string*> *q, boost::atomic<bool> *done);
        virtual ~PowerModel();
        virtual int calculate(TraceLine tr) = 0;
        virtual OutputVector getOutput() const = 0;
        int run();
    protected:
        boost::lockfree::queue<std::string*> *q;
        boost::atomic<bool> *done;
};
