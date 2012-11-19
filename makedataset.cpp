#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include "words.hpp"

using namespace std;

void print1ofNWordEncoding(string& word) {
	size_t wordId = words.getId(word);
	for(size_t i = 0; i < words.size(); i++)
		cout << (i == wordId) << " ";
}

void printTrainingSample(vector<string>& ngram) {
	for(int i = 0; i < ngram.size(); i++)
		print1ofNWordEncoding(ngram[i]);
	cout << endl;
	cout << 1 << endl;
}

void usage() {
	cout << "makedataset -n <NGRAM_SIZE>" << endl;
	exit(1);
}

int main(int argc, char **argv) {
	if(argc != 3 || strcmp(argv[1], "-n")) usage();
	int ngramSize = atoi(argv[2]);
	if(ngramSize < 2) usage();

	vector<string> ngram;
    string word;
    while(cin.good() && ngram.size() < ngramSize) {
    	getline(cin, word);
    	ngram.push_back(word);
    }
    while(cin.good()) {
    	// TODO: do something with the n-gram
    	printTrainingSample(ngram);

    	getline(cin, word);
    	ngram.push_back(word);
    	ngram.erase(ngram.begin(), ngram.begin()+1);
    }
    return 0;
}


