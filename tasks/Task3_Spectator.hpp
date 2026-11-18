#ifndef TASK3_SPECTATOR_HPP
#define TASK3_SPECTATOR_HPP

#include <set>                       // for std::set<int>
#include <deque>    
#include <queue>                 // for dynamic overflow queue
#include <map>
#include <vector>
#include <string>
#include "../shared/Spectator.hpp"
#include "../shared/PriorityQueue.hpp"
#include "../shared/CircularQueue.hpp"

class Task3_Spectator {
private:
    PriorityQueue<Spectator> vipQueue;
    CircularQueue<Spectator> mainQueue;
    std::deque<Spectator>    overflowQueue;

    // ─── Spectator hall configuration ───────────────────────────────
    static constexpr int SEATING_CAPACITY    = 50;
    static constexpr int MAIN_QUEUE_CAPACITY = 500;  // only one definition
    static constexpr int STREAM_STATIONS     = 5;    // five viewing screens
    static constexpr int STATION_DURATION    = 10;   // minutes per round

    // New seating chart members

    static constexpr int ROWS           = 6; // A -F
    static constexpr int COLS           = 10; // 1-10
    static constexpr int VIP_SEAT_COUNT = 10; // First 10 seats reserved for VIPs

    std::queue<std::string>              vipSeats;
    std::queue<std::string>          generalSeats;
    std::map<std::string, Spectator> seatingChart;

    std::queue<Spectator> seatWaitlist;

    // ID management
    std::set<int> usedIDs;    // already‐used spectator IDs
    int           nextAutoID = 1000;  // starting point for auto‐IDs

public:
    Task3_Spectator();
    ~Task3_Spectator();

    // I/O / Logic separation
    Spectator readSpectatorFromInput();
    void      enqueueSpectator(const Spectator& s);

    // Core spectator actions
    void checkInSpectator();
    void manageVIPSeating();
    void handleAudienceOverflow();
    void organizeLiveStreaming();
    void addSampleSpectators(int count);
    void addSamplesToMainQueue(int count);
    void runLiveStreamSimulator();
    void initializeSeats();
    void assignSeat(const Spectator& s);
    void displaySeatingChart() const;
    void releaseSeat(const std::string& seatID);
};

#endif // TASK3_SPECTATOR_HPP