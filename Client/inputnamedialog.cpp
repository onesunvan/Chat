#include "inputnamedialog.h"
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

InputNameDialog::InputNameDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
    txtName = new QLineEdit();

    QLabel * lblName = new QLabel("&Name");

    lblName->setBuddy(txtName);

    QPushButton * okBtn = new QPushButton("&Ok");
    QPushButton* cancelBtn = new QPushButton("&Cancel");

    connect(okBtn, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));

    QVBoxLayout * vbxLayout = new QVBoxLayout();
    vbxLayout->addWidget(lblName);
    vbxLayout->addWidget(txtName);

    QHBoxLayout * hbxLayout = new QHBoxLayout();
    hbxLayout->addWidget(okBtn);
    hbxLayout->addWidget(cancelBtn);

    vbxLayout->addLayout(hbxLayout);
    setLayout(vbxLayout);
}

QString InputNameDialog::name() const
{
    return txtName->text();
}
