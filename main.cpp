#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <ctime>
#include <iomanip>

#define BUFFERSIZE 100

std::string getTimestamp();

int main() {
    int parentToChild[2];
    int childToParent[2];

    pipe(parentToChild);
    pipe(childToParent);

    pid_t pid = fork();

    if (pid == 0) { // child
        close(parentToChild[1]);
        close(childToParent[0]);

        while (1) {
            char buffer[BUFFERSIZE] {};
            std::string msg;

            read(parentToChild[0], buffer, sizeof(buffer));

            std::cout << "[" << getTimestamp() << "] "
            << "Parent: " << buffer << std::endl;

            getline(std::cin, msg);

            write(childToParent[1], msg.c_str(), msg.size() + 1);
        }
        close(parentToChild[0]);
        close(childToParent[1]);
    } else { // parent
        close(parentToChild[0]);
        close(childToParent[1]);

        while (1) {
            std::string msg;
            char buffer[BUFFERSIZE] {};

            getline(std::cin, msg);

            write(parentToChild[1], msg.c_str(), msg.size() + 1);

            read(childToParent[0], buffer, sizeof(buffer));

            std::cout << "[" << getTimestamp() << "] "
            << "Child: " << buffer << std::endl;
        }

        close(parentToChild[1]);
        close(childToParent[0]);
    }
    return 0;
}

std::string getTimestamp() {
    auto now = std::time(nullptr);
    std::tm localTime {};
    localtime_r(&now, &localTime);
    std::ostringstream oss;
    oss << std::put_time(&localTime, "%H:%M:%S");

    return oss.str();
}
