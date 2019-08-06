#include "linked_lib.h"

#include <UnitTest++/UnitTest++.h>
#include <memory>
#include <iostream>
#include <stdexcept>

using namespace std;

// Example test case for push_back
// Testing push_back on a singleton
TEST(Push_back_of_one_element) {
  // Initialize the input linked-list to push_back
  List xs = make_shared<ListNode>();
  xs->data = 514;
  List ys = xs;
    List x2s = nullptr;
    push_back(x2s, 321);
  push_back(xs, 2017211);

  // Check that the output is a 2-element linked-list with correct order
  CHECK_EQUAL(ys, xs);
  CHECK_EQUAL(514, xs->data);
  CHECK(xs->next != nullptr);
  CHECK_EQUAL(2017211, xs->next->data);
  // Note: It is impossible to apply CHECK_EQUAL on nullptr, so we use
  // CHECK and manually compares for equality
  CHECK(xs->next->next == nullptr);
    CHECK(x2s->next == nullptr);
    CHECK_EQUAL(x2s->data, 321);
}

TEST(filter_nodes_with_data_over_limit){
  List xs = make_shared<ListNode>();
  xs -> data = 5;
  push_back(xs, 2);
  push_back(xs, 8);
  push_back(xs, 3);
  push_back(xs, 9);
  push_back(xs, 7);
  List ys = make_shared<ListNode>();
  ys -> data = 2;
  push_back(ys, 3);
  filter_lt(xs, 5);
  CHECK_EQUAL(ys -> data, xs -> data);
  CHECK_EQUAL(ys -> next -> data , xs -> next -> data);
  CHECK_EQUAL(ys -> next -> next, xs -> next -> next);
  CHECK_EQUAL(true, check_equal(xs, ys));

  List x2s = nullptr;
  List y2s = nullptr;
  filter_lt(x2s, 0);
  CHECK_EQUAL(y2s, x2s);
  CHECK_EQUAL(true, check_equal(x2s, y2s));

  List x3s = make_shared<ListNode>();
  x3s -> data = -5;
  List y3s = make_shared<ListNode>();
  y3s -> data = -5;
  filter_lt(x3s, -3);
  CHECK_EQUAL(y3s -> data, x3s -> data);
  CHECK_EQUAL(y3s -> next, x3s -> next);
  CHECK_EQUAL(true, check_equal(x3s, y3s));

  List x4s = make_shared<ListNode>();
  x4s -> data = 4;
  List y4s = nullptr;
  filter_lt(x4s, 3);
  CHECK_EQUAL(y4s, x4s);
  CHECK_EQUAL(true, check_equal(x4s, y4s));

  List x5s = make_shared<ListNode>();
  x5s -> data = 0;
  push_back(x5s, 5);
  List y5s = make_shared<ListNode>();
  y5s -> data = 0;
  filter_lt(x5s, 1);
  CHECK_EQUAL(y5s -> data, x5s -> data);
  CHECK_EQUAL(y5s -> next, x5s -> next);
  CHECK_EQUAL(true, check_equal(x5s, y5s));

  List x6s = make_shared<ListNode>();
  x6s -> data = 5;
  push_back(x6s, 1);
  List y6s = make_shared<ListNode>();
  y6s -> data = 1;
  filter_lt(x6s, 3);
  CHECK_EQUAL(y6s -> data, x6s -> data);
  CHECK_EQUAL(y6s -> next, x6s -> next);
  CHECK_EQUAL(true, check_equal(x6s, y6s));

  List x7s = make_shared<ListNode>();
  x7s -> data = 5;
  push_back(x7s, 4);
  List y7s = make_shared<ListNode>();
  y7s = nullptr;
  filter_lt(x7s, -3);
  CHECK_EQUAL(y7s, x7s);
  CHECK_EQUAL(true, check_equal(x7s, y7s));


  List x8s = make_shared<ListNode>();
  x8s -> data = 1;
  push_back(x8s, 2);
  List y8s = make_shared<ListNode>();
  y8s -> data = 1;
  push_back(y8s, 2);
  filter_lt(x8s, 4);
  CHECK_EQUAL(y8s -> data, x8s -> data);
  CHECK_EQUAL(y8s -> next -> data, x8s -> next -> data);
  CHECK_EQUAL(y8s -> next -> next, x8s -> next -> next);
  CHECK_EQUAL(true, check_equal(x8s, y8s));

  List x9s = make_shared<ListNode>();
  x9s -> data = 2;
  push_back(x9s, 1);
  push_back(x9s, 4);
  List y9s = make_shared<ListNode>();
  y9s -> data = 2;
  push_back(y9s, 1);
  filter_lt(x9s, 4);
  CHECK_EQUAL(y9s -> data, x9s -> data);
  CHECK_EQUAL(y9s -> next -> data, x9s -> next -> data);
  CHECK_EQUAL(y9s -> next -> next, x9s -> next -> next);
  CHECK_EQUAL(true, check_equal(x9s, y9s));

  List x10s = make_shared<ListNode>();
  x10s -> data = 2;
  push_back(x10s, 1);
  push_back(x10s, 0);
  push_back(x10s, 10);
  push_back(x10s, 3);
  push_back(x10s, 4);
  push_back(x10s, 5);
  List y10s = make_shared<ListNode>();
  y10s -> data = 2;
  push_back(y10s, 1);
  push_back(y10s, 0);
  push_back(y10s, 3);
  push_back(y10s, 4);
  push_back(y10s, 5);
  filter_lt(x10s, 7);
  CHECK_EQUAL(y10s -> data, x10s -> data);
  CHECK_EQUAL(y10s -> next -> data, x10s -> next -> data);
  CHECK_EQUAL(y10s -> next -> next -> data, x10s -> next -> next -> data);
  CHECK_EQUAL(y10s -> next -> next -> next -> data, x10s -> next -> next -> next -> data);
  CHECK_EQUAL(y10s -> next -> next -> next -> next -> data, x10s -> next -> next -> next -> next -> data);
  CHECK_EQUAL(y10s -> next -> next -> next -> next -> next -> data, x10s -> next -> next -> next -> next -> next -> data);
  CHECK_EQUAL(y10s -> next -> next -> next -> next -> next -> next, x10s -> next -> next -> next -> next -> next -> next);
  CHECK_EQUAL(true, check_equal(x10s, y10s));

  List x11s = make_shared<ListNode>();
  x11s -> data = 2;
  push_back(x11s, 1);
  push_back(x11s, 10);
  push_back(x11s, 11);
  push_back(x11s, 12);
  push_back(x11s, -4);
  push_back(x11s, 5);
  List y11s = make_shared<ListNode>();
  y11s -> data = 2;
  push_back(y11s, 1);
  push_back(y11s, -4);
  push_back(y11s, 5);
  filter_lt(x11s, 10);
  CHECK_EQUAL(true, check_equal(x11s, y11s));

  List x12s = make_shared<ListNode>();
  x12s -> data = 2;
  push_back(x12s, 10);
  push_back(x12s, 0);
  push_back(x12s, 1);
  push_back(x12s, 3);
  push_back(x12s, 11);
  push_back(x12s, 5);
  List y12s = make_shared<ListNode>();
  y12s -> data = 2;
  push_back(y12s, 0);
  push_back(y12s, 1);
  push_back(y12s, 3);
  push_back(y12s, 5);
  filter_lt(x12s, 7);
  CHECK_EQUAL(true, check_equal(x12s, y12s));

  List x13s = make_shared<ListNode>();
  x13s -> data = 2;
  push_back(x13s, 10);
  push_back(x13s, 11);
  push_back(x13s, -1);
  push_back(x13s, 13);
  push_back(x13s, 12);
  push_back(x13s, 5);
  List y13s = make_shared<ListNode>();
  y13s -> data = 2;
  push_back(y13s, -1);
  push_back(y13s, 5);
  filter_lt(x13s, 7);
  CHECK_EQUAL(true, check_equal(x13s, y13s));

  List x14s = make_shared<ListNode>();
  x14s -> data = 12;
  push_back(x14s, 10);
  push_back(x14s, -1);
  push_back(x14s, 2);
  push_back(x14s, 3);
  push_back(x14s, 11);
  push_back(x14s, 15);
  List y14s = make_shared<ListNode>();
  y14s -> data = -1;
  push_back(y14s, 2);
  push_back(y14s, 3);
  filter_lt(x14s, 7);
  CHECK_EQUAL(true, check_equal(x14s, y14s));

  List x15s = make_shared<ListNode>();
  x15s -> data = 12;
  push_back(x15s, 11);
  push_back(x15s, 14);
  push_back(x15s, 21);
  push_back(x15s, 33);
  push_back(x15s, 1000);
  push_back(x15s, 123);
  List y15s = make_shared<ListNode>();
  y15s = nullptr;
  filter_lt(x15s, 11);
  CHECK_EQUAL(true, check_equal(x15s, y15s));

  List x16s = make_shared<ListNode>();
  x16s -> data = 2;
  push_back(x16s, 11);
  push_back(x16s, 8);
  List y16s = make_shared<ListNode>();
  y16s -> data = 2;
  filter_lt(x16s, 8);
  CHECK_EQUAL(true, check_equal(x16s, y16s));

}

TEST(Pop_front){
    List test1 = nullptr;
    List test2 = make_shared<ListNode>();
    List test3 = make_shared<ListNode>();
    List test4 = make_shared<ListNode>();
    test2 -> data = 13;
    test3 -> data = 4;
    test4 -> data = 10;
    push_back(test2, 5);
    push_back(test4, 6);
    pop_front(test3);
    CHECK_EQUAL(13, pop_front(test2) -> data);
    CHECK(nullptr == pop_front(test4) -> next);
    CHECK(nullptr == test3);
    CHECK_THROW(pop_front(test1), runtime_error);
    CHECK_EQUAL(5, test2 -> data);
}

TEST(the_nth_element){
  List xs = nullptr;
  CHECK_THROW(nth_element(xs, 1), runtime_error);

  List x2s = make_shared<ListNode>();
  x2s -> data = 3;
  push_back(x2s, -2);
  push_back(x2s, 0);
    CHECK_THROW(nth_element(x2s,-1), runtime_error);
  CHECK_THROW(nth_element(x2s,3), runtime_error);
  CHECK_THROW(nth_element(x2s,5), runtime_error);
  CHECK_EQUAL(nth_element(x2s,0), 3);
  CHECK_EQUAL(nth_element(x2s,1), -2);
  CHECK_EQUAL(nth_element(x2s,2), 0);
}

// This function overloads the '<<' operator to enable printing on List
// types. For example, 'cout << xs << '\n';' will compile and print its
// content. However, we are still not able to write tests such as
// CHECK_EQUAL(nullptr, xs) since nullptr might be of any pointer type
// and the '<<' operator does not know how to print it.
ostream& operator<<(ostream& out, const List& lst)
{
  if (lst) {
    out << "<ListNode@" << static_cast<void*>(lst.get());
    out << " data=" << lst->data;
    out << " next=" << static_cast<void*>(lst->next.get());
    out << ">";
  } else {
    out << "(nullptr)";
  }
  return out;
}
