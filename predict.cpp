#include <cstdlib>
#include "words.hpp"

#include <fann.h>

char *argv_words[2];

char *net_file = NULL; //"wordPredict.net";

void usage() {
	fprintf(stderr, ""
			"usage: predict [options] <testFile.dat>\n"
			"	-f <file>    use neural network model from file\n"
			"\n"
			);
}

void parseOptions(int argc, char *argv[]) {
	int c;
	while((c = getopt(argc, argv, "f:")) != -1)
		switch(c) {
		case 'f':
			net_file = optarg;
			break;
		case '?':
			if(optopt == 'f')
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
	if(!net_file) {
		fprintf(stderr, "please specify the neural network model file with -f\n\n");
		usage();
		exit(1);
	}
	int wi = 0;
	for(int index = optind; index < argc; index++, wi++) {
		if(wi >= 2) {
			fprintf(stderr, "please specify exactly 2 words\n\n");
			exit(1);
		}
		argv_words[wi] = argv[index];
	}
	if(wi < 1) {
		fprintf(stderr, "please specify exactly 2 words\n\n");
		exit(1);
	}
}

void setWord(fann_type* input_vec, size_t word_i, string word) {
	size_t numWords = words.size();
	size_t wordId = words.getId(word);
	for(size_t i = 0; i < numWords; i++)
		input_vec[word_i * numWords + i] = i == wordId ? 1.0 : 0.0;
}

int main(int argc, char *argv[]) {
	parseOptions(argc, argv);

	words.readWordsFromFile();

	struct fann *ann = fann_create_from_file(net_file);

	size_t numWords = words.size();
	fann_type* out;
	fann_type* in = (fann_type *)calloc(numWords * 3, sizeof(fann_type));

	setWord(in, 0, argv_words[0]);
	setWord(in, 1, argv_words[1]);

	for(size_t i = 0; i < numWords; i++) {
		setWord(in, 2, words.get(i));
		out = fann_run(ann, in);
		fprintf(stdout, "%f %s\n", out[0], words.get(i).c_str());
	}

	fann_destroy(ann);

	return 0;
}


