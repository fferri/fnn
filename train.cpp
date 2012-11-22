#include <fann.h>
#include <cstdlib>
#include "words.hpp"

using namespace std;

int main(int argc, char *argv[]) {
	if(argc < 3) {
		cerr << "usage: train <trainfile.dat> <epochs>" << endl;
		exit(1);
	}
	const unsigned int word_local_encoding_size = 900;
	const unsigned int num_input_words = 3;

	const unsigned int num_layers = 4;
	const unsigned int num_input = word_local_encoding_size * num_input_words;
	const unsigned int num_neurons_hidden_1 = 100 * num_input_words;
	const unsigned int num_neurons_hidden_2 = 1000;
	const unsigned int num_output = 1;

	const float desired_error = (const float) 0.001;
	unsigned int max_epochs = atoi(argv[2]);
	const unsigned int epochs_between_reports = 1000;

	struct fann *ann = fann_create_standard(num_layers, num_input, num_neurons_hidden_1, num_neurons_hidden_2, num_output);

	fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

	struct fann_train_data *data;

	fann_train_on_file(ann, argv[1], max_epochs, epochs_between_reports, desired_error);

	fann_save(ann, "wordPredict.net");

	fann_destroy(ann);

	return 0;
}



