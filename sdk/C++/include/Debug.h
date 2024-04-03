#ifndef CODECRAFTSDK_DEBUG_H
#define CODECRAFTSDK_DEBUG_H

#include "iostream"
#include "fstream"

class Debug {

public:
    Debug();
    ~Debug();

    void log(std::string str);
    void warn(std::string str);
    void error(std::string str);


public:
    std::ofstream outFile;
};


#endif //CODECRAFTSDK_DEBUG_H
