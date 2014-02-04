#include "OutputFormatter.hpp"

#include <boost/numeric/conversion/cast.hpp>
#include "gnuplot_i.hpp"

using namespace std;

OutputFormatter::OutputFormatter(const OutputVector &statistics) {
    importAsDouble(statistics);
}

void OutputFormatter::saveBarchart(const string &filename, const string &title) const {
    Gnuplot plot("lines");
    plot.savetofigure(filename).plot_x(this->dVector, title);
}

void OutputFormatter::showBarchart(const string &title) const {
    Gnuplot plot(this->dVector, title, "lines");
    cout << "Press any char..." << endl;
    cin.get();

}


void OutputFormatter::importAsDouble(const OutputVector &statistics) {
    dVector.resize(statistics.size());
    transform(statistics.begin(), statistics.end(), dVector.begin(),
            [](unsigned long l) -> double {
            return l;
            });
}
