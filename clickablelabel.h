#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <QPoint>

class ClickableLabel : public QLabel
{
	Q_OBJECT
public:
	explicit ClickableLabel(QWidget* parent = 0);
	~ClickableLabel();
signals:
	void clicked(ClickableLabel* clickableLabel);
	void clicked(QPoint pos, ClickableLabel* clickableLabel);
protected:
	virtual void mouseReleaseEvent(QMouseEvent *event);
};

#endif // CLICKABLELABEL_H
