#include <vector>
#include <boost/thread.hpp>

#include "PowerModel.hpp"

class OutputFormatter {
    public:
        OutputFormatter(const OutputVector &statistics);
        void saveBarchart(const std::string &filename, const std::string &title = "<title>") const;
        void showBarchart(const std::string &title = "<title>") const;
    private:
        std::vector<double> dVector;

        void importAsDouble(const OutputVector &statistics);
};

