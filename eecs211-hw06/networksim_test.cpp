#include "errors.h"
#include "parsing.h"
#include "datagram.h"
#include "machines.h"
#include "system.h"

#include <UnitTest++/UnitTest++.h>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>

using namespace std;

// Sample test to demonstrate how to use shared_ptr::use_count() to test
// whether Node::disconnect() works or not. If it works, n1 and n2
// should no longer hold reference to each other, so the use_count()
// will decrease to 1. After this test case returns, you should also be
// able to see the message "machine 'NODE_ DISCONNECT Machine 2'
// deleted" nad "machine 'NODE_DISCONNECT Machine 1' deleted" because n1
// and n2 are the only reference to these two machines. After they go
// out of scope, the memory they hold will be deallocated.

//check disconnect node
TEST(NODE_DISCONNECT) {
    // Allocate the machine and connect them, as did in
    // System::create_machine and System::connect_machine.
    shared_ptr<Node> n1 = make_shared<Node>("NODE_DISCONNECT Machine 1",
                                            IP_address("1.2.3.4"));
    shared_ptr<Node> n2 = make_shared<Node>("NODE_DISCONNECT Machine 2",
                                            IP_address("5.6.7.8"));
    shared_ptr<Node> n3 = make_shared<Node>("NODE_DISCONNECT Machine 3",
                                            IP_address("1.0.2.0"));
    shared_ptr<Node> n4 = nullptr;

    n1->disconnect(n4);

    n1->connect(n2);
    n2->connect(n1);

    // Both n1 and n2 also hold a reference to each other.
    CHECK_EQUAL(2, n1.use_count());
    CHECK_EQUAL(2, n2.use_count());

    n1->disconnect(n2);
    n2->disconnect(n1);

    // No references other than 'n1' and 'n2' remained.
    CHECK_EQUAL(1, n1.use_count());
    CHECK_EQUAL(1, n2.use_count());

    n2->disconnect(n3);
    CHECK_EQUAL(1, n2.use_count());
    CHECK_EQUAL(1, n1.use_count());
    CHECK_EQUAL(1, n3.use_count());
}

//check IP_address(string)
TEST(PARSE_IP_ADRESS){
    IP_address ip1 ("192.168.0.1");
    IP_address ip2 ("255.255.255.0");
    IP_address ip3 ("0.0.0.0");


    CHECK_EQUAL(true, (ip1 == IP_address("192.168.0.1")));
    CHECK_EQUAL(true, (ip2 == IP_address("255.255.255.0")));
    CHECK_EQUAL(true, (ip3 == IP_address("0.0.0.0")));

    CHECK_THROW(IP_address("0.0.255.256"), err_code);
    CHECK_THROW(IP_address("-3.0.255.211"), err_code);
    CHECK_THROW(IP_address(".0.255.211"), err_code);
    CHECK_THROW(IP_address("3.0.255."), err_code);
    CHECK_THROW(IP_address("-3.0.255.211.2"), err_code);
    CHECK_THROW(IP_address("3.0. 255.211"), err_code);
    CHECK_THROW(IP_address("3.0..211"), err_code);
    CHECK_THROW(IP_address(""), err_code);
    CHECK_THROW(IP_address(" "), err_code);
    CHECK_THROW(IP_address("."), err_code);
    CHECK_THROW(IP_address("..."), err_code);
    CHECK_THROW(IP_address("."), err_code);
    CHECK_THROW(IP_address(".  .  ."), err_code);
    CHECK_THROW(IP_address("257.123.0.2"), err_code);
    CHECK_THROW(IP_address("25.1213.0.2"), err_code);
    CHECK_THROW(IP_address("217.123.430.2"), err_code);
}

//check ip construction
TEST(Construct_IP){
    IP_address ip1 ("1.2.3.4");
    stringstream stream;
    stream << (ip1);
    CHECK_EQUAL ("\"1.2.3.4\"", stream.str());
}

//check get_destination
TEST(GET_DESTINATION){
    IP_address from ("1.1.1.1");
    IP_address to ("2.1.1.1");
    IP_address wrong ("2.3.1.1");
    Datagram d (from, to, "hi there");
    CHECK_EQUAL(true, to == d.get_destination());
    CHECK_EQUAL(false, wrong == d.get_destination());
}

//check == operator
TEST(IP_ADDRESS_EQUAL){
    IP_address a ("1.1.1.1");
    IP_address b ("1.1.1.1");
    IP_address c ("1.2.1.1");
    CHECK_EQUAL(true, a == b);
    CHECK_EQUAL(false, a == c);
}

//check first_octad();
TEST(IP_ADDRESS_FIRST){
    IP_address a ("1.2.3.4");
    CHECK_EQUAL(1, a.first_octad());
}

//check datagram construction;
TEST(CONSTRUCT_DATAGRAM){
    Datagram test (IP_address("1.2.3.4"), IP_address("4.3.2.1"), "see you");
    stringstream stream;
    stream << (test);
    CHECK_EQUAL("datagram {src=\"1.2.3.4\", dst=\"4.3.2.1\", length=7, msg=\"see you\"}", stream.str());
    CHECK_THROW(Datagram test (IP_address("1.2.3.#"), IP_address("4.3.2.1"), "throw"), err_code);
    CHECK_THROW(Datagram test (IP_address(".2.3.4"), IP_address("4.3.2.1"), "throw"), err_code);
    CHECK_THROW(Datagram test (IP_address("1233.2.3.4"), IP_address("4.3.2.1"), "throw"), err_code);
    CHECK_THROW(Datagram test (IP_address(""), IP_address("4.3.2.1"), "throw"), err_code);
    CHECK_THROW(Datagram test (IP_address("1.2.3.4"), IP_address(". . ."), "throw"), err_code);
    CHECK_THROW(Datagram test (IP_address("1.2.3.4"), IP_address("."), "throw"), err_code);
}

//check node construction;
TEST(CONSTRUT_NODE){
    shared_ptr<Node> n1 = make_shared<Node>("Machine 1",IP_address("1.2.3.4"));
    shared_ptr<Node> n2 = make_shared<Node>("Machine 2", IP_address("5.6.7.8"));
    Node test ("test", IP_address("2.3.4.5"));
    test.connect(n1);
    test.connect(n2);
    stringstream stream;
    stream << (test);
    CHECK_EQUAL("node {\n    name = \"test\",\n    local_ip = \"2.3.4.5\",\n    "
                        "node_list = [ \"1.2.3.4\" \"5.6.7.8\" ],\n    data_list = []\n  }", stream.str());
}

// check create machine
TEST(SYSTEM_CREATE_MACHINE){
    System sys = System();

    CHECK_THROW(sys.create_machine("asd", "a", IP_address("1.2.3.4")), err_code);

    System sys2 = System();
    string increment;
    for (int i = 0; i < 50; i++){
        increment = to_string(i);
        sys2.create_machine("server", increment,
                            IP_address(increment + "." + increment + "." + increment + "." + increment));
    }
    CHECK_THROW(sys2.create_machine("wan", "full", IP_address("1.3.3.4")), err_code);
}

//check system construction;
TEST(SYSTEM_CONSTRUCT){
    System sys = System();
    stringstream stream;
    sys.create_machine("server", "cons", IP_address("1.2.3.4"));
    stream << (sys);
    CHECK_EQUAL ("system {\n  tick = 0,\n  \"1.2.3.4\" = node {\n    name = \"cons\",\n"
                         "    local_ip = \"1.2.3.4\",\n    node_list = [ ],\n "
                         "   data_list = []\n  }\n}\n", stream.str());
}

//check delete machine error;
TEST(SYSTEM_DELETE_MACHINE){
    System sys = System();
    sys.create_machine("wan",
                       "a",
                       IP_address("1.2.3.4"));
    sys.create_machine("wan",
                       "b",
                       IP_address("1.2.3.5"));
    sys.create_machine("server",
                       "c",
                       IP_address("1.2.3.12"));
    sys.create_machine("laptop",
                       "d",
                       IP_address("1.2.3.21"));
    CHECK_THROW(sys.delete_machine(IP_address("1.2.3.20")), err_code);
    sys.delete_machine(IP_address("1.2.3.4"));
}

//check connect machine error;
TEST(SYSTEM_CONNECT_MACHINE){
    System sys = System();
    sys.create_machine("wan",
                       "a",
                       IP_address("3.2.3.4"));
    sys.create_machine("wan",
                       "b",
                       IP_address("1.2.3.5"));
    sys.create_machine("server",
                       "c",
                       IP_address("1.2.3.12"));
    sys.create_machine("laptop",
                       "d",
                       IP_address("1.2.3.21"));
    CHECK_THROW(sys.connect_machine(IP_address("1.2.3.20"), IP_address("1.2.3.21")), err_code);
    CHECK_THROW(sys.connect_machine(IP_address("1.2.3.5"), IP_address("1.2.3.66")), err_code);
}

//check create and connect machine
TEST(CREATE_MACHINE_CONNECT_MACHINE){
    System test = System();
    test.create_machine("server", "testmachine 1", IP_address("1.2.3.4"));
    test.create_machine("server", "testmachine 2", IP_address("2.2.3.4"));
    CHECK_EQUAL(2, test.find_machine(IP_address("1.2.3.4")).use_count());
    CHECK_EQUAL(2, test.find_machine(IP_address("2.2.3.4")).use_count());
    test.connect_machine(IP_address("1.2.3.4"),IP_address("2.2.3.4"));
    CHECK_EQUAL(3, test.find_machine(IP_address("1.2.3.4")).use_count());
    CHECK_EQUAL(3, test.find_machine(IP_address("2.2.3.4")).use_count());
}








// Below are hw05 tests

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
