#include "words.hpp"

using namespace std;

Words::Words() {
}

void Words::add(size_t id, string word) {
	if(contains(word)) {
		cerr << "warning: duplicate word: " << word << endl;
		return;
	}
	fmap[id] = word;
	rmap[word] = id;
}

void Words::readWordsFromFile(string filename) {
	ifstream in(filename.c_str());
	string s;
	while(in.good()) {
		in >> s;
		add(fmap.size(), s);
	}
}

void Words::incrCount(string word) {
	size_t wordId;
	if(!contains(word)) {
		wordId = fmap.size();
		add(wordId, word);
		counts[wordId] = 1;
	} else {
		wordId = getId(word);
		counts[wordId]++;
	}
}

void Words::readStatsFile(string filename) {
	ifstream in(filename.c_str());
	int stat;
	string word;
	size_t wordId;

	while(in.good()) {
		in >> stat;
		in >> word;
		wordId = fmap.size();
		add(wordId, word);
		counts[wordId] = stat;
	}
}

string Words::get1ofNWordEncoding(size_t wordId) {
	string s = "";
	for(size_t i = 0; i < size(); i++) {
		if(i) s+= " ";
		s += (i == wordId);
	}
	return s;
}

size_t Words::getWordIdFrom1ofNEncoding(string s) {
	stringstream ss(s);
	string t;
	size_t wordId = 0;
	while(ss >> t) {
		wordId++;
		if(t == "1") return wordId;
	}
	return fmap.size();
}

Words words;

