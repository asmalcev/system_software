#include <iostream>
#include "string.h"

template <typename T>
class LinkedList {
private:
	struct Node {
		T data;
		Node * next;
		Node * prev;
	};

	size_t length;
	Node * head;
	Node * current;
	Node * tail;

public:

LinkedList() {
	head    = nullptr;
	current = nullptr;
	tail    = nullptr;
	length  = 0;
}

~LinkedList() {
	clear();
}

void clear() {
	while (head != nullptr) {
		current = head->next;
		delete head;
		head = current;
	}

	head    = nullptr;
	current = nullptr;
	tail    = nullptr;
	length  = 0;
}

const size_t getLength() {
	return length;
}

void pushNode(T data) {
	Node *tmp = new Node;
	tmp->data = data;
	tmp->next = nullptr;

	if (head == nullptr) {
		head = tail = tmp;
	} else {
		tail->next = tmp;
		tmp->prev = tail;
		tail = tail->next;
	}

	length++;
}

void shiftNode(T data) {
	Node *tmp = new Node;
	tmp->data = data;
	tmp->prev = nullptr;

	if (head == nullptr) {
		head = tail = tmp;
	} else {
		head->prev = tmp;
		tmp->next = head;
		head = head->prev;
	}

	length++;
}

void addNode(T data, size_t index) {
	if (index < 0 || index >= length) {
		throw std::out_of_range("Index is out of range");
	} else if (index == 0) {
		shiftNode(data);
	} else if (index == length - 1) {
		pushNode(data);
	} else {
		Node *cur = head;
		for (size_t i = 0; i < index - 1; i++) {
			cur = cur->next;
		}
		Node *tmp = new Node;
		tmp->data = data;
		tmp->prev = cur;
		tmp->next = cur->next;
		tmp->next->prev = tmp;
		cur->next = tmp;
		length++;
	}
}

bool empty() {
	return head == nullptr;
}

class iterator {
private:
  Node * current;
public:
iterator(Node * init) : current(init) {}

~iterator() {}

inline bool operator==(const iterator& rhs) const {
  return current == rhs.current;
}

inline bool operator!=(const iterator& rhs) const {
  return current != rhs.current;
}

inline bool hasNext() const {
	return current->next != nullptr;
}

inline bool hasPrev() const {
	return current->prev != nullptr;
}

T& operator++() {
  if (current) {
    current = current->next;
  }

  return current->data;
}

T& operator--() {
  if (current) {
    current = current->prev;
  }

  return current->data;
}

T& operator*() {
  return current->data;
}
};

iterator begin() {
  return iterator(head);
}

iterator end() {
  return iterator(nullptr);
}

iterator rbegin() {
  return iterator(tail);
}

iterator rend() {
  return iterator(nullptr);
}
};