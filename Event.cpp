#include "Event.h"
#include <array>

Event::Event() {}

// type-based constructors
Event::Event(Type t, Gpio adcPin, uint32_t adcValue) : m_type(t) {
  m_params[0] = static_cast<uint32_t>(adcPin);
  m_params[1] = adcValue;
}
Event::Event(Type t, uint32_t canEid, std::array<uint32_t, 8> canFrame) : m_type(t) {
  m_params[0] = canEid;

  // set data frame (max 8 bytes)
  // for (int i = 0; i < 8; i++) {
  //   m_params[1 + i] = canFrame[i];
  // }
}

Event::Type Event::type() {
  return m_type;
}

/**
 * @note Due to a terrible, terrible machine with no time to upgrade
 *       the toolchain for (no std::variant), the following
 *       member function implementation are, essentially, supporting
 *       c-style polymorphism
 */

// ADC event member functions
Gpio Event::adcPin() { return static_cast<Gpio>(m_params[0]); }

uint32_t Event::adcValue() { return m_params[1]; }

// CAN event member functions
uint32_t Event::canEid() { return m_params[0]; }

std::array<uint32_t, 8> Event::canFrame() {
  std::array<uint32_t, 8> frame = {0,0,0,0,0,0,0,0};

  // std::copy(m_params.begin() + 1, m_params.begin() + 1 + 8,
  //     frame.begin());

  return frame;
}
