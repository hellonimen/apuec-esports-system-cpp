#ifndef CIRCULARQUEUE_HPP
#define CIRCULARQUEUE_HPP

#include <stdexcept>

template <typename T>
class CircularQueue {
private:
    T* array;
    int front;
    int rear;
    int capacity;
    int size;

public:
    CircularQueue(int cap)
      : array(new T[cap]), front(0), rear(-1), capacity(cap), size(0) {}

    ~CircularQueue() {
        delete[] array;
    }
    
    void enqueue(const T& value) {
        if (isFull()) throw std::overflow_error("Queue is full");
        rear = (rear + 1) % capacity;
        array[rear] = value;
        ++size;
    }
    
    T dequeue() {
        if (isEmpty()) throw std::underflow_error("Queue is empty");
        T value = array[front];
        front = (front + 1) % capacity;
        --size;
        return value;
    }
    
    T getFront() const {
        if (isEmpty()) throw std::underflow_error("Queue is empty");
        return array[front];
    }
    
    bool isEmpty() const { return size == 0; }
    bool isFull()  const { return size == capacity; }
    int  getSize() const { return size; }
    
    void clear() {
        front = 0;
        rear  = -1;
        size  = 0;
    }
};

#endif // CIRCULARQUEUE_HPP