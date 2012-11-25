#include <iostream>
#include <cctype>
#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>

#include "words.hpp"
#include "filenames.hpp"
#include "substwords.hpp"

using namespace std;

#define iswhitespace(x) ((x)=='\n'||(x)=='\t'||(x)==' ')

long skipSent = 0;
long numSent = 0;

bool tolow = false;

int treshold = 0;

const char *stats_filename = NULL;

void usage() {
	fprintf(stderr, ""
			"usage: tokenizer [options]\n"
			"\n"
			"	-s <num>    skip <num> sentences at beginning\n"
			"	-n <num>    take only <num> sentences\n"
			"	-l          convert everything to lowercase\n"
			"	-t <num>    minimum word count threshold for including sentence\n"
			"	-f <file>   stats filename\n"
			"	-h          shows this message\n"
			"\n"
			);
}

void parseOptions(int argc, char *argv[]) {
	int c;
	while((c = getopt(argc, argv, "hs:n:t:lf:")) != -1)
		switch(c) {
		case 'h':
			usage();
			exit(0);
			break;
		case 's':
			skipSent = atol(optarg);
			break;
		case 'n':
			numSent = atol(optarg);
			break;
		case 'l':
			tolow = true;
			break;
        case 't':
            treshold = atoi(optarg);
            break;
        case 'f':
        	stats_filename = optarg;
        	break;
		case '?':
			if(optopt == 's' || optopt == 'n' || optopt == 't')
				fprintf(stderr, "option -%c requires an integer argument.\n\n", optopt);
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
	if(treshold > 0) {
		if(!stats_filename) {
			fprintf(stderr, "specify the stats filename\n\n");
			usage();
			exit(1);
		} else if(!fileExists(stats_filename)) {
			fprintf(stderr, "error: stats file does not exist\n\n");
			exit(1);
		}
	} else if(stats_filename) {
		fprintf(stderr, "info: stats filename not needed without the -t option\n\n");
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
		words.readStatsFile(stats_filename);

	while(cin.good()) {
		cin.get(c);

		// filter.cpp stuff moved here:
        if(!(isalnum(c) || ispunct(c) || iswhitespace(c))) continue;

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
								cout << performSubstitutions(*i) << endl;
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
