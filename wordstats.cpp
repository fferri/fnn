#include <iostream>
#include <string>
#include <cctype>
#include "words.hpp"

using namespace std;

int main(int argc, char **argv) {
    string word;
    while(cin.good()) {
    	getline(cin, word);
    	words.incrCount(word);
    }
    for(size_t i = 0; i < words.size(); i++) {
    	cout << words.getWordCount(i) << " " << words.get(i) << endl;
    }
    return 0;
}


