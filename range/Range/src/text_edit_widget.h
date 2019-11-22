/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   text_edit_widget.h                                       *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   2-nd November 2018                                       *
 *                                                                   *
 *  DESCRIPTION: Text edit widget class declaration                  *
 *********************************************************************/

#ifndef TEXT_EDIT_WIDGET_H
#define TEXT_EDIT_WIDGET_H

#include <QPlainTextEdit>
#include <QPushButton>

class TextEditWidget : public QWidget
{

    Q_OBJECT

    protected:

        //! Text edit.
        QPlainTextEdit *textEdit;
        //! Cancel button.
        QPushButton *cancelButton;
        //! OK button.
        QPushButton *okButton;

    public:

        //! Constructor.
        TextEditWidget(QWidget *parent = nullptr);

        //! Set text.
        void setText(const QString &text);

    private slots:

        //! Tesxt edit text has changed.
        void onTextEditChanged();

        //! On cancel button clicked.
        void onCancelButtonClicked();

        //! On ok button clicked.
        void onOkButtonClicked();

    signals:

        //! Position changed.
        void changed(const QString &text);

        //! Close signal.
        void closed(void);

};

#endif // TEXT_EDIT_WIDGET_H
