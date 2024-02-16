#pragma once

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <cctype>
#include <sstream>
#include <fstream>
#include <utility>
#include <algorithm>

using namespace std;

/**
 * Cleans a specific token by:
 * - Removing leading and trailing punctuation
 * - Converting to lowercase
 * If the token has no letters, returns the empty string.
 */
string cleanToken(string s) {
    if(s.size() == 0) {
        return s;
    } 

    string newS = "";
    int posFront = -1;
    int posBack = -1;

    for(int i = 0;i < s.size();i++) {
        if(posFront ==-1 && !ispunct(s.at(i))) {
            posFront = i;
        }
        if(posFront != -1 && !ispunct(s.at(i))) {
            posBack = i;
        }
    }

    if(posFront == -1 || posBack == -1) {
        return "";
    }

    newS = s.substr(posFront,(posBack - posFront) + 1);

    bool hasAlpha = false;

    for(char& c: newS) {
        if(isalpha(c)) {
            c = tolower(c);
            hasAlpha = true;
        }
    }

    return (hasAlpha)? newS : "";
}

/**
 * Returns a set of the cleaned tokens in the given text.
 */
set<string> gatherTokens(string text) {
    set<string> words;
    istringstream iss(text);
    string word;

    while (iss >> word) {
        word = cleanToken(word);
        if(word.size()>0) {
            words.insert(word);  
        }
    }
    return words;
}

/**
 * Builds the inverted index from the given data file. The existing contents
 * of `index` are left in place.
 */
int buildIndex(string filename, map<string, set<string>>& index) {
    ifstream file(filename);

    if(!file.is_open()) {
        return 0;
    }

    set<string> tokens;
    string line;
    string website;
    int checkLine = 0;
    int countWeb = 0;

    while(getline(file,line)) {
        if(checkLine % 2 == 0) {
            website = line;
            countWeb++;
        }
        if(checkLine % 2 != 0) {
            tokens = gatherTokens(line);
            for(string token : tokens) {
                index[token].insert(website);
            }
        }
        checkLine++;
    }
    return countWeb;
}

vector<pair<char,string>> makeVectPair(string str) {
    vector<pair<char,string>> pairs;
    istringstream iss(str);
    string word;

    while (iss >> word) {
        char modifier = 'x';
        if (!word.empty() && (word[0] == '+' || word[0] == '-')) {
            modifier = word[0];
            word = cleanToken(word); 
        }
        pairs.push_back(make_pair(modifier, cleanToken(word)));
    }
    return pairs;
}

bool checkWordInIndex(const map<string, set<string>>& index, const string word) {
    return index.count(word) > 0;
}

/**
 * Runs a search query on the provided index, and returns a set of results.
 *
 * Search terms are processed using set operations (union, intersection, difference).
 */
set<string> findQueryMatches(map<string, set<string>>& index, string sentence) {
   if (sentence.empty()) {
        return {};
    }

    vector<pair<char, string>> vecPair = makeVectPair(sentence);
    set<string> result;

    bool initialized = false;
    set<string> setCompare;

    for (int i = 0; i < vecPair.size(); i++) {
        char modifier = vecPair[i].first;
        string word = vecPair[i].second;

        if (!checkWordInIndex(index, word)) {
            if (modifier == '+') {
                return {};
            }
            continue;
        }

        if (!initialized) {
            setCompare = index[word];
            initialized = true;
            result = setCompare;
            continue;
        }

        if (modifier == 'x') {
            set<string> tempResult;
            set_union(setCompare.begin(), setCompare.end(),
                      index[word].begin(), index[word].end(),
                      inserter(tempResult, tempResult.begin()));
            result = tempResult;
        } else if (modifier == '+') {
            set<string> tempResult;
            set_intersection(setCompare.begin(), setCompare.end(),
                             index[word].begin(), index[word].end(),
                             inserter(tempResult, tempResult.begin()));
            result = tempResult;
        } else if (modifier == '-') {
            set<string> tempResult;
            set_difference(setCompare.begin(), setCompare.end(),
                           index[word].begin(), index[word].end(),
                           inserter(tempResult, tempResult.begin()));
            result = tempResult;
        }

        setCompare = result;
    }

    return result;
}

/**
 * Runs the main command loop for the search program
 */
void searchEngine(string filename) {
    map<string, set<string>> index;
    string query;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Invalid filename." << endl;
    }

    cout << "Stand by while building index...\n";
    buildIndex(filename, index);

    set<string> uniquePages;
    for (const auto& entry : index) {
        const set<string>& urls = entry.second;
        uniquePages.insert(urls.begin(), urls.end());
    }

    cout << "Indexed " << uniquePages.size()  << " pages containing " << index.size()<< " unique terms\n\n";

    while (true) {
        cout << "Enter query sentence (press enter to quit): ";
        getline(cin, query);

        if (query.empty()) {
            cout << "Thank you for searching!\n";
            break;
        }

        set<string> matches = findQueryMatches(index, query);

        cout << "Found " << matches.size() << " matching pages\n";
        for (const auto& url : matches) {
            cout << url << '\n';
        }
        cout << '\n';
    }
}
