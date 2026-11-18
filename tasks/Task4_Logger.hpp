#ifndef TASK4_LOGGER_HPP
#define TASK4_LOGGER_HPP

#include "Task1_Scheduler.hpp"
#include "../shared/Queue.hpp"
#include "../shared/HashMap.hpp"
#include "../shared/LinkedList.hpp"
#include "../shared/Stack.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>

// Simple performance tracking structure
struct PerformanceEntry {
    Player* player;
    int wins;
    int losses;
    int totalMatches;
    double winRate;
    std::string lastMatchResult;
    
    PerformanceEntry(Player* p) 
        : player(p), wins(0), losses(0), totalMatches(0), 
          winRate(0.0), lastMatchResult("No matches") {}
    
    void updateStats() {
        totalMatches = wins + losses;
        winRate = (totalMatches > 0) ? (double)wins / totalMatches * 100.0 : 0.0;
    }
    
    std::string toString() const {
        std::string result = player->getName() + " | Wins: " + std::to_string(wins) 
                           + " | Losses: " + std::to_string(losses) 
                           + " | Win Rate: " + std::to_string(winRate) + "%";
        return result;
    }
};

// Simple match result structure
struct MatchResult {
    int matchID;
    std::string stage;
    std::string game;
    Player* player1;
    Player* player2;
    Player* winner;
    std::string timestamp;
    
    MatchResult(Match* match) {
        matchID = match->matchID;
        stage = match->stage;
        player1 = match->player1;
        player2 = match->player2;
        winner = match->winner;
        game = (player1) ? player1->getGame() : "Unknown";
        
        // Simple timestamp
        time_t now = time(0);
        timestamp = std::to_string(now);
    }
    
    std::string toString() const {
        std::string result = "Match #" + std::to_string(matchID) + " [" + stage + "] "
                           + player1->getName() + " vs " + player2->getName()
                           + " -> Winner: " + winner->getName()
                           + " | Game: " + game;
        return result;
    }
};

// Simplified Task 4 Logger class
class Task4_Logger {
private:
    // Core data structures
    Queue<MatchResult*> recentResults;
    Stack<MatchResult*> matchHistory;
    HashMap<int, PerformanceEntry*> performanceMap;
    LinkedList<PerformanceEntry*> leaderboard;
    
    // Helper methods
    void updateLeaderboard();
    PerformanceEntry* getOrCreateEntry(Player* player);

public:
    Task4_Logger();
    ~Task4_Logger();
    
    // Core logging functionality
    void logMatchResult(Match* match);
    void logBatchResults(LinkedList<Match*>& matches);
    
    // Display functions - ALL CONST
    void viewRecentResults() const;
    void viewMatchHistory() const;
    void viewPlayerPerformance() const;
    void viewLeaderboard() const;
    void viewDetailedStats(int playerID) const;
    
    // Export functions
    void exportPlayerStatistics(const std::string& filename);
    void exportMatchHistory(const std::string& filename);
    void generateFullReport(const std::string& filename);
    
    // Analysis functions - ALL CONST
    void analyzePlayerTrends() const;
    void comparePlayerStats(int playerID1, int playerID2) const;
    void findTopPerformers(int topN = 5) const;
    
    // Utility functions
    void clearAllData();
    void undoLastEntry();
    int getTotalMatchesLogged() const;
    void showDataStructureInfo() const;
    void printBorder(char pattern = '=', int length = 60);
};

#endif // TASK4_LOGGER_HPP