/**
 * @file protocol.h
 * @brief Common protocol definitions for client-server communication
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdint>
#include <vector>
#include <string>

/**
 * @struct CharacterData
 * @brief Represents character information
 */
struct CharacterData {
    int32_t id = 0;
    std::string name{};
    std::string surname{};
    uint8_t age = 1;
    std::vector<uint8_t> image{};
    std::string bio{};
};

namespace Protocol {
// Command bytes

constexpr uint8_t GET_ALL = 0x01;
constexpr uint8_t ADD_CHARACTER = 0x02;
constexpr uint8_t REMOVE_CHARACTER = 0x03;
constexpr uint8_t GET_ONE = 0x04;
constexpr uint8_t UPDATE_CHARACTER = 0x05;

// Response codes
constexpr uint8_t RESP_SUCCESS = 0x80;
constexpr uint8_t RESP_ERROR = 0x81;

// Network settings
// this is the hardcoded server port
constexpr int PORT = 12345;

// message separator
constexpr uint8_t CR = '\r';
constexpr uint8_t LF = '\n';
}

#endif // PROTOCOL_H
