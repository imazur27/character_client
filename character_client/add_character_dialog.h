#ifndef ADD_CHARACTER_DIALOG_H
#define ADD_CHARACTER_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QMap>
#include "protocol.h"

namespace Ui {
class AddCharacterDialog;
}

/**
 * \brief The AddCharacterDialog class provides a dialog for adding a new character.
 *
 * This dialog allows users to input character data and validates the input before
 * accepting it. It inherits from QDialog and utilizes the Qt framework for its UI.
 */
class AddCharacterDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * \brief Constructs an AddCharacterDialog with the given parent widget.
     *
     * \param parent The parent widget for this dialog. Defaults to nullptr.
     */
    explicit AddCharacterDialog(QWidget* parent = nullptr);

    /**
     * \brief Destroys the AddCharacterDialog instance.
     */
    ~AddCharacterDialog();

    /**
     * \brief Retrieves the character data entered in the dialog.
     *
     * \return CharacterData The character data populated from the dialog fields.
     */
    CharacterData getCharacterData() const;

private slots:
    /**
     * \brief Overrides the accept method to handle dialog acceptance.
     *
     * This method is called when the user confirms the dialog (e.g., clicks OK).
     */
    void accept() override;

private:
    /**
     * \brief Displays an error message next to the specified widget.
     *
     * \param widget The widget next to which the error message will be displayed.
     * \param message The error message to display.
     */
    void showError(QWidget* widget, const QString& message);

    /**
     * \brief Clears any error message displayed next to the specified widget.
     *
     * \param widget The widget from which the error message will be cleared.
     */
    void clearError(QWidget* widget);

    /**
     * \brief Validates the character name input.
     *
     * \param name The character name to validate.
     * \return true if the name is valid; false otherwise.
     */
    bool validateName(const QString& name);

    /**
     * \brief Validates the entire form before submission.
     *
     * \return true if the form is valid; false otherwise.
     */
    bool validateForm();

    Ui::AddCharacterDialog* ui; ///< Pointer to the UI elements of the dialog.
    QMap<QWidget*, QLabel*> m_errorLabels; ///< Map of widgets to their corresponding error labels.
};

#endif // ADD_CHARACTER_DIALOG_H
