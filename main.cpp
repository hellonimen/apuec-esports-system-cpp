// File: main.cpp

#include <iostream>
#include <string>
#include <cstdlib>

#include "tasks/Task1_Scheduler.hpp"
#include "tasks/Task2_Registration.hpp"
#include "tasks/Task3_Spectator.hpp"
#include "tasks/Task4_Logger.hpp"

using namespace std;

enum class SpectatorMenu : int {
    Back            = 0,
    CheckIn         = 1,
    SeatVIPs        = 2,
    Overflow        = 3,
    LiveSlots       = 4,
    AddSample       = 5,
    ViewSeating     = 6,
    ReleaseSeat     = 7,
    RunSimulator    = 8
};

void displayMainMenu() {
    cout << "\n=========================================\n"
         << "    APUEC SYSTEM - MAIN MENU\n"
         << "=========================================\n"
         << "1. Match Scheduling & Player Progression\n"
         << "2. Tournament Registration & Player Queueing\n"
         << "3. Spectator Management\n"
         << "4. Result Logging & Performance History\n"
         << "0. Exit System\n"
         << "=========================================\n"
         << "Enter your choice: ";
}

void displayTask1Menu() {
    cout << "\n=== MATCH SCHEDULING & PLAYER PROGRESSION ===\n"
         << "1. Load Players from File\n"
         << "2. Add New Player\n"
         << "3. View All Players\n"
         << "4. Generate Match Schedule\n"
         << "5. Conduct Match\n"
         << "6. View Match Schedule\n"
         << "7. View Player Standings\n"
         << "8. View Tournament Bracket\n"
         << "9. View Match History\n"
         << "10. Save Match Results to File\n"
         << "0. Back to Main Menu\n"
         << "Enter your choice: ";
}

void displayTask2Menu() {
    cout << "\n=== TOURNAMENT REGISTRATION & PLAYER QUEUEING ===\n"
         << "1. Register New Player\n"
         << "2. Process Check-ins\n"
         << "3. Handle Player Withdrawal\n"
         << "4. Process Waitlist\n"
         << "5. View Registration Status\n"
         << "6. View All Queues\n"
         << "7. View Tournament Participants\n"
         << "8. Add Sample Registration Data\n"
         << "9. Data Structure Information\n"
         << "10. Save Registration Data to File\n"
         << "0. Back to Main Menu\n"
         << "Enter your choice: ";
}

void displayTask3Menu() {
    cout << "\n=== SPECTATOR MANAGEMENT ===\n"
         << "1. Check-in Spectator\n"
         << "2. Seat VIPs\n"
         << "3. Handle Overflow\n"
         << "4. Batch Live-Stream Slots\n"
         << "5. Add Sample Spectators\n"
         << "6. View Seating Chart\n"
         << "7. Release a Seat\n"
         << "8. Run Live-Stream Simulator\n"
         << "0. Back to Main Menu\n"
         << "Enter your choice: ";
}

void displayTask4Menu() {
    cout << "\n=== GAME RESULT LOGGING & PERFORMANCE HISTORY ===\n"
         << "1. Log Single Match Result\n"
         << "2. Log Batch Match Results\n"
         << "3. View Recent Match Results\n"
         << "4. View Complete Match History\n"
         << "5. View Player Performance Statistics\n"
         << "6. View Tournament Leaderboard\n"
         << "7. View Detailed Player Stats\n"
         << "8. Export Player Statistics\n"
         << "9. Export Match History\n"
         << "10. Generate Full Tournament Report\n"
         << "11. Analyze Player Trends\n"
         << "12. Compare Two Players\n"
         << "13. Find Top Performers\n"
         << "14. Undo Last Entry\n"
         << "15. Data Structure Information\n"
         << "16. Clear All Data\n"
         << "0. Back to Main Menu\n"
         << "Enter your choice: ";
}

void handleTask1(Task1_Scheduler& scheduler) {
    int choice;
    while (true) {
        displayTask1Menu();
        cin >> choice; cin.ignore();
        if (cin.fail()) { cin.clear(); cin.ignore(10000,'\n'); continue; }
        if (choice == 0) return;

        switch (choice) {
            case 1: {
                cout << "Enter filename (default: data/players.csv): ";
                string fn; getline(cin,fn);
                if (fn.empty()) fn="data/players.csv";
                scheduler.loadPlayersFromFile(fn);
                break;
            }
            case 2: {
                int id, rank; string name, uni, game;
                cout<<"Enter Player ID: "; cin>>id; cin.ignore();
                cout<<"Enter Name: "; getline(cin,name);
                cout<<"Enter University: "; getline(cin,uni);
                cout<<"Enter Game: "; getline(cin,game);
                cout<<"Enter Ranking: "; cin>>rank; cin.ignore();
                scheduler.addPlayer(new Player(id,name,uni,game,rank));
                break;
            }
            case 3:
                for (auto p : scheduler.getPlayers())
                    cout<<p->toString()<<"\n";
                break;
            case 4: scheduler.generateMatchSchedule(); break;
            case 5: {
                int mid; cout<<"Match ID: "; cin>>mid; cin.ignore();
                scheduler.conductMatch(mid);
                break;
            }
            case 6: scheduler.displayMatchSchedule(); break;
            case 7: scheduler.displayPlayerStandings(); break;
            case 8: scheduler.displayTournamentBracket(); break;
            case 9: scheduler.displayMatchHistory(); break;
            case 10: {
                cout<<"Filename (default: data/match_log.txt): ";
                string fn; getline(cin,fn);
                if (fn.empty()) fn="data/match_log.txt";
                scheduler.saveMatchResults(fn);
                break;
            }
            default:
                cout<<"Invalid choice\n";
        }
        cout<<"\nPress Enter to continue..."; cin.get();
    }
}

void handleTask2(Task2_Registration& reg) {
    int choice;
    while (true) {
        displayTask2Menu();
        cin >> choice; cin.ignore();
        if (cin.fail()) { cin.clear(); cin.ignore(10000,'\n'); continue; }
        if (choice == 0) return;

        switch (choice) {
            case 1:  reg.registerPlayer(); break;
            case 2:  reg.checkInPlayer(); break;
            case 3:  reg.handleWithdrawal(); break;
            case 4:  reg.processWaitlist(); break;
            case 5:  reg.displayRegistrationStatus(); break;
            case 6:  reg.displayAllQueues(); break;
            case 7:  reg.displayCheckedInPlayers(); break;
            case 8:  reg.addSampleRegistrationData(); break;
            case 9:  reg.showDataStructureInfo(); break;
            case 10: reg.saveRegistrationData(); break;
            default: cout<<"Invalid choice\n";
        }
        cout<<"\nPress Enter to continue..."; cin.get();
    }
}

void handleTask3(Task3_Spectator& spect) {
    while (true) {
        displayTask3Menu();

        int rawChoice;
        cin >> rawChoice; 
        cin.ignore();

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        auto choice = static_cast<SpectatorMenu>(rawChoice);

        // 0 → back to main menu
        if (choice == SpectatorMenu::Back)
            return;

        switch (choice) {
            case SpectatorMenu::CheckIn:
                spect.checkInSpectator();
                break;

            case SpectatorMenu::SeatVIPs:
                spect.manageVIPSeating();
                break;

            case SpectatorMenu::Overflow:
                spect.handleAudienceOverflow();
                break;

            case SpectatorMenu::LiveSlots:
                spect.organizeLiveStreaming();
                break;

            case SpectatorMenu::AddSample: {
                cout << "How many guests? ";
                int n; 
                cin >> n; cin.ignore();

                cout << "Load into (1) Normal queues or (2) MainQueue only? ";
                int mode;
                cin >> mode; cin.ignore();

                if (mode == 2)
                    spect.addSamplesToMainQueue(n);
                else
                    spect.addSampleSpectators(n);
                break;
            }

            case SpectatorMenu::ViewSeating:
                spect.displaySeatingChart();
                break;

            case SpectatorMenu::ReleaseSeat: {
                spect.displaySeatingChart();
                cout << "Enter seat ID to release (e.g. A5): ";
                string seatID;
                getline(cin, seatID);
                spect.releaseSeat(seatID);
                break;
            }

            case SpectatorMenu::RunSimulator:
                spect.runLiveStreamSimulator();
                break;

            default:
                cout << "Invalid choice\n";
                break;
        }

        cout << "\nPress Enter to continue...";
        cin.get();
    }
}

void handleTask4(Task1_Scheduler& scheduler) {
    Task4_Logger logger;
    int choice;
    
    while (true) {
        displayTask4Menu();
        cin >> choice; cin.ignore();
        if (cin.fail()) { cin.clear(); cin.ignore(10000,'\n'); continue; }
        if (choice == 0) return;

        switch (choice) {
            case 1: {
                // Log Single Match Result
                cout << "Available completed matches:\n";
                LinkedList<Match*> matches = scheduler.getMatches();
                bool hasCompleted = false;
                
                for (int i = 0; i < matches.getSize(); i++) {
                    if (matches[i]->status == "Completed") {
                        cout << "Match #" << matches[i]->matchID 
                             << " [" << matches[i]->stage << "] "
                             << matches[i]->player1->getName() << " vs "
                             << matches[i]->player2->getName()
                             << " -> Winner: " << matches[i]->winner->getName() << "\n";
                        hasCompleted = true;
                    }
                }
                
                if (!hasCompleted) {
                    cout << "No completed matches available. Please conduct matches first.\n";
                    break;
                }
                
                int mid; 
                cout << "Enter Match ID to log: "; 
                cin >> mid; cin.ignore();
                
                Match* targetMatch = nullptr;
                for (int i = 0; i < matches.getSize(); i++) {
                    if (matches[i]->matchID == mid && matches[i]->status == "Completed") {
                        targetMatch = matches[i];
                        break;
                    }
                }
                
                if (targetMatch) {
                    logger.logMatchResult(targetMatch);
                } else {
                    cout << "Match not found or not completed.\n";
                }
                break;
            }
            
            case 2: {
                // Log Batch Match Results
                LinkedList<Match*> matches = scheduler.getMatches();
                logger.logBatchResults(matches);
                break;
            }
            
            case 3:
                // View Recent Match Results
                logger.viewRecentResults();
                break;
                
            case 4:
                // View Complete Match History
                logger.viewMatchHistory();
                break;
                
            case 5:
                // View Player Performance Statistics
                logger.viewPlayerPerformance();
                break;
                
            case 6:
                // View Tournament Leaderboard
                logger.viewLeaderboard();
                break;
                
            case 7: {
                // View Detailed Player Stats
                int playerID;
                cout << "Enter Player ID: ";
                cin >> playerID; cin.ignore();
                logger.viewDetailedStats(playerID);
                break;
            }
            
            case 8: {
                // Export Player Statistics
                string filename;
                cout << "Enter filename (default: player_statistics.txt): ";
                getline(cin, filename);
                if (filename.empty()) filename = "player_statistics.txt";
                logger.exportPlayerStatistics(filename);
                break;
            }
            
            case 9: {
                // Export Match History
                string filename;
                cout << "Enter filename (default: match_history.txt): ";
                getline(cin, filename);
                if (filename.empty()) filename = "match_history.txt";
                logger.exportMatchHistory(filename);
                break;
            }
            
            case 10: {
                // Generate Full Tournament Report
                string filename;
                cout << "Enter filename (default: tournament_report.txt): ";
                getline(cin, filename);
                if (filename.empty()) filename = "tournament_report.txt";
                logger.generateFullReport(filename);
                break;
            }
            
            case 11:
                // Analyze Player Trends
                logger.analyzePlayerTrends();
                break;
                
            case 12: {
                // Compare Two Players
                int player1ID, player2ID;
                cout << "Enter first Player ID: ";
                cin >> player1ID; cin.ignore();
                cout << "Enter second Player ID: ";
                cin >> player2ID; cin.ignore();
                logger.comparePlayerStats(player1ID, player2ID);
                break;
            }
            
            case 13: {
                // Find Top Performers
                int topN;
                cout << "How many top performers to show (default 5): ";
                cin >> topN; cin.ignore();
                if (topN <= 0) topN = 5;
                logger.findTopPerformers(topN);
                break;
            }
            
            case 14:
                // Undo Last Entry
                logger.undoLastEntry();
                break;
                
            case 15:
                // Data Structure Information
                logger.showDataStructureInfo();
                break;
                
            case 16: {
                // Clear All Data
                char confirm;
                cout << "Are you sure you want to clear all logged data? (y/n): ";
                cin >> confirm; cin.ignore();
                if (confirm == 'y' || confirm == 'Y') {
                    logger.clearAllData();
                    cout << "All data cleared successfully.\n";
                }
                break;
            }
            
            default:
                cout << "Invalid choice. Please try again.\n";
        }
        cout << "\nPress Enter to continue..."; cin.get();
    }
}

int main() {
    Task1_Scheduler scheduler;
    Task2_Registration registration;
    Task3_Spectator spect;
    int choice;

    cout<<"=== APUEC MANAGEMENT SYSTEM ===\n";
    while (true) {
        displayMainMenu();
        cin >> choice; cin.ignore();
        if (cin.fail()) { cin.clear(); cin.ignore(10000,'\n'); continue; }

        switch (choice) {
            case 1: handleTask1(scheduler); break;
            case 2: handleTask2(registration); break;
            case 3: handleTask3(spect); break;
            case 4: handleTask4(scheduler); break;
            case 0: cout<<"\nExiting...\n"; return 0;
            default: cout<<"Invalid choice\n";
        }
    }
}
