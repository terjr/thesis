#include <vector>
#include <boost/thread.hpp>

class OutputFormatter {
    public:
        OutputFormatter(const std::vector<boost::atomic<long> > &statistics);
        void createBarchart() const;
    private:
        std::vector<double> dVector;

        void importAsDouble(const std::vector<boost::atomic<long> > &statistics);
};

