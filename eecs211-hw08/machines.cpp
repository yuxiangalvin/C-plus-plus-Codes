#include "machines.h"
#include "errors.h"
#include "linked_lib.h"

#include <iostream>
#include <stdlib.h>
#include <string>
#include <cmath>

using namespace std;

Node::Node(const string& n, IP_address ip)
        : local_ip_(ip), name_(n)
{}

Node::~Node() {}

void Node::format(ostream& os) const
{
    os << "    name = \"" << name_ << "\",\n    local_ip = " << local_ip_;
}

IP_address Node::get_ip() const
{
    return local_ip_;
}


Laptop::Laptop(const string& n, IP_address ip)
        : Node(n, ip)
{
    incoming_ = nullptr;
    outgoing_ = nullptr;
    server_ = nullptr;
}

bool Laptop::can_connect(const shared_ptr<Node>& x) const
{
    shared_ptr<Node> s = x;
    shared_ptr<Server> new_server = dynamic_pointer_cast<Server>(s);
    return (server_ == nullptr && new_server != nullptr);
    //return true;
}

void Laptop::connect(const shared_ptr<Node>& x)
{
    if (this -> can_connect(x)) {
        shared_ptr<Node> s = x;
        shared_ptr<Server> new_server = dynamic_pointer_cast<Server>(x);
        server_ = new_server;
    }else throw err_code::connect_failed;
}

void Laptop::disconnect(const shared_ptr<Node>& x)
{
    if (x == nullptr)
        return;
    if ((*server_).get_ip() == (*x).get_ip()){
        server_ = nullptr;
    }
}

size_t Laptop::send()
{
    if (server_ == nullptr || outgoing_ == nullptr) {
        return 0;
    }
    else {
        server_->receive(outgoing_);
        outgoing_ = nullptr;
        return 1;
    }
}

void Laptop::receive(Datagram* d)
{
    if ((*d).get_destination() == this->get_ip() && incoming_ == nullptr){
        incoming_ = d;
    }
    else{
        throw err_code::recv_blocked;
    }
}

void Laptop::allocate_datagram(const IP_address& dst,
                               const string& message)
{
    if(outgoing_ == nullptr){
        outgoing_ = new Datagram (this->get_ip(), dst, message);
    }else throw err_code::send_blocked;
}

string Laptop::release_datagram()
{
    if (incoming_ == nullptr){
        return"";
    }
    else{
        std::string message_out = (*incoming_).get_msg();
        Datagram* temp = incoming_;
        incoming_ = nullptr;
        delete temp;
        return message_out;
    }
}

void Laptop::format(ostream& os) const
{
    os << "laptop {\n";
    Node::format(os);
    os << ",\n    incoming = ";
    if (incoming_) os << *incoming_;
    else           os << "nullptr";
    os << ",\n    outgoing = ";
    if (outgoing_) os << *outgoing_;
    else           os << "nullptr";
    os << ",\n    server = ";
    if (server_ != nullptr) os << server_->get_ip();
    else                    os << "nullptr";
    os << "\n  }";
}

Laptop::~Laptop()
{
    if (incoming_ == nullptr)
        return;
    else{
        Datagram* temp = incoming_;
        incoming_ = nullptr;
        delete temp;
    }
    if (outgoing_ == nullptr)
        return;
    else{
        Datagram* temp = outgoing_;
        outgoing_ = nullptr;
        delete temp;
    }
}


Server::Server(const string& n, IP_address a)
        : Node(n, a) // REPLACE THIS LINE BY YOUR INITIALIZATION CODE HERE
{
    //node_list_ = nullptr;
    data_list_ = nullptr;
    num_laptops_ = 0;
    num_wans_ = 0;
}

bool Server::can_connect(const shared_ptr<Node>& x) const
{
    shared_ptr<Node> l = x;
    shared_ptr<Laptop> new_laptop = dynamic_pointer_cast<Laptop>(l);
    shared_ptr<Node> w = x;
    shared_ptr<WAN_node> new_wan = dynamic_pointer_cast<WAN_node>(w);
    return ((num_laptops_ < 8 && new_laptop != nullptr)
       || (num_wans_ < 4 && new_wan != nullptr));// [YOUR CODE HERE]
    //return true;
}

void Server::connect(const shared_ptr<Node>& x)
{
    if (this -> can_connect(x)) {
        shared_ptr<Node> s = x;
        shared_ptr<Laptop> new_laptop = dynamic_pointer_cast<Laptop>(x);
        if (new_laptop == nullptr){
            shared_ptr<WAN_node> new_wan = dynamic_pointer_cast<WAN_node>(x);
            node_list_.push_back(new_wan);
            num_wans_++;
        }else {
            node_list_.push_back(new_laptop);
            num_laptops_++;
        }
    }// [YOUR CODE HERE; similar to your HW6 Node::connect]
}

void Server::disconnect(const shared_ptr<Node>& x)
{
    if (x == nullptr)
        return;
    for(int i = 0; i < node_list_.size(); i++){
        if ((*node_list_[i]).get_ip() == (*x).get_ip()){
            node_list_[i] = node_list_[node_list_.size() - 1];
            node_list_.pop_back();
        }
    }
}

void Server::receive(Datagram* d)
{
    cout << this->get_ip() << "receive" << "\n";
    push_back(data_list_, d);
}

void Server::format(ostream& os) const
{
    os << "server {\n";
    Node::format(os);
    os << ",\n    node_list = [ ";
    for (auto& node : node_list_)
        os << node->get_ip() << ' ';
    os << "],\n    data_list = ";
    list_format(os, data_list_);
    os << "\n  }";
}

size_t Server::send() {
    size_t count = 0;
    List fail_data_list = nullptr;

    if (node_list_.empty()) {
        return 0;
    } else {
        List current_node = nullptr;
        // loop through all *Datagrams
        while (data_list_ != nullptr) {
            bool direct_or_not = false;
            current_node = pop_front(data_list_);
            //loop below go over each linked machine in node_list_
            for (int i = 0; i < node_list_.size(); i++) {
                //whether ==
                shared_ptr<Laptop> isLaptop = dynamic_pointer_cast<Laptop>(node_list_[i]);
                if ((*(current_node->data)).get_destination() == (*node_list_[i]).get_ip()
                        && isLaptop != nullptr) {
                    direct_or_not = true;
                    try {
                        (*node_list_[i]).receive(current_node->data);
                    } catch (err_code) {
                        push_back(fail_data_list, current_node->data);
                        current_node->data = nullptr;
                        break;//start next data
                    }
                    current_node->data = nullptr;
                    count += 1;
                    break;//start next data
                }
            }
            //no ==, so find closet first ip number
            if (!direct_or_not) {
                int difference = 256;
                Node *closest_ptr = &(*node_list_[0]);
                for (int j = 0; j < node_list_.size(); j++) {
                    shared_ptr<Laptop> isLaptop = dynamic_pointer_cast<Laptop>(node_list_[j]);
                    shared_ptr<WAN_node> isWan = dynamic_pointer_cast<WAN_node>(node_list_[j]);
                    if (abs(((*(current_node->data)).get_destination().first_octad())
                            - (*node_list_[j]).get_ip().first_octad()) < difference
                            && isLaptop == nullptr && isWan != nullptr) {
                        difference = abs(((*(current_node->data)).get_destination().first_octad())
                                         - (*node_list_[j]).get_ip().first_octad());
                        closest_ptr = &(*node_list_[j]);
                    }
                }

                try {
                    (*closest_ptr).receive(current_node->data);
                } catch (err_code) {
                    push_back(fail_data_list, current_node->data);
                    current_node->data = nullptr;
                    //delete closest_ptr;
                    break;//start next data
                }
                current_node->data = nullptr;
                //delete closest_ptr;
                count += 1;
                break;//start next data
            }
        }
        data_list_ = fail_data_list;
    }
        return count;
}

Server::~Server()
{
    while (data_list_ != nullptr) {
        List current_node = pop_front(data_list_);
        delete current_node -> data;
    }
}


WAN_node::WAN_node(const string& n, IP_address a)
        : Node(n, a) // REPLACE THIS LINE BY YOUR INITIALIZATION CODE HERE
{
    num_servers_ = 0;
    num_wans_ = 0;
    //node_list_ = nullptr;
    data_list_ = nullptr;
}

bool WAN_node::can_connect(const shared_ptr<Node>& x) const
{
    shared_ptr<Node> s = x;
    shared_ptr<Server> new_server = dynamic_pointer_cast<Server>(s);
    shared_ptr<Node> w = x;
    shared_ptr<WAN_node> new_wan = dynamic_pointer_cast<WAN_node>(w);
    return ((num_servers_ < 4 && new_server != nullptr)
       || (num_wans_ < 4 && new_wan != nullptr));
//    return true;
}

void WAN_node::connect(const shared_ptr<Node>& x)
{
    if (this -> can_connect(x)) {
        shared_ptr<Node> s = x;
        shared_ptr<Server> new_server = dynamic_pointer_cast<Server>(s);
        if (new_server == nullptr){
            shared_ptr<WAN_node> new_wan = dynamic_pointer_cast<WAN_node>(x);
            node_list_.push_back(new_wan);
            num_wans_++;
        }else{
            node_list_.push_back(new_server);
            num_servers_++;
        }
    }// [YOUR CODE HERE; similar to your HW6 Node::connect]
}

void WAN_node::disconnect(const shared_ptr<Node>& x)
{
    if (x == nullptr)
        return;
    for(int i = 0; i < node_list_.size(); i++){
        if ((*node_list_[i]).get_ip() == (*x).get_ip()){
            node_list_[i] = node_list_[node_list_.size() - 1];
            node_list_.pop_back();
        }
    }
}

void WAN_node::receive(Datagram* d)
{
    push_back(data_list_, d);
}


void WAN_node::format(ostream& os) const
{
    os << "WAN_node {\n";
    Node::format(os);
    os << ",\n    node_list = [ ";
    for (auto& node : node_list_)
        os << node->get_ip() << ' ';
    os << "],\n    data_list = ";
    list_format(os, data_list_);
    os << "\n  }";
}

size_t WAN_node::send()
{
    size_t count = 0;
    List fail_data_list = nullptr;
    bool direct_or_not = false;

    if (node_list_.empty()) {
        return 0;
    }
    else {
        List current_node = nullptr;
        // loop through all *Datagrams
        while (data_list_ != nullptr) {
            current_node = pop_front(data_list_);
            Node* closest_ptr = &(*node_list_[0]);
            int difference = 256;
            //loop below go over each linked machine in node_list_
            for (int i = 0; i < node_list_.size(); i++) {
                if (abs(((*(current_node->data)).get_destination().first_octad())
                        - (*node_list_[i]).get_ip().first_octad()) < difference) {
                    difference = abs(((*(current_node->data)).get_destination().first_octad())
                                     - (*node_list_[i]).get_ip().first_octad());
                    closest_ptr = &(*node_list_[i]);
                }
            }
            (*closest_ptr).receive(current_node->data);
            current_node->data = nullptr;
            //delete closest_ptr;
            count += 1;//start next data
        }
    }
    return count;
}

WAN_node::~WAN_node()
{
    // loop through all *Datagrams
    while (data_list_ != nullptr) {
        List current_node = pop_front(data_list_);
        delete current_node -> data;
    }
}

std::ostream& operator<<(std::ostream& os, const Node& n)
{
    n.format(os);
    return os;
}