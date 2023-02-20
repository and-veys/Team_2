#ifndef ABOUTUS_H
#define ABOUTUS_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>


class aboutus : public QMainWindow
{
    Q_OBJECT
public:
    explicit aboutus(QWidget *parent = nullptr);

signals:

private:
    QFile file;
    QTextStream stream();

private slots:
    void slotAboutUs();
};

#endif // ABOUTUS_H
