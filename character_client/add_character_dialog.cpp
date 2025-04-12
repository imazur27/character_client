#include "add_character_dialog.h"
#include "ui_add_character_dialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QBuffer>

AddCharacterDialog::AddCharacterDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::AddCharacterDialog)
{
    ui->setupUi(this);
    setWindowTitle("Add New Character");

    // Hide error labels by default
    ui->nameErrorLabel->hide();
    ui->surnameErrorLabel->hide();
    ui->imageErrorLabel->hide();

    ui->nameEdit->setMaxLength(50);
    ui->surnameEdit->setMaxLength(50);

    // Set up validators for name and surname
    QRegularExpression nameRegex("^[a-zA-Z]+$");
    ui->nameEdit->setValidator(new QRegularExpressionValidator(nameRegex, this));
    ui->surnameEdit->setValidator(new QRegularExpressionValidator(nameRegex, this));

    // Set default bio text
    ui->bioEdit->setPlainText("no bio yet");

    // Set age range
    ui->ageSpin->setMinimum(1);
    ui->ageSpin->setMaximum(150);

    // Map widgets to their error labels
    m_errorLabels.insert(ui->nameEdit, ui->nameErrorLabel);
    m_errorLabels.insert(ui->surnameEdit, ui->surnameErrorLabel);

    connect(ui->nameEdit, &QLineEdit::textChanged, this, &AddCharacterDialog::slotValidateInput);
    connect(ui->surnameEdit, &QLineEdit::textChanged, this, &AddCharacterDialog::slotValidateInput);
}

AddCharacterDialog::~AddCharacterDialog() {
    delete ui;
}

CharacterData AddCharacterDialog::getCharacterData() const {
    CharacterData character;
    character.name = ui->nameEdit->text().toStdString();
    character.surname = ui->surnameEdit->text().toStdString();
    character.age = ui->ageSpin->value();
    character.bio = ui->bioEdit->toPlainText().toStdString();

    if (m_imageLoaded) {
        QByteArray imageData;
        QBuffer buffer(&imageData);
        buffer.open(QIODevice::WriteOnly);
        m_pixmap.save(&buffer, "JPG");
        character.image.assign(imageData.begin(), imageData.end());
    }

    return character;
}

void AddCharacterDialog::slotValidateInput() {
    validateName(ui->nameEdit->text()) ? clearError(ui->nameEdit)
                                      : showError(ui->nameEdit, "Only letters allowed");

    validateName(ui->surnameEdit->text()) ? clearError(ui->surnameEdit)
                                         : showError(ui->surnameEdit, "Only letters allowed");
}

void AddCharacterDialog::accept() {
    if (!validateForm()) {
        QMessageBox::warning(this, "Validation Error", "Please fix all errors before submitting");
        return;
    }
    QDialog::accept();
}

bool AddCharacterDialog::validateName(const QString& name) {
    return !name.isEmpty() && name.contains(QRegularExpression("^[a-zA-Z]+$"));
}

bool AddCharacterDialog::validateForm() {
    bool valid = true;

    if (!validateName(ui->nameEdit->text())) {
        showError(ui->nameEdit, "Only letters allowed");
        valid = false;
    }

    if (!validateName(ui->surnameEdit->text())) {
        showError(ui->surnameEdit, "Only letters allowed");
        valid = false;
    }

    return valid;
}

void AddCharacterDialog::showError(QWidget* widget, const QString& message) {
    widget->setStyleSheet("border: 1px solid red;");

    if (m_errorLabels.contains(widget)) {
        QLabel *errorLabel = m_errorLabels.value(widget);
        errorLabel->setText(message);
        errorLabel->show();
    }
}

void AddCharacterDialog::clearError(QWidget* widget) {
    widget->setStyleSheet("");

    if (m_errorLabels.contains(widget)) {
        m_errorLabels.value(widget)->hide();
    }
}
