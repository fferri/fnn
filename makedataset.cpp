#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include "words.hpp"

using namespace std;

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

void outputSentenceData(vector<string>& sent, int target, int ngramSize) {
	for(int i = 0; i < (sent.size() - ngramSize + 1); i++) {
		for(int j = 0; j < ngramSize; j++)
			cout << (j ? " " : "") << words.get1ofNWordEncoding(words.getId(sent[i+j]));
		cout << endl << target << endl;
	}
}

int main(int argc, char **argv) {
	if(argc != 3 || strcmp(argv[1], "-n")) usage();
	int ngramSize = atoi(argv[2]);
	if(ngramSize < 2) usage();

	words.readWordsFromFile();

    string word;
    vector<string> sentence;

    while(cin.good()) {
    	getline(cin, word);
    	sentence.push_back(word);

    	if(word == ".") {
    		outputSentenceData(sentence, 1, ngramSize);
    		sentence.clear();
    		makeRandomSentence(sentence, 12);
    		outputSentenceData(sentence, -1, ngramSize);
    		sentence.clear();
    	}
    }
    return 0;
}


