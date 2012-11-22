#include <fann.h>
#include <cstdlib>
#include <unistd.h>
#include "words.hpp"

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
unsigned int epochs_between_reports = 100;

char *training_file = NULL;

void parseOptions(int argc, char *argv[]) {
	int c;
	while((c = getopt(argc, argv, "e:E:")) != -1)
		switch(c) {
		case 'e':
			max_epochs = atol(optarg);
			break;
		case 'E':
			epochs_between_reports = atol(optarg);
			break;
		case '?':
			if(optopt == 'e' || optopt == 'E')
				fprintf(stderr, "Option -%c requires an integer argument.\n", optopt);
			else if(isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			exit(1);
			break;
		default:
			abort();
			break;
		}
	for(int index = optind; index < argc; index++)
		if(training_file) {
			fprintf(stderr, "Specify only one training file!\n");
			exit(1);
		} else {
			training_file = argv[index];
		}
	if(!training_file) {
		fprintf(stderr, "Specify at least one training file!\n");
		exit(1);
	}
}

int main(int argc, char *argv[]) {
	parseOptions(argc, argv);

	struct fann *ann = fann_create_standard(num_layers, num_input, num_neurons_hidden_1, num_neurons_hidden_2, num_output);

	fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

	fann_train_on_file(ann, training_file, max_epochs, epochs_between_reports, desired_error);

	fann_save(ann, "wordPredict.net");

	fann_destroy(ann);

	return 0;
}



