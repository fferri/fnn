#include <fann.h>
#include "words.hpp"

int main() {
	struct fann *ann = fann_create_from_file("wordPredict.net");

	fann_destroy(ann);

	return 0;
}


