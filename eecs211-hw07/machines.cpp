#include "machines.h"
#include "errors.h"

#include <iostream>
#include <stdlib.h>
#include <string>
#include <cmath>

using namespace std;

Node::Node(const std::string& n, IP_address ip)
        : local_ip_(ip), name_(n), incoming_(nullptr)
{}

Node::~Node()
{
    if (incoming_ == nullptr)
        return;
    else{
        delete incoming_;
        incoming_ = nullptr;
    }
}

void Node::format(ostream& os) const
{
    os << "node {\n";
    os << "    name = \"" << name_ << "\",\n    local_ip = ";
    os << local_ip_;
    os << ",\n    incoming_ = ";
    if (incoming_ == nullptr) os << "nullptr";
    else                      os << *incoming_;
    os << ",\n    node_list = [ ";
    for (auto& node : node_list_) {
        os << node->get_ip();
        os << ' ';
    }
    os << "],\n    data_list = ";
    list_format(os, data_list_);
    os << "\n  }";
}

IP_address Node::get_ip() const
{
    return local_ip_;
}

void Node::connect(const shared_ptr<Node>& x)
{
    node_list_.push_back(x);
}

void Node::disconnect(const shared_ptr<Node>& x)
{
    if (x == nullptr)
        return;
    for(int i = 0; i < node_list_.size(); i++){
        if ((*node_list_[i]).name_ == (*x).name_){
            node_list_[i] = node_list_[node_list_.size() - 1];
            node_list_.pop_back();
        }
    }
}

size_t Node::send()
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
            //loop below go over each linked machine in node_list_
            for (int i = 0; i < node_list_.size(); i++) {

                //whether ==
                if ((*(current_node->data)).get_destination() == (*node_list_[i]).get_ip()) {
                    direct_or_not = true;
                    try {
                        (*node_list_[i]).receive(current_node->data);
                    } catch (err_code) {
                        push_back(fail_data_list, current_node->data);
                        current_node -> data = nullptr;
//                        delete current_node -> data;
//                        current_node -> data = nullptr;
                        break;//start next data
                    }
                    current_node -> data = nullptr;
//                    delete current_node -> data;
//                    current_node -> data = nullptr;
                    count += 1;
                    break;//start next data
                }
            }

            if (direct_or_not) {
                direct_or_not = false;
            }
            else { //no ==, so find closet first ip number
                    int difference = 256;
                    Node* closest_ptr = &(*node_list_[0]);
                    for (int j = 0; j < node_list_.size(); j++) {
                        if (abs(((*(current_node->data)).get_destination().first_octad())
                                - (*node_list_[j]).get_ip().first_octad()) < difference) {
                            difference = abs(((*(current_node->data)).get_destination().first_octad())
                                             - (*node_list_[j]).get_ip().first_octad());
                            closest_ptr = &(*node_list_[j]);
                        }
                    }

                    try {
                        (*closest_ptr).receive(current_node->data);
                    } catch (err_code) {
                        push_back(fail_data_list, current_node->data);
                        current_node -> data = nullptr;
//                        delete current_node -> data;
//                        current_node -> data = nullptr;
                        //delete closest_ptr;
                        break;//start next data
                    }
                current_node -> data = nullptr;
//                    delete current_node -> data;
//                    current_node -> data = nullptr;
                    //delete closest_ptr;
                    count += 1;
                    break;//start next data
                }
        }
        data_list_ = fail_data_list;
    }
    return count;
}


void Node::receive(Datagram* d)
{
    if ((*d).get_destination() == local_ip_){
        if (incoming_ == nullptr){
            incoming_ = d;
        }
        else{
            throw err_code::recv_blocked;
        }
    }
    else{
        push_back(data_list_, d);
    }
}

void Node::allocate_datagram(const IP_address& dst, const string& message)
{
    Datagram* data_ptr  = new Datagram(local_ip_, dst, message);
    push_back(data_list_, data_ptr);
}

std::string Node::release_datagram()
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

std::ostream& operator<<(std::ostream& os, const Node& n)
{
    n.format(os);
    return os;
}
