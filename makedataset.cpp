#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <unistd.h>

#include "words.hpp"
#include "filenames.hpp"

using namespace std;

long numTrainingSamples = 0;

int ngramSize = 3;

const char* words_filename = NULL;

void usage() {
	fprintf(stderr, ""
			"usage: makedataset [options]\n"
			"\n"
			"	-n <num>    N-gram size\n"
			"	-f <file>   wordlist filename\n"
			"\n"
			);
}

void parseOptions(int argc, char *argv[]) {
	int c;
	while((c = getopt(argc, argv, "n:f:")) != -1)
		switch(c) {
		case 'n':
			ngramSize = atoi(optarg);
			break;
        case 'f':
        	words_filename = optarg;
        	break;
		case '?':
			if(optopt == 'n' || optopt == 'f')
				fprintf(stderr, "option -%c requires an argument.\n\n", optopt);
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
	if(!words_filename) {
		fprintf(stderr, "specify the words filename\n\n");
		usage();
		exit(1);
	}
	if(!fileExists(words_filename)) {
		fprintf(stderr, "error: words filename does not exist\n\n");
		exit(1);
	}
	if(ngramSize < 3) {
		fprintf(stderr, "error: invalid ngram size: %d\n\n", ngramSize);
		usage();
		exit(1);
	}
}

void makeRandomSentence(vector<string>& sentence, int len) {
	while(len--) {
		string word;
		do {word = words.randWord();} while(word == ".");
		sentence.push_back(word);
	}
}

void outputSentenceData(vector<string>& sent, int target, int ngramSize, int count) {
	for(int i = 0; i < (sent.size() - ngramSize + 1); i++) {
		if(count) {
			numTrainingSamples++;
		} else {
			for(int j = 0; j < ngramSize; j++) {
				cout << (j ? " " : "") << words.get1ofNWordEncoding(words.getId(sent[i+j]));
			}
			cout << endl << target << endl;
		}
	}
}

int main(int argc, char **argv) {
	parseOptions(argc, argv);

	words.readWordsFromFile(words_filename);

	string word;

	vector<string> allWords;
	while(cin.good()) {
		getline(cin, word);
		allWords.push_back(word);
	}

	for(int count = 1; count >= 0; count--) {
		vector<string> sentence;

		if(!count) cout << numTrainingSamples << " " << (ngramSize * words.size()) << " 1" << endl;

		for(vector<string>::iterator i = allWords.begin(); i != allWords.end(); ++i) {
			word = *i;
			sentence.push_back(word);

			if(word == ".") {
				if(sentence.size() >= ngramSize) {
					outputSentenceData(sentence, 1, ngramSize, count);
					sentence.clear();
					makeRandomSentence(sentence, 32);
					outputSentenceData(sentence, -1, ngramSize, count);
					sentence.clear();
				} else {
					sentence.clear();
				}
			}
		}
	}
    return 0;
}


