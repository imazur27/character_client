/**
 * @file character_info_dialog.h
 * @brief Dialog for displaying and editing character information
 */

#ifndef CHARACTER_INFO_DIALOG_H
#define CHARACTER_INFO_DIALOG_H

#include <QDialog>
#include "protocol.h"

namespace Ui {
class CharacterInfoDialog;
}

/**
 * @class CharacterInfoDialog
 * @brief Modal dialog for viewing and editing character details
 */
class CharacterInfoDialog : public QDialog {
    Q_OBJECT

public:
    explicit CharacterInfoDialog(const CharacterData& character, QWidget* parent = nullptr);
    ~CharacterInfoDialog();

    CharacterData getCharacterData() const;

signals:
    void signalUpdateRequested(const CharacterData& character);

private slots:
    void slotUpdateClicked();

private:
    bool validateInput();

    Ui::CharacterInfoDialog* ui;
    CharacterData m_character;
};

#endif // CHARACTER_INFO_DIALOG_H
