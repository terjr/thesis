#include "OutputFormatter.hpp"

#include <boost/numeric/conversion/cast.hpp>
#include "gnuplot_i.hpp"

using namespace std;


OutputFormatter::OutputFormatter(const OutputVector &statistics) {
    plot = new Gnuplot("lines");
    importAsDouble(statistics);
}

OutputFormatter::~OutputFormatter() {
    delete plot;
}

void OutputFormatter::saveBarchart(const string &filename, const string &title) {
    plot->savetofigure(filename).plot_x(this->dVector, title);
}

void OutputFormatter::showBarchart(const string &title) {
    plot->plot_x(this->dVector, title).showonscreen();
    cout << "Displaying graph. Press any char to continue..." << endl;
    cin.get();

}

void OutputFormatter::addLabel(unsigned long x, unsigned long y, const string& label) {
    plot->addLabel(x,y,label);
}


void OutputFormatter::importAsDouble(const OutputVector &statistics) {
    dVector.resize(statistics.size());
    transform(statistics.begin(), statistics.end(), dVector.begin(),
            [](unsigned long l) -> double {
            return l;
            });
}
