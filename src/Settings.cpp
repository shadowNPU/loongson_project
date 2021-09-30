#include "Settings.h"
#include "ui_Settings.h"
#include <QtCore/QDebug>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMessageBox>


Settings::Settings(QWidget *parent) : QWidget(parent), ui(new Ui::Settings) {
    ui->setupUi(this);
    this->setWindowTitle("设置");

    ui->paraConfirmPushButton->setStyleSheet("QPushButton{background-color: rgb(240, 255, 240);border:2px groove gray;border-radius:12px;padding:2px 4px;border-style: outset;}"
                                             "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                             "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->addUserButton->setStyleSheet("QPushButton{background-color: rgb(211, 211, 211);border:2px groove gray;border-radius:5px;padding:2px 4px;border-style: outset;}"
                                     "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                     "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->deleteUserInfoButton->setStyleSheet("QPushButton{background-color: rgb(211, 211, 211);border:2px groove gray;border-radius:5px;padding:2px 4px;border-style: outset;}"
                                            "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                            "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->warningSettingButton->setStyleSheet("QPushButton{background-color: rgb(248, 248, 255);border:2px groove gray;border-radius:5px;padding:2px 4px;border-style: outset;}"
                                            "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                            "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->historyDataButton->setStyleSheet("QPushButton{background-color: rgb(248, 248, 255);border:2px groove gray;border-radius:5px;padding:2px 4px;border-style: outset;}"
                                         "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                         "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->settingConfirmButton->setStyleSheet("QPushButton{background-color: rgb(240, 255, 240);border:2px groove gray;border-radius:12px;padding:2px 4px;border-style: outset;}"
                                            "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                            "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->softwareInfoButton->setStyleSheet("QPushButton{background-color: rgb(248, 248, 255);border:2px groove gray;border-radius:5px;padding:2px 4px;border-style: outset;}"
                                          "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                          "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->parameterSettingButton->setStyleSheet("QPushButton{background-color: rgb(248, 248, 255);border:2px groove gray;border-radius:5px;padding:2px 4px;border-style: outset;}"
                                              "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                              "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");


    connect(ui->historyDataButton, &QPushButton::clicked, [this](bool checked) {
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->parameterSettingButton, &QPushButton::clicked, [this](bool checked) {
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->warningSettingButton, &QPushButton::clicked, [this](bool checked) {
        ui->stackedWidget->setCurrentIndex(2);
    });
    connect(ui->softwareInfoButton, &QPushButton::clicked, [this](bool checked) {
        ui->stackedWidget->setCurrentIndex(3);
    });

    // 用户
    // emit onParamsChanged(20);

    connect(ui->addUserButton, &QPushButton::clicked, [this](auto) {
        auto userName = ui->addUserInfoTextEdit->toPlainText();
        users.insert(userName);
        for (auto &name : users) {
            ui->currentUserInfoTextBrowser->append(name);
        }
    });

    connect(ui->deleteUserInfoButton, &QPushButton::clicked, [this](auto) {
        auto userName = ui->deleteUserInfoTextEdit->toPlainText();
        users.erase(userName);
        ui->currentUserInfoTextBrowser->clear();
        for (auto &name : users) {
            ui->currentUserInfoTextBrowser->append(name);
        }
    });

    connect(ui->paraConfirmPushButton, &QPushButton::clicked, [this](auto) {
        auto closingSize = ui->closingSizeTextEdit->toPlainText();
        auto gaussSize = ui->gaussSizeTextEdit->toPlainText();
        auto threshold = ui->thresholdTextEdit->toPlainText();
        auto maxAera = ui->maxAeraTextEdit->toPlainText();
        auto minAera = ui->minAeraTextEdit->toPlainText();
        auto rate = ui->rateTextEdit->toPlainText();

        emit paramsChanged(closingSize, gaussSize, threshold, rate, maxAera, minAera);
    });
}

Settings::~Settings() {
    delete ui;
}
