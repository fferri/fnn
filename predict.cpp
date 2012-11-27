#include <iostream>
#include <cstdlib>
#include <utility>
#include <algorithm>
#include <string>
#include <vector>

#include "words.hpp"
#include "filenames.hpp"

#include <floatfann.h>

using namespace std;

vector<string> argv_words;

string net_filename;
string words_filename;

int max_results = 0;

struct sort_score {
    bool operator()(const std::pair<size_t,double> &left, const std::pair<size_t,double> &right) {
        return left.second > right.second;
    }
};

void usage() {
	cerr
	<< "usage: predict [options] <word1> <word2> ..." << endl
	<< "	-f <file>    use neural network model from file" << endl
	<< "	-n <num>     output at most <num> reults" << endl
	<< endl;
}

void parseOptions(int argc, char *argv[]) {
	int c;
	while((c = getopt(argc, argv, "f:n:")) != -1)
		switch(c) {
		case 'f':
			net_filename = optarg;
			break;
		case 'n':
			max_results = atoi(optarg);
			break;
		case '?':
			if(optopt == 'f')
				cerr << "option -%c requires a file argument." << endl << endl;
			else if(isprint(optopt))
				cerr << "unknown option `-%c'." << endl << endl;
			else
				cerr << "unknown option character `\\x%x'." << endl << endl;
			usage();
			exit(1);
			break;
		default:
			abort();
			break;
		}
	if(net_filename.empty()) {
		cerr << "specify the neural network model file with -f" << endl << endl;
		usage();
		exit(1);
	}
	if(!fileExists(net_filename)) {
		cerr << "error: net file does not exist" << endl << endl;
		exit(1);
	}
	int wi = 0;
	for(int index = optind; index < argc; index++, wi++) {
		if(wi >= 2) {
			cerr << "specify exactly 2 words" << endl << endl;
			usage();
			exit(1);
		}
		argv_words.push_back(argv[index]);
	}
	if(wi < 1) {
		cerr << "specify exactly 2 words" << endl << endl;
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

	words_filename = getWordsFilename(net_filename).c_str();
	if(!fileExists(words_filename)) {
		cerr << "error: words filename '" << words_filename << "' does not exist" << endl << endl;
		exit(1);
	}
	words.readWordsFromFile(words_filename);

	for(int i = 0; i < argv_words.size(); i++)
		if(!words.contains(argv_words[i])) {
			cerr << "error: word " << argv_words[i] << " not in wordlist" << endl;
			exit(1);
		}

	struct fann *ann = fann_create_from_file(net_filename.c_str());

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

#ifndef SOFTMAX
	// compute softmax output from logits
	double sum = 0;
	for(size_t i = 0; i < numWords; i++) sum += exp(score[i].second);
	for(size_t i = 0; i < numWords; i++) score[i] = make_pair(score[i].first, exp(score[i].second)/sum);
#endif

	sort(score.begin(), score.end(), sort_score());

	// output scores
	for(size_t i = 0; i < score.size(); i++) {
		cerr << score[i].second << " " << words.get(score[i].first).c_str() << endl;
		if(max_results) if(!--max_results) break;
	}

	fann_destroy(ann);

	return 0;
}


