#ifndef FILEFUNCTION_H
#define FILEFUNCTION_H

#include <QFileDialog>
#include <QFile>
#include <QObject>
#include <QWidget>
#include <QTextEdit>

class FileFunction : public QObject
{
    Q_OBJECT
public:
    explicit FileFunction(QObject *parent = nullptr);

    ~FileFunction(){
        delete this;
    }
signals:
    void signalFileDataReady(QString *text);//Излучаем когда прочитали данные из файла
    void signalFileCloseCompleet();//Излучаем когда закрыли файл(ну вдруг пригодится..., например чтобы текстовые поля почистить)

public slots:

    void slotOpenFile();
    void slotSaveFile(QString *data);
    void slotSaveFileAs(QString *data);
    void slotCloseFile(QString *data);
//    void slotExitProgramm(QTextEdit* textEdit);
//    void slotCreateNewDoc(QTextEdit* textEdit);
//    void slotOpenForReadOnly_clicked(QTextEdit *textEdit);

private:
      QFile file;//Чтобы знать какой файл мы редактируем, новый или уже открытый.
        QString textFromFile;
};

#endif // FILEFUNCTION_H
