#include "OutputFormatter.hpp"

#include <boost/numeric/conversion/cast.hpp>
#include "gnuplot_i.hpp"

using namespace std;

OutputFormatter::OutputFormatter(const vector<boost::atomic<long> > &statistics) {
    importAsDouble(statistics);
}

void OutputFormatter::createBarchart() const {
    Gnuplot plot(this->dVector);
    plot.showonscreen();
}


void OutputFormatter::importAsDouble(const vector<boost::atomic<long> > &statistics) {
    dVector.resize(statistics.size());
    transform(statistics.begin(), statistics.end(), dVector.begin(),
            [](long l) -> double {
            return boost::numeric_cast<double>(l);
            });
}
