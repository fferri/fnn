#ifndef WORDS_HPP_
#define WORDS_HPP_

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdlib>

using std::string;
using std::map;

class Words {
public:
	Words();

	inline long getId(string word) {return rmap[word];}

	inline string get(size_t id) {return fmap[id];}

	inline size_t size() {return fmap.size();}

	inline bool contains(size_t wordId) {return fmap.count(wordId);}
	inline bool contains(string word) {return rmap.count(word);}

	inline string randWord() {return fmap[rand() % fmap.size()];}

	// used only when computing word counts, calls add() as well:
	void incrCount(string word);
	inline size_t getWordCount(size_t id) {return counts[id];}

	void readWordsFromFile(string filename = "commonWords.txt");
	void readStatsFile(string filename = "stats.txt");

	string get1ofNWordEncoding(size_t wordId);
	size_t getWordIdFrom1ofNEncoding(string s);

protected:
	void add(size_t id, string word);

	// id to word map
	map<size_t, string> fmap;

	// inverse mapping
	map<string, size_t> rmap;

	// word counts
	map<size_t, size_t> counts;
};

extern Words words;

#endif /* WORDS_HPP_ */
