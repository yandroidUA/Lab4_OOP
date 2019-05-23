#include "Text—lassifier.h"
#include <iostream>

using namespace std;

void dumpProfiles() {
	ifstream eng("English.txt");
	ifstream spa("Spanish.txt");
	ifstream fra("France.txt");
	ifstream ita("Italian.txt");

	ofstream itaOut("ItalianOut.txt");
	ofstream engOut("EnglishOut.txt");
	ofstream fraOut("FranceOut.txt");
	ofstream spaOut("SpanishOut.txt");

	Text—lassifier textClassifier;
	
	textClassifier.setCategory("English");
	textClassifier.learn(eng);
	textClassifier.dumpProfile(engOut);

	textClassifier.setCategory("Span");
	textClassifier.learn(spa);
	textClassifier.dumpProfile(spaOut);

	textClassifier.setCategory("Franch");
	textClassifier.learn(fra);
	textClassifier.dumpProfile(fraOut);

	textClassifier.setCategory("Itallian");
	textClassifier.learn(ita);
	textClassifier.dumpProfile(itaOut);
}

void analyseText(ifstream &file) {
	Text—lassifier textClassifier;
	Text—lassifier textDumpClassifier;

	ifstream itaOut("ItalianOut.txt");
	ifstream engOut("EnglishOut.txt");
	ifstream fraOut("FranceOut.txt");
	ifstream spaOut("SpanishOut.txt");

	long int score = 0;
	long int minScore = 0;

	textClassifier.learn(file);

	textDumpClassifier.learnfromDump(engOut);

	minScore = textClassifier.score(textDumpClassifier);

	if (minScore != UNKNOWN_CATEGORY && minScore != 0) {
		textClassifier.setCategory("English");
	}
	cout << "English score: " << minScore << endl;

	textDumpClassifier.learnfromDump(itaOut);

	score = textClassifier.score(textDumpClassifier);

	if (score != UNKNOWN_CATEGORY && score != 0 && score < minScore) {
		minScore = score;
		textClassifier.setCategory("Itallian");
	}
	cout << "Itallian score: " << score << endl;

	textDumpClassifier.learnfromDump(fraOut);

	score = textClassifier.score(textDumpClassifier);

	if (score != UNKNOWN_CATEGORY && score != 0 && score < minScore) {
		minScore = score;
		textClassifier.setCategory("French");
	}
	cout << "French score: " << score << endl;


	textDumpClassifier.learnfromDump(spaOut);

	score = textClassifier.score(textDumpClassifier);

	if (score != UNKNOWN_CATEGORY && score != 0 && score < minScore) {
		minScore = score;
		textClassifier.setCategory("Spanish");
	}
	cout << "Spanish score: " << score << endl;

	cout << "Category: " << textClassifier.category() << endl;
}

int main() {

	cout << "Program started..." << endl;

	ifstream unkEng("unknown-English.txt");
	ifstream unkFra("unknown-French.txt");
	ifstream unkIta("unknown-Itallian.txt");
	ifstream unkSpa("unknown-Spanish.txt");
	ifstream emptyFile("emptyFile.txt");

	dumpProfiles();
	cout << endl;
	analyseText(unkFra);
	cout << endl;
	analyseText(unkEng);
	cout << endl;
	analyseText(unkSpa);
	cout << endl;
	analyseText(unkIta);
	cout << endl;
	analyseText(emptyFile);
	cout << endl;

	system("pause");
	return 0;
}