#ifndef QMEASURESCALENDARVIEW_H
#define QMEASURESCALENDARVIEW_H

#include "Classes/cmeasure.h"
#include "qcalendarwidget.h"
#include "qpainter.h"
class QMeasuresCalendarView : public QCalendarWidget
{
    Q_OBJECT
public:
    QMeasuresCalendarView(QWidget* parent = 0);

    void updateMeasures(std::vector<QSharedPointer<CMeasure>> meas);

protected:
    void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
    {
        QCalendarWidget::paintCell(painter, rect, date);
        for(QSharedPointer<CMeasure> meas : measures){
            if(meas->getDate() == date){
                painter->setBrush(Qt::red);
                painter->drawEllipse(rect.topLeft() + QPoint(12, 7), 3, 3);
            }
        }
    }

private:
    std::vector<QSharedPointer<CMeasure>> measures;
};

#endif // QMEASURESCALENDARVIEW_H
