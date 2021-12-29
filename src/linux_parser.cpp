#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
// DONE
float LinuxParser::MemoryUtilization() { 
  string line;
  float memtotal = 0;
  // float memfree = 0;
  float memavail = 0;
  string key;
  string value;
  string unit;
  std::ifstream memstream(kProcDirectory+kMeminfoFilename);
  if (memstream.is_open()){
    while (std::getline(memstream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value >> unit;
      if(key=="MemTotal:"){
        memtotal = stol(value);
      };
      // Parsing both "free" and "available" memory to simplify future expansion of functionality.
      // if(key=="MemFree:"){
        // memfree = stof(value);
      // };
      if(key=="MemAvailable:"){
        memavail = stol(value);
      };
  }
  }
  float memutil = (1.0*(memtotal-memavail)/memtotal);
  return memutil; // Returned as percent
  };

// TODO: Read and return the system uptime
// DONE
long LinuxParser::UpTime() { 
  long uptime, idletime;
  string line;
  std::ifstream timestream(kProcDirectory+kUptimeFilename);
  if (timestream.is_open()) {
    std::getline(timestream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return uptime;
  }

// TODO: Read and return the number of jiffies for the system
// DONE
long LinuxParser::Jiffies() { 
  string line, key;
  long user, nice, system, idle, iowait, irq, softirq, total;
  std::ifstream cpustream(kProcDirectory+kStatFilename);
  if (cpustream.is_open()){
    std::getline(cpustream, line);
    std::istringstream linestream(line);
    linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq;
    total = user + nice + system + idle + iowait + irq + softirq;
  }
  return total;
  }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
// DONE
// Why don't I just instantiate a process object and call the appropriate values?
// Because the access may be different between windows and linux, this way we can call the linux parser in the process object to get the correct values, and then pass those to ncurses/
long LinuxParser::ActiveJiffies(int pid) {
  long utime, stime, cutime, cstime;
  string temp, line;
  vector<string> pidData;
  std::ifstream pidstream(kProcDirectory + to_string(pid) + kStatFilename);
  if (pidstream.is_open()){
    std::getline(pidstream, line);
    for (unsigned int i=0;i<line.length();i++){
      if (line[i]==' '){
        pidData.push_back(temp);
        temp = "";
      }
      else{
        temp.push_back(line[i]);
      }
    }
    pidData.push_back(temp);
  }
  utime = stol(pidData[13]);
  stime = stol(pidData[14]);
  cutime = stol(pidData[15]);
  cstime = stol(pidData[16]);
  long activejiffs = utime+stime+cutime+cstime;
  return activejiffs;
}

// TODO: Read and return the number of active jiffies for the system (as int)
// DONE
long LinuxParser::ActiveJiffies() {
  string line, key, trail;
  long user, nice, system, active;
  std::ifstream cpustream(kProcDirectory+kStatFilename);
  if (cpustream.is_open()){
    std::getline(cpustream, line);
    std::istringstream linestream(line);
    linestream >> key >> user >> nice >> system >> trail;
    active = user + nice + system;
  }
  return active;
  }

// TODO: Read and return the number of idle jiffies for the system (as int)
// DONE
long LinuxParser::IdleJiffies() { 
  return Jiffies() - ActiveJiffies();
 }

// TODO: Read and return CPU utilization (as a percentage)
// DONE
float LinuxParser::CpuUtilization() { 
  long beforeActJiffies = ActiveJiffies();
  long beforeTotalJiffies = Jiffies();
  sleep(1);
  long currActJiffies = ActiveJiffies();
  long currTotalJiffies = Jiffies();
  return (1.0* currActJiffies-beforeActJiffies)/(currTotalJiffies-beforeTotalJiffies);
  }

// TODO: Read and return the total number of processes (as int)
int LinuxParser::TotalProcesses() { 
  int procCount;
  string line, key, value;
  std::ifstream cpustream(kProcDirectory + kStatFilename);
   while (std::getline(cpustream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key=="processes"){
        procCount = stoi(value);
      };
    }
    return procCount;
}

// TODO: Read and return the number of running processes (as int)
int LinuxParser::RunningProcesses() {
  int actProcCount = 0;
  string line, key, value;
  std::ifstream cpustream(kProcDirectory + kStatFilename);
  while (std::getline(cpustream, line)) {
    std::istringstream linestream(line);
    linestream >> key >> value;
    if (key == "procs_running") {
      actProcCount = stoi(value);
    }
  }
  return actProcCount; 
}

// TODO: Read and return the command associated with a process (as string)
string LinuxParser::Command(int pid) { 
  string cmd;
  std::ifstream procstream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  std::getline(procstream, cmd);
  return cmd;
}

// TODO: Read and return the memory used by a process (as string, in kB)
string LinuxParser::Ram(int pid) { 
  string line, key, unit;
  string value = "";
  std::ifstream procstream(kProcDirectory + to_string(pid) + kStatusFilename);
  while (std::getline(procstream, line)) {
    std::istringstream linestream(line);
    linestream >> key >> value >> unit;
    if (key == "VmSize:") {
      return value;
    }
  }
  return value;
}

// TODO: Read and return the user ID associated with a process (as string)
string LinuxParser::Uid(int pid) { 
  string line, key, value, trail, uid;
  std::ifstream procstream(kProcDirectory + to_string(pid) + kStatusFilename);
  while (std::getline(procstream, line)){
    std::istringstream linestream(line);
    linestream >> key >> value >> trail;
    if (key == "Uid:"){
      uid = value;
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process (as string)
string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  string user, line, username, xval, id, trail;
  std::ifstream passwdstream(kPasswordPath);
  while (std::getline(passwdstream, line)){
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    linestream >> username >> xval >> id >> trail;
    if (id == uid){
      user = username;
    }
  }
  return user; 
}

// TODO: Read and return the uptime of a process (as long int, in units of jiffies)
long LinuxParser::UpTime(int pid) {
  long uptime;
  string temp, line;
  vector<string> pidData;
  std::ifstream pidstream(kProcDirectory + to_string(pid) + kStatFilename);
  if (pidstream.is_open()){
    std::getline(pidstream, line);
    for (unsigned int i=0;i<line.length();i++){
      if (line[i]==' '){
        pidData.push_back(temp);
        temp = "";
      }
      else{
        temp.push_back(line[i]);
      }
    }
    pidData.push_back(temp);
  }
  uptime = stoi(pidData[21])/sysconf(_SC_CLK_TCK);
  return uptime;
}
