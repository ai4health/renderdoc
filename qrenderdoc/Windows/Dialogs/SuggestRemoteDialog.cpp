/******************************************************************************
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Baldur Karlsson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#include "SuggestRemoteDialog.h"
#include <QMenu>
#include "ui_SuggestRemoteDialog.h"

SuggestRemoteDialog::SuggestRemoteDialog(const QString &driver, const QString &machineIdent,
                                         QWidget *parent)
    : QDialog(parent), ui(new Ui::SuggestRemoteDialog)
{
  ui->setupUi(this);

  m_WarningStart = tr("This %1 capture was originally created on a\n '%2' machine.\n\n")
                       .arg(driver)
                       .arg(machineIdent.trimmed());

  ui->warning->setText(m_WarningStart +
                       tr("Currently you have no remote context selected or configured\n") +
                       tr("to replay on. Would you like to load the capture locally or\n") +
                       tr("back out to configure one in Tools > Manage Remote Servers?"));

  m_Remotes = new QMenu(this);

  ui->remote->setEnabled(false);
  ui->remote->setIcon(QIcon());
  ui->remote->setText("No Remote");

  ui->remote->setMenu(m_Remotes);
}

SuggestRemoteDialog::~SuggestRemoteDialog()
{
  delete ui;
}

void SuggestRemoteDialog::remotesAdded()
{
  if(m_Remotes->isEmpty())
    return;

  // update text and buttons to reflect that remote hosts are configured
  ui->warning->setText(m_WarningStart +
                       tr("Currently you have no remote context selected, would you like\n") +
                       tr("to choose a remote context to replay on, or continue and load\n") +
                       tr("the capture locally?"));

  ui->remote->setEnabled(true);
  ui->remote->setIcon(QIcon(QPixmap(QString::fromUtf8(":/down_arrow.png"))));
  ui->remote->setText("Remote");
}

bool SuggestRemoteDialog::alwaysReplayLocally()
{
  return ui->alwaysLocal->isChecked();
}

void SuggestRemoteDialog::on_alwaysLocal_toggled(bool checked)
{
  ui->remote->setEnabled(!m_Remotes->isEmpty() && !checked);
}

void SuggestRemoteDialog::on_remote_clicked()
{
}

void SuggestRemoteDialog::on_local_clicked()
{
  m_Choice = Local;
  accept();
}

void SuggestRemoteDialog::on_cancel_clicked()
{
  m_Choice = Cancel;
  accept();
}