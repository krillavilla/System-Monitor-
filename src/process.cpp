// src/process.cpp
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
  UpdateCpuUtilization();
}

// Return process ID
int Process::Pid() { return pid_; }

// Calculate CPU utilization for this process
float Process::CpuUtilization() {
  UpdateCpuUtilization();
  return cpu_;
}

void Process::UpdateCpuUtilization() {
  // Get process stats
  string line;
  vector<string> values;
  string value;

  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid_) + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    // Read all values
    while (linestream >> value) {
      values.push_back(value);
    }

    if (values.size() < 22) return;

    long utime = std::stol(values[13]);
    long stime = std::stol(values[14]);
    long cutime = std::stol(values[15]);
    long cstime = std::stol(values[16]);
    long starttime = std::stol(values[21]);

    long total_time = utime + stime + cutime + cstime;
    long seconds = LinuxParser::UpTime() - (starttime / sysconf(_SC_CLK_TCK));

    if (seconds - prev_seconds_ > 0) {
      cpu_ = ((float)(total_time - prev_total_time_) / sysconf(_SC_CLK_TCK)) / (seconds - prev_seconds_);
    }

    prev_total_time_ = total_time;
    prev_seconds_ = seconds;
  }
}

// Return process command
string Process::Command() { return LinuxParser::Command(pid_); }

// Return process RAM usage
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Return process username
string Process::User() { return LinuxParser::User(pid_); }

// Return process uptime
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// Compare processes based on CPU usage (for sorting)
bool Process::operator<(Process const& a) const {
  return cpu_ > a.cpu_; // Sort in descending order
}