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
    std::string bio{};

// Serialize/deserialize
    std::vector<uint8_t> serialize() const;
    static CharacterData deserialize(const std::vector<uint8_t>& data);

// Helper methods to serialize/deserialize all characters
    static std::vector<uint8_t> serializeVector(const std::vector<CharacterData>& characters);
    static std::vector<CharacterData> deserializeVector(const std::vector<uint8_t>& data);

// Helper methods
    static void write_string(std::vector<uint8_t>& buffer, const std::string& str);
    static std::string read_string(const std::vector<uint8_t>& buffer, size_t& offset);
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

// message delimiter
constexpr std::string_view MESSAGE_DELIMITER = "\r\n";
constexpr uint8_t MESSAGE_DELIMITER_SIZE = 2;
}

#endif // PROTOCOL_H
