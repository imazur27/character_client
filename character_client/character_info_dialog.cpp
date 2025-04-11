#include "character_info_dialog.h"
#include "ui_character_info_dialog.h"
#include <QMessageBox>
#include <QFileDialog>

CharacterInfoDialog::CharacterInfoDialog(const CharacterData& character, QWidget* parent)
    : QDialog(parent),
      ui(new Ui::CharacterInfoDialog),
      m_character(character)
{
    ui->setupUi(this);
    setWindowTitle("Character Info");
    setModal(true);

    // Set initial values
    ui->idLabel->setText(QString::number(m_character.id));
    ui->nameEdit->setText(QString::fromStdString(m_character.name));
    ui->surnameEdit->setText(QString::fromStdString(m_character.surname));
    ui->ageSpin->setValue(m_character.age);
    ui->bioEdit->setPlainText(QString::fromStdString(m_character.bio));

    // Connect signals
    connect(ui->updateButton, &QPushButton::clicked, this, &CharacterInfoDialog::slotUpdateClicked);
}

CharacterInfoDialog::~CharacterInfoDialog() {
    delete ui;
}

CharacterData CharacterInfoDialog::getCharacterData() const {
    return m_character;
}

void CharacterInfoDialog::slotUpdateClicked() {
    if (!validateInput()) {
        return;
    }

    m_character.name = ui->nameEdit->text().toStdString();
    m_character.surname = ui->surnameEdit->text().toStdString();
    m_character.age = ui->ageSpin->value();
    m_character.bio = ui->bioEdit->toPlainText().toStdString();

    emit signalUpdateRequested(m_character);
    accept();
}

bool CharacterInfoDialog::validateInput() {
    if (ui->nameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Name cannot be empty");
        return false;
    }
    if (ui->surnameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Surname cannot be empty");
        return false;
    }
    if (ui->ageSpin->value() <= 0) {
        QMessageBox::warning(this, "Warning", "Age must be positive");
        return false;
    }
    return true;
}
