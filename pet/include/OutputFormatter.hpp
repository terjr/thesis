#include <vector>
#include <boost/thread.hpp>

#include "PowerModel.hpp"
#include "ProgramArguments.hpp"

class Gnuplot;

class OutputFormatter {
    public:
        OutputFormatter(const OutputVector &statistics, const options_t *options);
        ~OutputFormatter();
        void produceOutput();

        void addAnnotations(std::map<unsigned long, std::string> annot);
    private:
        std::vector<double> dVector;
        std::map<unsigned long, std::string> annotations;
        Gnuplot *plot;
        const options_t *options;

        void saveBarchart(const std::string &filename, const std::string &title = "<title>");
        void showBarchart(const std::string &title = "<title>");
        void importAsDouble(const OutputVector &statistics);
        void addLabel(unsigned long x, unsigned long y, const std::string &label); 
};

