#include <iostream>
#include <string>
#include <cctype>
#include "words.hpp"

using namespace std;

void usage() {
	fprintf(stderr, ""
			"usage: wordstats [options]\n"
			"	-h  shows this message\n"
			"\n"
			);
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
				fprintf(stderr, "option -%c requires an argument.\n\n", optopt);
			else if(isprint(optopt))
				fprintf(stderr, "unknown option `-%c'.\n\n", optopt);
			else
				fprintf(stderr, "unknown option character `\\x%x'.\n\n", optopt);
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


