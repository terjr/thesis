#pragma once

#include <boost/lockfree/queue.hpp>
#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <string>
#include <vector>

#include "PowerModel.hpp"

class SimpleModel : public PowerModel {
    public:
        SimpleModel(boost::lockfree::queue<std::string*> *q, boost::atomic<bool> *done);
        virtual ~SimpleModel() {};
        virtual int calculate(TraceLine tr);
        virtual OutputVector getOutput() const;
    private:
        OutputVector output;
        boost::mutex m;
};
