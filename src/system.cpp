#include <unistd.h>
#include <cstddef>
#include <string>
#include <vector>
#include <map>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"


using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    std::map<int,int> processMap = {
    };
    for (Process process: processes_){
        if (processMap.count(process.Pid()) == 0){
            processMap.insert({process.Pid(), 0});
        } else {
            processMap.at(process.Pid())++;
        };
    };
    vector<int> pids = LinuxParser::Pids();
    for (int pid: pids){
        if (processMap.count(pid) == 0){
            processMap.insert({pid, 0});
        } else {
            processMap.at(pid)++;
        };
    };
    for (auto &ele : processMap){
        if (ele.second > 0){
            continue;
        } else {
            // instantiate Process object
            Process proc(ele.first);
            processes_.emplace_back(proc);
        };
    };
    for (Process proc: processes_){
        proc.CpuUtilization();
    };
    processMap.clear();
    sort(processes_.begin(), processes_.end());
    // reverse(processes_.begin(), processes_.end());
    return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
