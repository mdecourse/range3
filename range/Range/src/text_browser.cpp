/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   text_browser.cpp                                         *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   27-th August 2013                                        *
 *                                                                   *
 *  DESCRIPTION: Text browser class definition                       *
 *********************************************************************/

#include <QFontDatabase>

#include "text_browser.h"

TextBrowser::TextBrowser(bool hasClearButton, QWidget *parent)
  : QTextBrowser(parent)
  , hasClearButton(hasClearButton)
{
    this->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    if (this->hasClearButton)
    {
        QIcon clearIcon(":/icons/file/pixmaps/range-clear.svg");

        this->clearButton = new QPushButton(clearIcon,"Clear",this);

        QObject::connect(this->clearButton,
                         &QPushButton::clicked,
                         this,
                         &TextBrowser::clear);
    }
}

void TextBrowser::resizeEvent(QResizeEvent *event)
{
    QTextBrowser::resizeEvent(event);

    if (!this->hasClearButton)
    {
        return;
    }

    int bw = this->clearButton->width();
    int bh = this->clearButton->height();
    int tw = this->width();

    int x = tw - bw - 25;
    int y = 5;

    this->clearButton->setGeometry(x,y,bw,bh);
}
