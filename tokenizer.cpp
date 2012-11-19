#include <iostream>
#include <cctype>
#include <string>

using namespace std;

#define iswhitespace(x) ((x)=='\n'||(x)=='\t'||(x)==' ')

int main(int argc, char **argv) {
    char c;
    string str;
    bool spaceChecked = false;

    while(cin.good()) {
	cin.get(c);

        if(ispunct(c) || iswhitespace(c)) {
		if(!str.empty()) {
			cout << str << endl;
			str.clear();
		}
		
		if(ispunct(c) || !spaceChecked)
			cout << c << endl;

		if(iswhitespace(c))
			spaceChecked = true;
		else
			spaceChecked = false;
	}
	else {
		str.push_back(c);
		spaceChecked = false;
	}
    }

    return 0;
}
