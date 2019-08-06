#include "parsing.h"
#include "interface.h"

#include <iostream>
#include <string>
#include <vector>


using namespace std;


int parse_int_helper(char n){
    const vector<char> ords{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    for(int i = 0; i < 10; ++i){
        if (n == ords[i]) return i;
    }
    throw runtime_error("can not convert");
}

int parse_int(const string& s) {
    if (s.empty())
        throw runtime_error("Imputed string can not be empty");
    int num;
    int result = 0;
    for (size_t i = 0; i < s.size(); i++){
        num = parse_int_helper(s[i]);
        result = (result * 10) + num;
    }
    return result;
}


bool all_space(string s){
    for (char each : s){
        if (each != ' ')
            return false;
    }
    return true;
}

bool even_quote(string s){
    int count = 0;
    for (char each : s){
        if (each == '\"')
            ++count;
    }
    if (count % 2 == 0)
        return true;
    else
        return false;
}

vector<string> tokenize(const string& line) {
    vector<string> result;

    if (line.empty())
        return result;

    if (all_space(line))
        return result;

    if (!even_quote(line))
        throw runtime_error("There must be even number of \"");

    int current_position = 0;
    while (current_position < line.size()) {
        if (line[current_position] == '\"'){
            string newString = "";
            current_position++;
            while(line[current_position] != '\"'){
                newString.push_back(line[current_position]);
                current_position++;
                if(current_position >= line.size()) {
                    result.push_back(newString);
                    return result;
                }
            }
            result.push_back(newString);
            current_position++;
        }
        else if(line[current_position] != ' '){
            string newString = "";
            while(line[current_position] != '\"' && line[current_position] != ' '){
                newString.push_back(line[current_position]);
                current_position++;
                if(current_position >= line.size()) {
                    result.push_back(newString);
                    return result;
                }
            }
            result.push_back(newString);
            current_position++;
        }
        else current_position++;
    }
    return result;
}
