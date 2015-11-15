#include "clickablelabel.h"
#include "QMouseEvent"

ClickableLabel::ClickableLabel(QWidget* parent)
    : QLabel(parent)
{
}

ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent* event)
{
	emit clicked(this);
	emit clicked(event->pos(), this);
}
