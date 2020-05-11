#ifndef RYZENSMARTTUNINGUTILITY_H
#define RYZENSMARTTUNINGUTILITY_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class RyzenSmartTuningUtility; }
QT_END_NAMESPACE

class RyzenSmartTuningUtility : public QMainWindow
{
    Q_OBJECT

public:
    RyzenSmartTuningUtility(QWidget *parent = nullptr);
    ~RyzenSmartTuningUtility();

private slots:
    void on_Text_highlighted(const QString &arg1);

    void on_label_linkActivated(const QString &link);

private:
    Ui::RyzenSmartTuningUtility *ui;
};
#endif // RYZENSMARTTUNINGUTILITY_H
