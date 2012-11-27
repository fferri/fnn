#include <cstdlib>
#include <sstream>
#include <string>
#include <boost/format.hpp>

#include <unistd.h>

#include "words.hpp"
#include "filenames.hpp"

#include <floatfann.h>

using namespace std;
using boost::format;

unsigned int word_local_encoding_size = 724;
unsigned int num_input_words = 3;

unsigned int num_layers = 4;
unsigned int num_input = word_local_encoding_size * num_input_words;
unsigned int num_neurons_hidden_1 = 500 * num_input_words;
unsigned int num_neurons_hidden_2 = 700;
#ifdef SOFTMAX
unsigned int num_output = 2;
#else
unsigned int num_output = 1;
#endif

float desired_error = 0.001;
unsigned int max_epochs = 10000;
unsigned int epochs_between_reports = 1;

string training_filename;
string validation_filename;

string net_filename;
string log_filename;
string words_filename;

ofstream log_file;

void log(string s) {
	cerr << s;
	if(log_file) {
		log_file << s;
		log_file.flush();
	}
}

void usage() {
	cerr
	<< "usage: train [options] <trainingFile.dat>" << endl
	<< endl
	<< "	-f <file>         save trained model to file" << endl
	<< "	-F <file>         check mse against validation file" << endl
	<< "	-d <err value>    desired error value (0..1) [default: " << desired_error << "]" << endl
	<< "	-e <num epochs>   set the number of max epochs [default: " << max_epochs << "]" << endl
	<< "	-E <num epochs>   epochs between reports [default: " << epochs_between_reports << "]" << endl
	<< "	-h <num hid 1>    set number of units in 1st hidden layer [default: " << num_neurons_hidden_1 << "]" << endl
	<< "	-H <num hid 2>    set number of units in 2nd hidden layer [default: " << num_neurons_hidden_2 << "]" << endl
	<< endl;
}

void parseOptions(int argc, char *argv[]) {
	int c;
	while((c = getopt(argc, argv, "e:E:h:H:f:F:d:")) != -1)
		switch(c) {
		case 'e':
			max_epochs = atol(optarg);
			break;
		case 'E':
			epochs_between_reports = atol(optarg);
			break;
		case 'h':
			num_neurons_hidden_1 = atoi(optarg);
			break;
		case 'H':
			num_neurons_hidden_2 = atoi(optarg);
			break;
		case 'f':
			net_filename = optarg;
			break;
		case 'F':
			validation_filename = optarg;
			break;
		case 'd':
			desired_error = atof(optarg);
			break;
		case '?':
			if(optopt == 'e' || optopt == 'E' || optopt == 'h' || optopt == 'H')
				cerr << "option -" << (char)optopt << " requires an integer argument." << endl << endl;
			if(optopt == 'd')
				cerr << "option -" << (char)optopt << " requires a floating point argument." << endl << endl;
			if(optopt == 'f' || optopt == 'F')
				cerr << "option -" << (char)optopt << " requires a file argument." << endl << endl;
			else if(isprint(optopt))
				cerr << "unknown option -" << (char)optopt << "." << endl << endl;
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
#ifndef DONT_AUTOGENERATE_MODEL_FILENAME
		// try to generate a net file name automatically
		cerr << "no net file name (-f) specified. trying to generate on automatically..." << endl;
		int i = 1;
		while(true) {
			net_filename = makeNetFilename(num_neurons_hidden_1, num_neurons_hidden_2, i++).c_str();
			if(!fileExists(net_filename)) break;
		}
		cerr << "  will use '" << net_filename << "' as net filename" << endl;
#else
		cerr << "please specify the neural network model file with -f" << endl << endl;
		usage();
		exit(1);
#endif
	}
	for(int index = optind; index < argc; index++)
		if(!training_filename.empty()) {
			cerr << "specify only one training file." << endl << endl;
			usage();
			exit(1);
		} else {
			training_filename = argv[index];
		}
	if(training_filename.empty()) {
		cerr << "specify a training file." << endl << endl;
		usage();
		exit(1);
	}
}

float get_classification_error_rate(struct fann* ann, fann_train_data* data) {
	size_t d = 0, e = 0;
	for(size_t i = 0; i < data->num_data; i++) {
		d++;
		fann_type* out = fann_run(ann, data->input[i]);
#ifdef SOFTMAX
		if(out[0] < 0.5 && data->output[i][0] > 0.5 ||
		   out[1] > 0.5 && data->output[i][1] < 0.5) e++;
#else
		if(out[0] * data->output[i][0] < 0) e++;
#endif
	}
	return (float)e/(float)d;
}

int main(int argc, char *argv[]) {
	parseOptions(argc, argv);

	words_filename = getWordsFilename(net_filename);
	if(!fileExists(words_filename)) {
		cerr << "error: " << words_filename << " does not exist." << endl;
		exit(1);
	}
	cerr << "reading wordlist from '" << words_filename << "'..." << endl;
	words.readWordsFromFile(words_filename);
	cerr << "  wordlist size is " << words.size() << endl;

	ifstream training_file(training_filename.c_str());
	if(training_file) {
		long a, b, c;
		training_file >> a >> b >> c;
		cerr << "training file header: " << a << " " << b << " " << c << endl;
		training_file.close();
		long sz2 = num_input_words * words.size();
		if(b != sz2) {
			cerr << "ERROR: training input size does not match vocabulary size." << endl;
			exit(1);
		}
		num_input = b;
	} else {
		cerr << "ERROR: training file does not exists." << endl;
		usage();
		exit(1);
	}

	struct fann *ann;
	if(fileExists(net_filename)) {
		cerr << "reading existing network file '" << net_filename << "'..." << endl;
		ann = fann_create_from_file(net_filename.c_str());
		cerr << "  input:   " << ann->num_input << " units" << endl;
		cerr << "  output:  " << ann->num_output << " units" << endl;
		if(ann->num_input != num_input) {
			cerr << "ERROR: network num_inputs does not match training data and/or vocabulary." << endl;
			exit(1);
		}
	} else {
		cerr << "will create a new neural network model file '" << net_filename << "' with " << num_layers << " layers:" << endl;
		cerr << "  input layer:      " << num_input << " units" << endl;
		cerr << "  hidden layer 1:   " << num_neurons_hidden_1 << "  units" << endl;
		cerr << "  hidden layer 2:   " << num_neurons_hidden_2 << "  units" << endl;
		cerr << "  output layer:     " << num_output << "  units" << endl;
		ann = fann_create_standard(num_layers, num_input, num_neurons_hidden_1, num_neurons_hidden_2, num_output);
#ifdef SOFTMAX
		fann_set_activation_function_layer(ann, FANN_SOFTMAX, 3);
#endif
	}

	log_filename = getLogFilename(net_filename);
	cerr << "logging performance to " << log_filename << endl;
	log_file.open(log_filename.c_str(), fstream::out | fstream::app);

	log("starting a new training session.\ncmdline arguments are:");
	for(int i = 0; i < argc; i++)
		log(" "), log(argv[i]);
	log("\n");

	fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

	fann_train_data *train_data = fann_read_train_from_file(training_filename.c_str());
	fann_train_data *validation_data = NULL;
	if(!validation_filename.empty()) validation_data = fann_read_train_from_file(validation_filename.c_str());

	for(long epoch = 0; epoch < max_epochs; epoch++) {
		float pseudo_mse = fann_train_epoch(ann, train_data);

		if(0 == (epoch % epochs_between_reports)) {
			stringstream ss;
			float cer = get_classification_error_rate(ann, train_data);
			if(validation_data) {
				fann_reset_MSE(ann);
				fann_test_data(ann, validation_data);
				float ve = fann_get_MSE(ann);
				float ver = get_classification_error_rate(ann, validation_data);
				ss << "epoch: " << (epoch + 1) << ", pseudoMSE: " << pseudo_mse << ", validationMSE: " << ve << ", classification err: " << cer << ", validation err: " << ver << endl;
				log(ss.str());
			} else {
				ss << "epoch: " << (epoch + 1) << ", pseudoMSE: " << pseudo_mse << ", classification err: " << cer << endl;
				log(ss.str());
			}
		}

		{string cmd = "mv \"" + net_filename + "\" \"" + net_filename + ".old\""; system(cmd.c_str());}

		fann_save(ann, net_filename.c_str());

		{string cmd = "rm \"" + net_filename + ".old\""; system(cmd.c_str());}

		if(pseudo_mse < desired_error) {
			stringstream ss;
			ss << "epoch: " << (epoch + 1) << ", reached desired error. terminating." << endl;
			log(ss.str());
			break;
		}
	}

	fann_destroy(ann);

	return 0;
}



