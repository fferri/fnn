#include <cstdlib>
#include <sstream>
#include <unistd.h>

#include "words.hpp"
#include "filenames.hpp"

#include <floatfann.h>

using namespace std;

unsigned int word_local_encoding_size = 724;
unsigned int num_input_words = 3;

unsigned int num_layers = 4;
unsigned int num_input = word_local_encoding_size * num_input_words;
unsigned int num_neurons_hidden_1 = 500 * num_input_words;
unsigned int num_neurons_hidden_2 = 700;
unsigned int num_output = 1;

float desired_error = 0.001;
unsigned int max_epochs = 10000;
unsigned int epochs_between_reports = 1;

char *training_file = NULL;
char *validation_file = NULL;

const char *net_filename = NULL;
const char *log_filename = NULL;
const char *words_filename = NULL;

#define fprintf_x2(fd1, fd2, fmt, ...) fprintf(fd1, fmt, ##__VA_ARGS__), fprintf(fd2, fmt, ##__VA_ARGS__)

void usage() {
	fprintf(stderr, ""
			"usage: train [options] <trainingFile.dat>\n"
			"\n"
			"	-f <file>         save trained model to file\n"
			"	-F <file>         check mse against validation file\n"
			"	-d <err value>    desired error value (0..1) [default: %f]\n"
			"	-e <num epochs>   set the number of max epochs [default: %d]\n"
			"	-E <num epochs>   epochs between reports [default: %d]\n"
			"	-h <num hid 1>    set number of units in 1st hidden layer [default: %d]\n"
			"	-H <num hid 2>    set number of units in 2nd hidden layer [default: %d]\n"
			"\n",
			desired_error, max_epochs, epochs_between_reports, num_neurons_hidden_1, num_neurons_hidden_2);
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
			validation_file = optarg;
			break;
		case 'd':
			desired_error = atof(optarg);
			break;
		case '?':
			if(optopt == 'e' || optopt == 'E' || optopt == 'h' || optopt == 'H')
				fprintf(stderr, "option -%c requires an integer argument.\n\n", optopt);
			if(optopt == 'd')
				fprintf(stderr, "option -%c requires a floating point argument.\n\n", optopt);
			if(optopt == 'f' || optopt == 'F')
				fprintf(stderr, "option -%c requires a file argument.\n\n", optopt);
			else if(isprint(optopt))
				fprintf(stderr, "unknown option `-%c'.\n\n", optopt);
			else
				fprintf(stderr, "unknown option character `\\x%x'.\n\n", optopt);
			usage();
			exit(1);
			break;
		default:
			abort();
			break;
		}
	if(!net_filename) {
#ifndef DONT_AUTOGENERATE_MODEL_FILENAME
		// try to generate a net file name automatically
		fprintf(stderr, "no net file name (-f) specified. trying to generate on automatically...\n");
		int i = 1;
		while(true) {
			net_filename = makeNetFilename(num_neurons_hidden_1, num_neurons_hidden_2, i++).c_str();
			if(!fileExists(net_filename)) break;
		}
#else
		fprintf(stderr, "please specify the neural network model file with -f\n\n");
		usage();
		exit(1);
#endif
	}
	for(int index = optind; index < argc; index++)
		if(training_file) {
			fprintf(stderr, "specify only one training file!\n\n");
			usage();
			exit(1);
		} else {
			training_file = argv[index];
		}
	if(!training_file) {
		fprintf(stderr, "specify at least one training file!\n\n");
		usage();
		exit(1);
	}
}

float get_classification_error_rate(struct fann* ann, fann_train_data* data) {
	size_t d = 0, e = 0;
	for(size_t i = 0; i < data->num_data; i++) {
		d++;
		fann_type* out = fann_run(ann, data->input[i]);
		if(out[0] * data->output[i][0] < 0) e++;
	}
	return (float)e/(float)d;
}

int main(int argc, char *argv[]) {
	parseOptions(argc, argv);

	words_filename = getWordsFilename(net_filename).c_str();
	if(!fileExists(words_filename)) {
		cerr << "error: " << words_filename << " does not exist" << endl;
		exit(1);
	}
	words.readWordsFromFile(words_filename);
	fprintf(stderr, "vocabulary size is %ld\n", words.size());

	if(FILE * file = fopen(training_file, "r")) {
		long a, b, c;
		fscanf(file, "%ld %ld %ld", &a, &b, &c);
		fprintf(stderr, "training header: %ld %ld %ld\n", a, b, c);
		fclose(file);
		long sz2 = num_input_words * words.size();
		if(b != sz2) {
			fprintf(stderr, "ERROR: training input size does not match vocabulary size!\n");
			exit(1);
		}
		num_input = b;
	} else {
		fprintf(stderr, "ERROR: training file does not exists");
		usage();
		exit(1);
	}

	struct fann *ann;
	if(FILE * file = fopen(net_filename, "r")) {
		fclose(file);
		fprintf(stderr, "reading existing network file '%s'...\n", net_filename);
		ann = fann_create_from_file(net_filename);
		fprintf(stderr, "  input:   %d units\n", ann->num_input);
		fprintf(stderr, "  output:  %d units\n", ann->num_output);
		if(ann->num_input != num_input) {
			fprintf(stderr, "ERROR: network num_inputs does not match training data and/or vocabulary!\n");
			exit(1);
		}
	} else {
		fprintf(stderr, "will create a new neural network model file '%s' with %d layers:\n"
				"  input layer:      %d units\n"
				"  hidden layer 1:   %d units\n"
				"  hidden layer 2:   %d units\n"
				"  output layer:     %d units\n",
				net_filename, num_layers, num_input, num_neurons_hidden_1, num_neurons_hidden_2, num_output);
		ann = fann_create_standard(num_layers, num_input, num_neurons_hidden_1, num_neurons_hidden_2, num_output);
	}

	log_filename = getLogFilename(net_filename).c_str();
	fprintf(stderr, "logging performance to %s\n", log_filename);
	FILE *logfile = fopen(log_filename, "a");

	fprintf_x2(stderr, logfile, "starting a new training session.\ncmdline arguments are:");
	for(int i = 0; i < argc; i++)
		fprintf_x2(stderr, logfile, " %s", argv[i]);
	fprintf_x2(stderr, logfile, "\n");
	fflush(logfile);

	fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

	fann_train_data *train_data = fann_read_train_from_file(training_file);
	fann_train_data *validation_data = NULL;
	if(validation_file) validation_data = fann_read_train_from_file(validation_file);

	for(long epoch = 0; epoch < max_epochs; epoch++) {
		float pseudo_mse = fann_train_epoch(ann, train_data);

		if(0 == (epoch % epochs_between_reports)) {
			float cer = get_classification_error_rate(ann, train_data);
			if(validation_data) {
				fann_reset_MSE(ann);
				fann_test_data(ann, validation_data);
				float ve = fann_get_MSE(ann);
				float ver = get_classification_error_rate(ann, validation_data);
				fprintf_x2(stderr, logfile, "epoch: %ld, pseudoMSE: %f, validationMSE: %f, classification err: %f, validation err: %f\n", (1+epoch), pseudo_mse, ve, cer, ver);
				fflush(logfile);
			} else {
				fprintf_x2(stderr, logfile, "epoch: %ld, pseudoMSE: %f, classification err: %f\n", (1+epoch), pseudo_mse, cer);
				fflush(logfile);
			}
		}
		fann_save(ann, net_filename);

		if(pseudo_mse < desired_error) {
			fprintf_x2(stderr, logfile, "epoch: %ld, reached desired error. terminating.\n", (1+epoch));
			break;
		}
	}

	fann_destroy(ann);

	return 0;
}



