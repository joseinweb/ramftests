#pragma once
#ifndef MODULE_NAME
#define MODULE_NAME ramfapp
#endif

#include <WPEFramework/com/com.h>
#include <WPEFramework/core/core.h>

#include <memory>
#include <iostream>

using namespace WPEFramework;
using namespace std;

template <typename T>

inline T retrieveInputFromUser(const std::string& prompt, bool allowEmpty, T defaultValue)
{
    std::cout << prompt;
    T value;

    if (!(std::cin >> value)) {
        if (allowEmpty) {
            std::cin.clear(); // clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
            return defaultValue;
        } else {
            // If empty input is not allowed, keep prompting
            while (!(std::cin >> value)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. " << prompt;
            }
        }
    }
    return value;
}
