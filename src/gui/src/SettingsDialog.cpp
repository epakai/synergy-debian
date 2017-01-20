/*
 * synergy -- mouse and keyboard sharing utility
 * Copyright (C) 2012-2016 Symless Ltd.
 * Copyright (C) 2008 Volker Lanz (vl@fidra.de)
 * 
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file LICENSE that should have accompanied this file.
 * 
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SettingsDialog.h"

#include "CoreInterface.h"
#include "SynergyLocale.h"
#include "QSynergyApplication.h"
#include "QUtility.h"
#include "AppConfig.h"
#include "EditionType.h"
#include "SslCertificate.h"
#include "MainWindow.h"

#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

static const char networkSecurity[] = "ns";

SettingsDialog::SettingsDialog(QWidget* parent, AppConfig& config) :
	QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
	Ui::SettingsDialogBase(),
       m_appConfig(config)
{
	setupUi(this);

	m_Locale.fillLanguageComboBox(m_pComboLanguage);

	m_pLineEditScreenName->setText(appConfig().screenName());
	m_pSpinBoxPort->setValue(appConfig().port());
	m_pLineEditInterface->setText(appConfig().interface());
	m_pComboLogLevel->setCurrentIndex(appConfig().logLevel());
	m_pCheckBoxLogToFile->setChecked(appConfig().logToFile());
	m_pLineEditLogFilename->setText(appConfig().logFilename());
	setIndexFromItemData(m_pComboLanguage, appConfig().language());
	m_pCheckBoxAutoHide->setChecked(appConfig().getAutoHide());

#if defined(Q_OS_WIN)
       m_pComboElevate->setCurrentIndex(static_cast<int>(appConfig().elevateMode()));

	m_pCheckBoxAutoHide->hide();
#else
	// elevate checkbox is only useful on ms windows.
	m_pLabelElevate->hide();
	m_pComboElevate->hide();
#endif

	m_pCheckBoxEnableCrypto->setChecked(m_appConfig.getCryptoEnabled());
	m_pCheckBoxEnableCrypto->setEnabled(m_appConfig.edition() == kPro);
}

void SettingsDialog::accept()
{
	appConfig().setScreenName(m_pLineEditScreenName->text());
	appConfig().setPort(m_pSpinBoxPort->value());
	appConfig().setInterface(m_pLineEditInterface->text());
	appConfig().setLogLevel(m_pComboLogLevel->currentIndex());
	appConfig().setLogToFile(m_pCheckBoxLogToFile->isChecked());
	appConfig().setLogFilename(m_pLineEditLogFilename->text());
	appConfig().setLanguage(m_pComboLanguage->itemData(m_pComboLanguage->currentIndex()).toString());
       appConfig().setElevateMode(static_cast<ElevateMode>(m_pComboElevate->currentIndex()));
	appConfig().setAutoHide(m_pCheckBoxAutoHide->isChecked());
	appConfig().saveSettings();
	QDialog::accept();
}

void SettingsDialog::reject()
{
	if (appConfig().language() != m_pComboLanguage->itemData(m_pComboLanguage->currentIndex()).toString()) {
		QSynergyApplication::getInstance()->switchTranslator(appConfig().language());
	}

	QDialog::reject();
}

void SettingsDialog::changeEvent(QEvent* event)
{
	if (event != 0)
	{
		switch (event->type())
		{
		case QEvent::LanguageChange:
			{
				int logLevelIndex = m_pComboLogLevel->currentIndex();

				m_pComboLanguage->blockSignals(true);
				retranslateUi(this);
				m_pComboLanguage->blockSignals(false);

				m_pComboLogLevel->setCurrentIndex(logLevelIndex);
				break;
			}

		default:
			QDialog::changeEvent(event);
		}
	}
}

void SettingsDialog::on_m_pCheckBoxLogToFile_stateChanged(int i)
{
	bool checked = i == 2;

	m_pLineEditLogFilename->setEnabled(checked);
	m_pButtonBrowseLog->setEnabled(checked);
}

void SettingsDialog::on_m_pButtonBrowseLog_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(
		this, tr("Save log file to..."),
		m_pLineEditLogFilename->text(),
		"Logs (*.log *.txt)");

	if (!fileName.isEmpty())
	{
		m_pLineEditLogFilename->setText(fileName);
	}
}

void SettingsDialog::on_m_pComboLanguage_currentIndexChanged(int index)
{
	QString ietfCode = m_pComboLanguage->itemData(index).toString();
	QSynergyApplication::getInstance()->switchTranslator(ietfCode);
}

void SettingsDialog::on_m_pCheckBoxEnableCrypto_toggled(bool checked)
{
	m_appConfig.setCryptoEnabled(checked);
	m_appConfig.saveSettings();
	if (checked) {
		SslCertificate sslCertificate;
		sslCertificate.generateCertificate();
		MainWindow& mainWindow = dynamic_cast<MainWindow&> (*this->parent());
		mainWindow.updateLocalFingerprint();
	}
}
