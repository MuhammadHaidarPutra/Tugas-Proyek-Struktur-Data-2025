#ifndef LOG_H
#define LOG_H

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

class NodeLog {
public:
    string timestamp;
    string adminName;
    string aksi;
    NodeLog* next;
    NodeLog* prev; 

    NodeLog(string admin, string action) {
        time_t now = time(0);
        tm *ltm = localtime(&now);

        ostringstream oss;
        oss << (1900 + ltm->tm_year) << "-"
            << setw(2) << setfill('0') << (1 + ltm->tm_mon) << "-"
            << setw(2) << setfill('0') << ltm->tm_mday << " "
            << setw(2) << setfill('0') << ltm->tm_hour << ":"
            << setw(2) << setfill('0') << ltm->tm_min << ":"
            << setw(2) << setfill('0') << ltm->tm_sec;

        timestamp = oss.str();
        adminName = admin;
        aksi = action;
        next = NULL;
        prev = NULL;
    }
};

#endif // LOG_H
