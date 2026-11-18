#ifndef TASK2_REGISTRATION_HPP
#define TASK2_REGISTRATION_HPP

#include "../shared/Player.hpp"
#include "../shared/PriorityQueue_refactored.hpp"  // Custom priority queue
#include "../shared/Queue.hpp"                     // Custom FIFO queue
#include "../shared/CircularQueue.hpp"             // Custom circular queue

class Task2_Registration {
private:
    // Custom data structures - NO STL containers
    PriorityQueue<Player*> priorityQueue;          // VIP, Early-bird, Wildcard (min-heap)
    Queue<Player*> regularQueue;                   // Regular FIFO registration (linked-list)
    CircularQueue<Player*> checkedInPlayers;       // Tournament participants (circular array)
    CircularQueue<Player*> waitingList;            // Replacement queue (circular array)
    
    static const int MAX_CAPACITY = 64;

public:
    // Constructor with proper member initialization
    Task2_Registration();
    ~Task2_Registration();
    
    // Core Task 2 functionality using custom data structures
    void registerPlayer();        // Demonstrates Priority Queue + FIFO Queue routing
    void checkInPlayer();         // Demonstrates coordinated multi-queue processing
    void handleWithdrawal();      // Demonstrates Circular Queue replacement
    void processWaitlist();       // Demonstrates Circular Queue promotion
    
    // Display and management functions showcasing custom implementation
    void displayRegistrationStatus();   // Shows real-time queue statistics
    void displayAllQueues();            // Shows all custom data structure states
    void displayCheckedInPlayers();     // Shows CircularQueue management
    void addSampleRegistrationData();   // Demonstrates queue operations
    void saveRegistrationData();        // Exports custom structure statistics
    void showDataStructureInfo();       // Explains custom implementation details
};

#endif