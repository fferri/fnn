#include <cstdlib>
#include <sstream>
#include <unistd.h>
#include "words.hpp"

#include <fann.h>

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

char *training_file = NULL;

const char *net_file = "wordPredict.net";

void usage() {
	fprintf(stderr, ""
			"usage: train [options] <trainingFile.dat>\n"
			"\n"
			"	-e <num epochs>   set the number of max epochs [default: %ld]\n"
			"	-h <num hid 1>    set number of units in 1st hidden layer [default: %ld]\n"
			"	-H <num hid 2>    set number of units in 2nd hidden layer [default: %ld]\n"
			,
			max_epochs, num_neurons_hidden_1, num_neurons_hidden_2);
}

long getVocabularySize() {
	words.readWordsFromFile();
	fprintf(stderr, "vocabulary size is %ld\n", words.size());
	return words.size();
}

long getTrainingInputSize(FILE* f) {
	long a, b, c;
	fscanf(f, "%ld %ld %ld", &a, &b, &c);
	fprintf(stderr, "training header: %ld %ld %ld\n", a, b, c);
	return b;
}

void parseOptions(int argc, char *argv[]) {
	int c;
	while((c = getopt(argc, argv, "e:h:H:")) != -1)
		switch(c) {
		case 'e':
			max_epochs = atol(optarg);
			break;
		case 'h':
			num_neurons_hidden_1 = atoi(optarg);
			break;
		case 'H':
			num_neurons_hidden_2 = atoi(optarg);
			break;
		case '?':
			if(optopt == 'e' || optopt == 'E')
				fprintf(stderr, "Option -%c requires an integer argument.\n", optopt);
			else if(isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			usage();
			exit(1);
			break;
		default:
			abort();
			break;
		}
	for(int index = optind; index < argc; index++)
		if(training_file) {
			fprintf(stderr, "Specify only one training file!\n");
			usage();
			exit(1);
		} else {
			training_file = argv[index];
		}
	if(!training_file) {
		fprintf(stderr, "Specify at least one training file!\n");
		usage();
		exit(1);
	}
}

int main(int argc, char *argv[]) {
	parseOptions(argc, argv);

	if(FILE * file = fopen(training_file, "r")) {
		long sz1 = getTrainingInputSize(file);
		fclose(file);
		long sz2 = num_input_words * getVocabularySize();
		if(sz1 != sz2) {
			fprintf(stderr, "ERROR: training input size does not match vocabulary size!\n");
			exit(1);
		}
		num_input = sz1;
	} else {
		fprintf(stderr, "ERROR: training file does not exists");
		usage();
		exit(1);
	}

	struct fann *ann;
	if(FILE * file = fopen(net_file, "r")) {
		fclose(file);
		fprintf(stderr, "reading existing network file '%s'...\n", net_file);
		ann = fann_create_from_file(net_file);
		if(ann->num_input != num_input) {
			fprintf(stderr, "ERROR: network num_inputs does not match training data and/or vocabulary!\n");
			exit(1);
		}
	} else {
		ann = fann_create_standard(num_layers, num_input, num_neurons_hidden_1, num_neurons_hidden_2, num_output);
	}

	fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

	for(long epoch = 0; epoch < max_epochs; epoch++) {
		fann_train_on_file(ann, training_file, 1, 1, desired_error);
		fann_save(ann, net_file);
	}

	fann_destroy(ann);

	return 0;
}



