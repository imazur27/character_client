#ifndef ADD_CHARACTER_DIALOG_H
#define ADD_CHARACTER_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QMap>
#include <QPixmap>
#include "protocol.h"

namespace Ui {
class AddCharacterDialog;
}

class AddCharacterDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddCharacterDialog(QWidget* parent = nullptr);
    ~AddCharacterDialog();

    CharacterData getCharacterData() const;

private slots:
    void slotValidateInput();
    void accept() override;

private:
    void showError(QWidget* widget, const QString& message);
    void clearError(QWidget* widget);
    bool validateName(const QString& name);
    bool validateForm();

    Ui::AddCharacterDialog* ui;
    QPixmap m_pixmap;
    bool m_imageLoaded = false;

    QMap<QWidget*, QLabel*> m_errorLabels{};
};

#endif // ADD_CHARACTER_DIALOG_H
