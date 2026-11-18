// shared/TemplateInstantiations.cpp
#include "PriorityQueue.hpp"
#include "CircularQueue.hpp"
#include "Spectator.hpp"

// Force instantiation of the templates for Spectator:
template class PriorityQueue<Spectator>;
template class CircularQueue<Spectator>;