#include "system.h"
#include "machines.h"

#include <string>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

System::System()
        : tick_(0) {}

void System::format(std::ostream& os, const vector<IP_address>& ips0) const
{
    vector<IP_address> ips = ips0;
    if (ips.empty()) {
        for (size_t i = 0; i < network_.size(); ++i)
            if (network_[i] != nullptr)
                ips.push_back(network_[i]->get_ip());
    }
    os << "system {\n  tick = " << tick_ << ",\n";
    for (IP_address& ip : ips) {
        os << "  ";
        os << ip;
        os << " = ";
        int idx = -1;
        for (int i = 0; i < MAX_MACHINES; ++i) {
            if (network_[i] != nullptr && network_[i]->get_ip() == ip) {
                idx = i;
                break;
            }
        }
        if (idx < 0)
            throw err_code::no_such_machine;
        os << *network_[idx];
        os << '\n';
    }
    os << "}\n";
}

void System::create_machine(const string& type,
                            const string& name,
                            const IP_address& ip)
{
    if (type != "laptop" && type != "server" && type != "wan")
        throw err_code::unknown_machine_type;

    for (int i = 0; i < network_.size(); ++i){
        if (network_[i] == nullptr){
            if (type == "laptop") {
                network_[i] = make_shared<Laptop>(name, ip);
            }else if(type == "server"){
                network_[i] = make_shared<Server>(name, ip);
            }else
                network_[i] = make_shared<WAN_node>(name, ip);
            cout << "System::create_machine: " << type << ' ' << name << ' ';
            cout << ip;
            cout << '\n';
            return;
        }
    }
    throw err_code::network_full;
}

void System::delete_machine(const IP_address& ip)
{
    std::shared_ptr<Node> disconnected_machine = nullptr;
    for (int i = 0; i < network_.size(); ++i) {
        if (network_[i] != nullptr && (*network_[i]).get_ip() == ip) {
            disconnected_machine = network_[i];
            network_[i] = nullptr;
            break;
        }
    }

    if (disconnected_machine == nullptr)
        throw err_code::no_such_machine;

    for (int j = 0; j < network_.size(); ++j){
        if ((network_[j] != nullptr) && (!((*network_[j]).get_ip() == ip))){
            (*network_[j]).disconnect(disconnected_machine);
        }
    }
    cout << "System::delete_machine: ";
    cout << ip;
    cout << '\n';
}

void System::connect_machine(const IP_address& ip1, const IP_address& ip2)
{
    for (int i = 0; i < MAX_MACHINES; ++i){
        if ((network_[i] != nullptr) && (*network_[i]).get_ip() == ip1){
            for (int j = 0; j < MAX_MACHINES; ++j){
                if ((network_[j] != nullptr) && (*network_[j]).get_ip() == ip2){
                    if(network_[j] -> can_connect(network_[i]) && network_[i] -> can_connect(network_[j])) {
                        (network_[j])->connect(network_[i]);
                        (network_[i])->connect(network_[j]);
                        cout << "System::connect_machine: ";
                        cout << ip1;
                        cout << ", ";
                        cout << ip2;
                        cout << '\n';
                        return;
                    }
                }
            }
        }
    }
    throw err_code::no_such_machine;// [YOUR HW6 CODE HERE; also, check can_connect() first]
}

std::shared_ptr<Node> System::find_machine(const IP_address& ip){
    for(size_t i = 0; i < 50; i++){
        if (network_[i] != nullptr && network_[i] -> get_ip() ==  ip){
            return network_[i];
        }
    }
    throw err_code::no_such_machine;
}

void System::allocate_datagram(const IP_address& ip1,
                               const IP_address& ip2,
                               const string& message)
{
    shared_ptr<Node> src = find_machine(ip1);
    shared_ptr<Node> dst = find_machine(ip2);
    shared_ptr<Laptop> x = dynamic_pointer_cast<Laptop>(src);
    shared_ptr<Laptop> y = dynamic_pointer_cast<Laptop>(dst);
    if (x == nullptr || y == nullptr) throw err_code::no_such_machine;
    x -> allocate_datagram(ip2, message);
    cout << "System::allocate_datagram: ";
    cout << ip1;
    cout << ", ";
    cout << ip2;
    cout << ", \"" << message << "\"\n";
}

void System::release_datagram(const IP_address& ip)
{
    shared_ptr<Node> m = find_machine(ip);
    shared_ptr<Laptop> x = dynamic_pointer_cast<Laptop>(m);
    if (x == nullptr) throw err_code::no_such_machine;
    x -> release_datagram();

    cout << "System::release_datagram: ";
    cout << ip;
    cout << '\n';
}

void System::time_click()
{
    for(std::shared_ptr<Node>& machine : network_){
        if(machine == nullptr) break;
        machine -> send();
    }
    ++tick_;
    cout << "System::time_click\n";
}

std::ostream& operator<<(std::ostream& os, const System& sys)
{
    sys.format(os, {});
    return os;
}
