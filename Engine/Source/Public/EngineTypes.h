#pragma once
#include "Debug/PDebug.h"

// System Libs
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>

// Redefine of the standard string
typedef std::string PString;

// Redefine all of the unsigned ints
typedef uint8_t PUi8;
typedef uint16_t PUi16;
typedef uint32_t PUi32;
typedef uint64_t PUi64;

// Redefine the standard vector
template <typename T>
using TArray = std::vector<T>;

// Redefine smart pointers
// Redefine shared pointer
template <typename T>
using TShared = std::shared_ptr<T>;
// Redefine unique pointer
template <typename T>
using TUnique = std::unique_ptr<T>;
// Redefine weak pointer
template <typename T>
using TWeak = std::weak_ptr<T>;

// Redefine the make_shared function
template <typename T, typename... Args> // ... = expecting more than 1
TShared<T> TMakeShared(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

// Redefine the make_unique function
template <typename T, typename... Args>
TUnique<T> TMakeUnique(Args&&... args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
}