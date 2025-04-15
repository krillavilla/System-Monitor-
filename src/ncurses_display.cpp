// src/ncurses_display.cpp
#include "ncurses_display.h"

#include <curses.h>

#include <chrono>
#include <string>
#include <thread>
#include <vector>

#include "format.h"
#include "system.h"

using std::string;
using std::to_string;

// Implementation for ProgressBar
std::string NCursesDisplay::ProgressBar(float percent) {
  std::string result{"0%"};
  int size{50};
  float bars{percent * size};

  for (int i = 0; i < size; ++i) {
    result += i <= bars ? '|' : ' ';
  }

  std::string display{std::to_string(percent * 100).substr(0, 4)};
  if (percent < 0.1 || percent == 1.0)
    display = std::to_string(percent * 100).substr(0, 3);
  return result + " " + display + "/100%";
}

// Fix for DisplaySystem function with zero-length format warnings fixed
void NCursesDisplay::DisplaySystem(System& system, WINDOW* window) {
  int row{0};
  mvwprintw(window, ++row, 2, "OS: %s", system.OperatingSystem().c_str());
  mvwprintw(window, ++row, 2, "Kernel: %s", system.Kernel().c_str());
  mvwprintw(window, ++row, 2, "CPU: ");
  wattron(window, COLOR_PAIR(1));
  wmove(window, row, 10);  // Use wmove instead of mvwprintw with empty string
  wprintw(window, "%s", ProgressBar(system.Cpu().Utilization()).c_str());
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, ++row, 2, "Memory: ");
  wattron(window, COLOR_PAIR(1));
  wmove(window, row, 10);  // Use wmove instead of mvwprintw with empty string
  wprintw(window, "%s", ProgressBar(system.MemoryUtilization()).c_str());
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, ++row, 2, "Total Processes: %s",
            to_string(system.TotalProcesses()).c_str());
  mvwprintw(window, ++row, 2, "Running Processes: %s",
            to_string(system.RunningProcesses()).c_str());
  mvwprintw(window, ++row, 2, "Up Time: %s",
            Format::ElapsedTime(system.UpTime()).c_str());
  wrefresh(window);
}

// Fix for DisplayProcesses function
void NCursesDisplay::DisplayProcesses(std::vector<Process>& processes,
                                      WINDOW* window, int n) {
  int row{0};
  int const pid_column{2};
  int const user_column{9};
  int const cpu_column{20};
  int const ram_column{30};
  int const time_column{40};
  int const command_column{52};
  wattron(window, COLOR_PAIR(2));
  mvwprintw(window, ++row, pid_column, "PID");
  mvwprintw(window, row, user_column, "USER");
  mvwprintw(window, row, cpu_column, "CPU[%%]");
  mvwprintw(window, row, ram_column, "RAM[MB]");
  mvwprintw(window, row, time_column, "TIME+");
  mvwprintw(window, row, command_column, "COMMAND");
  wattroff(window, COLOR_PAIR(2));

  // Convert to size_t to avoid signed/unsigned comparison warnings
  size_t display_processes = processes.size();
  if (display_processes > static_cast<size_t>(n)) {
    display_processes = static_cast<size_t>(n);
  }

  for (size_t i = 0; i < display_processes; ++i) {
    mvwprintw(window, ++row, pid_column, "%d", processes[i].Pid());
    mvwprintw(window, row, user_column, "%s", processes[i].User().c_str());
    float cpu = processes[i].CpuUtilization() * 100;
    mvwprintw(window, row, cpu_column, "%.1f", cpu);
    mvwprintw(window, row, ram_column, "%s", processes[i].Ram().c_str());
    mvwprintw(window, row, time_column, "%s",
              Format::ElapsedTime(processes[i].UpTime()).c_str());

    // Get max width available for command
    int max_width = getmaxx(window) - command_column;
    string cmd = processes[i].Command();
    if (cmd.length() > static_cast<size_t>(max_width)) {
      cmd = cmd.substr(0, max_width);
    }
    mvwprintw(window, row, command_column, "%s", cmd.c_str());
  }
}

// Implementation for Display
void NCursesDisplay::Display(System& system, int n) {
  initscr();      // start ncurses
  noecho();       // do not print input values
  cbreak();       // terminate ncurses on ctrl + c
  start_color();  // enable color

  int x_max{getmaxx(stdscr)};
  WINDOW* system_window = newwin(9, x_max - 1, 0, 0);

  // Get the maximum y coordinate of the system window
  int system_window_height = getmaxy(system_window);

  // Use the height to position the process window
  WINDOW* process_window = newwin(3 + n, x_max - 1, system_window_height, 0);

  while (true) {
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    box(system_window, 0, 0);
    box(process_window, 0, 0);
    DisplaySystem(system, system_window);
    DisplayProcesses(system.Processes(), process_window, n);
    wrefresh(system_window);
    wrefresh(process_window);
    refresh();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  endwin();
}