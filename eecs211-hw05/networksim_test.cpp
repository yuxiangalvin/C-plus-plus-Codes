#include "errors.h"
#include "parsing.h"
#include "datagram.h"

#include <UnitTest++/UnitTest++.h>
#include <memory>
#include <iostream>
#include <stdexcept>

using namespace std;

TEST(TOKENIZING){
    string tokens1 = "   de@f.com  \"ghi j-k \" w == \"z\"";
    string tokens2 = "hel\"l\"o world \"\" \"eecs   211\"";
    string tokens3 = "\"abc\\\"\"def\"";
    vector<string> t1 = {"de@f.com", "ghi j-k ", "w", "==", "z"};
    vector<string> t2 = {"hel", "l", "o", "world", "", "eecs   211"};
    vector<string> t3 = {"abc\\", "def"};
    string s1 = "192 168 0 1";
    string s2 = "255 255 255 0";
    string s3 = "0 0 0 0";

    string s4 = "";
    string s5 = " w  ";
    string s6 = "  ";
    string s7 = " ";

    string s8 = "\"\"";

    string s9 = "\" ";
    string s10 = " \"";
    string s11 = "\"4\" \"";
    string s12 = "\"";

    string s13 = " \"$3- 4 \"  ";

    vector<string> ip1 = {"192", "168", "0", "1"};
    vector<string> ip2 = {"255", "255", "255", "0"};
    vector<string> ip3 = {"0", "0", "0", "0"};
    vector<string> ip4 = {};
    vector<string> ip5 = {"w"};
    vector<string> ip6 = {};
    vector<string> ip7 = {};
    vector<string> ip8 = {""};
    vector<string> ip13 = {"$3- 4 "};
    CHECK_EQUAL(t1.size(),tokenize(tokens1).size());
    CHECK_EQUAL(t2.size(),tokenize(tokens2).size());
    CHECK_EQUAL(t3.size(),tokenize(tokens3).size());
    CHECK_EQUAL(ip1.size(),tokenize(s1).size());
    CHECK_EQUAL(ip2.size(),tokenize(s2).size());
    CHECK_EQUAL(ip3.size(),tokenize(s3).size());
    CHECK_EQUAL(ip4.size(),tokenize(s4).size());
    CHECK_EQUAL(ip5.size(),tokenize(s5).size());
    CHECK_EQUAL(ip6.size(),tokenize(s6).size());
    CHECK_EQUAL(ip7.size(),tokenize(s7).size());
    CHECK_EQUAL(ip8.size(),tokenize(s8).size());
    CHECK_EQUAL(ip13.size(),tokenize(s13).size());
    for(int i = 0; i < t1.size(); ++i) {
        CHECK_EQUAL(t1[i], tokenize(tokens1)[i]);
    }
    for(int i = 0; i < t2.size(); ++i) {
        CHECK_EQUAL(t2[i], tokenize(tokens2)[i]);
    }
    for(int i = 0; i < t3.size(); ++i) {
        CHECK_EQUAL(t3[i], tokenize(tokens3)[i]);
    }
    for(int i = 0; i < 4; ++i) {
        CHECK_EQUAL(ip1[i], tokenize(s1)[i]);
    }
    for(int i = 0; i < 4; ++i) {
        CHECK_EQUAL(ip2[i], tokenize(s2)[i]);
    }
    for(int i = 0; i < 4; ++i) {
        CHECK_EQUAL(ip3[i], tokenize(s3)[i]);
    }
    for(int i = 0; i < 1; ++i) {
        CHECK_EQUAL(ip5[i], tokenize(s5)[i]);
    }
    for(int i = 0; i < 1; ++i) {
        CHECK_EQUAL(ip8[i], tokenize(s8)[i]);
    }
    for(int i = 0; i < 1; ++i) {
        CHECK_EQUAL(ip13[i], tokenize(s13)[i]);
    }

    CHECK_THROW(tokenize(s9), runtime_error);
    CHECK_THROW(tokenize(s10), runtime_error);
    CHECK_THROW(tokenize(s11), runtime_error);
    CHECK_THROW(tokenize(s12), runtime_error);
    CHECK_THROW(tokenize("\"23 *\"23-1\"33"), runtime_error);
}



TEST(Parse_integer){
    string s1 = "0";
    int i1 = 0;
    string s2 = "0210831";
    int i2 = 210831;
    string s3 = "0010432000";
    int i3 = 10432000;
    string s4 = "310200";
    int i4 = 310200;
    string s5 = "010";
    int i5 = 10;
    string s6 = "0002147483647";
    int i6 = 2147483647;
    string s7 = "3";
    int i7 = 3;
    string s8 = " 54";
    string s9 = "";
    string s10 = "3*4";
    string s11 = "\n";
    string s12 = "-3412";




    int is1 = parse_int(s1);
    int is2 = parse_int(s2);
    int is3 = parse_int(s3);
    int is4 = parse_int(s4);
    int is5 = parse_int(s5);
    int is6 = parse_int(s6);
    int is7 = parse_int(s7);

    CHECK_EQUAL(i1, is1);
    CHECK_EQUAL(i2, is2);
    CHECK_EQUAL(i3, is3);
    CHECK_EQUAL(i4, is4);
    CHECK_EQUAL(i5, is5);
    CHECK_EQUAL(i6, is6);
    CHECK_EQUAL(i7, is7);

    CHECK_THROW(parse_int(s8), runtime_error);
    CHECK_THROW(parse_int(s9), runtime_error);
    CHECK_THROW(parse_int(s10), runtime_error);
    CHECK_THROW(parse_int(s11), runtime_error);
    CHECK_THROW(parse_int(s12), runtime_error);
}

TEST(IP_address){
    IP_address ip1 = {192, 168, 0, 1};
    IP_address ip2 = {255, 255, 255, 0};
    IP_address ip3 = {0, 0, 0, 0};
    string s1 = "192.168.0.1";
    string s2 = "255.255.255.0";
    string s3 = "0.0.0.0";
    string s4 = "0.0.255.256";
    string s5 = "-3.0.255.211";
    string s6 = ".0.255.211";
    string s7 = "3.0.255.";
    string s8 = "-3.0.255.211.2";
    string s9 = "3.0. 255.211";
    string s10 = "3.0..211";
    string s11 = "";
    string s12 = " ";
    string s13 = ".";

    CHECK_EQUAL(ip1.size(),parse_IP(s1).size());
    CHECK_EQUAL(ip2.size(),parse_IP(s2).size());
    CHECK_EQUAL(ip3.size(),parse_IP(s3).size());
    for(int i = 0; i < 4; ++i) {
        CHECK_EQUAL(ip1[i], parse_IP(s1)[i]);
    }
    for(int i = 0; i < 4; ++i) {
        CHECK_EQUAL(ip2[i], parse_IP(s2)[i]);
    }
    for(int i = 0; i < 3; ++i) {
        CHECK_EQUAL(ip3[i], parse_IP(s3)[i]);
    }

    CHECK_THROW(parse_IP(s4), err_code);
    CHECK_THROW(parse_IP(s5), err_code);
    CHECK_THROW(parse_IP(s6), err_code);
    CHECK_THROW(parse_IP(s7), err_code);
    CHECK_THROW(parse_IP(s8), err_code);
    CHECK_THROW(parse_IP(s9), err_code);
    CHECK_THROW(parse_IP(s10), err_code);
    CHECK_THROW(parse_IP(s11), err_code);
    CHECK_THROW(parse_IP(s12), err_code);
    CHECK_THROW(parse_IP(s13), err_code);
    CHECK_THROW(parse_IP("..."), err_code);
    CHECK_THROW(parse_IP("."), err_code);
    CHECK_THROW(parse_IP(".  .  ."), err_code);
    CHECK_THROW(parse_IP("257.123.0.2"), err_code);
    CHECK_THROW(parse_IP("25.1213.0.2"), err_code);
    CHECK_THROW(parse_IP("217.123.430.2"), err_code);
}
