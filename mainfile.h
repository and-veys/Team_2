#ifndef MAINFILE_H
#define MAINFILE_H

#include <QFile>
#include <QObject>

class MainWindow;

class MainFile : public QFile
{
    Q_OBJECT
public:
    MainFile();
    ~MainFile();
    bool create(QWidget * wnd, bool save);
    bool save(const QString & data);
    bool load(QString & data);
signals:
    void sendWindowCaption(QString);
    void sendMessage(QString);

};

#endif // MAINFILE_H
