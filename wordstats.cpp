#include <iostream>
#include <string>
#include <cctype>
#include "words.hpp"

using namespace std;

void usage() {
	cerr
	<< "usage: wordstats [options]" << endl
	<< "	-h  shows this message" << endl
	<< endl;
}

void parseOptions(int argc, char *argv[]) {
	int c;
	while((c = getopt(argc, argv, "h")) != -1)
		switch(c) {
		case 'h':
			usage();
			exit(0);
			break;
		case '?':
			if(optopt == 's')
				cerr << "option -" << (char)optopt << " requires an argument." << endl << endl;
			else if(isprint(optopt))
				cerr << "unknown option `-" << (char)optopt << "'." << endl << endl;
			else
				cerr << "unknown option character." << endl << endl;
			usage();
			exit(1);
			break;
		default:
			abort();
			break;
		}
}

int main(int argc, char *argv[]) {
	parseOptions(argc, argv);

    string word;
    while(cin.good()) {
    	getline(cin, word);
    	if(word.length())
    		words.incrCount(word);
    }
    for(size_t i = 0; i < words.size(); i++) {
    	cout << words.getWordCount(i) << " " << words.get(i) << endl;
    }
    return 0;
}


