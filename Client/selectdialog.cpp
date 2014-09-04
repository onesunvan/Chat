#include "selectdialog.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

SelectDialog::SelectDialog(QString message, QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
    QLabel * lblMessage = new QLabel(message);

    QPushButton * yesBtn = new QPushButton("&Yes");
    QPushButton* noBtn = new QPushButton("&No");

    connect(yesBtn, SIGNAL(clicked()), this, SLOT(accept()));
    connect(noBtn, SIGNAL(clicked()), this, SLOT(reject()));

    QVBoxLayout * vbxLayout = new QVBoxLayout();
    vbxLayout->addWidget(lblMessage);

    QHBoxLayout * hbxLayout = new QHBoxLayout();
    hbxLayout->addWidget(yesBtn);
    hbxLayout->addWidget(noBtn);

    vbxLayout->addLayout(hbxLayout);
    setLayout(vbxLayout);
}
