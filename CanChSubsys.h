// Copyright (c) 2018 Formula Slug. All Rights Reserved.

#pragma once

#include <mutex>
#include <vector>
#include <stdint.h>
#include "hal.h"
#include "ch.h"
#include "CanBus.h"
#include "Event.h"
#include "EventQueue.h"

class CanChSubsys {
  private:
    CanBus& m_canBus;
    chibios_rt::Mutex& m_canBusMut;
    EventQueue& m_eventQueue;
  public:
    // @param canBus Reference to canBus containing bus configuration
    //        and lower-level tx/rx utilities
    // @param eventQueue Reference to queue to send this subsystem's
    //        events to. The event queue notifies itself.
    CanChSubsys(CanBus& cb, chibios_rt::Mutex& cbMut, EventQueue& eq);

    // @brief Queue a TX CAN Frame for transmission
    void startSend(CANTxFrame& msg);

    // @brief Inf loop that MUST be called within created static thread
    void runTxThread();
    void runRxThread();
};
