// Copyright (c) 2013-2018 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#ifndef AUDIOSERVICE_H
#define AUDIOSERVICE_H

#include <QtGlobal>
#include <QDebug>
#include <QObject>
#include <QStringList>

#include <glib.h>
#include <luna-service2/lunaservice.h>

/*!
 * \class AudioService
 *
 * \brief Provides QML property bindings for the service com.palm.audio
 *
 * This class provides
 * \li Properties and methods for the associated service.
 * \li Qt types for properties, parameters and return values.
 *
 * \see Service
 */

class AudioService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int volume READ volume NOTIFY volumeChanged)
    Q_PROPERTY(int volumeMax READ volumeMax NOTIFY volumeMaxChanged)
    Q_PROPERTY(bool muted READ muted NOTIFY mutedChanged)
    Q_PROPERTY(QString scenario READ scenario NOTIFY scenarioChanged)
    Q_PROPERTY(bool disabled READ disabled NOTIFY disabledChanged)
    Q_PROPERTY(QString appId READ appId WRITE setAppId NOTIFY appIdChanged)
    Q_PROPERTY(QString cause READ cause NOTIFY causeChanged)
    Q_PROPERTY(QStringList changed READ changed NOTIFY changedChanged)

public:
    AudioService(QObject* parent = 0);
    ~AudioService();

    int volume();
    int volumeMin();
    int volumeMax();
    bool muted();
    QString scenario();
    bool disabled();
    QString cause();
    QStringList changed();

    QString appId() { return m_appId; }
    void setAppId(const QString& appId);

signals:
    void volumeChanged();
    void volumeMaxChanged();
    void mutedChanged();
    void volumeUpdated();
    void scenarioChanged();
    void disabledChanged();
    void appIdChanged();
    void causeChanged();
    void changedChanged();

public :
    Q_INVOKABLE void volumeUp();
    Q_INVOKABLE void volumeDown();
    Q_INVOKABLE void volumeKeeping();
    Q_INVOKABLE void toggleVolumeMute();

private:
    GMainLoop *m_mainLoop;
    LSHandle *m_handle;

    bool m_isInitialized;
    bool m_isSubscribed;

    bool m_muted;
    int m_volume;
    int m_volumeMin;
    int m_volumeMax;
    QString m_scenario;
    bool m_disabled;
    QString m_appId;
    QString m_cause;
    QStringList m_changed;
    void* m_serverStatusCookie;

    bool attachLSBus();
    bool detachLSBus();
    bool registerServerStatus();
    bool unregisterServerStatus();

    bool initVolumeStatus();
    void changeVolumeMute(bool mute);

    void setVolume(int vol);
    void setVolumeMax(int vol);
    void setMuted(bool status);
    void setScenario(const QString& scenario);
    void setDisabled(bool status);
    void setCause(const QString& cause);
    void setChanged(const QStringList& changed);

    bool requestService(const char *uri, const char *payload, LSFilterFunc callback = NULL);
    static bool handleVolumeUpdate(LSHandle *handle, LSMessage *reply, void *ctx);
    static bool handleMuteUpdate(LSHandle *handle, LSMessage *reply, void *ctx);
    static bool handleGetVolume(LSHandle *handle, LSMessage *reply, void *ctx);
    static bool handleServiceStatus(LSHandle *sh, const char *serviceName, bool connected, void *ctx);
};

#endif // AUDIOSERVICE_H
