#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid){
    pid_ = pid;
    cmd_ = LinuxParser::Command(pid_);
    user_ = LinuxParser::User(pid_);
    cpu_ = 0.69;
    beforeProcJiffies_ = 0;
    beforeTotalJiffies_ = 0;
};

// TODO: Return this process's ID
int Process::Pid() { return pid_;}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    // LINES COMMENTED OUT FOR DYNAMIC CPU UTILIZATION UNTIL SORTING ISSUE RESOLVED
    long currProcJiffies = LinuxParser::ActiveJiffies(pid_);
    long currTotalJiffies = LinuxParser::Jiffies();
    // long procJiffies = currProcJiffies - beforeProcJiffies_;
    // long totalJiffies = currTotalJiffies - beforeTotalJiffies_;
    beforeProcJiffies_ = currProcJiffies;
    beforeTotalJiffies_ = currTotalJiffies;
    cpu_ = (1.0*currProcJiffies/currTotalJiffies);
    // cpu_ = (1.0*procJiffies / totalJiffies);
    return cpu_;
    }

// TODO: Return the command that generated this process
string Process::Command() { return cmd_; }

// TODO: Return this process's memory utilization
string Process::Ram() { 
    string memuse = "0";
    memuse = to_string(std::stoi(LinuxParser::Ram(pid_))/1024);
    return memuse; }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime() - LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) { 
    return a.cpu_ < cpu_;
}