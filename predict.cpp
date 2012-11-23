#include <cstdlib>
#include "words.hpp"

#include <fann.h>

char *argv_words[2];

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
	int wi = 0;
	for(int index = optind; index < argc; index++, wi++) {
		if(wi >= 2) {
			fprintf(stderr, "please specify exactly 2 words\n");
			exit(1);
		}
		argv_words[wi] = argv[index];
	}
	if(wi < 1) {
		fprintf(stderr, "please specify exactly 2 words\n");
		exit(1);
	}
}

int main(int argc, char *argv[]) {
	parseOptions(argc, argv);

	words.readWordsFromFile();

	struct fann *ann = fann_create_from_file("wordPredict.net");

	size_t numWords = words.size();
	fann_type* out;
	fann_type* in = (fann_type *)calloc(numWords * 3, sizeof(fann_type));

	size_t wid0 = words.getId(argv_words[0]),
			wid1 = words.getId(argv_words[1]);
	in[wid0] = 1;
	in[numWords + wid1] = 1;

	for(size_t i = 0; i < words.size(); i++) {
		for(size_t j = 0; j < words.size(); j++) in[numWords * 2 + j] = 0;
		in[numWords * 2 + i] = 1;

		out = fann_run(ann, in);
		fprintf(stdout, "%f %s\n", out[0], words.get(i).c_str());
	}

	fann_destroy(ann);

	return 0;
}


