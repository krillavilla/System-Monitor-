// src/processor.cpp
#include "processor.h"
#include "linux_parser.h"

// Calculate CPU utilization
float Processor::Utilization() {
  std::vector<std::string> cpu_stats = LinuxParser::CpuUtilization();

  long user = std::stol(cpu_stats[LinuxParser::kUser_]);
  long nice = std::stol(cpu_stats[LinuxParser::kNice_]);
  long system = std::stol(cpu_stats[LinuxParser::kSystem_]);
  long idle = std::stol(cpu_stats[LinuxParser::kIdle_]);
  long iowait = std::stol(cpu_stats[LinuxParser::kIOwait_]);
  long irq = std::stol(cpu_stats[LinuxParser::kIRQ_]);
  long softirq = std::stol(cpu_stats[LinuxParser::kSoftIRQ_]);
  long steal = std::stol(cpu_stats[LinuxParser::kSteal_]);

  long idle_time = idle + iowait;
  long non_idle_time = user + nice + system + irq + softirq + steal;
  long total_time = idle_time + non_idle_time;

  // Calculate delta values
  long total_d = total_time - prev_total_;
  long idle_d = idle_time - prev_idle_;

  // Store current values for next calculation
  prev_idle_ = idle_time;
  prev_total_ = total_time;
  prev_non_idle_ = non_idle_time;

  if (total_d == 0) return 0.0;

  // Calculate CPU usage percentage
  return (float)(total_d - idle_d) / total_d;
}