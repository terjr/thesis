#pragma once
#include <boost/lockfree/spsc_queue.hpp>
#ifndef lfspscqueue
    #define lfspscqueue boost::lockfree::spsc_queue<std::string*,boost::lockfree::capacity<1024>>
#endif

