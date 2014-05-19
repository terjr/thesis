#include "OutputFormatter.hpp"

#include <boost/numeric/conversion/cast.hpp>
#include "ProgramArguments.hpp"
#include "gnuplot_i.hpp"

using namespace std;


/**
 * Create a new instance of OutputFormatter using a given dataset and a set of options
 */
OutputFormatter::OutputFormatter(const std::vector<unsigned long> &statistics, const options_t *options, double scale) : annotations(), scale(scale), options(options) {
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

void OutputFormatter::saveBarchart(const string &filename) {
    if (!options->title.empty())
        saveBarchart(filename, options->title);
    else
        saveBarchart(filename, options->inputName);
}

void OutputFormatter::showBarchart() {
    if (!options->title.empty())
        showBarchart(options->title);
    else
        showBarchart(options->inputName);
}

void OutputFormatter::saveBarchart(const string &filename, const string &title) {
    if (plot)
        plot->savetofigure(filename).plot_xy(this->tVector, this->dVector, title);
    else
        cerr << "Cannot save barchart without Gnuplot instance." << endl;
}

void OutputFormatter::showBarchart(const string &title) {
    plot->plot_xy(this->tVector, this->dVector, title).showonscreen();
    cout << "Displaying graph. Press any char to continue..." << endl;
    cin.get();
}

void OutputFormatter::addLabel(unsigned long x, unsigned long y, const string& label) {
    plot->addLabel(x*scale,y,label);
}

void OutputFormatter::addAnnotations(std::map<unsigned long, std::string> annot) {
    std::string prev;
    if (options->outputFormat == Graph) {
        for (auto it=annot.begin(); it!=annot.end(); ++it) {
            if (prev != it->second) {
                prev = it->second;
                addLabel(it->first, dVector[it->first]+10, it->second);
            }
        }
    } else {
        annotations.insert(annot.begin(), annot.end());
    }
}


void OutputFormatter::importAsDouble(const std::vector<unsigned long> &statistics) {
    dVector.resize(statistics.size());
    transform(statistics.begin(), statistics.end(), dVector.begin(),
            [](unsigned long l) -> double {
            return l;
            });
    for (unsigned long i = 0; i < statistics.size(); i++) {
        tVector.push_back(i*scale);
    }
}

/**
 * Produce output from current dataset
 * with current options
 */
void OutputFormatter::produceOutput() {
    switch (options->outputFormat) {
        case Graph:
            plot->set_xlabel("ms");
            plot->set_ylabel("mA");
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
                    *out << i << " " << dVector[i] << " " << annotations[i] << '\n';

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

                fprintf(out, "/-------------------------------------\\\n");
                fprintf(out, "|   Bucket   | milliAmps  |   Symbol  |\n");
                fprintf(out, "|-------------------------------------|\n");

                for (unsigned long i = 0; i < dVector.size(); ++i)
                    fprintf(out, "|%11lu |%11f | %11s |\n", i, dVector[i], annotations[i].c_str());
                fprintf(out, "\\------------------------------------/\n");

                if (out == stdout)
                    fclose(out);

                break;
            }
        default:
            cerr << "Could not show output." << endl;
    }
}
