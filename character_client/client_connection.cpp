#include "client_connection.h"
#include <stdexcept>
#include <cstring>

#include <QHostAddress>

ClientConnection::ClientConnection(QObject* parent)
    : QObject(parent), m_socket(new QTcpSocket(this))
{
    connect(m_socket, &QTcpSocket::connected, this, &ClientConnection::slotConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &ClientConnection::slotDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &ClientConnection::slotReadyRead);
// In Qt 5.14 and earlier there is signal error(), no errorOccured
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            this, &ClientConnection::slotError);
}

ClientConnection::~ClientConnection() {
    m_socket->disconnectFromHost();
}

void ClientConnection::connectToServer(const QString& host) {
    m_socket->connectToHost(host, Protocol::PORT);
}

std::vector<uint8_t> ClientConnection::serializeId(int id) {
    std::vector<uint8_t> data(sizeof(id));
    std::memcpy(data.data(), &id, sizeof(id));
    return data;
}

bool ClientConnection::findMessageBoundary(size_t &pos)
{
    using namespace Protocol;
// Search for CRLF sequence
    auto it = std::search(
                m_buffer.begin(), m_buffer.end(),
                Protocol::MESSAGE_DELIMITER.begin(), Protocol::MESSAGE_DELIMITER.end()
                );
// Change the pos to next iteration
    if (it != m_buffer.end()) {
        pos = std::distance(m_buffer.begin(), it);
        return true;
    }
    return false;
}

void ClientConnection::getAllCharacters() {
    sendRequest(Protocol::GET_ALL);
    m_expectedResponse = Protocol::GET_ALL;
}

void ClientConnection::getCharacter(int id) {
    sendRequest(Protocol::GET_ONE, serializeId(id));
    m_expectedResponse = Protocol::GET_ONE;
}

void ClientConnection::slotRemoveCharacter(int id) {
    sendRequest(Protocol::REMOVE_CHARACTER, serializeId(id));
    m_expectedResponse = Protocol::REMOVE_CHARACTER;
}

void ClientConnection::slotUpdateCharacter(const CharacterData& character) {
    sendRequest(Protocol::UPDATE_CHARACTER, character.serialize());
    m_expectedResponse = Protocol::UPDATE_CHARACTER;
}

void ClientConnection::addCharacter(const CharacterData& character) {
    sendRequest(Protocol::ADD_CHARACTER, character.serialize());
    m_expectedResponse = Protocol::ADD_CHARACTER;
}

void ClientConnection::sendRequest(uint8_t command, const std::vector<uint8_t>& data) {
    m_lastCommand = command;
    if (m_socket->state() != QAbstractSocket::ConnectedState) {
        emit signalOperationCompleted(false, "Not connected to server");
        return;
    }

    // Prepend command byte
    std::vector<uint8_t> packet;
    // 1 for command
    packet.reserve(1 + data.size() + Protocol::MESSAGE_DELIMITER_SIZE);
    packet.push_back(command);
    packet.insert(packet.end(), data.begin(), data.end());

    packet.insert(packet.end(), Protocol::MESSAGE_DELIMITER.begin(), Protocol::MESSAGE_DELIMITER.end());

    m_socket->write(reinterpret_cast<const char*>(packet.data()), packet.size());
    m_socket->flush();
}

void ClientConnection::slotConnected() {
    emit signalConnectionEstablished();
}

void ClientConnection::slotDisconnected() {
    emit signalOperationCompleted(false, "Disconnected from server");
}

void ClientConnection::slotReadyRead() {
    QByteArray newData = m_socket->readAll();
    m_buffer.insert(m_buffer.end(), newData.begin(), newData.end());

    size_t pos = 0;

    // taking into account TCP messages framing and stacking
    while (findMessageBoundary(pos)) {
        std::vector<uint8_t> message(m_buffer.begin(), m_buffer.begin() + pos);
        processResponse(message);
        // Remove message + CRLF
        m_buffer.erase(m_buffer.begin(), m_buffer.begin() + pos + Protocol::MESSAGE_DELIMITER_SIZE);
        // Reset for next search
        pos = 0;
    }
}

void ClientConnection::processResponse(const std::vector<uint8_t>& data) {
    if (data.empty()) {
        emit signalOperationCompleted(false, "Empty response from server");
        return;
    }

    uint8_t responseType = data[0];
    std::vector<uint8_t> payload(data.begin() + 1, data.end());

    if (responseType == Protocol::RESP_ERROR) {
        emit signalOperationCompleted(false, "Server returned error");
        return;
    }

    try {
        switch (responseType) {
        case Protocol::GET_ALL:
            if (payload.size() == 0) {
                emit signalOperationCompleted(false, "Empty db");
                break;
            }
            emit signalCharactersReceived(CharacterData::deserializeVector(payload));
            break;

        case Protocol::GET_ONE:
            emit signalCharacterReceived(CharacterData::deserialize(payload));
            break;

        case Protocol::ADD_CHARACTER:
            emit signalOperationCompleted(true, "Add successful");
            break;
        case Protocol::UPDATE_CHARACTER:
            emit signalOperationCompleted(true, "Update successful");
            break;
        case Protocol::REMOVE_CHARACTER:
            emit signalOperationCompleted(true, "Remove successful");
            break;

        case Protocol::RESP_SUCCESS:
            emit signalOperationCompleted(true, "Operation successful");
            break;

        default:
            emit signalOperationCompleted(false, "Unknown response type");
        }
    } catch (const std::exception& e) {
        emit signalOperationCompleted(false, QString("Processing error: %1").arg(e.what()));
    }
}

void ClientConnection::slotError(QAbstractSocket::SocketError error) {
    Q_UNUSED(error)
    emit signalConnectionFailed(m_socket->errorString());
}
