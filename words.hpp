#ifndef WORDS_HPP_
#define WORDS_HPP_

#include <map>
#include <string>
#include <iostream>
#include <fstream>

using std::string;
using std::map;

class Words {
public:
	Words();
	inline long getId(string& word) {return rmap[word];}
	inline string get(long id) {return map[id];}
	inline size_t size() {return map.size();}
	inline bool contains(string& word) {return rmap.count(word);}

protected:
	void add(size_t id, string& word);
	void readWordsFromFile(string& filename);

	// id to word map
	map<size_t, string> map;

	// inverse mapping
	map<string, size_t> rmap;

	static const string filename = "commonWords.txt";
};

extern Words words;

#endif /* WORDS_HPP_ */
