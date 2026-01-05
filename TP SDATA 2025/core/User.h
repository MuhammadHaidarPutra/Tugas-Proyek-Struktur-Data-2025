#ifndef USER_H
#define USER_H

#include <string>

using namespace std;

class User {
public:
    string username;
    string password;
    string role;
    string nama;
    
    User() {}
    
    User(string u, string p, string r, string n) 
        : username(u), password(p), role(r), nama(n) {}
};

#endif // USER_H
