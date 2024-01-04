#ifndef QLOADFARM_H
#define QLOADFARM_H

#include <QDialog>


namespace Ui {
class QLoadFarm;
}

class QLoadFarm : public QDialog
{
    Q_OBJECT

public:
    explicit QLoadFarm(QWidget *parent = nullptr);
    ~QLoadFarm();

private:
    void done(int);

private:
    Ui::QLoadFarm *ui;
};

#endif // QLOADFARM_H
