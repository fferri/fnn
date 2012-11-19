#include <fstream>
#include <iostream>
#include <cctype>

using namespace std;

typedef istreambuf_iterator<char> buf_iter;

int map[256] = {0};
int numChars;

void initTable() {
    numChars = 0;
    for(unsigned char c = 0; c <= 127; c++) {
        if(isalnum(c) || ispunct(c) || c == ' ')
            map[c] = ++numChars;
    }
}

void makeVector(int i, ofstream& out) {
    for(int j = 1; j <= numChars; j++) {
        if(j > 1) out << " ";
        out << ((j == i) ? "1" : "0");
    }
    out << endl;
}

int main(int argc, char **argv) {
    initTable();
	char c;
    int space = 0;
    int first = 1;
    while(cin.good()) {
        cin.get(c);
        if(isalnum(c) || ispunct(c) || c == ' ' || c == '\n') {
            if(c == '\n') c = ' ';
            if(c != ' ' || !space) {
                cout << c;
            }
            if(c == ' ') space = 1;
            else space = 0;
        }
	}
    return 0;
}
