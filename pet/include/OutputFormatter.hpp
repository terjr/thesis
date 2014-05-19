#include <vector>
#include <boost/thread.hpp>

#include "ProgramArguments.hpp"

class Gnuplot;


/**
 * This class is an abstraction of generating different outputs from PET.
 * OutputFormatter can generate graphs or text, possibly with annotations,
 * depending on options and datasets.
 */
class OutputFormatter {
    public:
        OutputFormatter(const std::vector<unsigned long> &statistics, const options_t *options, const double scale);
        ~OutputFormatter();
        void produceOutput();

        void addAnnotations(std::map<unsigned long, std::string> annot);
    private:
        std::vector<double> dVector;
        std::vector<double> tVector;
        std::map<unsigned long, std::string> annotations;
        double scale;
        Gnuplot *plot;
        const options_t *options;

        void saveBarchart(const std::string &filename, const std::string &title);
        void saveBarchart(const std::string &filename);
        void showBarchart(const std::string &title);
        void showBarchart();
        void importAsDouble(const std::vector<unsigned long> &statistics);
        void addLabel(unsigned long x, unsigned long y, const std::string &label); 
};

