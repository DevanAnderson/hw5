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

int iter = 0;


// Add prototypes of helper functions here
void wordleHelper(string prefix, std::set<string>& words, const std::string& in, std::string floating,const std::set<string>& dict);


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here

    std::set<string> words;
    wordleHelper("", words, in, floating, dict);
    return words;
}

// Define any helper functions here
void wordleHelper(string prefix, std::set<string>& words, const std::string& in, std::string floating, const std::set<string>& dict){

    //if the prefix is the same size as in, check if it is a valid word in the dictionary
    if(prefix.size() == in.size()){
        if(dict.find(prefix) != dict.end()){
            //check if we have no more floating characters
            if(floating.size() == 0){
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
        }

        //too slow, maybe remove this
        //if the character we are inserting happens to be one of the floating characters, erase it from the floating characters
        // string floating2 = floating;
        // for(int i = 0; i < floating.size(); ++i){
        //     if(c == floating2.at(i)){
        //         floating2.erase(i, 1);
        //         //break to erase only one of the floating characters
        //         break;
        //     }
        // }

        wordleHelper(prefix + c, words, in, floating2, dict);
    }
}
