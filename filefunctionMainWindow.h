#ifndef FILEFUNCTIONMAINWINDOW_H
#define FILEFUNCTIONMAINWINDOW_H
//Это надо дописать в mainwindow.h

    //Вихров
private:
    FileFunction *fileFunction;

public slots:
    void slotRcvFileData(QString *text);
    void slotSaveDocument(bool action);
    void slotCreateDocument();
    void slotCloceDocument();
signals:
    void signalSaveDocument(QString *text);
    void signalSaveDocumentAs(QString *text);
    void signalCloseDocument(QString *text);

#endif // FILEFUNCTIONMAINWINDOW_H
