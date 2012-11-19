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
	inline long getId(string word) {return rmap[word];}
	inline string get(long id) {return fmap[id];}
	inline size_t size() {return fmap.size();}
	inline bool contains(string word) {return rmap.count(word);}

	// used only when computing word counts, calls add() as well:
	void incrCount(size_t wordId);
	inline void incrCount(string word) {incrCount(getId(word));}

protected:
	void add(size_t id, string word);
	void readWordsFromFile(string filename);

	// id to word map
	map<size_t, string> fmap;

	// inverse mapping
	map<string, size_t> rmap;

	// word counts <word_id, count>
	map<size_t, size_t> counts;
};

extern Words words;

#endif /* WORDS_HPP_ */
