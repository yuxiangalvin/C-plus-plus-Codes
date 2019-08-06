#pragma once

#include <array>
#include <string>
#include <iostream>

bool not_number (char ch);

class IP_address
{
    friend class Grader;
private:
    std::array<int, 4> ip_;

public:
    IP_address(std::string ip);
    bool operator==(const IP_address& that) const;
    int first_octad() const;

    friend std::ostream& operator<<(std::ostream&, const IP_address&);
};

std::ostream& operator<<(std::ostream&, const IP_address&);

class Datagram
{
    friend class Grader;
private:
    IP_address  src_, dst_;
    size_t      length_;
    std::string msg_;

public:
    Datagram(const IP_address& s, const IP_address& d, const std::string& m);
    IP_address get_destination() const;

    friend std::ostream& operator<<(std::ostream&, const Datagram&);
};

std::ostream& operator<<(std::ostream&, const Datagram&);
