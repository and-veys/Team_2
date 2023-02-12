#ifndef FILEFUNCTION_H
#define FILEFUNCTION_H

#include <QFileDialog>
#include <QFile>
#include <QWidget>
#include <QTextEdit>


class fileFunction
{
public:
    fileFunction();

public slots:
    void slotSaveFile(QString *textEdit);
    void slotSaveFileAs(QString *textEdit);
    void slotOpenFile(QTextEdit *textEdit);
    void slotCloseFile(QTextEdit* textEdit);
    void slotExitProgramm(QTextEdit* textEdit);
    void slotCreateNewDoc(QTextEdit* textEdit);
    void slotOpenForReadOnly_clicked(QTextEdit *textEdit);
private:
      QFile file;//Чтобы знать какой файл мы редактируем, новый или уже открытый.
};

#endif // FILEFUNCTION_H
