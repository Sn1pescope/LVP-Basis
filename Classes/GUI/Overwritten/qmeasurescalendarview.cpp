#include "qmeasurescalendarview.h"

QMeasuresCalendarView::QMeasuresCalendarView(QWidget* parent) : QCalendarWidget(parent)
{

}

void QMeasuresCalendarView::updateMeasures(std::vector<QSharedPointer<CMeasure>> meas){
    measures = meas;
    this->repaint();
}
