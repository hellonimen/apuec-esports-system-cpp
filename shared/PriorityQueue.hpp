#ifndef PRIORITYQUEUE_HPP
#define PRIORITYQUEUE_HPP

#include <stdexcept>

template <typename T>
class PriorityQueue {
private:
    struct Node {
        T data;
        int priority;
        Node* next;
        Node(const T& value, int prio)
          : data(value), priority(prio), next(nullptr) {}
    };
    
    Node* front;
    int size;

public:
    PriorityQueue()
      : front(nullptr), size(0) {}
    
    ~PriorityQueue() {
        clear();
    }
    
    void enqueue(const T& value, int priority) {
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
    
    T dequeue() {
        if (isEmpty()) throw std::underflow_error("PriorityQueue is empty");
        Node* node = front;
        front = front->next;
        T value = node->data;
        delete node;
        --size;
        return value;
    }
    
    T peek() const {
        if (isEmpty()) throw std::underflow_error("PriorityQueue is empty");
        return front->data;
    }
    
    bool isEmpty() const { return front == nullptr; }
    int  getSize() const  { return size; }
    
    void clear() {
        while (front) {
            Node* tmp = front;
            front = front->next;
            delete tmp;
        }
        size = 0;
    }
};

#endif // PRIORITYQUEUE_HPP