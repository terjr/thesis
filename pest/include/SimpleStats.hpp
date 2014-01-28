#pragma once

#include <boost/lockfree/queue.hpp>
#include <boost/atomic.hpp>
#include <string>

int countAdds(boost::lockfree::queue<std::string*> *q, boost::atomic<bool> *done, boost::atomic<unsigned long> *count);
