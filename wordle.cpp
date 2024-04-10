#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void wordleHelper(const string& prefix, std::set<string>& words, const std::string& in, std::string& floating, const std::set<string>& dict);


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    //count the number of dashes
    // int empty = 0;
    // for(int i = 0; i < in.size(); ++i){
    //     if(in.at(i) == '-'){
    //         ++empty;
    //     }
    // }
    
    string floating2 = floating;
    std::set<string> words;
    wordleHelper("", words, in, floating2, dict);
    return words;
}

// Define any helper functions here
void wordleHelper(const string& prefix, std::set<string>& words, const std::string& in, std::string& floating, const std::set<string>& dict){

    //if the prefix is the same size as in
    if(prefix.size() == in.size()){
        //check if we have no more floating characters
        if(floating.size() == 0){
            //check if it is a valid word
            if(dict.find(prefix) != dict.end()){
                #ifdef DEBUG
                    cerr << "Inserting: " << prefix << endl;
                #endif
                words.insert(prefix);
            }
        }
        return;
    }

    //if there is a "green" letter, append it to the prefix and go to the next letter
    if(in.at(prefix.size()) != '-'){
        wordleHelper(prefix + in.at(prefix.size()), words, in, floating, dict);
        return;
    }

    //if there are more floating characters than positions left to fill, then we hit a dead end
    if(prefix.size() + floating.size() > in.size()){
        return;
    }

    //if there are exactly the same amount of floating characters as there are open positions to fill, then we know it is a combination of these floating characters
    if(prefix.size() + floating.size() == in.size()){
        for(int i = 0; i < floating.size(); ++i){
            string floating2 = floating;
            //erase the floating character that we are appending
            //floating2 = floating.substr(0, i) + floating.substr(i + 1);
            floating2.erase(i, 1);
            wordleHelper(prefix + floating.at(i), words, in, floating2, dict);
        }
        return;
    }

    //otherwise, try all letters
    for(char c = 'a'; c <= 'z'; ++c){
        #ifdef DEBUG
        cerr << "Prefix:" << prefix << ' ' << "Floating: " << floating <<endl;
        #endif

        string floating2 = floating;
        size_t loc = floating.find(c);
        if(loc != string::npos){
            floating2.erase(loc, 1);
            //floating2 = floating.substr(0, loc) + floating.substr(loc + 1);
        }

        wordleHelper(prefix + c, words, in, floating2, dict);
    }
}
