#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->groupBox_password->setVisible(false);
    ui->pushButton_back->setVisible(false);
    ui->textEdit_input->setReadOnly(true);
    login_state = next;
    m_usrname="";
    m_password="";
    Qclient = new QTcpSocket();

    Qclient->connectToHost(QHostAddress("129.211.90.130"),1234);
    if(!Qclient->waitForConnected(10000))
    {
        ui->textBrowser_info->append("failed to connect");
    }

    connect(ui->pushButton_next,SIGNAL(clicked()),this,SLOT(deal_with_log()));
    connect(Qclient,SIGNAL(connected()),this,SLOT(succes_connected()));
    connect(Qclient,SIGNAL(readyRead()),this,SLOT(recv_message()));
    connect(ui->pushButton_back,SIGNAL(clicked()),this,SLOT(to_back()));
    connect(ui->pushButton_clear,SIGNAL(clicked()),this,SLOT(clear_input()));
    connect(ui->pushButton_send,SIGNAL(clicked()),this,SLOT(send_input()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::deal_with_log(){
    if(login_state==next){
        //如果按钮状态为下一步
        QString usrname=ui->lineEdit_usrname->text();
        Qclient->write(usrname.toUtf8());
        m_usrname = usrname;
    }

    else if(login_state==signup){
        //如果按钮状态为注册
        QString password = ui->lineEdit_password->text();
        Qclient->write(password.toUtf8());
    }

    else{
        //如果按钮状态为登录
        QString password = ui->lineEdit_password->text();
        Qclient->write(password.toUtf8());
    }
}

void MainWindow::succes_connected(){
    ui->textBrowser_info->append("成功与服务端建立连接!");
}

void MainWindow::recv_message(){
    QByteArray array = Qclient->readAll();
    QString recv = array;

    if(QString::compare(recv,"enter-the-sign-up")==0){
        ui->groupBox_password->setVisible(true);
        ui->pushButton_next->setText("注册");
        //ui->pushButton_back->setVisible(true);
        ui->textBrowser_info->append("用户名不存在，请输入密码进行注册");
        login_state = signup;
    }

    else if(QString::compare(recv,"enter-the-sign-in")==0){
        ui->groupBox_password->setVisible(true);
        ui->pushButton_next->setText("登录");
        //ui->pushButton_back->setVisible(true);
        ui->textBrowser_info->append("用户名有效，请输入密码进行登录");
        login_state = signin;
    }

    else if(QString::compare(recv,"enter-the-chatting-room")==0){
        ui->groupBox_login->setVisible(false);
        ui->textEdit_input->setReadOnly(false);
        ui->textBrowser_info->append("-----------------");
        ui->textBrowser_info->append("登录成功，进入聊天室");
        ui->textBrowser_info->append("-----------------");
    }

    else if(QString::compare(recv,"the-password-is-wrong")==0){
        ui->textBrowser_info->append("密码错误，请重新输入");
        ui->lineEdit_password->clear();
    }

    else if((QString::compare(recv,"enter-the-chatting-room-by-signup")==0)){
        ui->groupBox_login->setVisible(false);
        ui->textEdit_input->setReadOnly(false);
        ui->textBrowser_info->append("注册成功，即将自动登录进入聊天室");
        ui->textBrowser_info->append("-----------------");
        ui->textBrowser_info->append("登录成功，进入聊天室");
        ui->textBrowser_info->append("-----------------");
    }
    else{
        ui->textBrowser_info->append(recv);
    }
}

void MainWindow::to_back(){
    ui->pushButton_next->setText("下一步");
    ui->pushButton_back->setVisible(false);
    ui->lineEdit_password->clear();
    ui->groupBox_password->setVisible(false);
}

void MainWindow::clear_input(){
    ui->textEdit_input->clear();
}

void MainWindow::send_input(){
    QString text = ui->textEdit_input->toPlainText();
    QString text_send = m_usrname;
    text_send.append(":");
    text_send.append(text);
    if(text.length())
        Qclient->write(text_send.toUtf8());//若长度不为0，发送到服务端
}
