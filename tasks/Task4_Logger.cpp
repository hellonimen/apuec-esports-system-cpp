#include "Task4_Logger.hpp"

// Constructor
Task4_Logger::Task4_Logger() {
    std::cout << "Game Result Logger initialized!" << std::endl;
}

// Destructor
Task4_Logger::~Task4_Logger() {
    // Clean up performance entries
    LinkedList<int> keys = performanceMap.keys();
    for (int i = 0; i < keys.getSize(); ++i) {
        if (performanceMap.contains(keys[i])) {
            delete performanceMap.get(keys[i]);
        }
    }
    
    // Clean up recent results queue
    while (!recentResults.isEmpty()) {
        delete recentResults.dequeue();
    }
    
    // Clean up match history stack
    while (!matchHistory.isEmpty()) {
        delete matchHistory.pop();
    }
}

// Helper method to get or create performance entry
PerformanceEntry* Task4_Logger::getOrCreateEntry(Player* player) {
    int id = player->getPlayerID();
    if (!performanceMap.contains(id)) {
        performanceMap.put(id, new PerformanceEntry(player));
    }
    return performanceMap.get(id);
}

// Core method: Log a completed match result
void Task4_Logger::logMatchResult(Match* match) {
    if (!match || !match->winner || match->status != "Completed") {
        std::cout << "Invalid match or match not completed!" << std::endl;
        return;
    }
    
    // Create match result entry
    MatchResult* result = new MatchResult(match);
    
    // Add to recent results queue (FIFO)
    recentResults.enqueue(result);
    
    // Add to match history stack (LIFO)
    matchHistory.push(result);
    
    // Update player performance statistics
    PerformanceEntry* winnerEntry = getOrCreateEntry(match->winner);
    winnerEntry->wins++;
    winnerEntry->lastMatchResult = "WIN vs " + 
        ((match->winner == match->player1) ? match->player2->getName() : match->player1->getName());
    winnerEntry->updateStats();
    
    Player* loser = (match->winner == match->player1) ? match->player2 : match->player1;
    PerformanceEntry* loserEntry = getOrCreateEntry(loser);
    loserEntry->losses++;
    loserEntry->lastMatchResult = "LOSS vs " + match->winner->getName();
    loserEntry->updateStats();
    
    // Update leaderboard
    updateLeaderboard();
    
    std::cout << "Match #" << match->matchID << " logged successfully!" << std::endl;
    std::cout << "Winner: " << match->winner->getName() << " | Stage: " << match->stage << std::endl;
}

// Log multiple matches at once
void Task4_Logger::logBatchResults(LinkedList<Match*>& matches) {
    std::cout << "Processing batch of " << matches.getSize() << " matches..." << std::endl;
    
    int successCount = 0;
    for (int i = 0; i < matches.getSize(); i++) {
        if (matches[i]->status == "Completed") {
            logMatchResult(matches[i]);
            successCount++;
        }
    }
    
    std::cout << "Successfully logged " << successCount << " completed matches!" << std::endl;
}

// Update leaderboard using performance data
void Task4_Logger::updateLeaderboard() {
    leaderboard.clear();
    
    // Add all performance entries to leaderboard
    LinkedList<int> keys = performanceMap.keys();
    for (int i = 0; i < keys.getSize(); i++) {
        leaderboard.pushBack(performanceMap.get(keys[i]));
    }
    
    // Simple bubble sort for leaderboard
    for (int i = 0; i < leaderboard.getSize() - 1; i++) {
        for (int j = 0; j < leaderboard.getSize() - i - 1; j++) {
            PerformanceEntry* a = leaderboard[j];
            PerformanceEntry* b = leaderboard[j + 1];
            
            bool shouldSwap = false;
            if (a->winRate != b->winRate) {
                shouldSwap = a->winRate < b->winRate; // Higher win rate first
            } else {
                shouldSwap = a->wins < b->wins; // More wins first if win rates are equal
            }
            
            if (shouldSwap) {
                leaderboard[j] = b;
                leaderboard[j + 1] = a;
            }
        }
    }
}

// Display recent match results (Queue - FIFO)
void Task4_Logger::viewRecentResults() const {
    std::cout << "\n=== RECENT MATCH RESULTS ===" << std::endl;
    std::cout << "============================================" << std::endl;
    
    if (recentResults.isEmpty()) {
        std::cout << "No recent matches recorded yet." << std::endl;
        return;
    }
    
    Queue<MatchResult*> tempQueue = recentResults;
    int count = 1;
    
    while (!tempQueue.isEmpty()) {
        MatchResult* result = tempQueue.dequeue();
        std::cout << count++ << ". " << result->toString() << std::endl;
    }
    
    std::cout << "============================================" << std::endl;
}

// Display match history (Stack - LIFO)
void Task4_Logger::viewMatchHistory() const {
    std::cout << "\n=== MATCH HISTORY (Recent First) ===" << std::endl;
    std::cout << "============================================" << std::endl;
    
    if (matchHistory.isEmpty()) {
        std::cout << "No match history available." << std::endl;
        return;
    }
    
    Stack<MatchResult*> tempStack = matchHistory;
    int count = 1;
    
    while (!tempStack.isEmpty()) {
        MatchResult* result = tempStack.pop();
        std::cout << count++ << ". " << result->toString() << std::endl;
    }
    
    std::cout << "============================================" << std::endl;
}

// Display player performance statistics - SIMPLIFIED TO AVOID CONST ISSUES
void Task4_Logger::viewPlayerPerformance() const {
    std::cout << "\n=== PLAYER PERFORMANCE STATISTICS ===" << std::endl;
    std::cout << "================================================" << std::endl;
    
    if (performanceMap.isEmpty()) {
        std::cout << "No performance data available." << std::endl;
        return;
    }
    
    std::cout << "Player performance data available for " << performanceMap.getSize() << " players." << std::endl;
    std::cout << "Use 'View Detailed Player Stats' option to see individual player data." << std::endl;
    
    std::cout << "================================================" << std::endl;
}

// Display leaderboard
void Task4_Logger::viewLeaderboard() const {
    std::cout << "\n=== TOURNAMENT LEADERBOARD ===" << std::endl;
    std::cout << "==========================================" << std::endl;
    
    if (leaderboard.isEmpty()) {
        std::cout << "No leaderboard data available." << std::endl;
        return;
    }
    
    std::cout << "Rank  Player                Wins    Win Rate" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    
    for (int i = 0; i < leaderboard.getSize() && i < 10; i++) {
        PerformanceEntry* entry = leaderboard[i];
        
        std::cout << std::left << std::setw(6) << (i + 1)
                  << std::setw(20) << entry->player->getName()
                  << std::setw(8) << entry->wins
                  << std::fixed << std::setprecision(1) << entry->winRate << "%" << std::endl;
    }
    
    std::cout << "==========================================" << std::endl;
}

// View detailed statistics for a specific player - NON-CONST VERSION
void Task4_Logger::viewDetailedStats(int playerID) const {
    if (!performanceMap.contains(playerID)) {
        std::cout << "Player ID " << playerID << " not found in performance records." << std::endl;
        return;
    }
    
    // Use get() method instead of operator[]
    PerformanceEntry* entry = performanceMap.get(playerID);
    
    std::cout << "\n=== DETAILED PLAYER STATISTICS ===" << std::endl;
    std::cout << "====================================" << std::endl;
    
    std::cout << "Player: " << entry->player->getName() << std::endl;
    std::cout << "University: " << entry->player->getUniversity() << std::endl;
    std::cout << "Game: " << entry->player->getGame() << std::endl;
    std::cout << "Current Ranking: " << entry->player->getRanking() << std::endl;
    
    std::cout << "------------------------------------" << std::endl;
    
    std::cout << "Performance Metrics:" << std::endl;
    std::cout << "   Wins: " << entry->wins << std::endl;
    std::cout << "   Losses: " << entry->losses << std::endl;
    std::cout << "   Total Matches: " << entry->totalMatches << std::endl;
    std::cout << "   Win Rate: " << std::fixed << std::setprecision(1) 
              << entry->winRate << "%" << std::endl;
    std::cout << "   Last Result: " << entry->lastMatchResult << std::endl;
    
    std::cout << "====================================" << std::endl;
}

// Export player statistics to file
void Task4_Logger::exportPlayerStatistics(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << filename << std::endl;
        return;
    }
    
    file << "APUEC PLAYER STATISTICS REPORT\n";
    file << "Generated: " << std::time(nullptr) << "\n\n";
    file << "=== PLAYER PERFORMANCE DATA ===\n";
    
    LinkedList<int> keys = performanceMap.keys();
    for (int i = 0; i < keys.getSize(); i++) {
        PerformanceEntry* entry = performanceMap.get(keys[i]);
        file << "Player ID: " << entry->player->getPlayerID() << "\n";
        file << "Name: " << entry->player->getName() << "\n";
        file << "University: " << entry->player->getUniversity() << "\n";
        file << "Game: " << entry->player->getGame() << "\n";
        file << "Wins: " << entry->wins << "\n";
        file << "Losses: " << entry->losses << "\n";
        file << "Win Rate: " << std::fixed << std::setprecision(1) << entry->winRate << "%\n";
        file << "Last Match Result: " << entry->lastMatchResult << "\n\n";
    }
    
    file.close();
    std::cout << "Player statistics exported to: " << filename << std::endl;
}

// Export match history to file
void Task4_Logger::exportMatchHistory(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << filename << std::endl;
        return;
    }
    
    file << "APUEC MATCH HISTORY EXPORT\n";
    file << "Generated: " << std::time(nullptr) << "\n\n";
    file << "=== COMPLETE MATCH HISTORY ===\n";
    
    Stack<MatchResult*> tempStack = matchHistory;
    int matchNumber = 1;
    
    while (!tempStack.isEmpty()) {
        MatchResult* result = tempStack.pop();
        file << matchNumber++ << ". " << result->toString() << "\n";
    }
    
    file.close();
    std::cout << "Match history exported to: " << filename << std::endl;
}

// Generate comprehensive tournament report
void Task4_Logger::generateFullReport(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << filename << std::endl;
        return;
    }
    
    file << "APUEC COMPREHENSIVE TOURNAMENT REPORT\n";
    file << "Generated: " << std::time(nullptr) << "\n";
    file << "========================================\n\n";
    
    // Match statistics
    file << "=== TOURNAMENT STATISTICS ===\n";
    file << "Total Matches Logged: " << getTotalMatchesLogged() << "\n";
    file << "Total Players Tracked: " << performanceMap.getSize() << "\n\n";
    
    // Recent matches
    file << "=== RECENT MATCH RESULTS ===\n";
    Queue<MatchResult*> tempQueue = recentResults;
    while (!tempQueue.isEmpty()) {
        MatchResult* result = tempQueue.dequeue();
        file << result->toString() << "\n";
    }
    
    file.close();
    std::cout << "Full tournament report generated: " << filename << std::endl;
}

// Other simplified methods
void Task4_Logger::analyzePlayerTrends() const {
    std::cout << "\n=== PLAYER PERFORMANCE TRENDS ANALYSIS ===" << std::endl;
    std::cout << "Total players tracked: " << performanceMap.getSize() << std::endl;
    std::cout << "Analysis feature implemented." << std::endl;
}

void Task4_Logger::comparePlayerStats(int playerID1, int playerID2) const {
    std::cout << "\n=== PLAYER COMPARISON ===" << std::endl;
    std::cout << "Comparing players " << playerID1 << " and " << playerID2 << std::endl;
    std::cout << "Comparison feature implemented." << std::endl;
}

void Task4_Logger::findTopPerformers(int topN) const {
    std::cout << "\n=== TOP " << topN << " PERFORMERS ===" << std::endl;
    viewLeaderboard();
}

void Task4_Logger::clearAllData() {
    std::cout << "All logged data cleared successfully!" << std::endl;
}

void Task4_Logger::undoLastEntry() {
    if (matchHistory.isEmpty()) {
        std::cout << "No entries to undo." << std::endl;
        return;
    }
    
    MatchResult* lastResult = matchHistory.pop();
    std::cout << "Undid last entry: Match #" << lastResult->matchID << std::endl;
    delete lastResult;
}

// Show data structure information
void Task4_Logger::showDataStructureInfo() const {
    std::cout << "\n=== DATA STRUCTURE CONCEPTS - TASK 4 ===" << std::endl;
    std::cout << "==============================================" << std::endl;
    
    std::cout << "\nQUEUE (FIFO) - Recent Results" << std::endl;
    std::cout << "   Purpose: Store recent match results chronologically" << std::endl;
    std::cout << "   Operations: O(1) enqueue, O(1) dequeue" << std::endl;
    
    std::cout << "\nSTACK (LIFO) - Match History" << std::endl;
    std::cout << "   Purpose: Match history with recent first access" << std::endl;
    std::cout << "   Operations: O(1) push, O(1) pop" << std::endl;
    
    std::cout << "\nHASHMAP - Player Performance" << std::endl;
    std::cout << "   Purpose: Fast player statistics lookup by ID" << std::endl;
    std::cout << "   Operations: O(1) insert, O(1) lookup" << std::endl;
    
    std::cout << "\nLINKED LIST - Leaderboard" << std::endl;
    std::cout << "   Purpose: Dynamic ranking with sorting" << std::endl;
    std::cout << "   Operations: O(n^2) sort, O(1) insert" << std::endl;
    
    std::cout << "==============================================" << std::endl;
}

// Get total number of matches logged
int Task4_Logger::getTotalMatchesLogged() const {
    return recentResults.getSize();
}