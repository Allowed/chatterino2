#pragma once

#include "fancybutton.hpp"

#include <QWidget>

namespace chatterino {
namespace widgets {

class NotebookButton : public FancyButton
{
    Q_OBJECT

public:
    static const int IconPlus = 0;
    static const int IconUser = 1;
    static const int IconSettings = 2;

    int icon = 0;

    NotebookButton(BaseWidget *parent);

protected:
    void paintEvent(QPaintEvent *) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void clicked();

private:
    bool mouseOver = false;
    bool mouseDown = false;
    QPoint mousePos;
};

}  // namespace widgets
}  // namespace chatterino
