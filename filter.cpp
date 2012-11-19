#include <fstream>
#include <iostream>
#include <cctype>

using namespace std;

#define iswhitespace(x) ((x) == '\n' || (x) == '\t' || (x) == ' ')

int main(int argc, char **argv) {
	char c;
    int space = 0;
    while(cin.good()) {
        cin.get(c);
        if(isalnum(c) || ispunct(c) || iswhitespace(c)) {
            if(iswhitespace(c)) c = ' ';
            if(c != ' ' || !space) cout << c;
            space = (c == ' ');
        }
	}
    return 0;
}
