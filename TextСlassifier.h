#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#define MAX_SCORE 512

#define LOWER_NGRAM_LEN	2
#define UPPER_NGRAM_LEN	5
//Довжина профілів категорій й текстів, що розпізнаються повинна бути однаковою і обмеженою. Наприклад,
#define CATEGORY_LEN 1000
#define UNKNOWN_CATEGORY (CATEGORY_LEN) * (MAX_SCORE)

using namespace std;

/* N-grams frequencies profile */
typedef vector<pair<string, unsigned long>> frequency_vec;
typedef vector<pair<string, unsigned long>>::iterator freq_iterator;
typedef vector<string>::iterator vector_str_iterator;
typedef pair<string, unsigned long> freq_pair;

class TextСlassifier {
public:
	/* Name of category defaults to 'Unknown.' */
	TextСlassifier();

	/*Param classification - name of the category (e.g., "English").    */
	TextСlassifier(const string category);

	/* returns the map N-grams and their frequencies.     */
	frequency_vec freqs() { return _freqs; }

	/* returns the name of the classifier.*/
	string category() { return _category; }

	/*Param classification - name of the category.*/
	void setCategory(string& cat) { _category = cat; }
	void setCategory(const char *str) { _category = str; }

	/*learns the frequencies of N-grams in a corpus.*/
	void learn(istream& in);

	/*dumps the frequencies of N-grams in a corpus.
	File structure is as follows:
	* lines which start with c - comments
	* line which starts with s - classification
	* lines with N-grams start with n  and contain
	* N-gram and its quantity separated with space or tab*/
	void dumpProfile(ostream& out);

	/*learns the frequencies of N-grams from a file created by.
	dumpProfile() function */
	void learnfromDump(istream& in);

	/*returns distance measure between 2 TextСlassifiers*/
	long int score(const TextСlassifier &);

private:
	/*! internal frequency map */
	frequency_vec _freqs;
	
	/*! internal name of classifer */
	string _category;

	vector<string> readFile(istream &in);

	void generateAndAddNGramProfile(string word, int n);
	void transformString(string& word, int n);
	void generateNGram(const string& word, vector<string>& wordNGrams, int n);
	void addNGram(const string& nGram);
	void sort();
	void parse(vector<string>& fileStrings);

	bool isLetterValid(int letterCode);

	freq_iterator find(const string& nGram);
	freq_iterator getEnd() { return _freqs.end(); }

	long int getScore(const string &nGram, int _index);
};