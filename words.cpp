#include "words.hpp"

using namespace std;

Words::Words() {
	readWordsFromFile(Words::filename);
}

void Words::add(long id, string& word) {
	if(contains(s)) {
		cerr << "warning: duplicate word: " << word << endl;
		return;
	}
	map[id] = word;
	rmap[word] = id;
}

void Words::readWordsFromFile(string& filename) {
	ifstream in(filename);
	string s;
	while(in.good()) {
		in >> s;
		add(newId(), s);
	}
}

Words words = new Words();

// test:

int main(int argc, char *argv[]) {
	cout << "that = " << words.getId("that") << endl;
}
