// include/processor.h
#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

class Processor {
 public:
  float Utilization();

 private:
  long prev_idle_ = 0;
  long prev_total_ = 0;
  long prev_non_idle_ = 0;
};

#endif