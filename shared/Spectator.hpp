// shared/Spectator.hpp

#ifndef SPECTATOR_HPP
#define SPECTATOR_HPP

#include <string>

struct Spectator {
    int         id;
    std::string name;
    bool        isVIP;
    int         priorityLevel;

    Spectator() = default;
    Spectator(int _id, const std::string& _name, bool _isVIP, int prio = 0)
      : id(_id), name(_name), isVIP(_isVIP), priorityLevel(prio) {}
};

#endif // SPECTATOR_HPP