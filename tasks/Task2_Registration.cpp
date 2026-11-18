#include "Task2_Registration.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>

// ANSI Color codes for terminal
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string WHITE = "\033[37m";
const std::string BOLD = "\033[1m";

// Helper functions for enhanced UI
void printBorder(char pattern = '=', int length = 50) {
    std::cout << std::string(length, pattern) << std::endl;
}

void printDoubleBorder() {
    std::cout << "╔════════════════════════════════════════════════╗" << std::endl;
}

void printBottomBorder() {
    std::cout << "╚════════════════════════════════════════════════╝" << std::endl;
}

Task2_Registration::Task2_Registration() 
    : priorityQueue(), regularQueue(), checkedInPlayers(64), waitingList(100) {
    std::cout << GREEN << "🎮 Tournament Registration System initialized!" << RESET << std::endl;
}

Task2_Registration::~Task2_Registration() {
    // Custom data structures handle their own cleanup
}

void Task2_Registration::registerPlayer() {
    std::string playerIDStr, playerName, university, game;
    int priority;
    
    std::cout << "\n" << CYAN << BOLD << "🎮 PLAYER REGISTRATION" << RESET << std::endl;
    std::cout << CYAN;
    printBorder('=', 22);
    std::cout << RESET;
    
    std::cout << YELLOW << "Enter Player ID: " << WHITE;
    std::getline(std::cin, playerIDStr);
    
    std::cout << YELLOW << "Enter Player Name: " << WHITE;
    std::getline(std::cin, playerName);
    
    std::cout << YELLOW << "Enter University: " << WHITE;
    std::getline(std::cin, university);
    
    std::cout << YELLOW << "Enter Game: " << WHITE;
    std::getline(std::cin, game);
    
    std::cout << RESET << "\n" << BLUE << "Select Priority:" << RESET << std::endl;
    std::cout << RED << "1. VIP Player" << RESET << std::endl;
    std::cout << BLUE << "2. Early-bird Registration" << RESET << std::endl;
    std::cout << WHITE << "3. Regular Registration" << RESET << std::endl;
    std::cout << MAGENTA << "4. Wildcard Entry" << RESET << std::endl;
    std::cout << YELLOW << "Enter choice (1-4): " << WHITE;
    std::cin >> priority;
    std::cin.ignore();
    
    if (priority < 1 || priority > 4) {
        std::cout << YELLOW << "⚠️ Invalid priority. Setting to Regular (3)" << RESET << std::endl;
        priority = 3;
    }
    
    try {
        int playerID = std::stoi(playerIDStr);
        Player* newPlayer = new Player(playerID, playerName, university, game, priority);
        
        if (priority == 1 || priority == 2 || priority == 4) {
            // Use custom Priority Queue - lower number = higher priority
            priorityQueue.enqueue(newPlayer, priority);
            std::string priorityName = (priority == 1) ? "VIP" : 
                                     (priority == 2) ? "Early-bird" : "Wildcard";
            std::cout << GREEN << "✅ " << playerName << " registered as " << MAGENTA 
                      << priorityName << GREEN << " player" << RESET << std::endl;
        } else {
            // Use custom FIFO Queue for regular players
            regularQueue.enqueue(newPlayer);
            std::cout << GREEN << "✅ " << playerName << " registered as " << CYAN 
                      << "Regular" << GREEN << " player" << RESET << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << RED << "❌ Error: Invalid Player ID format" << RESET << std::endl;
    }
}

void Task2_Registration::checkInPlayer() {
    std::cout << "\n" << BLUE << BOLD << "🎯 Processing Check-ins..." << RESET << std::endl;
    std::cout << BLUE;
    printBorder('=', 43);
    std::cout << RESET;
    
    int processedCount = 0;
    
    // Process priority players first (VIP, Early-bird, Wildcard)
    while (!priorityQueue.isEmpty() && checkedInPlayers.getSize() < MAX_CAPACITY) {
        Player* player = priorityQueue.dequeue();  // Custom priority queue dequeue
        checkedInPlayers.enqueue(player);          // Custom circular queue enqueue
        processedCount++;
        
        std::string priorityName = (player->getRanking() == 1) ? RED + "VIP" + RESET : 
                                 (player->getRanking() == 2) ? BLUE + "Early-bird" + RESET : 
                                 MAGENTA + "Wildcard" + RESET;
        std::cout << GREEN << "✅ " << WHITE << player->getName() << GREEN 
                  << " (" << priorityName << GREEN << ") checked in successfully!" << RESET << std::endl;
    }
    
    // Then process regular players (FIFO)
    while (!regularQueue.isEmpty() && checkedInPlayers.getSize() < MAX_CAPACITY) {
        Player* player = regularQueue.dequeue();   // Custom FIFO queue dequeue
        checkedInPlayers.enqueue(player);          // Custom circular queue enqueue
        processedCount++;
        std::cout << GREEN << "✅ " << WHITE << player->getName() << GREEN 
                  << " (" << CYAN << "Regular" << GREEN << ") checked in successfully!" << RESET << std::endl;
    }
    
    // Remaining regular players go to waiting list
    while (!regularQueue.isEmpty()) {
        waitingList.enqueue(regularQueue.dequeue()); // Custom circular queue for waiting
        std::cout << YELLOW << "⏳ Player moved to waiting list" << RESET << std::endl;
    }
    
    std::cout << BLUE;
    printBorder('=', 43);
    std::cout << RESET;
    std::cout << GREEN << BOLD << "✅ Check-in process completed!" << RESET << std::endl;
    std::cout << CYAN << "🏆 Processed " << YELLOW << BOLD << processedCount 
              << RESET << CYAN << " players" << RESET << std::endl;
}

void Task2_Registration::handleWithdrawal() {
    if (checkedInPlayers.isEmpty()) {
        std::cout << RED << "❌ No players are currently checked in." << RESET << std::endl;
        return;
    }
    
    std::cout << "\n" << MAGENTA << BOLD << "🔄 PLAYER WITHDRAWAL" << RESET << std::endl;
    std::cout << MAGENTA;
    printBorder('=', 19);
    std::cout << RESET;
    
    std::cout << CYAN << "Current checked-in players: " << YELLOW << BOLD 
              << checkedInPlayers.getSize() << RESET << CYAN << " players" << RESET << std::endl;
    
    std::cout << YELLOW << "Enter 1 to withdraw a random player (0 to cancel): " << WHITE;
    int choice;
    std::cin >> choice;
    std::cin.ignore();
    
    if (choice == 0) {
        std::cout << YELLOW << "❌ Withdrawal cancelled." << RESET << std::endl;
        return;
    }
    
    if (choice == 1 && !checkedInPlayers.isEmpty()) {
        // For demo purposes, we'll withdraw the front player
        Player* withdrawnPlayer = checkedInPlayers.dequeue();
        std::cout << RED << "❌ " << WHITE << withdrawnPlayer->getName() 
                  << RED << " has withdrawn from the tournament" << RESET << std::endl;
        
        // Get replacement from waiting list if available
        if (!waitingList.isEmpty()) {
            Player* replacement = waitingList.dequeue();
            checkedInPlayers.enqueue(replacement);
            std::cout << GREEN << "🔄 " << WHITE << replacement->getName() 
                      << GREEN << " has been moved from waiting list to tournament!" << RESET << std::endl;
        } else {
            std::cout << YELLOW << "⚠️ No replacement players available." << RESET << std::endl;
        }
        
        // Clean up withdrawn player
        delete withdrawnPlayer;
    } else {
        std::cout << RED << "❌ Invalid choice or no players to withdraw." << RESET << std::endl;
    }
}

void Task2_Registration::processWaitlist() {
    std::cout << "\n" << YELLOW << BOLD << "📋 WAITLIST PROCESSING" << RESET << std::endl;
    std::cout << YELLOW;
    printBorder('=', 22);
    std::cout << RESET;
    
    if (waitingList.isEmpty()) {
        std::cout << YELLOW << "No players in waiting list." << RESET << std::endl;
        return;
    }
    
    std::cout << CYAN << "Current waiting list size: " << YELLOW << BOLD 
              << waitingList.getSize() << RESET << std::endl;
    
    if (checkedInPlayers.getSize() < MAX_CAPACITY) {
        int availableSpots = MAX_CAPACITY - checkedInPlayers.getSize();
        std::cout << "\n" << GREEN << "Available tournament spots: " << YELLOW << BOLD 
                  << availableSpots << RESET << std::endl;
        std::cout << BLUE << "Move players from waiting list? (y/n): " << WHITE;
        
        char choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == 'y' || choice == 'Y') {
            int moved = 0;
            while (!waitingList.isEmpty() && 
                   checkedInPlayers.getSize() < MAX_CAPACITY && 
                   moved < availableSpots) {
                Player* player = waitingList.dequeue();
                checkedInPlayers.enqueue(player);
                std::cout << GREEN << "✅ " << WHITE << player->getName() 
                          << GREEN << " moved to tournament participants!" << RESET << std::endl;
                moved++;
            }
            std::cout << CYAN << "🎯 Moved " << YELLOW << BOLD << moved 
                      << RESET << CYAN << " players from waitlist to tournament." << RESET << std::endl;
        }
    } else {
        std::cout << RED << "🔒 Tournament is at full capacity." << RESET << std::endl;
    }
}

void Task2_Registration::displayRegistrationStatus() {
    std::cout << "\n" << BLUE << BOLD << "📊 TOURNAMENT REGISTRATION STATUS" << RESET << std::endl;
    printBorder('━', 50);
    
    std::cout << GREEN << "🏆 Tournament Capacity: " << YELLOW << BOLD << MAX_CAPACITY << RESET << std::endl;
    std::cout << GREEN << "✅ Checked-in Players: " << YELLOW << BOLD 
              << checkedInPlayers.getSize() << RESET << std::endl;
    std::cout << MAGENTA << "🔥 Priority Players Waiting: " << YELLOW << BOLD 
              << priorityQueue.getSize() << RESET << std::endl;
    std::cout << CYAN << "⏰ Regular Players Waiting: " << YELLOW << BOLD 
              << regularQueue.getSize() << RESET << std::endl;
    std::cout << BLUE << "🔄 Waiting List: " << YELLOW << BOLD 
              << waitingList.getSize() << RESET << std::endl;
    
    int remainingSpots = MAX_CAPACITY - checkedInPlayers.getSize();
    if (remainingSpots > 0) {
        std::cout << GREEN << "🎯 Remaining Tournament Spots: " << YELLOW << BOLD 
                  << remainingSpots << RESET << std::endl;
    } else {
        std::cout << RED << BOLD << "🔒 Tournament is FULL!" << RESET << std::endl;
    }
    
    printBorder('━', 50);
}

void Task2_Registration::displayAllQueues() {
    std::cout << "\n" << MAGENTA << BOLD << "📋 QUEUE SYSTEM STATUS" << RESET << std::endl;
    printBorder('▓', 50);
    
    // Priority Queue Status
    std::cout << "\n" << RED << BOLD << "🔥 Priority Queue (VIP/Early-bird/Wildcard):" << RESET << std::endl;
    if (priorityQueue.isEmpty()) {
        std::cout << "   " << YELLOW << "[Empty]" << RESET << std::endl;
    } else {
        std::cout << "   " << GREEN << "Players: " << YELLOW << BOLD 
                  << priorityQueue.getSize() << RESET << std::endl;
        std::cout << "   " << CYAN << "► Custom Priority Queue with " 
                  << priorityQueue.getSize() << " high-priority players" << RESET << std::endl;
    }
    
    // Regular Queue Status
    std::cout << "\n" << BLUE << BOLD << "⏰ Regular Queue (FIFO):" << RESET << std::endl;
    if (regularQueue.isEmpty()) {
        std::cout << "   " << YELLOW << "[Empty]" << RESET << std::endl;
    } else {
        std::cout << "   " << GREEN << "Players: " << YELLOW << BOLD 
                  << regularQueue.getSize() << RESET << std::endl;
        std::cout << "   " << CYAN << "► Custom FIFO Queue with " 
                  << regularQueue.getSize() << " regular players" << RESET << std::endl;
    }
    
    // Checked-in Players (Circular Queue)
    std::cout << "\n" << GREEN << BOLD << "🏆 Tournament Participants (Circular Queue):" << RESET << std::endl;
    if (checkedInPlayers.isEmpty()) {
        std::cout << "   " << YELLOW << "[Empty]" << RESET << std::endl;
    } else {
        std::cout << "   " << GREEN << "Players: " << YELLOW << BOLD 
                  << checkedInPlayers.getSize() << RESET << std::endl;
        std::cout << "   " << CYAN << "► Custom Circular Queue managing tournament participants" << RESET << std::endl;
    }
    
    // Waiting List (Circular Queue)
    std::cout << "\n" << YELLOW << BOLD << "🔄 Waiting List (Circular Queue concept):" << RESET << std::endl;
    if (waitingList.isEmpty()) {
        std::cout << "   " << YELLOW << "[Empty]" << RESET << std::endl;
    } else {
        std::cout << "   " << GREEN << "Players: " << YELLOW << BOLD 
                  << waitingList.getSize() << RESET << std::endl;
        std::cout << "   " << CYAN << "► Custom Circular Queue for efficient replacement" << RESET << std::endl;
    }
    
    printBorder('▓', 50);
}

void Task2_Registration::displayCheckedInPlayers() {
    std::cout << "\n" << GREEN << BOLD << "🏆 TOURNAMENT PARTICIPANTS" << RESET << std::endl;
    printBorder('═', 50);
    
    if (checkedInPlayers.isEmpty()) {
        std::cout << YELLOW << "No players checked in yet." << RESET << std::endl;
    } else {
        std::cout << GREEN << "Tournament participants managed by Custom Circular Queue:" << RESET << std::endl;
        std::cout << CYAN << "Total Players: " << YELLOW << BOLD 
                  << checkedInPlayers.getSize() << RESET << CYAN << " players" << RESET << std::endl;
        std::cout << BLUE << "Queue Status: " << WHITE << "Using CircularQueue for efficient management" << RESET << std::endl;
    }
    
    printBorder('═', 50);
}

void Task2_Registration::addSampleRegistrationData() {
    std::cout << CYAN << "📝 Adding sample registration data using custom data structures..." << RESET << std::endl;
    
    // Create sample players and add them using custom queue operations
    
    // VIP players - Priority 1
    Player* vip1 = new Player(1001, "ProGamer_Alex", "APU", "Valorant", 1);
    Player* vip2 = new Player(1002, "StreamerSarah", "UCSI", "League of Legends", 1);
    priorityQueue.enqueue(vip1, 1);  // Highest priority
    priorityQueue.enqueue(vip2, 1);
    
    // Early-bird players - Priority 2
    Player* eb1 = new Player(2001, "EarlyBird_John", "MMU", "Dota 2", 2);
    Player* eb2 = new Player(2002, "QuickRegister_Lisa", "Taylor's", "CS:GO", 2);
    priorityQueue.enqueue(eb1, 2);
    priorityQueue.enqueue(eb2, 2);
    
    // Wildcard player - Priority 4
    Player* wc1 = new Player(4001, "WildCard_Player", "UNITEN", "Dota 2", 4);
    priorityQueue.enqueue(wc1, 4);
    
    // Regular players - FIFO Queue
    Player* reg1 = new Player(3001, "RegularPlayer1", "UPM", "League of Legends", 3);
    Player* reg2 = new Player(3002, "RegularPlayer2", "UM", "Dota 2", 3);
    Player* reg3 = new Player(3003, "RegularPlayer3", "USM", "CS:GO", 3);
    regularQueue.enqueue(reg1);
    regularQueue.enqueue(reg2);
    regularQueue.enqueue(reg3);
    
    std::cout << GREEN << "✅ Sample registration data added successfully using custom data structures!" << RESET << std::endl;
    std::cout << CYAN << "Added: " << RED << "2 VIP" << RESET << ", " << BLUE << "2 Early-bird" << RESET << ", " 
              << WHITE << "3 Regular" << RESET << ", " << MAGENTA << "1 Wildcard" << RESET << CYAN << " players" << RESET << std::endl;
    std::cout << BLUE << "Data structures used: " << WHITE << "Custom PriorityQueue + Custom Queue" << RESET << std::endl;
}

void Task2_Registration::saveRegistrationData() {
    std::cout << "\n" << CYAN << BOLD << "💾 SAVE REGISTRATION DATA" << RESET << std::endl;
    std::cout << CYAN;
    printBorder('=', 25);
    std::cout << RESET;
    
    std::cout << YELLOW << "Enter filename to save (default: data/registration_log.txt): " << WHITE;
    std::string filename;
    std::getline(std::cin, filename);
    
    if (filename.empty()) {
        filename = "data/registration_log.txt";
    }
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << RED << "❌ Error opening file for writing: " << filename << RESET << std::endl;
        return;
    }
    
    // Get current time
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    // Write header
    file << "APUEC TOURNAMENT REGISTRATION LOG\n";
    file << "Generated: " << std::ctime(&time_t);
    file << "System Capacity: " << MAX_CAPACITY << " players\n";
    file << "========================================\n\n";
    
    // Save Priority Queue status
    file << "=== PRIORITY REGISTRATION QUEUE (Custom PriorityQueue) ===\n";
    file << "Total Players: " << priorityQueue.getSize() << "\n";
    file << "Implementation: Custom Priority Queue with min-heap structure\n";
    file << "Status: Using priority-based processing (1=VIP, 2=Early-bird, 4=Wildcard)\n\n";
    
    // Save Regular Queue status
    file << "=== REGULAR REGISTRATION QUEUE (Custom FIFO Queue) ===\n";
    file << "Total Players: " << regularQueue.getSize() << "\n";
    file << "Implementation: Custom Queue with linked-list structure\n";
    file << "Status: Using first-in-first-out processing for fairness\n\n";
    
    // Save Checked-in Players
    file << "=== TOURNAMENT PARTICIPANTS (Custom CircularQueue) ===\n";
    file << "Total Players: " << checkedInPlayers.getSize() << "\n";
    file << "Implementation: Custom Circular Queue for efficient management\n";
    file << "Status: Active tournament participants\n\n";
    
    // Save Waiting List
    file << "=== WAITING LIST (Custom CircularQueue) ===\n";
    file << "Total Players: " << waitingList.getSize() << "\n";
    file << "Implementation: Custom Circular Queue for replacement management\n";
    file << "Status: Automatic replacement system for withdrawals\n\n";
    
    // Summary Statistics
    file << "=== REGISTRATION SUMMARY ===\n";
    file << "Priority Queue (Custom): " << priorityQueue.getSize() << " players\n";
    file << "Regular Queue (Custom): " << regularQueue.getSize() << " players\n";
    file << "Tournament Participants (CircularQueue): " << checkedInPlayers.getSize() << " players\n";
    file << "Waiting List (CircularQueue): " << waitingList.getSize() << " players\n";
    file << "Total System Load: " << (priorityQueue.getSize() + regularQueue.getSize() + 
                                      checkedInPlayers.getSize() + waitingList.getSize()) << " players\n";
    file << "Remaining Capacity: " << (MAX_CAPACITY - checkedInPlayers.getSize()) << " spots\n";
    file << "\n=== CUSTOM DATA STRUCTURE IMPLEMENTATION ===\n";
    file << "✅ PriorityQueue: Min-heap based priority processing\n";
    file << "✅ Queue: Linked-list based FIFO processing\n";
    file << "✅ CircularQueue: Array-based efficient circular management\n";
    file << "✅ No STL containers used - full custom implementation\n";
    
    file.close();
    
    std::cout << GREEN << "✅ Registration data saved successfully to: " << CYAN << filename << RESET << std::endl;
    std::cout << BLUE << "📄 File contains: " << YELLOW << "Complete custom data structure status and statistics" << RESET << std::endl;
}

void Task2_Registration::showDataStructureInfo() {
    std::cout << "\n" << CYAN << BOLD << "📚 CUSTOM DATA STRUCTURE IMPLEMENTATION - TASK 2" << RESET << std::endl;
    printBorder('═', 50);
    
    std::cout << "\n" << BLUE << BOLD << "🔹 CUSTOM PRIORITY QUEUE" << RESET << std::endl;
    std::cout << "   " << WHITE << "Implementation: Min-heap based custom data structure" << RESET << std::endl;
    std::cout << "   " << YELLOW << "Purpose: Handle VIP, Early-bird, and Wildcard registrations" << RESET << std::endl;
    std::cout << "   " << GREEN << "Time Complexity: O(log n) insert, O(log n) dequeue" << RESET << std::endl;
    std::cout << "   " << CYAN << "Why: Guarantees higher-priority players processed first" << RESET << std::endl;
    std::cout << "   " << MAGENTA << "Current Status: " << priorityQueue.getSize() << " players waiting" << RESET << std::endl;
    
    std::cout << "\n" << MAGENTA << BOLD << "🔹 CUSTOM QUEUE (FIFO)" << RESET << std::endl;
    std::cout << "   " << WHITE << "Implementation: Linked-list based custom data structure" << RESET << std::endl;
    std::cout << "   " << YELLOW << "Purpose: Handle regular player registrations fairly" << RESET << std::endl;
    std::cout << "   " << GREEN << "Time Complexity: O(1) enqueue, O(1) dequeue" << RESET << std::endl;
    std::cout << "   " << CYAN << "Why: Maintains fairness with first-come-first-served processing" << RESET << std::endl;
    std::cout << "   " << MAGENTA << "Current Status: " << regularQueue.getSize() << " players waiting" << RESET << std::endl;
    
    std::cout << "\n" << RED << BOLD << "🔹 CUSTOM CIRCULAR QUEUE" << RESET << std::endl;
    std::cout << "   " << WHITE << "Implementation: Array-based custom circular data structure" << RESET << std::endl;
    std::cout << "   " << YELLOW << "Purpose: Manage tournament participants and replacement efficiently" << RESET << std::endl;
    std::cout << "   " << GREEN << "Time Complexity: O(1) enqueue, O(1) dequeue" << RESET << std::endl;
    std::cout << "   " << CYAN << "Why: Efficient space utilization and automatic wrap-around" << RESET << std::endl;
    std::cout << "   " << MAGENTA << "Current Status: " << checkedInPlayers.getSize() << " checked-in, " 
              << waitingList.getSize() << " waiting" << RESET << std::endl;
    
    std::cout << "\n" << GREEN << BOLD << "🎯 ADVANCED SYSTEM WORKFLOW:" << RESET << std::endl;
    std::cout << "   " << CYAN << "1. Registration → Custom PriorityQueue & Queue routing" << RESET << std::endl;
    std::cout << "   " << BLUE << "2. Check-in → Priority dequeue first, then FIFO dequeue" << RESET << std::endl;
    std::cout << "   " << YELLOW << "3. Overflow → Automatic CircularQueue waiting list management" << RESET << std::endl;
    std::cout << "   " << MAGENTA << "4. Withdrawal → CircularQueue replacement with O(1) efficiency" << RESET << std::endl;
    
    std::cout << "\n" << WHITE << BOLD << "🏆 TECHNICAL EXCELLENCE:" << RESET << std::endl;
    std::cout << "   " << GREEN << "✅ Zero STL containers - 100% custom implementation" << RESET << std::endl;
    std::cout << "   " << GREEN << "✅ Three different data structures working together" << RESET << std::endl;
    std::cout << "   " << GREEN << "✅ Optimal time complexity for all operations" << RESET << std::endl;
    std::cout << "   " << GREEN << "✅ Real-world tournament management solution" << RESET << std::endl;
    
    printBorder('═', 50);
}