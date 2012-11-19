#include <iostream>
#include <cctype>
#include <string>
#include <map>

using namespace std;

#define iswhitespace(x) ((x)=='\n'||(x)=='\t'||(x)==' ')

int main(int argc, char **argv) {
	char c;
	string str;

	while(cin.good()) {
		cin.get(c);

		if(ispunct(c) || iswhitespace(c)) {
			if(!str.empty()) {
				cout << str << endl;
				str.clear();
			}
		}

		if(!iswhitespace(c)) {
			str.push_back(c);

			if(ispunct(c)) {
				cout << str << endl;
				str.clear();
			}
		}
	}

	return 0;
}
