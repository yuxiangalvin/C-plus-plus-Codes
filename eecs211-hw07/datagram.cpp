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

IP_address::IP_address(string s)
{
    std::array<int, 4> ip {0, 0, 0, 0};

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
    ip_ = ip;
}


bool IP_address::operator==(const IP_address& that) const
{
    for (int i=0; i < 4; i++) {
        if (ip_[i] != that.ip_[i])
            return false;
    }
    return true;
}

int IP_address::first_octad() const
{
    return ip_[0];
}

std::ostream& operator<<(std::ostream& os, const IP_address& addr)
{
    return os << '"'
            << addr.ip_[0] << '.'
            << addr.ip_[1] << '.'
            << addr.ip_[2] << '.'
            << addr.ip_[3] << '"';
}

Datagram::Datagram(const IP_address& s, const IP_address& d, const string& m)
        : src_(s), dst_(d), msg_(m)
{
    length_ = m.length();
};

IP_address Datagram::get_destination() const
{
    return dst_;
}

std::string Datagram::get_msg() const
{
    return msg_;
}

std::ostream& operator<<(std::ostream& os, const Datagram& dg)
{
    os << "datagram {";
    os << "src=" << dg.src_;
    os << ", dst=" << dg.dst_;
    os << ", length=" << dg.length_;
    os << ", msg=\"" << dg.msg_ << "\"";
    os << "}";
    return os;
}
