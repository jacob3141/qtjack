///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    This file is part of QJackAudio.                                       //
//    Copyright (C) 2014 Jacob Dawid, jacob@omg-it.works                     //
//                                                                           //
//    QJackAudio is free software: you can redistribute it and/or modify     //
//    it under the terms of the GNU General Public License as published by   //
//    the Free Software Foundation, either version 3 of the License, or      //
//    (at your option) any later version.                                    //
//                                                                           //
//    QJackAudio is distributed in the hope that it will be useful,          //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of         //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
//    GNU General Public License for more details.                           //
//                                                                           //
//    You should have received a copy of the GNU General Public License      //
//    along with QJackAudio. If not, see <http://www.gnu.org/licenses/>.     //
//                                                                           //
//    It is possible to obtain a closed-source license of QJackAudio.        //
//    If you're interested, contact me at: jacob@omg-it.works                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// Own includes
#include "channelwidget.h"
#include "ui_channelwidget.h"

// QJackAudio includes
#include <QSampleBuffer>
#include <QUnits>

ChannelWidget::ChannelWidget(int channelNumber, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelWidget)
{
    ui->setupUi(this);
    ui->channelNumberLabel->setText(QString("%1").arg(channelNumber));

    _channelIn = QJackClient::instance()->registerAudioInPort(QString("ch%1_in").arg(channelNumber));
    _auxSend = QJackClient::instance()->registerAudioOutPort(QString("ch%1_aux_send").arg(channelNumber));
    _auxReturn = QJackClient::instance()->registerAudioInPort(QString("ch%1_aux_ret").arg(channelNumber));
    _channelOut = QJackClient::instance()->registerAudioOutPort(QString("ch%1_out").arg(channelNumber));

    _inputStage = new QAmplifier();
    _inputStage->setGain(ui->gainDial->value());
    _faderStage = new QAmplifier();
    _faderStage->setGain(ui->volumeVerticalSlider->value());
    _auxPre = new QAmplifier();
    _auxPre->setGain(ui->auxSendDial->value());
    _auxPost = new QAmplifier();
    _auxPost->setGain(ui->auxReturnDial->value());
    _equalizer = new QEqualizer(64, 32);

    connect(ui->gainDial, SIGNAL(valueChanged(int)), _inputStage, SLOT(setGain(int)));
    connect(ui->volumeVerticalSlider, SIGNAL(valueChanged(int)), _faderStage, SLOT(setGain(int)));
}

ChannelWidget::~ChannelWidget()
{
    delete ui;
}

void ChannelWidget::process(QSampleBuffer targetSampleBuffer)
{
    QSampleBuffer inputSampleBuffer = _channelIn->sampleBuffer();
    inputSampleBuffer.copyTo(targetSampleBuffer);

    _inputStage->process(targetSampleBuffer);

    if(ui->equalizerOnPushButton->isChecked()) {
        _equalizer->process(targetSampleBuffer);
    }

    if(ui->auxOnPushButton->isChecked()) {
        // Attenuate signal
        _auxPre->process(targetSampleBuffer);
        // Send signal
        targetSampleBuffer.copyTo(_auxSend->sampleBuffer());
        // Take received signal
        _auxReturn->sampleBuffer().copyTo(targetSampleBuffer);
        // Attenuate signal
        _auxPost->process(targetSampleBuffer);
    }

    _faderStage->process(targetSampleBuffer);    
    _peak = QUnits::linearToDb(targetSampleBuffer.peak());

    targetSampleBuffer.copyTo(_channelOut->sampleBuffer());
}

void ChannelWidget::updateInterface()
{
    ui->progressBar->setValue((int)_peak);
}

double ChannelWidget::panorama()
{
    return (double)ui->panDial->value() / 100.0;
}

bool ChannelWidget::isInSubGroup12()
{
    return ui->subgroup12PushButton->isChecked();
}

bool ChannelWidget::isInSubGroup34()
{
    return ui->subgroup34PushButton->isChecked();
}

bool ChannelWidget::isInSubGroup56()
{
    return ui->subgroup56PushButton->isChecked();
}

bool ChannelWidget::isInSubGroup78()
{
    return ui->subgroup78PushButton->isChecked();
}

bool ChannelWidget::isMuted()
{
    return ui->mutePushButton->isChecked();
}

bool ChannelWidget::isSoloed()
{
    return ui->soloPushButton->isChecked();
}

bool ChannelWidget::isOnMain()
{
    return ui->mainPushButton->isChecked();
}
