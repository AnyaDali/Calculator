
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <memory>
#include <QStatusBar>
#include <QString>
#include <QLabel>
#include <stack>
#include <variant>
#include <cmath>
#include <functional>
#include <iostream>
QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE

class Widget : public QWidget

{
    Q_OBJECT
    static constexpr double EPS = 0.000000001;
    enum class typeStack{NEG_NUM, POS_NUM, OPERATION };

    std::unique_ptr<QGridLayout> layout; // сетка
    std::unique_ptr<QStatusBar> statusBar; //информационная панель
    std::unique_ptr<QLabel> infoNumber;
    std::unique_ptr<QLabel> infoSign;
    bool isFloat;
    bool isPositive;
    std::stack<std::pair<QString, typeStack>> stackCalc;
    QString regNum;
    std::pair<QString, bool> memory;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    Ui::Calculator *ui;

    QPushButton* createButtonNum(const QString &); //
    QPushButton* createButtonOp(const QString& ); //
    QPushButton* createButtonEqual();
    QPushButton* createButtonUnaryOp(const QString&);
    QPushButton* createButtonClear();
    QPushButton* createButtonClearAll();
    QPushButton* createButtonBackSpace();
    QPushButton* createButtonMC();
    QPushButton* createButtonMR();
    QPushButton* createButtonMS();
    QPushButton* createButtonMplus();

    void PushNumber();
public slots:
    void slotButtonNumClicked();
    void slotButtonOpClicked();
    void slotButtonEqualClicked();
    void slotButtonUnaryOpClicked();
    void slotButtonClearClicked();
    void slotButtonClearAllClicked();
    void slotButtonBackSpace();
    void slotButtonMCClicked();
    void slotButtonMRClicked();
    void slotButtonMSClicked();
    void slotButtonMplusClicked();
};

#endif // WIDGET_H
