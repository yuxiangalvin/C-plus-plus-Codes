#include "errors.h"
#include "parsing.h"
#include "datagram.h"
#include "machines.h"
#include "system.h"

#include <UnitTest++/UnitTest++.h>
#include <array>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace std;

// A system of four connected machines. They are used in the following unit
// tests
class SystemF
{
public:
    array<shared_ptr<Node>, 4> ms_;

    SystemF()
    {
        ms_[0] = make_shared<Node>("L", IP_address("192.168.0.1"));
        ms_[1] = make_shared<Node>("S", IP_address("140.112.239.8"));
        ms_[2] = make_shared<Node>("T", IP_address("140.112.253.11"));
        ms_[3] = make_shared<Node>("W", IP_address("8.8.8.8"));
        for (size_t i = 1; i < 4; ++i) {
            ms_[0]->connect(ms_[i]);
            ms_[i]->connect(ms_[0]);
        }
    }

    ~SystemF()
    {
        // Break the connection between the machines to free the memory
        for (size_t i = 1; i < 4; ++i) {
            ms_[0]->disconnect(ms_[i]);
            ms_[i]->disconnect(ms_[0]);
        }
    }
};

// Test the specification "If the destination of the Datagram is
// in the connected machines, send the datagram to that machine."
TEST(SEND_BEST_MACHINE_DESTINATION_MATCH)
{
    SystemF sys;
    sys.ms_[0]->allocate_datagram(sys.ms_[2]->get_ip(), "ping");
    CHECK_EQUAL(1, sys.ms_[0]->send()); // successfully sent one datagram
    CHECK_EQUAL("ping", sys.ms_[2]->release_datagram());
}

// Test the specification "Otherwise, send it to the connected machine
// whose first number of IP address is closest to the first number of
// the destination."
TEST(SEND_BEST_MACHINE_CLOSEST_FIRST_NUMBER)
{
    SystemF sys2;
    sys2.ms_[0]->allocate_datagram(IP_address("9.8.7.6"), "pong");
    CHECK_EQUAL(1, sys2.ms_[0]->send()); // successfully sent one datagram
    CHECK_EQUAL(1, sys2.ms_[3]->send()); // ms_[3] is not the destination, but
                                        // we can send it out again
}

// Test whether send can handle machine with no Datagram in datalist.
TEST(SEND_BEST_MACHINE_NO_MESSAGE)
{
    SystemF sys3;
    CHECK_EQUAL(0, sys3.ms_[0]->send());
}

// Test whether send will fail if dst machine buffer is full and whether the source machine keeps the datagram
TEST(SEND_BEST_MACHINE_BLOCKED)
{
    SystemF sys4;
    sys4.ms_[0]->allocate_datagram(sys4.ms_[2]->get_ip(), "ping");
    sys4.ms_[0]->allocate_datagram(sys4.ms_[2]->get_ip(), "pong");
    CHECK_EQUAL(1, sys4.ms_[0]->send()); // successfully sent one datagram
    CHECK_EQUAL(0, sys4.ms_[0]->send()); // datagram blocked
    CHECK_EQUAL("ping", sys4.ms_[2]->release_datagram());
    CHECK_EQUAL(1, sys4.ms_[0]->send()); // successfully sent the remaining datagram
}

// Test whether receive can block
TEST(RECEIVE_BLOCKED){
    SystemF sys10;
    Datagram* datagram1 = new Datagram(IP_address("192.168.0.11"), sys10.ms_[1]->get_ip(), "hi");
    Datagram* datagram2 = new Datagram(IP_address("192.168.0.10"), sys10.ms_[1]->get_ip(), "hi2");
    sys10.ms_[1] -> receive(datagram1);
    CHECK_THROW(sys10.ms_[1]->receive(datagram2), err_code);
    sys10.ms_[1] -> release_datagram();
    delete datagram2;
}

//Test whether send can deal with not-connected machine system
TEST(SEND_BEST_MACHINE_NOT_CONNECT){
    SystemF sys9;
    sys9.ms_[0]->allocate_datagram(sys9.ms_[2]->get_ip(), "ping");
    for (size_t i = 1; i < 4; ++i) {
        sys9.ms_[0]->disconnect(sys9.ms_[i]);
        sys9.ms_[i]->disconnect(sys9.ms_[0]);
    }
    CHECK_EQUAL(0, sys9.ms_[0]->send()); // datagram blocked
    for (size_t i = 1; i < 4; ++i) {
        sys9.ms_[0]->connect(sys9.ms_[i]);
        sys9.ms_[i]->connect(sys9.ms_[0]);
    }
    CHECK_EQUAL(1, sys9.ms_[0]->send()); // successfully sent one datagram
    CHECK_EQUAL("ping", sys9.ms_[2]->release_datagram());
}

//Test whether machine can successfully receive message from its buffer
TEST(RECEIVE_MESSAGE){
    SystemF sys5;
    sys5.ms_[0]->allocate_datagram(sys5.ms_[2]->get_ip(), "ping");
    CHECK_EQUAL(1, sys5.ms_[0]->send()); // successfully sent one datagram
    CHECK_EQUAL("ping", sys5.ms_[2]->release_datagram()); //this shows machine successfully receive the datagram
}

//Test whether machine can successfully pass message from its buffer if dst is not current machine
TEST(RECEIVE_PASS_MESSAGE){
    SystemF sys6;
    sys6.ms_[0]->allocate_datagram(IP_address("9.8.7.6"), "pong");
    CHECK_EQUAL(1, sys6.ms_[0]->send()); // successfully sent one datagram
    CHECK_EQUAL(1, sys6.ms_[3]->send()); // ms_[3] is not the destination, but it can send out again, shows that receive
    // can receive message which does not reach destination
}

//Test whether release can deal with empty buffer
TEST(RELEASE_MACHINE_NO_DATA_IN_BUFFER)
{
    SystemF sys7;
    CHECK_EQUAL("", sys7.ms_[1]->release_datagram());
}

//Test whether release can deal with filled buffer
TEST(RELEASE_MACHINE_WITH_DATA_IN_BUFFER)
{
    SystemF sys8;
    sys8.ms_[0]->allocate_datagram(sys8.ms_[2]->get_ip(), "ping");
    CHECK_EQUAL(1, sys8.ms_[0]->send()); // successfully sent one datagram
    CHECK_EQUAL("ping", sys8.ms_[2]->release_datagram());
    CHECK_EQUAL("", sys8.ms_[2]->release_datagram());
}

//Test for system::allocating datagrams, release datagrams and time_click
TEST(SYSTEM){
    System sys;
    sys.create_machine("laptop", "machine1", IP_address("192.168.0.1"));
    sys.create_machine("laptop", "machine2", IP_address("192.168.0.2"));
    sys.create_machine("laptop", "machine3", IP_address("192.168.0.3"));
    sys.create_machine("laptop", "machine4", IP_address("192.168.0.4"));
    sys.connect_machine(IP_address("192.168.0.1"), IP_address("192.168.0.2"));
    sys.time_click();
    sys.allocate_datagram(IP_address("192.168.0.1"), IP_address("192.168.0.2"), "hi");
    sys.allocate_datagram(IP_address("192.168.0.1"), IP_address("193.168.0.7"), "hi");
    sys.release_datagram(IP_address("192.168.0.2"));
    CHECK_THROW(sys.allocate_datagram(IP_address("192.168.0.5"), IP_address("192.168.0.2"), "hi"),
                err_code);
    CHECK_THROW(sys.release_datagram(IP_address("192.168.0.5")),
                err_code);
    sys.time_click();
}



// Following are checks from hw06



