#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include "words.hpp"

using namespace std;

long numTrainingSamples = 0;

void usage() {
	cout << "makedataset -n <NGRAM_SIZE>" << endl;
	exit(1);
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
	if(argc != 3 || strcmp(argv[1], "-n")) usage();
	int ngramSize = atoi(argv[2]);
	if(ngramSize < 2) usage();

	words.readWordsFromFile();

	string word;

	vector<string> allWords;
	while(cin.good()) {
		getline(cin, word);
		allWords.push_back(word);
	}

	for(int count = 1; count >= 0; count--) {
		vector<string> sentence;

		if(!count) cout << numTrainingSamples << " " << words.size() << " 1" << endl;

		for(vector<string>::iterator i = allWords.begin(); i != allWords.end(); ++i) {
			word = *i;
			sentence.push_back(word);

			if(word == ".") {
				if(sentence.size() >= ngramSize) {
					outputSentenceData(sentence, 1, ngramSize, count);
					sentence.clear();
					makeRandomSentence(sentence, 12);
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


