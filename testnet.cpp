#include <cstdlib>
#include "words.hpp"

#include <fann.h>

char *test_file = NULL;

void usage() {
	fprintf(stderr, ""
			"usage: predict [options] <testFile.dat>\n"
			"\n"
			);

}
void parseOptions(int argc, char *argv[]) {
	int c;
	while((c = getopt(argc, argv, "e:")) != -1)
		switch(c) {
		case 'e':
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
		if(test_file) {
			fprintf(stderr, "Specify only one training file!\n");
			usage();
			exit(1);
		} else {
			test_file = argv[index];
		}
	if(!test_file) {
		fprintf(stderr, "Specify at least one training file!\n");
		usage();
		exit(1);
	}
}

int main(int argc, char *argv[]) {
	parseOptions(argc, argv);

	struct fann *ann = fann_create_from_file("wordPredict.net");

	//fann_type *input = (fann_type *)calloc(ann->num_input, sizeof(fann_type));
	fann_train_data * data = fann_read_train_from_file(test_file);
	fann_type* out;

	size_t num_cases = 0, num_errors = 0;
	for(size_t i = 0; i < data->num_data; i++) {
		out = fann_run(ann, data->input[i]);
		num_cases++;
		if(out[0] * data->output[i][0] < 0) num_errors++;

		float err = num_errors * 100.0 / (float)num_cases;
		fprintf(stdout, "\rClassification error: %f%%       ", err);
	}
	fprintf(stdout, "\n");


	fann_destroy(ann);

	return 0;
}


