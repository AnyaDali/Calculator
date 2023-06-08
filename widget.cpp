
#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent),
    layout(std::make_unique<QGridLayout>()),
    statusBar(std::make_unique<QStatusBar>()),
    infoNumber(std::make_unique<QLabel>("0")),
    infoSign(std::make_unique<QLabel>()), isFloat(false),
    regNum(""), memory(),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setMaximumSize(300, 400);

    setMinimumSize(470, 400);
    // установка панели
    infoNumber->setAlignment(Qt::AlignRight);
    infoNumber->setStyleSheet("QLabel{font-size: 30px;}");
    infoSign->setMaximumWidth(20);
    infoSign->setStyleSheet("QLabel{font-size: 30px;}");
    statusBar->addWidget(infoSign.get(), Qt::AlignLeft);
    statusBar->addWidget(infoNumber.get(), Qt::AlignAbsolute);

    statusBar->setStyleSheet("QStatusBar{background-color: white; border: 1px solid #000; }");
    layout->addWidget(statusBar.get(), 0, 0, 6, 0, Qt::AlignmentFlag::AlignTop);

    QPushButton* btnClear =  createButtonClear();
    QPushButton* btnClearAll = createButtonClearAll();
    QPushButton* btnBackSpace = createButtonBackSpace();

    QPushButton* btnMS = createButtonMS();
    QPushButton* btnMR = createButtonMR();
    QPushButton* btnMC = createButtonMC();
    QPushButton* btnMplus = createButtonMplus();
    layout->addWidget(btnBackSpace, 1, 1, 1, 2);
    layout->addWidget(btnClear, 1, 3, 1, 2);
    layout->addWidget(btnClearAll, 1, 5, 1, 2);
    layout->addWidget(btnMC, 2, 1);
    layout->addWidget(btnMR, 3, 1);
    layout->addWidget(btnMS, 4, 1);
    layout->addWidget(btnMplus, 5, 1);

    // установка кнопок
    QPushButton* btn0 = createButtonNum("0");
    QPushButton* btn1 = createButtonNum("1");
    QPushButton* btn2 = createButtonNum("2");
    QPushButton* btn3 = createButtonNum("3");
    QPushButton* btn4 = createButtonNum("4");
    QPushButton* btn5 = createButtonNum("5");
    QPushButton* btn6 = createButtonNum("6");
    QPushButton* btn7 = createButtonNum("7");
    QPushButton* btn8 = createButtonNum("8");
    QPushButton* btn9 = createButtonNum("9");
    QPushButton* btnDot = createButtonNum(".");
    QPushButton* btnSign = createButtonNum("+-");
    layout->addWidget(btn7, 2, 2);
    layout->addWidget(btn8, 2, 3);
    layout->addWidget(btn9, 2, 4);
    layout->addWidget(btn4, 3, 2);
    layout->addWidget(btn5, 3, 3);
    layout->addWidget(btn6, 3, 4);
    layout->addWidget(btn1, 4, 2);
    layout->addWidget(btn2, 4, 3);
    layout->addWidget(btn3, 4, 4);
    layout->addWidget(btn0, 5, 2);
    layout->addWidget(btnDot, 5, 3);
    layout->addWidget(btnSign, 5, 4);
    QPushButton* btnDiv = createButtonOp("/");
    QPushButton* btnMult = createButtonOp("*");
    QPushButton* btnMinus = createButtonOp("-");
    QPushButton* btnPlus = createButtonOp("+");
    layout->addWidget(btnDiv, 2, 5);
    layout->addWidget(btnMult, 3, 5);
    layout->addWidget(btnMinus, 4, 5);
    layout->addWidget(btnPlus, 5, 5);

    QPushButton* btnSqrt = createButtonUnaryOp("Sqrt");
    QPushButton* btnSquare = createButtonUnaryOp("x^2");
    QPushButton* btnObr = createButtonUnaryOp("1/x");
    QPushButton* btnEq = createButtonEqual();
    layout->addWidget(btnSqrt, 2, 6);
    layout->addWidget(btnSquare, 3, 6);
    layout->addWidget(btnObr, 4, 6);
    layout->addWidget(btnEq, 5, 6);

    setLayout(layout.get());
}

QPushButton* Widget::createButtonNum(const QString & nameButton) {
    QPushButton* pcmd = new QPushButton{nameButton};
    pcmd->setMinimumSize(70, 50);
    connect(pcmd, SIGNAL(clicked()), this, SLOT(slotButtonNumClicked()));
    return pcmd;
}

QPushButton* Widget::createButtonOp(const QString& nameOp) {
    QPushButton* pcmd = new QPushButton{nameOp};
    pcmd->setMinimumSize(70, 50);
    connect(pcmd, SIGNAL(clicked()), this, SLOT(slotButtonOpClicked()));
    return pcmd;
}

QPushButton* Widget::createButtonEqual() {
    QPushButton* pcmd = new QPushButton{"="};
    pcmd->setMinimumSize(70, 50);
    connect(pcmd, SIGNAL(clicked()), this, SLOT(slotButtonEqualClicked()));
    return pcmd;
}

QPushButton* Widget::createButtonUnaryOp(const QString& name) {
    QPushButton* pcmd = new QPushButton{name};
    pcmd->setMinimumSize(70, 50);
    connect(pcmd, SIGNAL(clicked()), this, SLOT(slotButtonUnaryOpClicked()));
    return pcmd;
}

void Widget::PushNumber() {
    if (!regNum.isEmpty()) { // поместить число в стек
        stackCalc.emplace(std::move(regNum), infoSign->text() == "-" ? typeStack::NEG_NUM : typeStack::POS_NUM);
        infoSign->setText("");
        regNum = "";
        isFloat = false;
    }
}

void Widget::slotButtonUnaryOpClicked() {
    PushNumber();
    if (stackCalc.empty()) {
        return;
    }
    QPushButton* pcmd = reinterpret_cast<QPushButton*>(sender());

    auto [valStr, type] = stackCalc.top();
    stackCalc.pop();
    double val = valStr.toDouble();
    if (type == typeStack::NEG_NUM) {
        val = -val;
    }
    if (pcmd->text() == "Sqrt") {
        if (val < 0) {
            infoNumber->setText("Неверный ввод");
            regNum = "0";
            isFloat = false;
            infoSign->setText("");
            while (!stackCalc.empty()) {
                stackCalc.pop();
            }
            return;
        }
        stackCalc.emplace(QString::number(std::sqrt(val)), typeStack::POS_NUM);
    } else if (pcmd->text() == "x^2") {
        stackCalc.emplace(QString::number(val * val), typeStack::POS_NUM);
    } else if (pcmd->text() == "1/x") {
        if (std::abs(val) < 0) {
            infoNumber->setText("Деление на ноль невозможно");
            regNum = "";
            isFloat = false;
            infoSign->setText("");
            while (!stackCalc.empty()) {
                stackCalc.pop();
            }
            return;
        }
        double res = 1.0 / val;
        stackCalc.emplace(QString::number(std::abs(res)), res < 0 ?  typeStack::NEG_NUM : typeStack::POS_NUM);
    }
    infoNumber->setText(stackCalc.top().first);
    infoSign->setText(stackCalc.top().second == typeStack::NEG_NUM ? "-" : "");

}

void Widget::slotButtonNumClicked() {
    QPushButton* pcmd = reinterpret_cast<QPushButton*>(sender());
    if (!stackCalc.empty() && stackCalc.top().second != typeStack::OPERATION) {
        while(!stackCalc.empty()) { // очистить стек
            stackCalc.pop();
        }
    }

    if (pcmd->text() != "+-" && pcmd->text() != ".") {
        if (regNum != "0") {
            regNum += pcmd->text();
        } else {
            regNum = pcmd->text();
        }
    } else if (pcmd->text() == "." && !isFloat) {
        isFloat = true;
        if (regNum.isEmpty()) {
            regNum = "0";
        }
        regNum += pcmd->text();
    } else if (pcmd->text() == "+-") {
        if (infoSign->text() == "-") {
            infoSign->setText(QString{});
        } else {
            infoSign->setText("-");
        }
    }
    infoNumber->setText(regNum);

}

void Widget::slotButtonOpClicked() {
    PushNumber();
    if (stackCalc.empty()) {
        return;
    }
    QPushButton* pcmd = reinterpret_cast<QPushButton*>(sender());
    if (auto [val, type] = stackCalc.top(); type == typeStack::OPERATION) {
        stackCalc.pop();
    } else if (stackCalc.size() == 3) {
        slotButtonEqualClicked();
    }
    stackCalc.emplace(pcmd->text(), typeStack::OPERATION);
}

void Widget::slotButtonEqualClicked() {
    PushNumber();
    if (stackCalc.size() == 3) {
        auto [secondOperand, typeSecond] = stackCalc.top();
        stackCalc.pop();
        auto [operatorCalc, typeOp] = stackCalc.top();
        stackCalc.pop();
        auto [firstOperand, typeFirst] = stackCalc.top();
        stackCalc.pop();
        double fOp = firstOperand.toDouble();
        if (typeFirst == typeStack::NEG_NUM) {
            fOp = -fOp;
        }
        double sOp = secondOperand.toDouble();
        if (typeSecond == typeStack::NEG_NUM) {
            sOp = -sOp;
        }
        if (operatorCalc == "+") {
            double res = fOp + sOp;
            stackCalc.emplace(QString::number(std::abs(res)), res < 0 ? typeStack::NEG_NUM : typeStack::POS_NUM);
        } else if (operatorCalc == "-") {
            double res = fOp - sOp;
            stackCalc.emplace(QString::number(std::abs(res)), res < 0 ? typeStack::NEG_NUM : typeStack::POS_NUM);
        } else if (operatorCalc == "/") {
            if (std::abs(sOp) < EPS) {
                infoNumber->setText("Деление на ноль невозможно");
                regNum = "";
                isFloat = false;
                infoSign->setText("");
                while (!stackCalc.empty()) {
                    stackCalc.pop();
                }
                return;
            } else {
                double res = fOp / sOp;
                stackCalc.emplace(QString::number(std::abs(res)), res < 0 ? typeStack::NEG_NUM : typeStack::POS_NUM);
            }
        } else if (operatorCalc == "*") {
            double res = fOp * sOp;
            stackCalc.emplace(QString::number(std::abs(res)), res < 0 ? typeStack::NEG_NUM : typeStack::POS_NUM);
        }

        infoNumber->setText(stackCalc.top().first);
        infoSign->setText(stackCalc.top().second == typeStack::NEG_NUM ? "-" : "");

    }
}

QPushButton* Widget::createButtonClear() {
    QPushButton* pcmd = new QPushButton{"Clear"};
    pcmd->setMinimumSize(140, 50);
    connect(pcmd, SIGNAL(clicked()), this, SLOT(slotButtonClearClicked()));
    return pcmd;
}

QPushButton* Widget::createButtonClearAll() {
    QPushButton* pcmd = new QPushButton{"Clear All"};
    pcmd->setMinimumSize(140, 50);
    connect(pcmd, SIGNAL(clicked()), this, SLOT(slotButtonClearAllClicked()));
    return pcmd;
}

QPushButton* Widget::createButtonBackSpace() {
    QPushButton* pcmd = new QPushButton{"BackSpace"};
    pcmd->setMinimumSize(140, 50);
    connect(pcmd, SIGNAL(clicked()), this, SLOT(slotButtonBackSpace()));
    return pcmd;
}

QPushButton* Widget::createButtonMC() {
    QPushButton* pcmd = new QPushButton{"MC"};
    pcmd->setMinimumSize(70, 50);
    connect(pcmd, SIGNAL(clicked()), this, SLOT(slotButtonMCClicked()));
    return pcmd;
}

QPushButton* Widget::createButtonMR() {
    QPushButton* pcmd = new QPushButton{"MR"};
    pcmd->setMinimumSize(70, 50);
    connect(pcmd, SIGNAL(clicked()), this, SLOT(slotButtonMRClicked()));
    return pcmd;
}

QPushButton* Widget::createButtonMS() {
    QPushButton* pcmd = new QPushButton{"MS"};
    pcmd->setMinimumSize(70, 50);
    connect(pcmd, SIGNAL(clicked()), this, SLOT(slotButtonMSClicked()));
    return pcmd;
}

QPushButton* Widget::createButtonMplus() {
    QPushButton* pcmd = new QPushButton{"M+"};
    pcmd->setMinimumSize(70, 50);
    connect(pcmd, SIGNAL(clicked()), this, SLOT(slotButtonMplusClicked()));
    return pcmd;
}


void Widget::slotButtonMCClicked() {
    memory.first = "";
}

void Widget::slotButtonMRClicked() {
    if (!memory.first.isEmpty()) { // если не пусто
        infoNumber->setText(memory.first);
        infoSign->setText(memory.second ? "" : "-");
        regNum = memory.first;
    }
}

// запомнить число на дисплее
void Widget::slotButtonMSClicked() {
    memory = std::make_pair(infoNumber->text(), infoSign->text() == "-" ? false : true);
    qDebug() << memory.first;
}

void Widget::slotButtonMplusClicked() {
    if (memory.first.isEmpty()) {
        memory = std::make_pair(infoNumber->text(), infoSign->text() == "-" ? false : true);
    } else {
        double x = memory.first.toDouble();
        if (!memory.second) {
            x = -x;
        }
        // считывается число с дисплея
        double y = infoNumber->text().toDouble();
        if (infoSign->text() == "-") {
            y = -y;
        }
        x += y;
        memory = std::make_pair(QString::number(std::abs(x)), x < 0 ? false : true);
    }
}

void Widget::slotButtonClearClicked() {
    infoNumber->setText("0");
    infoSign->setText("");
    regNum = "";
}

void Widget::slotButtonClearAllClicked() {
    while(!stackCalc.empty()) {
        stackCalc.pop();
    }
    infoNumber->setText("0");
    infoSign->setText("");
    regNum = "";
}

void Widget::slotButtonBackSpace() {
    if (regNum != "0") {
        regNum.remove(regNum.size() - 1, regNum.size());
        if (regNum.isEmpty()) {
            regNum = "0";
        }
        infoNumber->setText(regNum);
    }
}

Widget::~Widget()
{
    delete ui;
}


