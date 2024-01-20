#ifndef QMEASURESCALENDARVIEW_H
#define QMEASURESCALENDARVIEW_H

#include "Classes/cmeasure.h"
#include "qcalendarwidget.h"
#include "qpainter.h"
#include <cmath>
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
        painter->setPen(Qt::red);
        if(date == QDate::currentDate()){
            painter->drawLine(rect.bottomLeft(), rect.bottomRight());
            painter->drawLine(rect.bottomRight(), rect.topRight());
            painter->drawLine(rect.topRight(), rect.topLeft());
            painter->drawLine(rect.topLeft(), rect.bottomLeft());
        }
        int numMeas = 0;
        bool allDone = true;
        for(QSharedPointer<CMeasure> meas : measures){
            if(meas->getDate() == date){
                if(allDone && meas->getState() != CMeasure::STATE_DONE){
                    allDone = false;
                }
                if(numMeas == 1){
                    //Draw plus
                    painter->setBrush(Qt::red);
                    painter->drawText(rect.topLeft() + QPoint(8, 18), "+");
                    numMeas++;
                    continue;
                }else if(numMeas > 1){
                    numMeas++;
                    continue;
                }
                //painter->setBrush(Qt::red);
                //painter->drawEllipse(rect.topLeft() + QPoint(12, 7), 3, 3);
                painter->drawImage(rect.topLeft(), QImage(CMeasure::ICONS_TYPES.value(meas->getType())).scaled(QSize(rect.width()/2, rect.height()), Qt::KeepAspectRatio));
                numMeas++;
            }
        }
        if(numMeas > 1){
            painter->drawText(rect.topLeft() + QPoint(20, 18), QString::number(numMeas-1));
        }
        if(allDone && numMeas > 0){
            painter->drawImage(rect.topRight() - QPoint(rect.width()/4, 0), QImage(":/icons/check.png").scaled(QSize(rect.width()/4, rect.height()/2), Qt::KeepAspectRatio));
        }
    }

private:
    std::vector<QSharedPointer<CMeasure>> measures;
};

#endif // QMEASURESCALENDARVIEW_H
