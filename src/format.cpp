#include <string>
#include <iomanip>
#include <sstream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long hours = seconds / 3600;
  seconds %= 3600;
  long minutes = seconds / 60;
  seconds %= 60;

  std::ostringstream stream;
  stream << std::setfill('0') << std::setw(2) << hours << ":"
         << std::setfill('0') << std::setw(2) << minutes << ":"
         << std::setfill('0') << std::setw(2) << seconds;
  return stream.str();
}
