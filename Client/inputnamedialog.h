#ifndef INPUTNAMEDIALOG_H
#define INPUTNAMEDIALOG_H

#include <QDialog>

class QLineEdit;

class InputNameDialog : public QDialog
{
    Q_OBJECT


private:
    QLineEdit * txtName;

public:
    explicit InputNameDialog(QWidget *parent = 0);

    QString name() const;

signals:

public slots:

};

#endif // INPUTNAMEDIALOG_H
