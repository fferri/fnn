#include <iostream>
#include <cctype>
#include <cstring>
#include <string>
#include "words.hpp"
#include <unistd.h>

using namespace std;

#define iswhitespace(x) ((x)=='\n'||(x)=='\t'||(x)==' ')

long skipSent = 0;
long numSent = -1;
int tolow = 0;

void parseOptions(int argc, char *argv[]) {
	char *cvalue = NULL;
	int index;
	int c;

	opterr = 0;

	while((c = getopt(argc, argv, "s:n:l")) != -1)
		switch(c) {
		case 's':
			skipSent = atoi(optarg);
			break;
		case 'n':
			numSent = atoi(optarg);
			break;
		case 'l':
			tolow = 1;
			break;
		case '?':
			if(optopt == 's' || optopt == 'n')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
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

	//printf("aflag = %d, bflag = %d, cvalue = %s\n", aflag, bflag, cvalue);

	//for(index = optind; index < argc; index++)
		//printf("Non-option argument %s\n", argv[index]);
	//return 0;
}

int main(int argc, char **argv) {
	char c;
	string str;
	bool todel = false;

	long n = 0;
	bool print = true;
	words.readStatsFile();
	 
	parseOptions(argc, argv);

	while(cin.good()) {
		cin.get(c);

		print = (n >= skipSent) && (numSent == -1 || n < (skipSent + numSent));

		if(tolow) c = tolower(c);

		if(ispunct(c) || iswhitespace(c)) {
			if(!str.empty()) {
				if(print) cout << str << endl;
				str.clear();
			}
		}

		if(!iswhitespace(c)) {
			str.push_back(c);

			if(ispunct(c)) {
				if(print) cout << str << endl;
				str.clear();

				if(c == '.') n++;
			}
		}
	}

	return 0;
}
