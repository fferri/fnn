#include <string>

using namespace std;

bool matchDigits(string& s, size_t start = 0, size_t end = 0) {
	if(!end) end = s.length();
	for(size_t i = start; i < end; i++)
		if(!isdigit(s[i])) return false;
	return true;
}

bool isNumber(string& s) {
	return matchDigits(s);
}

bool isOrdinal(string& s) {
	if(s.length() < 3) return false;
	if(!matchDigits(s, 0, s.length() - 2)) return false;
	string last2 = s.substr(s.length() - 2);
	char l3 = s[s.length() - 3];
	if(last2 == "th") return true;
	if(last2 == "st" && l3 == '1') return true;
	if(last2 == "nd" && l3 == '2') return true;
	if(last2 == "rd" && l3 == '3') return true;
	return false;
}

string performSubstitutions(string word) {
	if(isOrdinal(word)) return "NUM_ORDINAL";
	if(isNumber(word)) return "NUM_CARDINAL";
	return word;
}
