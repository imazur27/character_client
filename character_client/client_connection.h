#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <vector>
#include "protocol.h"

class ClientConnection : public QObject {
    Q_OBJECT

public:
    explicit ClientConnection(QObject* parent = nullptr);
    ~ClientConnection();

    void connectToServer(const QString& host);
    void getAllCharacters();
    uint8_t lastCommand() const { return m_lastCommand; }

signals:
    void signalConnectionEstablished();
    void signalConnectionFailed(const QString& error);
    void signalCharactersReceived(const std::vector<CharacterData>& characters);
    void signalOperationCompleted(bool success, const QString& message);

public slots:
    void slotUpdateCharacter(const CharacterData& character);
    void slotRemoveCharacter(int id);

private slots:
    void slotConnected();
    void slotDisconnected();
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError error);

private:
    void sendRequest(uint8_t command, const std::vector<uint8_t>& data = {});
    void processResponse(const std::vector<uint8_t>& data);
    bool findMessageBoundary(size_t& pos);

    QTcpSocket* m_socket;
    std::vector<uint8_t> m_buffer;
    uint8_t m_expectedResponse = 0;
    uint8_t m_lastCommand = 0;
};

#endif // CLIENT_CONNECTION_H
