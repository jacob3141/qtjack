/* This file is part of EAR, an audio processing tool.
 *
 * Copyright (C) 2011 Otto Ritter, Jacob Dawid
 * otto.ritter.or@googlemail.com
 * jacob.dawid@cybercatalyst.net
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the Affero GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the Affero GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "jackadapter.h"

#include <QFileDialog>
#include <QMessageBox>

#define MUSIC_COMBO_TEXT "Ext. Source"
#define WHITE_NOISE_COMBO_TEXT "White Noise"
#define PINK_NOISE_COMBO_TEXT "Pink Noise"
#define FILE_TYPES "*.csv"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    showMaximized();

    if(JackAdapter::instance()->connectToServer("EAR")){
        m_earProcessor = new EarProcessor();
        JackAdapter::instance()->setProcessor(m_earProcessor);
        JackAdapter::instance()->startAudioProcessing();
    } else {
        qDebug("Please start the JACK Server before launching EAR.");
        QApplication::quit();
        exit(0);
    }

    m_visualizer = new VisualizerWidget(m_earProcessor, this);
    setCentralWidget(m_visualizer);

    m_updateTimer.setInterval(1000 / 50);
    m_updateTimer.setSingleShot(false);
    m_updateTimer.start();
    connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(updateGUI()));
    connect(ui->automaticAdaption, SIGNAL(toggled(bool)),
            m_earProcessor, SLOT(setAutomaticAdaptionActive(bool)));
    connect(ui->bypass, SIGNAL(toggled(bool)),
            m_earProcessor, SLOT(setBypassActive(bool)));
    ui->signalSourceComboBox->addItem(MUSIC_COMBO_TEXT);
    ui->signalSourceComboBox->addItem(WHITE_NOISE_COMBO_TEXT);
    ui->signalSourceComboBox->addItem(PINK_NOISE_COMBO_TEXT);

    connect(ui->signalSourceComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(signalSourceChanged(QString)));
    connect(ui->resetControlsPushButton, SIGNAL(clicked()), this, SLOT(resetControls()));

    connect(ui->actionLoadLeft, SIGNAL(triggered()), this, SLOT(loadLeftEqualizer()));
    connect(ui->actionSaveLeft, SIGNAL(triggered()), this, SLOT(saveLeftEqualizer()));
    connect(ui->actionLoadRight, SIGNAL(triggered()), this, SLOT(loadRightEqualizer()));
    connect(ui->actionSaveRight, SIGNAL(triggered()), this, SLOT(saveRightEqualizer()));

    connect(ui->calibrateAction, SIGNAL(triggered()), m_earProcessor, SLOT(calibrate()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *closeEvent) {
    JackAdapter::instance()->stopAudioProcessing();
    QMainWindow::closeEvent(closeEvent);
}

void MainWindow::updateGUI() {
    ui->microphoneLeftChannelLevel->setValue(m_earProcessor->microphoneLevelLeft());
    ui->microphoneRightChannelLevel->setValue(m_earProcessor->microphoneLevelRight());
    ui->musicSourceLeftChannelLevel->setValue(m_earProcessor->signalSourceLevelLeft());
    ui->musicSourceRightChannelLevel->setValue(m_earProcessor->signalSourceLevelRight());
    ui->statusbar->showMessage(QString("Server statistics: CPU Load: %1% - Sample Rate: %2 Hz - Buffer Size: %3 Samples")
                               .arg((int)JackAdapter::instance()->cpuLoad())
                               .arg(JackAdapter::instance()->sampleRate())
                               .arg(JackAdapter::instance()->bufferSize()));
    m_visualizer->updateGL();
}

void MainWindow::signalSourceChanged(QString text) {
    if(text == MUSIC_COMBO_TEXT)
        m_earProcessor->setSignalSource(EarProcessor::ExternalSource);
    if(text == WHITE_NOISE_COMBO_TEXT)
        m_earProcessor->setSignalSource(EarProcessor::WhiteNoise);
    if(text == PINK_NOISE_COMBO_TEXT)
        m_earProcessor->setSignalSource(EarProcessor::PinkNoise);
}

void MainWindow::resetControls() {
    DigitalEqualizer *leftEqualizer = m_earProcessor->leftEqualizer();
    DigitalEqualizer *rightEqualizer = m_earProcessor->rightEqualizer();
    double *controls;

    leftEqualizer->acquireControls();
    controls = leftEqualizer->controls();
    for(int i = 0; i < 2048; i++)
        controls[i] = 0.01;
    leftEqualizer->releaseControls();

    rightEqualizer->acquireControls();
    controls = rightEqualizer->controls();
    for(int i = 0; i < 2048; i++)
        controls[i] = 0.01;
    rightEqualizer->releaseControls();
}

void MainWindow::loadLeftEqualizer() {
    QDesktopServices desktopServices;
    QString homeLocation = desktopServices.storageLocation(QDesktopServices::HomeLocation);
    QString fileName = QFileDialog::getOpenFileName(this, "Load Left Equalizer", homeLocation, FILE_TYPES);
    if(!m_earProcessor->leftEqualizer()->loadControlsFromFile(fileName)) {
        QMessageBox::warning(this, "Error Loading File", "There was an error loading the specified file.");
    }
}

void MainWindow::loadRightEqualizer() {
    QDesktopServices desktopServices;
    QString homeLocation = desktopServices.storageLocation(QDesktopServices::HomeLocation);
    QString fileName = QFileDialog::getOpenFileName(this, "Load Right Equalizer", homeLocation, FILE_TYPES);
    if(!m_earProcessor->rightEqualizer()->loadControlsFromFile(fileName)) {
        QMessageBox::warning(this, "Error Loading File", "There was an error loading the specified file.");
    }
}

void MainWindow::saveLeftEqualizer() {
    QDesktopServices desktopServices;
    QString homeLocation = desktopServices.storageLocation(QDesktopServices::HomeLocation);
    QString fileName = QFileDialog::getSaveFileName(this, "Save Left Equalizer", homeLocation, FILE_TYPES);
    if(!m_earProcessor->leftEqualizer()->saveControlsToFile(fileName)) {
        QMessageBox::warning(this, "Error Saving File", "There was an error saving the specified file.");
    }
}

void MainWindow::saveRightEqualizer() {
    QDesktopServices desktopServices;
    QString homeLocation = desktopServices.storageLocation(QDesktopServices::HomeLocation);
    QString fileName = QFileDialog::getSaveFileName(this, "Save Right Equalizer", homeLocation, FILE_TYPES);
    if(!m_earProcessor->rightEqualizer()->saveControlsToFile(fileName)) {
        QMessageBox::warning(this, "Error Saving File", "There was an error saving the specified file.");
    }
}
