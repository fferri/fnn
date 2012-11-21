#include <iostream>
#include <cctype>
#include <cstring>
#include <string>
#include "words.hpp"

using namespace std;

#define iswhitespace(x) ((x)=='\n'||(x)=='\t'||(x)==' ')

int treshold = 50;

int main(int argc, char **argv) {
	char c;
	string str;
	int tolow = 0;
	bool todel = false;

	if(argc == 2 && !strcmp(argv[1], "-l")) tolow = 1;
	words.readStatsFile();
	 
	while(cin.good()) {
		cin.get(c);

		if(tolow) c = tolower(c);

		if(ispunct(c) || iswhitespace(c)) {
			if(!str.empty() && !todel) {
				if(words.getWordCount(words.getId(str)) >= treshold)
					cout << str << endl;
				else
					todel = true;

				str.clear();
			}
		}

		if(!iswhitespace(c)) {
			str.push_back(c);

			if(ispunct(c)) {
				if(!todel)
					cout << str << endl;

				if(c == '.') todel = false;
				str.clear();
			}
		}
	}

	return 0;
}
