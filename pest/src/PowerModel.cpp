#include "PowerModel.hpp"

using namespace boost;

#define DELETE_STACK_SIZE 1024
PowerModel::PowerModel(lockfree::queue<std::string*> *q, atomic<bool> *done, unsigned long bucket_size) : q(q), done(done), bucket_size(bucket_size), output()  { }

PowerModel::~PowerModel()
{
    for (unsigned long i = 0; i < output.size(); ++i)
    {
        if (output[i])
        {
            delete output[i];
            output[i] = 0;
        }
    }
};

OutputVector PowerModel::getOutput() const
{
    return output;
}

int inline clean_stack(std::string *delete_stack[DELETE_STACK_SIZE], int ds) {
    for (int i = 0; i < ds; i++)
        delete delete_stack[i];
    return 0;
}

int PowerModel::run()
{
    std::string *s = 0;
    std::string *delete_stack[DELETE_STACK_SIZE];
    int ds = 0;
    while (!(*done) || !q->empty()) {
        while (q->pop(s)) {
            calculate(TraceLine(*s));
            if (ds < DELETE_STACK_SIZE) {
                delete_stack[ds++] = s;
            } else {
                ds = clean_stack(delete_stack, ds);
                delete s;
            }
        }
        ds = clean_stack(delete_stack, ds);
        this_thread::yield();
    }
    ds = clean_stack(delete_stack, ds);
    std::cout << "Thread " << this_thread::get_id() << std::endl;
    return 0;
}
