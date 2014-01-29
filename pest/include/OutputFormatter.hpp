#include <vector>
#include <boost/thread.hpp>

#include "PowerModel.hpp"

class OutputFormatter {
    public:
        OutputFormatter(const OutputVector &statistics);
        void showBarchart(const std::string &title = "<title>") const;
    private:
        std::vector<double> dVector;

        void importAsDouble(const OutputVector &statistics);
};

