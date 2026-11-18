// shared/CircularQueue.ipp

#include "CircularQueue.hpp"
#include <stdexcept>

template<typename T>
CircularQueue<T>::CircularQueue(int cap)
  : array(new T[cap]), front(0), rear(-1), capacity(cap), size(0) {}

template<typename T>
CircularQueue<T>::~CircularQueue() {
    delete[] array;
}

template<typename T>
void CircularQueue<T>::enqueue(const T& value) {
    if (isFull()) throw std::overflow_error("Queue is full");
    rear = (rear + 1) % capacity;
    array[rear] = value;
    ++size;
}

template<typename T>
T CircularQueue<T>::dequeue() {
    if (isEmpty()) throw std::underflow_error("Queue is empty");
    T value = array[front];
    front = (front + 1) % capacity;
    --size;
    return value;
}

template<typename T>
T CircularQueue<T>::getFront() const {
    if (isEmpty()) throw std::underflow_error("Queue is empty");
    return array[front];
}

template<typename T>
bool CircularQueue<T>::isEmpty() const { return size == 0; }

template<typename T>
bool CircularQueue<T>::isFull() const { return size == capacity; }

template<typename T>
int CircularQueue<T>::getSize() const { return size; }

template<typename T>
void CircularQueue<T>::clear() {
    front = 0;
    rear  = -1;
    size  = 0;
}