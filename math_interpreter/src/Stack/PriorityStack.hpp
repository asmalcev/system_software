#pragma once

template <typename T>
class PriorityStack {
protected:
struct Node {
  T data;
  Node * below;
};

size_t size;
size_t maxSize;
Node * top;

public:
PriorityStack(int maxSizeOfStack = -1) {
  top     = nullptr;
  maxSize = maxSizeOfStack;
  size    = 0;
}

~PriorityStack() {
  Node * tmp;
  while (top != nullptr) {
    tmp = top;
    top = top->below;
    delete tmp;
  }
}

void push(T data) {
  if (maxSize != -1 && maxSize == size) {
		throw std::out_of_range("Stack is full");
  } else {
    Node * tmp = new Node;
    tmp->data  = data;

    if (top == nullptr) {
      top        = tmp;
      top->below = nullptr;
    } else {
      Node * current = top,
           * prev    = top;

      while (current->below && data <= current->data) {
        prev    = current;
        current = current->below;
      }

      if (current == prev) {
        if (data < top->data) {
          tmp->below = top->below;
          top->below = tmp;
        } else {
          tmp->below = top;
          top        = tmp;
        }
      } else if (current->below == nullptr) {
        if (data <= current->data) {
          current->below = tmp;
          tmp->below     = nullptr;
        } else {
          prev->below = tmp;
          tmp->below  = current;
        }
      } else {
        prev->below = tmp;
        tmp->below  = current;
      }
    }
    size++;
  }
}

T getTop() const {
  if (top == nullptr) {
    throw std::out_of_range("Stack is empty");
  } else {
    return top->data;
  }
}

T pull() {
  if (top == nullptr) {
    throw std::out_of_range("Stack is empty");
  } else {
    Node * tmp;
    tmp            = top;
    T dataToReturn = tmp->data;
    top            = top->below;

    delete tmp;
    return dataToReturn;
    size--;
  }
}

bool isEmpty() {
  return top == nullptr;
}

bool isFull() {
  return size == maxSize;
}
};