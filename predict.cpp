#include <fann.h>
#include "words.hpp"

int main() {
	struct fann *ann = fann_create_from_file("wordPredict.net");

	fann_type *input = (fann_type *)calloc(sizeof(fann_type) * 100);

	fann_run(ann, input);

	fann_destroy(ann);

	return 0;
}


