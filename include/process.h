#ifndef PROCESS_H
#define PROCESS_H

#include <processor.h>

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp


  // TODO: Declare any necessary private members
 private:
 int pid_;
 long prev_seconds_;
 float cpu_ = 0.0;
 long prev_total_time_ = 0;
 long prev_system_time_ = 0;
 void UpdateCpuUtilization();


};

#endif