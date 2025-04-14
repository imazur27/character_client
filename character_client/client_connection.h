#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <vector>
#include "protocol.h"

/**
 * \class ClientConnection
 * \brief Handles TCP communication with the character server
 * \ingroup Network
 *
 * \details Manages all client-side network operations including:
 * - Connection establishment
 * - Request/response handling
 * - Character data serialization
 * - Error recovery
 *
 * Uses Qt's signal-slot mechanism for asynchronous operation.
 */
class ClientConnection : public QObject {
    Q_OBJECT

public:
    /**
     * \brief Constructs a new ClientConnection
     * \param parent Optional QObject parent
     */
    explicit ClientConnection(QObject* parent = nullptr);

    /**
     * \brief Destructor - ensures proper socket cleanup
     */
    ~ClientConnection();

    /**
     * \brief Initiates connection to server
     * \param host Server hostname/IP address
     *
     * \note Emits signalConnectionEstablished() or signalConnectionFailed()
     */
    void connectToServer(const QString& host);

    /**
     * \brief Requests all characters from server
     * \see Protocol::GET_ALL
     */
    void getAllCharacters();

    /**
     * \brief Requests single character by ID
     * \param id Character ID to retrieve
     * \see Protocol::GET_ONE
     */
    void getCharacter(int id);

    /**
     * \brief Adds new character to server
     * \param character Character data to add
     * \see Protocol::ADD_CHARACTER
     */
    void addCharacter(const CharacterData& character);

    /**
     * \brief Returns last command sent to server
     * \return uint8_t Last command byte
     */
    uint8_t lastCommand() const { return m_lastCommand; }

signals:
    /**
     * \brief Emitted when connection is established
     */
    void signalConnectionEstablished();

    /**
     * \brief Emitted when connection fails
     * \param error Error description
     */
    void signalConnectionFailed(const QString& error);

    /**
     * \brief Emitted when multiple characters are received
     * \param characters Vector of character data
     */
    void signalCharactersReceived(const std::vector<CharacterData>& characters);

    /**
     * \brief Emitted when single character is received
     * \param character Character data
     */
    void signalCharacterReceived(const CharacterData& character);

    /**
     * \brief Emitted when operation completes
     * \param success True if operation succeeded
     * \param message Status message
     */
    void signalOperationCompleted(bool success, const QString& message);

public slots:
    /**
     * \brief Updates existing character on server
     * \param character Modified character data
     * \see Protocol::UPDATE_CHARACTER
     */
    void slotUpdateCharacter(const CharacterData& character);

    /**
     * \brief Removes character from server
     * \param id Character ID to remove
     * \see Protocol::REMOVE_CHARACTER
     */
    void slotRemoveCharacter(int id);

private slots:
    /**
     * \brief Handles successful connection
     */
    void slotConnected();

    /**
     * \brief Handles server disconnection
     */
    void slotDisconnected();

    /**
     * \brief Processes incoming data from server
     */
    void slotReadyRead();

    /**
     * \brief Handles socket errors
     * \param error Error code
     */
    void slotError(QAbstractSocket::SocketError error);

private:
    /**
     * \brief Sends request to server
     * \param command Protocol command byte
     * \param data Optional request payload
     */
    void sendRequest(uint8_t command, const std::vector<uint8_t>& data = {});

    /**
     * \brief Processes server response
     * \param data Received binary data
     */
    void processResponse(const std::vector<uint8_t>& data);

    /**
     * \brief Serializes ID for network transmission
     * \param id ID to serialize
     * \return std::vector<uint8_t> 4-byte big-endian representation
     */
    std::vector<uint8_t> serializeId(int id);

    /**
     * \brief Finds message boundary in buffer
     * \param pos [out] Position of delimiter if found
     * \return bool True if delimiter found
     */
    bool findMessageBoundary(size_t& pos);

    QTcpSocket* m_socket;                     ///< TCP socket instance
    std::vector<uint8_t> m_buffer;            ///< Raw data buffer
    uint8_t m_expectedResponse = 0;          ///< Expected response code
    uint8_t m_lastCommand = 0;               ///< Last sent command
};

#endif // CLIENT_CONNECTION_H
