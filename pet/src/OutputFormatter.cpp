#include "OutputFormatter.hpp"

#include <boost/numeric/conversion/cast.hpp>
#include "ProgramArguments.hpp"
#include "gnuplot_i.hpp"

using namespace std;


OutputFormatter::OutputFormatter(const OutputVector &statistics, const options_t *options) : options(options) {
    importAsDouble(statistics);
    if (Graph == options->outputFormat) {
        plot = new Gnuplot("lines");
    }
}

OutputFormatter::~OutputFormatter() {
    if (Graph == options->outputFormat) {
        delete plot;
    }
}

void OutputFormatter::saveBarchart(const string &filename, const string &title) {
    if (plot)
        plot->savetofigure(filename).plot_x(this->dVector, title);
    else
        cerr << "Cannot save barchart without Gnuplot instance." << endl;
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

void OutputFormatter::produceOutput() {
    switch (options->outputFormat) {
        case Graph:
            if (!options->output.empty())
                saveBarchart(options->output);
            else
                showBarchart();
            break;
        case Plain:
            {
                // TODO: Output to file if specified
                ostream *out;
                if (!options->output.empty())
                    out = new ofstream(options->output);
                else
                    out = &cout;

                for (unsigned long i = 0; i < dVector.size(); ++i)
                    *out << i << " " << dVector[i] << '\n';

                if (out != &cout)
                    delete out;

                break;
            }
        case Table:
            {
                FILE *out;
                if (!options->output.empty())
                    out = fopen(options->output.c_str(), "w+");
                else
                    out = stdout;

                fprintf(out, "/-------------------------\\\n");
                fprintf(out, "|   Bucket   |   Energy   |\n");
                fprintf(out, "|-------------------------|\n");

                for (unsigned long i = 0; i < dVector.size(); ++i)
                    fprintf(out, "|%11lu |%11f |\n", i, dVector[i]);
                fprintf(out, "\\-------------------------/\n");

                if (out == stdout)
                    fclose(out);

                break;
            }
        default:
            cerr << "Could not show output." << endl;
    }
}