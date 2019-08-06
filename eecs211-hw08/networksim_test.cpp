#include "errors.h"
#include "parsing.h"
#include "datagram.h"
#include "machines.h"
#include "system.h"

#include <UnitTest++/UnitTest++.h>
#include <memory>
#include <iostream>
#include <stdexcept>

using namespace std;


// A system of four connected machines. They are used in the following unit
// tests
class SystemF
{
public:
//    array<shared_ptr<Node>, 9> ms_;
    shared_ptr<Laptop> lap1;
    shared_ptr<Laptop> lap2;
    shared_ptr<Laptop> lap3;
    shared_ptr<Server> s1;
    shared_ptr<Server> s2;
    shared_ptr<Server> s3;
    shared_ptr<WAN_node> w1;
    shared_ptr<WAN_node> w2;
    shared_ptr<WAN_node> w3;

    SystemF() {
        lap1 = make_shared<Laptop>("machine1", IP_address("50.168.0.1"));
        lap2 = make_shared<Laptop>("machine2", IP_address("75.168.0.1"));
        s1 = make_shared<Server>("server1", IP_address("100.168.0.1"));
        w1 = make_shared<WAN_node>("wan1", IP_address("150.168.0.1"));
        w2 = make_shared<WAN_node>("wan2", IP_address("120.168.0.1"));
        w3 = make_shared<WAN_node>("wan3", IP_address("180.168.0.1"));
        s2 = make_shared<Server>("server2", IP_address("200.168.0.1"));
        s3 = make_shared<Server>("server3", IP_address("220.168.0.1"));
        lap3 = make_shared<Laptop>("machine3", IP_address("250.168.0.1"));

        lap1->connect(s1);
        s1->connect(lap1);
        lap2->connect(s1);
        s1->connect(lap2);
        s1->connect(w1);
        w1->connect(s1);
        s1->connect(w2);
        w2->connect(s1);
        w1->connect(w3);
        w3->connect(w1);
        s2->connect(w3);
        w3->connect(s2);
        w3->connect(s3);
        s3->connect(w3);
        s3->connect(lap3);
        lap3->connect(s3);
    }

    ~SystemF()
    {
        // Break the connection between the machines to free the memory
        lap1->disconnect(s1);
        s1->disconnect(lap1);
        lap2->disconnect(s1);
        s1->disconnect(lap2);
        s1->disconnect(w1);
        w1->disconnect(s1);
        s1->disconnect(w2);
        w2->disconnect(s1);
        w1->disconnect(w3);
        w3->disconnect(w1);
        s2->disconnect(w3);
        w3->disconnect(s2);
        w3->disconnect(s3);
        s3->disconnect(w3);
        s3->disconnect(lap3);
        lap3->disconnect(s3);
    }
};

//check outgoing is set to null ptr after send
TEST(outgoing_is_nullptr_after_send){
    SystemF sys;
    sys.lap1->Laptop::allocate_datagram(sys.lap3->get_ip(), "lap1_lap3");
    CHECK_EQUAL(1, sys.lap1->send());
    CHECK_EQUAL(0, sys.lap1->send());
}


//Test Server send to wan when machine is not in node_list
TEST(Server_send_to_wan)
{
    SystemF sys;
    sys.lap1->allocate_datagram(sys.lap3->get_ip(), "lap1_lap3");
    Datagram* test = new Datagram(sys.lap1->get_ip(), sys.lap3->get_ip(), "lap1_lap3");
    sys.s1->receive(test);
    CHECK_EQUAL(1, sys.s1->send());
    CHECK_EQUAL(1, sys.lap1->send());
    CHECK_EQUAL(0, sys.lap1->send());
    CHECK_EQUAL(1, sys.s1->send());
    CHECK_EQUAL(2, sys.w1->send());
}

// Test the specification "If the destination of the Datagram is
// in the connected machines, send the datagram to that machine."
TEST(SEND_BEST_MACHINE_DESTINATION_MATCH)
{
    SystemF sys;
    sys.lap1->allocate_datagram(sys.lap2->get_ip(), "lap1_lap2");
    CHECK_EQUAL(1, sys.lap1->send()); // successfully sent one datagram
    CHECK_EQUAL(1, sys.s1->send());
    CHECK_EQUAL("lap1_lap2", sys.lap2->release_datagram());
}

// Test the specification "Otherwise, send it to the connected machine
// whose first number of IP address is closest to the first number of
// the destination."
TEST(SEND_BEST_MACHINE_CLOSEST_FIRST_NUMBER)
{
    SystemF sys2;
    sys2.lap1->allocate_datagram(IP_address("250.8.7.6"), "no dst machine");
    CHECK_EQUAL(1, sys2.lap1->send()); // successfully sent one datagram
    CHECK_EQUAL(1, sys2.s1->send());
    CHECK_EQUAL(1, sys2.w1->send());
    CHECK_EQUAL(1, sys2.w3->send());
//    Datagram* test = new Datagram(sys2.lap1->get_ip(), IP_address("250.8.7.6"), "no dst machine");
//    sys2.s3->receive(test);
    CHECK_EQUAL(1, sys2.s3->send());
    //CHECK_EQUAL(1, sys2.s3->send());
    CHECK_EQUAL(1, sys2.w3->send());
    CHECK_EQUAL(1, sys2.s3->send()); //lap3 can send out it again
}

// Test whether send can handle machine with no Datagram in datalist.
TEST(SEND_BEST_MACHINE_NO_MESSAGE)
{
    SystemF sys3;
    CHECK_EQUAL(0, sys3.lap1->send());
}

// Test whether send will fail if dst machine buffer is full and whether the source machine keeps the datagram
TEST(SEND_BEST_MACHINE_BLOCKED)
{
    SystemF sys4;
    sys4.lap1->allocate_datagram(sys4.lap2->get_ip(), "lap1_lap2");
    CHECK_EQUAL(1, sys4.lap1->send()); // successfully sent one datagram
    CHECK_EQUAL(1, sys4.s1->send());
    sys4.lap1->allocate_datagram(sys4.lap2->get_ip(), "lap1_lap2_fail");
    CHECK_EQUAL(1, sys4.lap1->send());
    CHECK_EQUAL(0, sys4.s1->send());// datagram blocked
}

// Test whether receive can block
TEST(RECEIVE_BLOCKED){
    SystemF sys10;
    Datagram* datagram1 = new Datagram(IP_address("192.168.0.11"), sys10.lap1->get_ip(), "hi");
    Datagram* datagram2 = new Datagram(IP_address("192.168.0.10"), sys10.lap1->get_ip(), "hi2");
    sys10.lap1 -> receive(datagram1);
    CHECK_THROW(sys10.lap1->receive(datagram2), err_code);
    sys10.lap1 -> release_datagram();
    delete datagram2;
}

//Test whether send can deal with not-connected machine system
TEST(SEND_BEST_MACHINE_NOT_CONNECT){
    SystemF sys9;
    sys9.lap1->allocate_datagram(sys9.lap2->get_ip(), "lap1_lap2");
    sys9.lap1->disconnect(sys9.s1);
    sys9.s1->disconnect(sys9.lap1);
    CHECK_EQUAL(0, sys9.lap1->send()); // datagram blocked
    sys9.lap1->connect(sys9.s1);
    sys9.s1->connect(sys9.lap1);
    CHECK_EQUAL(1, sys9.lap1->send()); // successfully sent one datagram
}

//Test whether machine can successfully receive message from its buffer
TEST(RECEIVE_MESSAGE){
    SystemF sys5;
    sys5.lap1->allocate_datagram(sys5.lap2->get_ip(), "lap1_lap2");
    CHECK_EQUAL(1, sys5.lap1->send()); // successfully sent one datagram
    CHECK_EQUAL(1, sys5.s1->send()); // successfully sent one datagram
    CHECK_EQUAL("lap1_lap2", sys5.lap2->release_datagram()); //this shows machine successfully receive the datagram
}

//Test whether machine can successfully pass message from its buffer if dst is not current machine
TEST(RECEIVE_PASS_MESSAGE){
    SystemF sys6;
    sys6.lap1->allocate_datagram(IP_address("9.8.7.6"), "pass");
    CHECK_EQUAL(1, sys6.lap1->send()); // successfully sent one datagram
    CHECK_EQUAL(1, sys6.s1->send()); // s1 is not the destination, but it can send out again, shows that receive
    // can receive message which does not reach destination
    CHECK_EQUAL(1, sys6.w2->send());//w1 is not destination but can send out again.
}

//Test whether release can deal with empty buffer
TEST(RELEASE_MACHINE_NO_DATA_IN_BUFFER)
{
    SystemF sys7;
    CHECK_EQUAL("", sys7.lap1->release_datagram());
}

//Test whether release can deal with filled buffer
TEST(RELEASE_MACHINE_WITH_DATA_IN_BUFFER__Can_TRANSFER_MESSAGE_TO_DEST_THROUGH_ALL_MACHINES)
{
    SystemF sys2;
    sys2.lap1->allocate_datagram(sys2.lap3->get_ip(), "lap1_lap3");
    CHECK_EQUAL(1, sys2.lap1->send()); // successfully sent one datagram
    CHECK_EQUAL(1, sys2.s1->send());
    CHECK_EQUAL(1, sys2.w1->send());
    CHECK_EQUAL(1, sys2.w3->send());
    CHECK_EQUAL(1, sys2.s3->send());
    CHECK_EQUAL("lap1_lap3", sys2.lap3->release_datagram()); //this shows lap3 machine successfully receive the datagram
}

//Test for system::allocating datagrams, release datagrams and time_click
TEST(SYSTEM){
    System sys;
    sys.create_machine("laptop", "machine1", IP_address("192.168.0.1"));
    sys.create_machine("laptop", "machine2", IP_address("192.168.0.2"));
    sys.create_machine("laptop", "machine3", IP_address("192.168.0.3"));
    sys.create_machine("laptop", "machine4", IP_address("192.168.0.4"));
    CHECK_THROW(sys.connect_machine(IP_address("192.168.0.1"), IP_address("192.168.0.2")), err_code);
    sys.time_click();
    sys.allocate_datagram(IP_address("192.168.0.1"), IP_address("192.168.0.2"), "hi");
    CHECK_THROW(sys.allocate_datagram(IP_address("192.168.0.2"), IP_address("193.168.0.7"), "hi"), err_code);
    sys.release_datagram(IP_address("192.168.0.2"));
    CHECK_THROW(sys.allocate_datagram(IP_address("192.168.0.5"), IP_address("192.168.0.2"), "hi"),
                err_code);
    CHECK_THROW(sys.release_datagram(IP_address("192.168.0.5")),
                err_code);
    sys.time_click();
}

//CHECK system commands
TEST(System_function_tests){
    System sys;
    sys.create_machine("laptop", "machine1", IP_address("25.168.0.1"));
    sys.create_machine("laptop", "machine2", IP_address("50.168.0.1"));
    sys.create_machine("laptop", "machine3", IP_address("150.168.0.1"));
    sys.create_machine("laptop", "machine4", IP_address("250.168.0.1"));
    sys.create_machine("server", "server1", IP_address("75.168.0.2"));
    sys.create_machine("server", "server3", IP_address("150.168.0.2"));
    sys.create_machine("server", "server4", IP_address("200.168.0.2"));
    sys.create_machine("wan", "wan1", IP_address("125.168.0.3"));
    sys.create_machine("wan", "wan2", IP_address("100.168.0.3"));
    sys.create_machine("wan", "wan4", IP_address("175.168.0.3"));

    sys.connect_machine(IP_address("25.168.0.1"), IP_address("75.168.0.2"));
    sys.connect_machine(IP_address("50.168.0.1"), IP_address("75.168.0.2"));
    sys.connect_machine(IP_address("100.168.0.3"), IP_address("75.168.0.2"));
    sys.connect_machine(IP_address("125.168.0.3"), IP_address("75.168.0.2"));

    sys.connect_machine(IP_address("125.168.0.3"), IP_address("150.168.0.2"));
    sys.connect_machine(IP_address("125.168.0.3"), IP_address("175.168.0.3"));

    sys.connect_machine(IP_address("150.168.0.2"), IP_address("150.168.0.1"));

    sys.connect_machine(IP_address("175.168.0.3"), IP_address("200.168.0.2"));

    sys.connect_machine(IP_address("200.168.0.2"), IP_address("250.168.0.1"));

    //check release empty imcoming_
    sys.release_datagram(IP_address("25.168.0.1"));


    //CHECK connect will throw error about incorrect machine type when connect wan and laptop
    CHECK_THROW(sys.connect_machine(IP_address("25.168.0.1"), IP_address("125.168.0.3")),err_code);
    //CHECK connect will throw error about incorrect machine type when connect laptop and laptop
    CHECK_THROW(sys.connect_machine(IP_address("25.168.0.1"), IP_address("50.168.0.1")),err_code);
    //CHECK connect will throw error about incorrect machine type when connect server and server
    CHECK_THROW(sys.connect_machine(IP_address("75.168.0.2"), IP_address("150.168.0.2")),err_code);

    //Check allocate_machine can not be called on wan
    CHECK_THROW(sys.allocate_datagram(IP_address("125.168.0.3"), IP_address("50.168.0.1"), "hi"), err_code);
    //Check allocate_machine can not be called on server
    CHECK_THROW(sys.allocate_datagram(IP_address("150.168.0.2"), IP_address("50.168.0.1"), "hi"),err_code);

    //Check allocate_machine can not have dst as wan
    CHECK_THROW(sys.allocate_datagram(IP_address("50.168.0.1"), IP_address("125.168.0.3"), "hi"), err_code);
    //Check allocate_machine can not have dst as server
    CHECK_THROW(sys.allocate_datagram(IP_address("50.168.0.1"), IP_address("150.168.0.2"), "hi"),err_code);

    sys.allocate_datagram(IP_address("25.168.0.1"), IP_address("50.168.0.1"), "lap1_lap2");
    sys.allocate_datagram(IP_address("50.168.0.1"), IP_address("150.168.0.1"), "lap2_lap3");
    sys.allocate_datagram(IP_address("150.168.0.1"), IP_address("250.168.0.1"), "lap3_lap4");
    sys.allocate_datagram(IP_address("250.168.0.1"), IP_address("25.168.0.1"), "lap4_lap1");

    sys.time_click();
}

//check all conditions of failing connection because of too many connections
TEST(SYSTEM_CONNECT_FAILS_FULL){
    System sys;
    sys.create_machine("laptop", "machine1", IP_address("25.168.0.1"));
    sys.create_machine("laptop", "machine2", IP_address("50.168.0.1"));
    sys.create_machine("laptop", "machine3", IP_address("150.168.0.1"));
    sys.create_machine("laptop", "machine4", IP_address("250.168.0.1"));
    sys.create_machine("laptop", "machine5", IP_address("25.168.1.1"));
    sys.create_machine("laptop", "machine6", IP_address("50.168.1.1"));
    sys.create_machine("laptop", "machine7", IP_address("150.168.1.1"));
    sys.create_machine("laptop", "machine8", IP_address("250.168.1.1"));
    sys.create_machine("laptop", "machine9", IP_address("210.168.1.1"));
    sys.create_machine("server", "server", IP_address("0.168.0.2"));
    sys.create_machine("server", "server1", IP_address("75.168.0.2"));
    sys.create_machine("server", "server2", IP_address("175.168.0.2"));
    sys.create_machine("server", "server3", IP_address("150.168.0.2"));
    sys.create_machine("server", "server4", IP_address("200.168.0.2"));
    sys.create_machine("server", "server5", IP_address("205.168.0.2"));
    sys.create_machine("wan", "wan", IP_address("0.168.0.3"));
    sys.create_machine("wan", "wan1", IP_address("125.168.0.3"));
    sys.create_machine("wan", "wan2", IP_address("100.168.0.3"));
    sys.create_machine("wan", "wan3", IP_address("173.168.0.3"));
    sys.create_machine("wan", "wan4", IP_address("175.168.0.3"));
    sys.create_machine("wan", "wan5", IP_address("172.168.0.3"));

    sys.connect_machine(IP_address("25.168.0.1"), IP_address("0.168.0.2"));
    sys.connect_machine(IP_address("50.168.0.1"), IP_address("0.168.0.2"));
    sys.connect_machine(IP_address("150.168.0.1"), IP_address("0.168.0.2"));
    sys.connect_machine(IP_address("250.168.0.1"), IP_address("0.168.0.2"));
    sys.connect_machine(IP_address("25.168.1.1"), IP_address("0.168.0.2"));
    sys.connect_machine(IP_address("50.168.1.1"), IP_address("0.168.0.2"));
    sys.connect_machine(IP_address("150.168.1.1"), IP_address("0.168.0.2"));
    sys.connect_machine(IP_address("250.168.1.1"), IP_address("0.168.0.2"));
    CHECK_THROW(sys.connect_machine(IP_address("210.168.1.1"), IP_address("0.168.0.2")), err_code);

    sys.connect_machine(IP_address("125.168.0.3"), IP_address("0.168.0.2"));
    sys.connect_machine(IP_address("175.168.0.3"), IP_address("0.168.0.2"));
    sys.connect_machine(IP_address("100.168.0.3"), IP_address("0.168.0.2"));
    sys.connect_machine(IP_address("172.168.0.3"), IP_address("0.168.0.2"));
    CHECK_THROW(sys.connect_machine(IP_address("173.168.0.3"), IP_address("0.168.0.2")), err_code);

    sys.connect_machine(IP_address("125.168.0.3"), IP_address("0.168.0.3"));
    sys.connect_machine(IP_address("175.168.0.3"), IP_address("0.168.0.3"));
    sys.connect_machine(IP_address("100.168.0.3"), IP_address("0.168.0.3"));
    sys.connect_machine(IP_address("172.168.0.3"), IP_address("0.168.0.3"));
    CHECK_THROW(sys.connect_machine(IP_address("173.168.0.3"), IP_address("0.168.0.3")), err_code);

    sys.connect_machine(IP_address("75.168.0.2"), IP_address("0.168.0.3"));
    sys.connect_machine(IP_address("175.168.0.2"), IP_address("0.168.0.3"));
    sys.connect_machine(IP_address("150.168.0.2"), IP_address("0.168.0.3"));
    sys.connect_machine(IP_address("200.168.0.2"), IP_address("0.168.0.3"));
    CHECK_THROW(sys.connect_machine(IP_address("205.168.0.2"), IP_address("0.168.0.3")), err_code);
}

// Following are checks from hw06

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