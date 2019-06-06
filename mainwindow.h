#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QMessageBox>
//#include <QDesktopServices>
//#include <QUrl>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    typedef enum{next,signin,signup}login_states;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpSocket *Qclient;
    login_states login_state;
    QString m_usrname;
    QString m_password;
private slots:
    void deal_with_log();
    void succes_connected();
    void recv_message();
    void to_back();
    void clear_input();
    void send_input();
};

#endif // MAINWINDOW_H
