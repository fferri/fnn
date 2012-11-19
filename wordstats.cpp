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
    return 0;
}


