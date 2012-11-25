#include "filenames.hpp"

#include <fstream>
#include <boost/format.hpp>

string stripSuffix(string s, string suf) {
	auto pos = s.rfind(suf);
	if(pos == string::npos) return s;
	else return s.substr(0, pos);
}

string getLogFilename(string net_file) {
	return stripSuffix(net_file, ".net") + ".log";
}

string getWordsFilename(string net_file) {
	return stripSuffix(net_file, ".net") + ".words";
}

string makeNetFilename(int hid1, int hid2, int seq) {
	return str(boost::format("wordpredict-h%1%-H%2%-%3$04d.net") % hid1 % hid2 % seq);
}

bool fileExists(string filename) {
  std::ifstream ifile(filename.c_str());
  return ifile;
}
