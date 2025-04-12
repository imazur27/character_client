/**
 * @file main_window.h
 * @brief Main application window with character table
 */

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "client_connection.h"

namespace Ui {
class MainWindow;
}

/**
 * @class MainWindow
 * @brief Main application window with character table and controls
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void slotConnectionEstablished();
    void slotConnectionFailed(const QString& error);
    void slotCharactersReceived(const std::vector<CharacterData>& characters);
    void slotCharacterReceived(const CharacterData& character);
    void slotOperationCompleted(bool success, const QString& message);
    void slotShowInfoClicked();
    void slotAddClicked();
    void slotAddCompleted(bool success, const QString& message);

private:
    void setupTable();
    void refreshCharacters();
    void showCharacterInfo(int id);
    void showError(const QString& message);

    Ui::MainWindow* ui;
    ClientConnection* m_connection;
    QStandardItemModel* m_model;
};

#endif // MAIN_WINDOW_H
