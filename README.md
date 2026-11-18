# APUEC: Esports Championship Management System (C++)

A console application that runs a university esports tournament end to end: scheduling matches, registering and queueing players, managing spectators, and logging results. Its core data structures (linked list, stack, queue, circular queue, priority queue and hash map) are implemented from scratch rather than taken from the standard library, and each module is built around the structure that fits its problem.

Built for the Data Structures module at Asia Pacific University as a group assignment. It is shared here with my teammates' permission, with names removed.

## Modules

| # | Module | Data structures used |
|---|---|---|
| 1 | Match Scheduling & Player Progression | Linked list (players, matches), stack (match history), hash map (players by game) |
| 2 | Tournament Registration & Player Queueing | Priority queue (VIP, early-bird and wildcard players, min-heap), FIFO queue (regular registration), circular queues (checked-in players and the replacement waiting list) |
| 3 | Spectator Management | Priority queue (VIP spectators), circular queue (main queue); seat assignment, overflow handling and a live-streaming simulator |
| 4 | Result Logging & Performance History | Queue (recent results), stack (history with undo of the last entry), hash map (performance by player), linked list (leaderboard); exports statistics and reports |

What each module does:

- **Scheduling:** load players from `data/players.csv`, generate qualifier, group and knockout matches, conduct matches, and view standings, the tournament bracket and match history.
- **Registration:** route players through the priority and regular queues, check players in, and handle a withdrawal by promoting the next player from the waiting list.
- **Spectators:** check spectators in, manage VIP seating and audience overflow.
- **Logging:** record match results, keep a leaderboard, undo the last entry, and export reports.

## Build and run

Requires a C++11 compiler (`g++`) and `make`.

```
make          # build
make run      # build and run
make clean    # remove build output
```

Verified with `g++` 11 on Linux: a clean build completes without errors, and the menu starts and exits normally. The program reads sample players from `data/players.csv` (fictional names), and writes its own logs to `data/` while running.

## Project layout

```
main.cpp          menu and program flow
shared/           data structures (LinkedList, Stack, Queue, CircularQueue,
                  PriorityQueue, HashMap) plus Player and Spectator types
tasks/            the four modules (Task1_Scheduler ... Task4_Logger)
data/players.csv  sample player data
Makefile
```

## What I would improve

- Add automated tests for each data structure (insert, remove, boundary cases) so the structures are verified independently of the menu.
- Replace the interactive menu with a small command interface so scenarios can be scripted.
