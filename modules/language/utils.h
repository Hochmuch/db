#pragma once
#include <algorithm>
#include <cctype>
#include <locale>
#include <string>

void ltrim(std::string &s);

void rtrim(std::string &s);

void trim(std::string &s);

std::string toUpper(std::string s);