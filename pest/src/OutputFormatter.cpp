#include "OutputFormatter.hpp"

#include <boost/numeric/conversion/cast.hpp>
#include "gnuplot_i.hpp"

using namespace std;

OutputFormatter::OutputFormatter(const OutputVector &statistics) {
    importAsDouble(statistics);
}

void OutputFormatter::saveBarchart(const string &filename, const string &title) const {
    Gnuplot plot(this->dVector, title, "lines");
    plot.savetofigure(filename);
}

void OutputFormatter::showBarchart(const string &title) const {
    Gnuplot plot(this->dVector, title, "lines");
    plot.showonscreen();
    cout << "Press any char..." << endl;
    cin.get();

}


void OutputFormatter::importAsDouble(const OutputVector &statistics) {
    dVector.resize(statistics.size());
    transform(statistics.begin(), statistics.end(), dVector.begin(),
            [](boost::atomic<unsigned long> *l) -> double {
            return *l;
            });
}
