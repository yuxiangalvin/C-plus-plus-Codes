#pragma once

#include <array>
#include <string>

using IP_address = std::array<int, 4>;

bool not_number (char ch);

IP_address parse_IP(std::string s);


