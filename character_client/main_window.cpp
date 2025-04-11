#include "main_window.h"
#include "ui_main_window.h"
#include <QMessageBox>
#include <QHeaderView>

#include "character_info_dialog.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_connection(new ClientConnection(this)),
      m_model(new QStandardItemModel(this))
{
    ui->setupUi(this);
    setWindowTitle("Character Database Client");

    // Setup table
    setupTable();

    // Connect UI signals
    connect(ui->showInfoButton, &QPushButton::clicked, this, &MainWindow::slotShowInfoClicked);

    // Connect network signals
    connect(
                m_connection, &ClientConnection::signalConnectionEstablished,
                this, &MainWindow::slotConnectionEstablished
                );
    connect(
                m_connection, &ClientConnection::signalConnectionFailed,
                this, &MainWindow::slotConnectionFailed
                );
    connect(
                m_connection, &ClientConnection::signalCharactersReceived,
                this, &MainWindow::slotCharactersReceived
                );
    connect(
                m_connection, &ClientConnection::signalOperationCompleted,
                this, &MainWindow::slotOperationCompleted
                );

    // Connect to server, address hardcoded
    m_connection->connectToServer("10.0.2.5");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupTable() {
    m_model->setColumnCount(5);
    m_model->setHorizontalHeaderLabels({"ID", "Name", "Surname", "Age", "Bio"});
    ui->tableView->setModel(m_model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setVisible(false);
}

void MainWindow::refreshCharacters() {
    m_connection->getAllCharacters();
}

void MainWindow::slotConnectionEstablished() {
    refreshCharacters();
}

void MainWindow::slotConnectionFailed(const QString& error) {
    showError("Connection failed: " + error);
}

void MainWindow::slotCharactersReceived(const std::vector<CharacterData>& characters) {
    m_model->removeRows(0, m_model->rowCount());
    for (const auto& character : characters) {
        QList<QStandardItem*> items;
        items << new QStandardItem(QString::number(character.id));
        items << new QStandardItem(QString::fromStdString(character.name));
        items << new QStandardItem(QString::fromStdString(character.surname));
        items << new QStandardItem(QString::number(character.age));
        items << new QStandardItem(QString::fromStdString(character.bio));
        m_model->appendRow(items);
    }
}

void MainWindow::slotOperationCompleted(bool success, const QString& message) {
    if (!success) {
        showError(message);
    } else {
        refreshCharacters();
    }
}

void MainWindow::slotShowInfoClicked() {
    QModelIndexList selected = ui->tableView->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        showError("No character selected");
        return;
    }

    int id = m_model->item(selected.first().row(), 0)->text().toInt();
    showCharacterInfo(id);
}

void MainWindow::showCharacterInfo(int id) {
    // TODO: need to get character. Connection must sends the request with id, that will be get from
    // selected row in a table
}

void MainWindow::showError(const QString& message) {
    QMessageBox::critical(this, "Error", message);
}
