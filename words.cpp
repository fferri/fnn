#include "words.hpp"

using namespace std;

Words::Words() {
	readWordsFromFile("commonWords.txt");
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

Words words;

// test:

int main(int argc, char *argv[]) {
	cout << "that = " << words.getId("that") << endl;
}
