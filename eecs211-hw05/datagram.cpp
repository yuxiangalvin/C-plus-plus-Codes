#include "datagram.h"
#include "parsing.h"
#include "errors.h"

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

bool not_number (char ch){
    if (ch != '0' && ch != '1' &&
        ch != '2' && ch != '3' &&
        ch != '4' && ch != '5' &&
        ch != '6' && ch != '7' &&
        ch != '8' && ch != '9')
        return true;

    return false;
}

IP_address parse_IP(string s)
{
    IP_address ip {0, 0, 0, 0};

    if (s.empty())
        throw err_code::bad_ip_address;

    for (int i = 0; i < s.size(); i++){
        if(s[i] != '.' && not_number(s[i]) )
        throw err_code::bad_ip_address;
    }

    if (s[0]=='.')
        throw err_code::bad_ip_address;

    if (s[(s.size()-1)]=='.')
        throw err_code::bad_ip_address;

    int count_dot = 0;

    for (int i = 0; i < s.size(); i++){
        if (s[i] == '.'){
            if (s[i + 1] == '.')
                throw err_code::bad_ip_address;
            ++count_dot;
        }
    }

    if (count_dot != 3)
        throw err_code::bad_ip_address;

    for (int i = 0; i < s.size(); i++){
        if(s[i] == '.') s[i] = ' ';
    }
    vector<string> ipString = tokenize(s);
    for (int i = 0; i < ip.size(); i++){
        if (parse_int(ipString[i]) > 255 || parse_int(ipString[i]) < 0)
            throw err_code::bad_ip_address;
        ip[i] = parse_int(ipString[i]);
    }
    return ip;
}

