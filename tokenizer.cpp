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

string stats_filename;

void usage() {
	cerr
	<< "usage: tokenizer [options]" << endl
	<< endl
	<< "	-s <num>    skip <num> sentences at beginning" << endl
	<< "	-n <num>    take only <num> sentences" << endl
	<< "	-l          convert everything to lowercase" << endl
	<< "	-t <num>    minimum word count threshold for including sentence" << endl
	<< "	-f <file>   stats filename" << endl
	<< "	-h          shows this message" << endl
	<< endl;
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
				cerr << "option -" << (char)optopt << " requires an integer argument." << endl << endl;
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
	if(treshold > 0) {
		if(stats_filename.empty()) {
			cerr << "specify the stats filename" << endl << endl;
			usage();
			exit(1);
		} else if(!fileExists(stats_filename)) {
			cerr << "error: stats file does not exist" << endl << endl;
			exit(1);
		}
	} else if(!stats_filename.empty()) {
		cerr << "info: stats filename not needed without the -t option" << endl << endl;
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
