#ifndef SETTING_H
#define SETTING_H

#include <QtWidgets/QWidget>
#include <set>


namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

signals:
    void paramsChanged(QString closingSize, QString gaussSize, QString threshold, QString rate, QString maxAera, QString minAera);

private:
    Ui::Settings *ui;
    std::set<QString> users;
};

#endif // SETTING_H
