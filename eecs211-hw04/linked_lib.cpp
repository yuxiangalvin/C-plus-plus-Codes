#include "linked_lib.h"

#include <memory>
#include <stdexcept>

using namespace std;

// Delete the nodes in linked list which is greater than limit.
void filter_lt(List& front, int limit)
{
    while ((front != nullptr) && ((front -> data) >= limit)){
            front = front -> next;
        }

    if (front == nullptr)
            return;

    List temp = front;
    while ((temp -> next) != nullptr){
        if (((temp -> next) -> data) >= limit)
            (temp->next) = ((temp->next)->next);

        else if (((temp -> next) -> data) < limit)
            temp = (temp -> next);
    }

    return;
}

// Add one element to the linked list.
void push_back(List& front, int data)
{
    if (front == nullptr) {
        front = make_shared<ListNode>(ListNode {data, nullptr});
    }
    else {
        List temp = front;
        List temp2 = make_shared<ListNode>();
        temp2->data = data;
        while (temp->next != nullptr) temp = temp->next;
        temp->next = temp2;
    }
    // [YOUR CODE HERE]
}

// Remove the first node and return the removed node.
List pop_front(List& front)
{
    if(front == nullptr) throw runtime_error("input is an empty list");
    List newPtr = front;
    front = front -> next;
    newPtr -> next = nullptr;
    return newPtr;
}

// Return the (n+1)th node
int& nth_element(List& front, size_t n)
{
    if (n < 0) throw runtime_error("n can not be negative");
    if (front == nullptr) throw runtime_error("the list is empty");
    List temp = front;
    for(int i = 0; i < n; i ++){
        if(temp == nullptr) throw runtime_error("There are not so many elements");
        temp = temp -> next;
    }
    if(temp == nullptr) throw runtime_error("There are not so many elements");
    return temp -> data;
}

// Return the size of a linked list.
size_t size_list(List front)
{
    size_t count = 0;
    while (front != nullptr){
        count++;
        front = front -> next;
    }
    return count;
}

// Check whether two linked lists are the same.
bool check_equal(List x, List y) {
    bool result = true;
    if (size_list(x) != size_list(y)) {
        result = false;
        return result;
    }

    if (x == nullptr && y == nullptr)
        return true;

    for (int i = 0; i <= size_list(x); i++) {
        if (x->data != y->data) {
            result = false;
            return result;
        }
        x = x->next;
        y = y->next;
    }
    return result;
}
