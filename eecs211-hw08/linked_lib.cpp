#include "linked_lib.h"

#include <iostream>

using namespace std;

ListNode::ListNode()
{
    data = nullptr;
    next = nullptr;
}

ListNode::~ListNode()
{
    if (data != nullptr){
        data = nullptr;
    }
}

void push_back(List& front, Datagram* data)
{
    if (front == nullptr) {
        ListNode newListNode = ListNode();
        newListNode.data = data;
        newListNode.next = nullptr;
        front = make_shared<ListNode>(newListNode);
    }
    else {
        List temp = front;
        List temp2 = make_shared<ListNode>();
        temp2->data = data;
        while (temp->next != nullptr) temp = temp->next;
        temp->next = temp2;
    }
}

List pop_front(List& front)
{
    if(front == nullptr) throw runtime_error("input is an empty list");
    List newPtr = front;
    front = front -> next;
    newPtr -> next = nullptr;
    return newPtr;
}
void list_format(ostream& os, const List& front)
{
    if (front == nullptr) {
        os << "[]";
        return;
    }
    os << "[\n";
    List curr = front;
    while (curr != nullptr) {
        os << "      " << curr->data;
        curr = curr->next;
        if (curr != nullptr)
            os << ",\n";
    }
    os << "\n    ]";
}
