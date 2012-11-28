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

string words_filename;

void usage() {
	cerr
	<< "usage: makedataset [options]" << endl
	<< endl
	<< "	-n <num>    N-gram size [default: " << ngramSize << "]" << endl
	<< "	-f <file>   wordlist filename" << endl
	<< endl;
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
				cerr << "option -" << (char)optopt << " requires an argument." << endl << endl;
			else if(isprint(optopt))
				cerr << "unknown option `-" << (char)optopt << "'." << endl << endl;
			else
				cerr << "unknown option character." << endl << endl;
			usage();
			exit(1);
			break;
		default:
			abort();
			break;
		}
	if(words_filename.empty()) {
		cerr << "specify the words filename." << endl << endl;
		usage();
		exit(1);
	}
	if(!fileExists(words_filename)) {
		cerr << "error: words filename does not exist." << endl << endl;
		exit(1);
	}
	if(ngramSize < 3) {
		cerr << "error: invalid ngram size: " << ngramSize << endl << endl;
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
	sentence.push_back(".");
}

void outputSentenceData(vector<string>& sent, int target, int ngramSize, int count) {
	for(int i = 0; i < (sent.size() - ngramSize + 1); i++) {
		if(count) {
			numTrainingSamples++;
		} else {
			for(int j = 0; j < ngramSize; j++) {
				cout << (j ? " " : "") << words.get1ofNWordEncoding(words.getId(sent[i+j]));
#if PARTITION_NET
				cout << " 1"; // bias
#endif
			}
#ifdef SOFTMAX
			cout << endl << ((target > 0) ? "1 0" : "0 1") << endl;
#else
			cout << endl << target << endl;
#endif
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

		if(!count) {
			unsigned int input_size = ngramSize * words.size();
			unsigned int output_size = 1;
#ifdef PARTITION_NET
			input_size += ngramSize;
#endif

#ifdef SOFTMAX
			output_size = 2;
#endif
            cout << numTrainingSamples << " " << input_size << " " << output_size << endl;
        }

		for(vector<string>::iterator i = allWords.begin(); i != allWords.end(); ++i) {
			word = *i;
			sentence.push_back(word);

			if(word == ".") {
				if(sentence.size() >= ngramSize) {
					outputSentenceData(sentence, 1, ngramSize, count);
					sentence.clear();
					makeRandomSentence(sentence, 30);
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


