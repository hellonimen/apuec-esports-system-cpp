#include "Task3_Spectator.hpp"
#include <iostream>
#include <limits>
#include <vector>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

// Read one spectator’s details (with auto‐ID & duplicate‐check)
Spectator Task3_Spectator::readSpectatorFromInput() {
    // 1) Choose or enter ID
    char autoChar;
    int  id;
    cout<<"Auto-assign ID? (y/n): ";
    while (true) {
        cin>>autoChar; cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (autoChar=='y'||autoChar=='Y') {
            id = nextAutoID++;
            cout<<"Assigned ID: "<<id<<"\n";
            break;
        }
        if (autoChar=='n'||autoChar=='N') {
            cout<<"Enter Spectator ID: ";
            cin>>id; cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        cout<<"Please enter y or n: ";
    }
    // 2) Duplicate check
    if (usedIDs.count(id)) {
        cout<<"Error: ID "<<id<<" already checked in.\n";
        return readSpectatorFromInput();
    }
    usedIDs.insert(id);

    // 3) Read name
    string name;
    cout<<"Name: ";
    getline(cin, name);

    // 4) Read VIP status
    char vipChar;
    bool isVIP;
    cout<<"VIP? (y/n): ";
    while (true) {
        cin>>vipChar; cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (vipChar=='y'||vipChar=='Y') { isVIP=true; break; }
        if (vipChar=='n'||vipChar=='N') { isVIP=false; break; }
        cout<<"Please enter y or n: ";
    }

    int prio = isVIP ? 1 : 0;
    return Spectator{id, name, isVIP, prio};
}

// Enqueue into VIP, main, or overflow
void Task3_Spectator::enqueueSpectator(const Spectator& s) {
    if (s.isVIP) {
        vipQueue.enqueue(s, s.priorityLevel);
    } else if (mainQueue.getSize() >= SEATING_CAPACITY) {
        overflowQueue.push_back(s);
    } else {
        mainQueue.enqueue(s);
    }
}

Task3_Spectator::Task3_Spectator()
  : vipQueue(),
    mainQueue(MAIN_QUEUE_CAPACITY)
{
    initializeSeats();
}

Task3_Spectator::~Task3_Spectator() {
    vipQueue.clear();
    mainQueue.clear();
    overflowQueue.clear();
}

// Batch check-in loop
void Task3_Spectator::checkInSpectator() {
    char again = 'y';
    while (again=='y' || again=='Y') {
        Spectator s = readSpectatorFromInput();
        enqueueSpectator(s);
        assignSeat(s); 
        cout<<"Checked in: "<<s.name
            <<" ("<<(s.isVIP?"VIP":"General")<<")\n";

        cout<<"Check in another spectator? (y/n): ";
        cin>>again; cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Seat all VIPs in order
void Task3_Spectator::manageVIPSeating() {
    cout<<"\n-- VIP Seating --\n";
    int seat=1;
    while (!vipQueue.isEmpty()) {
        auto s = vipQueue.dequeue();
        cout<<seat++<<". "<<s.name<<" (ID "<<s.id<<")\n";
    }
}

// Push extra general spectators into overflow
void Task3_Spectator::handleAudienceOverflow() {
    cout<<"\n-- Overflow Handling --\n";
    while (mainQueue.getSize() > SEATING_CAPACITY) {
        auto s = mainQueue.dequeue();
        overflowQueue.push_back(s);
        cout<<"Overflowed: "<<s.name<<" (ID "<<s.id<<")\n";
    }
}

// Round‐based viewing station scheduler
void Task3_Spectator::organizeLiveStreaming() {
    int round=1;
    while (!vipQueue.isEmpty()
           || !mainQueue.isEmpty()
           || !overflowQueue.empty())
    {
        cout<<"\n-- Live-Stream Round "<<round++
            <<" ("<<STATION_DURATION<<" min each) --\n";

for (int station = 1; station <= STREAM_STATIONS; ++station) {
    Spectator s;

    if (!vipQueue.isEmpty()) {
        s = vipQueue.dequeue();
    }
    else if (!mainQueue.isEmpty()) {
        s = mainQueue.dequeue();
    }
    else if (!overflowQueue.empty()) {
        s = overflowQueue.front();    // get the spectator
        overflowQueue.pop_front();    // remove them
    }
    else {
        break;
    }

    cout << "Station " << station << ": "
         << s.name << " (ID " << s.id
         << (s.isVIP ? ", VIP" : "") << ")\n";
}

        cout<<"Press Enter to start next round...";
        cin.get();
    }
    cout<<"\nAll spectators have had a viewing slot.\n";
}

// ─── Bulk sample generator for quick demos ─────────────────────────
void Task3_Spectator::addSampleSpectators(int count) {
  for (int i = 0; i < count; ++i) {
    int id = nextAutoID++;
    std::ostringstream os;
    os << "Guest" << (i+1);
    bool isVIP = ((i+1) % 10 == 0);
    Spectator s{id, os.str(), isVIP, isVIP?1:0};
    enqueueSpectator(s);
    assignSeat(s); 
  }
  cout << count << " sample spectators added.\n";
}

void Task3_Spectator::addSamplesToMainQueue(int count) {
    for (int i = 0; i < count; ++i) {
        int id = nextAutoID++;
        std::ostringstream os;
        os << "Guest" << (i + 1);
        bool isVIP = ((i + 1) % 10 == 0);
        Spectator s{id, os.str(), isVIP, isVIP ? 1 : 0};
        mainQueue.enqueue(s);
        usedIDs.insert(s.id);
        assignSeat(s);
    }
    cout << count << " guests added directly to mainQueue.\n";
}

void Task3_Spectator::runLiveStreamSimulator() {
    std::vector<Spectator> currentViewers;

    // initial fill
    auto fillSlots = [&]() {
        while ((int)currentViewers.size() < STREAM_STATIONS) {
            Spectator s;
            if (!vipQueue.isEmpty())          s = vipQueue.dequeue();
            else if (!mainQueue.isEmpty())    s = mainQueue.dequeue();
            else if (!overflowQueue.empty()) {
                s = overflowQueue.front();
                overflowQueue.pop_front();
            }
            else break;
            currentViewers.push_back(s);
        }
    };

    fillSlots();

    while (true) {
        // Clear screen (unix)
        std::cout << "\x1B[2J\x1B[H";

        // Display current viewers
        std::cout << "=== LIVE STREAM SIMULATOR ===\n";
        for (int i = 0; i < (int)currentViewers.size(); ++i) {
            auto &s = currentViewers[i];
            std::cout << "Slot " << (i+1)
                      << ": " << s.name
                      << " (ID " << s.id
                      << (s.isVIP? ", VIP":"") << ")\n";
        }
        if (currentViewers.empty()) {
            std::cout << "(no one on stream right now)\n";
        }

        // Show waiting counts
        std::cout << "\nQueue sizes: VIP=" << vipQueue.getSize()
                  << "  Main=" << mainQueue.getSize()
                  << "  Overflow=" << overflowQueue.size() << "\n\n";

        std::cout << "Press ENTER to simulate one leaving, 'q'+ENTER to quit simulator...";
        std::string line;
        std::getline(std::cin, line);
        if (!line.empty() && (line[0]=='q' || line[0]=='Q'))
            break;

        // Simulate one leaving: remove from front of currentViewers
        if (!currentViewers.empty()) {
            // optional: re-enqueue outgoing viewer into overflow
            overflowQueue.push_back(currentViewers.front());
            currentViewers.erase(currentViewers.begin());
        }
        // fill the freed slot
        fillSlots();
    }
}

void Task3_Spectator::initializeSeats() {
  // Rows A–F
  for (int r = 0; r < ROWS; ++r) {
    char row = 'A' + r;
    for (int c = 1; c <= COLS; ++c) {
      std::ostringstream os;
      os << row << c;
      std::string seatID = os.str();
      if ((int)vipSeats.size() < VIP_SEAT_COUNT)
        vipSeats.push(seatID);
      else
        generalSeats.push(seatID);
    }
  }
}

void Task3_Spectator::assignSeat(const Spectator& s) {
    std::string seatID;
    if (s.isVIP && !vipSeats.empty()) {
        seatID = vipSeats.front(); vipSeats.pop();
    }
    else if (!generalSeats.empty()) {
        seatID = generalSeats.front(); generalSeats.pop();
    }
    else {
        // no seats → join the seat‐waitlist
        cout << "⚠️  All seats full. " << s.name
             << " added to seat‐waitlist.\n";
        seatWaitlist.push(s);
        return;
    }
    seatingChart[seatID] = s;
    cout << "Assigned " << s.name
         << " to seat " << seatID << "\n";
}

#include <iomanip>   // for std::setw
#include <sstream>

void Task3_Spectator::displaySeatingChart() const {
    // ─── define a fixed width for each bracketed cell ───
    constexpr int CELL_WIDTH = 8;  

    cout << "\n=== ATRIUM LEVEL 3 SEATING ===\n\n";

    // Header row (column numbers)
    cout << "    ";
    for (int c = 1; c <= COLS; ++c) {
        cout << "[" 
             << setw(CELL_WIDTH) << left << c 
             << right << "]";
    }
    cout << "\n";

    // Each row A–F
    for (int r = 0; r < ROWS; ++r) {
        char row = 'A' + r;
        cout << " " << row << "  ";

        for (int c = 1; c <= COLS; ++c) {
            // Build seat ID, e.g. "A1" or "C10"
            ostringstream os;
            os << row << c;
            string seatID = os.str();

            // Determine cell text (name or seatID)
            string cellText;
            auto it = seatingChart.find(seatID);
            if (it != seatingChart.end()) {
                cellText = it->second.name.substr(0, CELL_WIDTH);
            } else {
                cellText = seatID;
            }

            // Print in [      ] with fixed width
            cout << "[" 
                 << setw(CELL_WIDTH) << left << cellText 
                 << right << "]";
        }
        cout << "\n";
    }
    cout << "\n";
}

void Task3_Spectator::releaseSeat(const std::string& seatID) {
    auto it = seatingChart.find(seatID);
    if (it == seatingChart.end()) {
        cout << "Seat " << seatID << " is already empty or invalid.\n";
        return;
    }
    Spectator vacated = it->second;
    seatingChart.erase(it);

    // Return the seat ID back to the appropriate pool
    if (vacated.isVIP)       vipSeats.push(seatID);
    else /* general */        generalSeats.push(seatID);

    cout << "Released seat " << seatID 
         << " (was occupied by " << vacated.name << ").\n";

    // If anyone’s waiting, assign them immediately
    if (!seatWaitlist.empty()) {
        Spectator next = seatWaitlist.front();
        seatWaitlist.pop();
        assignSeat(next);
    }
}
