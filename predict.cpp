#include <cstdlib>
#include <utility>
#include <vector>
#include "words.hpp"

#include <fann.h>

using namespace std;

char *argv_words[2];

char *net_file = NULL;
int max_results = 0;

struct sort_score {
    bool operator()(const std::pair<size_t,double> &left, const std::pair<size_t,double> &right) {
        return left.second > right.second;
    }
};

void usage() {
	fprintf(stderr, ""
			"usage: predict [options] <word1> <word2>\n"
			"	-f <file>    use neural network model from file\n"
			"	-n <num>     output at most <num> reults\n"
			"\n"
			);
}

void parseOptions(int argc, char *argv[]) {
	int c;
	while((c = getopt(argc, argv, "f:n:")) != -1)
		switch(c) {
		case 'f':
			net_file = optarg;
			break;
		case 'n':
			max_results = atoi(optarg);
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
			usage();
			exit(1);
		}
		argv_words[wi] = argv[index];
	}
	if(wi < 1) {
		fprintf(stderr, "please specify exactly 2 words\n\n");
		usage();
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

	std::vector<std::pair<size_t, double> > score;

	for(size_t i = 0; i < numWords; i++) {
		setWord(in, 2, words.get(i));
		out = fann_run(ann, in);
		score.push_back(make_pair(i, out[0]));
	}

	// compute softmax output from logits
	double sum = 0;
	for(size_t i = 0; i < numWords; i++) sum += exp(score[i].second);
	for(size_t i = 0; i < numWords; i++) score[i] = make_pair(score[i].first, exp(score[i].second)/sum);

	sort(score.begin(), score.end(), sort_score());

	// output scores
	for(size_t i = 0; i < score.size(); i++) {
		fprintf(stdout, "%f %s\n", score[i].second, words.get(score[i].first).c_str());
		if(max_results) if(!--max_results) break;
	}

	fann_destroy(ann);

	return 0;
}


