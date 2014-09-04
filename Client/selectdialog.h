#ifndef SELECTDIALOG_H
#define SELECTDIALOG_H

#include <QDialog>

class SelectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SelectDialog(QString message, QWidget *parent = 0);

signals:

public slots:

};

#endif // SELECTDIALOG_H
