#include <vector>
#include <boost/thread.hpp>

#include "PowerModel.hpp"

class Gnuplot;

class OutputFormatter {
    public:
        OutputFormatter(const OutputVector &statistics);
        ~OutputFormatter();
        void saveBarchart(const std::string &filename, const std::string &title = "<title>");
        void showBarchart(const std::string &title = "<title>");
        void addLabel(unsigned long x, unsigned long y, const std::string &label);
    private:
        std::vector<double> dVector;
        Gnuplot *plot;

        void importAsDouble(const OutputVector &statistics);
};

