#pragma once

#include <vector>

class System;

class InputSystem
{
    std::vector<System *> m_subscribers;

public:
    void Subscribe(System *subscriber) { m_subscribers.emplace_back(subscriber); }
    void BuildEvents();
};

