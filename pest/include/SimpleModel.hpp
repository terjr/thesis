#pragma once

#include <boost/lockfree/queue.hpp>
#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <string>
#include <vector>

#include "PowerModel.hpp"

class SimpleModel : public PowerModel {
    public:
        SimpleModel(boost::lockfree::queue<std::string*> *q, boost::atomic<bool> *done, unsigned long bucket_size = 100000);
        virtual ~SimpleModel();
        virtual int calculate(TraceLine tr);
    private:
        boost::mutex m;
};
