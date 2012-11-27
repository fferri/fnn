#include <cstdlib>
#include <iostream>
#include <string>

#include "words.hpp"

#include <floatfann.h>

using namespace std;

string test_filename;

string net_filename;

void usage() {
	cerr
	<< "usage: testnet [options] <testFile.dat>" << endl
	<< "	-f <file>    use neural network model from file" << endl
	<< endl;

}
void parseOptions(int argc, char *argv[]) {
	int c;
	while((c = getopt(argc, argv, "f:")) != -1)
		switch(c) {
		case 'f':
			net_filename = optarg;
			break;
		case '?':
			if(optopt == 'f')
				cerr << "option -" << (char)optopt << " requires a file argument." << endl << endl;
			else if(isprint(optopt))
				cerr << "unknown option `-" << (char)optopt << "'." << endl << endl;
			else
				cerr << "unknown option character." << endl << endl;
			usage();
			exit(1);
			break;
		default:
			abort();
			break;
		}
	if(net_filename.empty()) {
		cerr << "specify the neural network model file with -f" << endl << endl;
		usage();
		exit(1);
	}
	for(int index = optind; index < argc; index++)
		if(!test_filename.empty()) {
			cerr << "specify only one training file!" << endl << endl;
			usage();
			exit(1);
		} else {
			test_filename = argv[index];
		}
	if(test_filename.empty()) {
		cerr << "specify at least one training file!" << endl << endl;
		usage();
		exit(1);
	}
}

int main(int argc, char *argv[]) {
	parseOptions(argc, argv);

	struct fann *ann = fann_create_from_file(net_filename.c_str());

	fann_train_data * data = fann_read_train_from_file(test_filename.c_str());
	fann_type* out;

	size_t num_cases = 0, num_errors = 0;
	for(size_t i = 0; i < data->num_data; i++) {
		out = fann_run(ann, data->input[i]);
		num_cases++;
#ifdef SOFTMAX
		if(out[0] < 0.5 && data->output[i][0] > 0.5 ||
		   out[1] > 0.5 && data->output[i][1] < 0.5) num_errors++;
#else
		if(out[0] * data->output[i][0] < 0) num_errors++;
#endif
		float err = num_errors * 100.0 / (float)num_cases;
		cerr << "\rClassification error: " << err << "%       ";
	}
	cerr << endl;


	fann_destroy(ann);

	return 0;
}


