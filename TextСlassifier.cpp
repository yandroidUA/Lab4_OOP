#include "Text—lassifier.h"
#include <algorithm>

Text—lassifier::Text—lassifier() {
	_category = "Unknown";
}

Text—lassifier::Text—lassifier(const string category) {
	_category = category;
}

bool Text—lassifier::isLetterValid(int letterCode) {
	return letterCode != ' ' && letterCode != ',' && letterCode != '\n' && letterCode != -1 && letterCode != '\t'
		&& letterCode != '.' && letterCode != ',' && letterCode != '[' && letterCode != ']' && letterCode != '!'
		&& letterCode != ':' && letterCode != '0' && letterCode != '1' && letterCode != '2' && letterCode != '3'
		&& letterCode != '4' && letterCode != '5' && letterCode != '6' && letterCode != '7' && letterCode != '8'
		&& letterCode != '9';
}

vector<string> Text—lassifier::readFile(istream &in) {
	vector<string> fileStrings;

	string buffer;

	while (!in.eof()) {
		int letter = in.get();

		if (!isLetterValid(letter)) {
			if (!buffer.empty()) {
				fileStrings.push_back(buffer);
				//cout << "Readed: " << buffer << endl;
				buffer = "";
			}
			continue;
		}

		buffer += letter;
	}

	if (!buffer.empty()) {
		fileStrings.push_back(buffer);
		buffer = "";
	}

	return fileStrings;
}

freq_iterator Text—lassifier::find(const string& nGram) {
	freq_iterator freq_iter;

	for (freq_iter = _freqs.begin(); freq_iter != _freqs.end(); freq_iter++) {
		if ((*freq_iter).first == nGram) {
			return freq_iter;
		}
	}
	return freq_iter;
}

void Text—lassifier::transformString(string &word, int n) {
	word.insert(0, "_");
	
	for (int i = 1; i < n; i++) {
		word.append("_");
	}
}

void Text—lassifier::addNGram(const string& nGram) {
	freq_iterator iterator = find(nGram);

	if (iterator == getEnd()) {
		_freqs.push_back(freq_pair(nGram, 1));
		return;
	}

	(*iterator).second += 1;
}

void Text—lassifier::generateNGram(const string& word, vector<string>& wordNGrams, int n) {

	string buffer;
	
	for (int i = 0; i <= word.size() - n; i++) {
		for (int j = i; j < n + i; j++) {
			buffer += word[j];
		}
		wordNGrams.push_back(buffer);
		buffer = "";
	}
}

void Text—lassifier::generateAndAddNGramProfile(string word, int n) {
	transformString(word, n);

	vector<string> wordNGrams;
	generateNGram(word, wordNGrams, n);
	
	for (vector_str_iterator iter = wordNGrams.begin(); iter != wordNGrams.end(); iter++) {
		addNGram(*iter);
	}

	bool test = true;
}

void Text—lassifier::learn(istream& in) {
	vector<string> fileWords = readFile(in);

	_freqs.clear();

	for (vector_str_iterator iter = fileWords.begin(); iter != fileWords.end(); iter++) {
		generateAndAddNGramProfile(*iter, 2);
		generateAndAddNGramProfile(*iter, 3);
		generateAndAddNGramProfile(*iter, 4);
		generateAndAddNGramProfile(*iter, 5);
	}

	sort();
}

void Text—lassifier::dumpProfile(ostream& out) {

	out << "s:\t" << _category << endl;

	for (freq_iterator iter = _freqs.begin(); iter != _freqs.end(); iter++) {
		out << "n: " << (*iter).first << "\t" << (*iter).second << endl;
	}

}

void Text—lassifier::sort() {
	std::sort(_freqs.begin(), _freqs.end(), [](const freq_pair &a, const freq_pair &b) {
		return a.second > b.second;
	});
}

void Text—lassifier::parse(vector<string>& fileStrings) {

	bool nextNGram = false;
	bool nextCategory = false;
	bool nextComment = false;

	for (vector_str_iterator iter = fileStrings.begin(); iter != fileStrings.end(); iter++) {
		if ((*iter) == "n") {
			nextNGram = true;
			nextCategory = false;
			nextComment = false;
			continue;
		}

		if ((*iter) == "s") {
			nextNGram = false;
			nextCategory = true;
			nextComment = false;
			continue;
		}

		if ((*iter) == "c") {
			nextNGram = false;
			nextCategory = false;
			nextComment = true;
			continue;
		}

		if (nextNGram) {
			addNGram(*iter);
			nextNGram = false;
			nextCategory = false;
			nextComment = false;
			continue;
		}

		if (nextCategory) {
			setCategory(*iter);
			nextNGram = false;
			nextCategory = false;
			nextComment = false;
		}
	}
}

void Text—lassifier::learnfromDump(istream& in) {
	vector<string> filesString = readFile(in);

	_freqs.clear();
	_category = "Unknown";

	parse(filesString);
	sort();
}

long int Text—lassifier::getScore(const string &nGram, int _index) {
	int index = 0;

	for (freq_iterator iter = _freqs.begin(); index < CATEGORY_LEN && iter != _freqs.end(); iter++) {
		if ((*iter).first == nGram) {
			return (index > _index) ? index - _index : _index - index;
		}

		index++;
	}

	return MAX_SCORE;
}

long int Text—lassifier::score(const Text—lassifier &textClassifier) {
	int index = 0;
	long int result = 0;
	
	for (freq_iterator iter = _freqs.begin(); index < CATEGORY_LEN && iter != _freqs.end(); iter++) {
		result += const_cast<Text—lassifier&>(textClassifier).getScore((*iter).first, index);
		index++;
	}

	return result;
}