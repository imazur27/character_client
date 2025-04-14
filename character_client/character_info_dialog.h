/**
 * \file character_info_dialog.h
 * \brief Dialog for displaying and editing character information
 */

#ifndef CHARACTER_INFO_DIALOG_H
#define CHARACTER_INFO_DIALOG_H

#include <QDialog>
#include "protocol.h"

namespace Ui {
class CharacterInfoDialog;
}

/**
 * \class CharacterInfoDialog
 * \brief Modal dialog for viewing and editing character details
 */
class CharacterInfoDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * \brief Constructs a CharacterInfoDialog with the specified character data.
     *
     * \param character The character data to be displayed and edited in the dialog.
     * \param parent The parent widget for this dialog. Defaults to nullptr.
     */
    explicit CharacterInfoDialog(const CharacterData& character, QWidget* parent = nullptr);

    /**
     * \brief Destroys the CharacterInfoDialog instance.
     */
    ~CharacterInfoDialog();

    /**
     * \brief Retrieves the character data after editing.
     *
     * \return CharacterData The character data populated from the dialog fields.
     */
    CharacterData getCharacterData() const;

signals:
    /**
     * \brief Signal emitted when a remove request is made for a character.
     *
     * \param id The ID of the character to be removed.
     */
    void signalRemoveRequested(int id);

    /**
     * \brief Signal emitted when an update request is made for a character.
     *
     * \param character The updated character data.
     */
    void signalUpdateRequested(const CharacterData& character);

private slots:
    /**
     * \brief Slot called when the update button is clicked.
     *
     * This method handles the logic for updating the character information.
     */
    void slotUpdateClicked();

    /**
     * \brief Slot called when the remove button is clicked.
     *
     * This method handles the logic for removing the character.
     */
    void slotRemoveClicked();

private:
    /**
     * \brief Validates the input data in the dialog.
     *
     * \return true if the input is valid; false otherwise.
     */
    bool validateInput();

    Ui::CharacterInfoDialog* ui; ///< Pointer to the UI elements of the dialog.
    CharacterData m_character; ///< The character data being edited.
};

#endif // CHARACTER_INFO_DIALOG_H
