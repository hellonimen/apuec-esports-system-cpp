// shared/PriorityQueue.ipp

#include "PriorityQueue.hpp"
#include <stdexcept>

template<typename T>
PriorityQueue<T>::PriorityQueue()
  : front(nullptr), size(0) {}

template<typename T>
PriorityQueue<T>::~PriorityQueue() {
    clear();
}

template<typename T>
void PriorityQueue<T>::enqueue(const T& value, int priority) {
    Node* node = new Node(value, priority);
    if (!front || priority > front->priority) {
        node->next = front;
        front = node;
    } else {
        Node* curr = front;
        while (curr->next && curr->next->priority >= priority)
            curr = curr->next;
        node->next = curr->next;
        curr->next = node;
    }
    ++size;
}

template<typename T>
T PriorityQueue<T>::dequeue() {
    if (isEmpty()) throw std::underflow_error("PriorityQueue is empty");
    Node* node = front;
    front = front->next;
    T value = node->data;
    delete node;
    --size;
    return value;
}

template<typename T>
T PriorityQueue<T>::peek() const {
    if (isEmpty()) throw std::underflow_error("PriorityQueue is empty");
    return front->data;
}

template<typename T>
bool PriorityQueue<T>::isEmpty() const { return front == nullptr; }

template<typename T>
int PriorityQueue<T>::getSize() const { return size; }

template<typename T>
void PriorityQueue<T>::clear() {
    while (front) {
        Node* tmp = front;
        front = front->next;
        delete tmp;
    }
    size = 0;
}