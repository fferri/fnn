#include <iostream>
#include <cctype>
#include <cstring>
#include <string>
#include <vector>
#include "words.hpp"
#include <unistd.h>

using namespace std;

#define iswhitespace(x) ((x)=='\n'||(x)=='\t'||(x)==' ')

// sentences to skip at beginning
long skipSent = 0;

// sentences to output and then stop
long numSent = 0;

// convert to lowercase
int tolow = 0;

// minimum word frequency treshold for not discarding sentence
int treshold = 0;

void parseOptions(int argc, char *argv[]) {
	char *cvalue = NULL;
	int index;
	int c;

	opterr = 0;

	while((c = getopt(argc, argv, "s:n:t:l")) != -1)
		switch(c) {
		case 's':
			skipSent = atol(optarg);
			break;
		case 'n':
			numSent = atol(optarg);
			break;
		case 'l':
			tolow = 1;
			break;
        case 't':
            treshold = atoi(optarg);
            break;
		case '?':
			if(optopt == 's' || optopt == 'n' || optopt == 't')
				fprintf(stderr, "Option -%c requires an integer argument.\n", optopt);
			else if(isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			exit(1);
			break;
		default:
			abort();
			break;
		}
}

int main(int argc, char **argv) {
	char c;
	string str;
	vector<string> sent;
	bool todel = false;

	parseOptions(argc, argv);

	long n = -skipSent;
	if(treshold > 0)
		words.readStatsFile();

	while(cin.good()) {
		cin.get(c);

		if(tolow) c = tolower(c);

		if(ispunct(c) || iswhitespace(c)) {
			if(!str.empty()) {
                if(words.getWordCount(words.getId(str)) < treshold)
                	todel = true;

                sent.push_back(str);
				str.clear();
			}
		}

		if(!iswhitespace(c)) {
			str.push_back(c);

			if(ispunct(c)) {
				sent.push_back(str);
				str.clear();

				if(c == '.') {
					if(!todel) {
						if(n >= 0) {
							for(vector<string>::iterator i = sent.begin(); i != sent.end(); i++)
								cout << *i << endl;
						}
						n++;
					}

					sent.clear();
                    todel = false;
                }
			}
		}

		if(n >= numSent && numSent > 0) break;
	}

	return 0;
}
