#ifndef FILENAMES_HPP_
#define FILENAMES_HPP_

#include <string>

using std::string;

string getLogFilename(string net_file);

string getWordsFilename(string net_file);

string makeNetFilename(int hid1, int hid2, int seq);

bool fileExists(const char *filename);

#endif /* FILENAMES_HPP_ */
