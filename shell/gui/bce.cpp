/*
 *	Copyright (C) 2012-2013, The BCE Project authors. All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions
 *	are met:
 *	1. Redistributions of source code must retain the above copyright
 *	   notice, this list of conditions and the following disclaimer.
 *	2. Redistributions in binary form must reproduce the above copyright
 *	   notice, this list of conditions and the following disclaimer in the
 *	   documentation and/or other materials provided with the distribution.
 *
 *	THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 *	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *	ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 *	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 *	OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *	HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *	OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *	SUCH DAMAGE.
*/

#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <exception>
#include <QSettings>
#include <QMessageBox>
#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QTranslator>
#include <common/exception.hpp>
#include <math/integer.hpp>
#include <balancer.hpp>
#include "bce.hpp"
#include "ui_bce.h"
#include "setup.hpp"
#include "variable.hpp"
#include "../../version.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow) {
	QSettings settings(BCE_SETTINGS_ROOT, BCE_SETTINGS_WINDOW);
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("state").toByteArray(), BCE_REVISION);
	ui->setupUi(this);
	ui->txtEquation->clear();
	ui->txtResult->clear();
	setWindowTitle(MainWindow::tr("BCE++(TM) Chemical Equation Balancer"));
	ui->lblMessage_1->setText(QLabel::tr("Chemical Equation:"));
	ui->lblMessage_2->setText(QLabel::tr("Result:"));
	ui->cmdLoadFile->setText(QPushButton::tr("Load File(&L)"));
	ui->cmdBalance->setText(QPushButton::tr("Balance(&B)"));
	ui->cmdExit->setText(QPushButton::tr("Exit(&E)"));
}

MainWindow::~MainWindow() {
	delete ui;
}

void removeSpaces(string &dest) {
	string r;
	string::iterator it;

	r.clear();

	for (it = dest.begin(); it != dest.end(); it++) {
		if (*it == BCE_DESCRIPTOR) {
			break;
		}
		if (!isspace(*it)) {
			r.insert(r.end(), *it);
		}
	}

	dest = r;
}

void MainWindow::closeEvent(QCloseEvent *event) {
	event->accept();
	QSettings settings(BCE_SETTINGS_ROOT, BCE_SETTINGS_WINDOW);
	settings.setValue("geometry", saveGeometry());
	settings.setValue("state", saveState(BCE_REVISION));
}

void MainWindow::on_cmdBalance_clicked() {
	string input, output;
	input = ui->txtEquation->text().toStdString();
	removeSpaces(input);
	if (input.length() == 0) {
		QMessageBox::critical(this, QMessageBox::tr("Error"), QMessageBox::tr("Please type a valid chemical equation."), QMessageBox::Ok);
		ui->txtEquation->setFocus();
		return;
	}
	ui->cmdBalance->setEnabled(false);
	ui->cmdLoadFile->setEnabled(false);
	if (balancerAuto(input, output) == false) {

		ui->cmdBalance->setEnabled(true);
		ui->cmdLoadFile->setEnabled(true);
		QMessageBox::critical(this, QMessageBox::tr("Error"), QMessageBox::tr("Can't balance this chemical equation."), QMessageBox::Ok);
		ui->txtEquation->setFocus();
		return;
	}
	ui->cmdBalance->setEnabled(true);
	ui->cmdLoadFile->setEnabled(true);
	ui->txtResult->clear();
	ui->txtResult->insertPlainText(QString(output.c_str()));
	ui->cmdBalance->setFocus();
}

void MainWindow::on_cmdExit_clicked() {
	close();
}

void MainWindow::on_cmdLoadFile_clicked() {
	QMessageBox errmsg;
	string input, output;
	QString path = QFileDialog::getOpenFileName(this,  QFileDialog::tr("Open"), "", QFileDialog::tr("Text Files(*.txt);;Test cases(*.in);;All Files(*)"));
	ifstream file;
	ui->cmdBalance->setEnabled(false);
	ui->cmdLoadFile->setEnabled(false);
	if (path.length() != 0) {
		try {
			try {
				ui->txtResult->clear();
				file.open(path.toStdString().c_str());
				while (file.eof() == false) {
					getline(file, input);
					removeSpaces(input);
					if (input.length() == 0) {
						continue;
					}
					if (balancerAuto(input, output) == true) {
						ui->txtResult->insertPlainText(QString((output + "\n").c_str()));
					} else {
						output = BCE_CANNOT_BALANCE;
						ui->txtResult->insertPlainText(QString((output + "\n").c_str()));
					}
				}
				file.close();
			} catch(xsvException &error) {
				errmsg.setWindowTitle(QMessageBox::tr("Error"));
				errmsg.setText(QMessageBox::tr("Error while balancing chemical equations from file."));
				errmsg.setIcon(QMessageBox::Critical);
				errmsg.setInformativeText(QMessageBox::tr("Please contact us and show this message."));
				errmsg.setDetailedText((QMessageBox::tr("ID: %1\nFile: %2\nLine: %3\nFunction: %4\nDescription: %5")).arg(QString(integer(error.getID()).toString().c_str()), QString(error.getFile().c_str()), QString(integer(error.getLine()).toString().c_str()), QString(error.getFunction().c_str()), QString(error.getDescription().c_str())));
				errmsg.exec();
			}
		} catch(exception &error2) {
			errmsg.setWindowTitle(QMessageBox::tr("Error"));
			errmsg.setText(QMessageBox::tr("Error while balancing chemical equations from file."));
			errmsg.setIcon(QMessageBox::Critical);
			errmsg.setInformativeText(QMessageBox::tr("Please contact us and show this message."));
			errmsg.setDetailedText(QString(string(error2.what()).c_str()));
			errmsg.exec();
		}
	}
	ui->cmdBalance->setEnabled(true);
	ui->cmdLoadFile->setEnabled(true);
	ui->cmdLoadFile->setFocus();
}

