#pragma once

#include "interface.h"

#include <string>
#include <vector>

int parse_int_helper(char n);
int parse_int(const std::string& s);
std::vector<std::string> tokenize(const std::string& line);

bool all_space(std::string s);
bool even_quote(std::string s);
std::vector<std::string> tokenize(const std::string& line);