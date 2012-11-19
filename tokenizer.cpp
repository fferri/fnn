#include <iostream>
#include <cctype>
#include <string>
#include <map>

using namespace std;

#define iswhitespace(x) ((x)=='\n'||(x)=='\t'||(x)==' ')

void print(string& str, map<string, unsigned int>& DB) {
	map<string, unsigned int>::iterator it;
	cout << str << "\t";
	it = DB.find(str);
	if(it != DB.end())
		it->second++;
	else
		it = DB.insert(it, pair<string, unsigned int>(str, 1));

	cout << it->second << endl;
}

int main(int argc, char **argv) {
    char c;
    string str;
    map<string, unsigned int> DB;

    while(cin.good()) {
	cin.get(c);

        if(ispunct(c) || iswhitespace(c)) {
		if(!str.empty()) {
			print(str, DB);
			str.clear();
		}
	}	
	
	if(!iswhitespace(c)) {
		str.push_back(c);

		if(ispunct(c)) {
			print(str, DB);
			str.clear();
		}
	}
    }

    return 0;
}
