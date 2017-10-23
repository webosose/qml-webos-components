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

#include "audioservice.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

// Audio service luna API
#define AUDIOSERVICE                    "com.webos.audio"
#define AUDIOSERVICE_API_GET_VOLUME     "luna://"AUDIOSERVICE"/getVolume"
#define AUDIOSERVICE_API_VOLUME_UP      "luna://"AUDIOSERVICE"/volumeUp"
#define AUDIOSERVICE_API_VOLUME_DOWN    "luna://"AUDIOSERVICE"/volumeDown"
#define AUDIOSERVICE_API_SETMUTED       "luna://"AUDIOSERVICE"/setMuted"
#define AUDIOSERVICE_API_VOLUME_KEEPING "luna://"AUDIOSERVICE"/volumeKeeping"

// Audio scenario
#define AUDIOSERVICE_SCENARIO_TV_SPEAKER    "mastervolume_tv_speaker"

AudioService::AudioService(QObject* parent)
    : QObject(parent)
    , m_mainLoop(NULL)
    , m_handle(NULL)
    , m_isInitialized(false)
    , m_isSubscribed(false)
    , m_muted(false)
    , m_volume(0)
    , m_volumeMin(0)
    , m_volumeMax(100)
    , m_scenario(AUDIOSERVICE_SCENARIO_TV_SPEAKER)
    , m_disabled(false)
    , m_serverStatusCookie(NULL)
{
}

AudioService::~AudioService()
{
    unregisterServerStatus();
    detachLSBus();
}

bool AudioService::attachLSBus()
{
    if (m_appId.isEmpty()) {
        qFatal("appId not specified for AudioService");
    }

    if (!m_mainLoop) {
        if (!(m_mainLoop = g_main_loop_new(NULL, FALSE))) {
            qDebug() << __PRETTY_FUNCTION__ << "failed to g_main_loop_new!!";
            return false;
        }
    }

    if (!LSRegisterPubPriv(m_appId.toUtf8().constData(), &m_handle, false, NULL)) {
        qDebug() << __PRETTY_FUNCTION__ << "failed to LSRegisterPubPriv!!";
        g_main_loop_unref(m_mainLoop);
        m_mainLoop = NULL;
        return false;
    }

    if (!LSGmainAttach(m_handle, m_mainLoop, NULL)) {
        qDebug() << __PRETTY_FUNCTION__ << "failed to LSGmainAttach!!";

        // Undo initialization
        LSUnregister(m_handle, NULL);
        m_handle = NULL;
        g_main_loop_unref(m_mainLoop);
        m_mainLoop = NULL;

        return false;
    }

    return true;
}

bool AudioService::registerServerStatus()
{
    if (!LSRegisterServerStatusEx(m_handle, AUDIOSERVICE, AudioService::handleServiceStatus, this,
        &m_serverStatusCookie, NULL)) {
        detachLSBus();
        return false;
    }
    return true;
}

bool AudioService::unregisterServerStatus()
{
    if (!LSCancelServerStatus(m_handle, m_serverStatusCookie, NULL)){
        return false;
    }
    return true;
}

bool AudioService::detachLSBus()
{
    if (m_handle) {
        LSUnregister(m_handle, NULL);
        m_handle = NULL;
    }

    if (m_mainLoop) {
        g_main_loop_unref(m_mainLoop);
    }

    return true;
}

bool AudioService::initVolumeStatus()
{
    requestService(AUDIOSERVICE_API_GET_VOLUME, "{\"subscribe\" : true}", AudioService::handleGetVolume);

    return true;
}

int AudioService::volume()
{
    return m_volume;
}

void AudioService::setVolume(int vol)
{
    if (m_volume != vol) {
        m_volume = vol;
        emit volumeChanged();
    }
}

int AudioService::volumeMin()
{
    return m_volumeMin;
}

int AudioService::volumeMax()
{
    return m_volumeMax;
}

void AudioService::setVolumeMax(int vol)
{
    if (m_volumeMax != vol) {
        m_volumeMax = vol;
        emit volumeMaxChanged();
    }
}

bool AudioService::muted()
{
    return m_muted;
}

void AudioService::setMuted(bool status)
{
    if (m_muted != status) {
        m_muted = status;
        emit mutedChanged();
    }
}

QString AudioService::scenario()
{
    return m_scenario;
}

void AudioService::setScenario(const QString& scenario)
{
    if (m_scenario != scenario) {
        m_scenario = scenario;
        emit scenarioChanged();
    }
}

bool AudioService::disabled()
{
    return m_disabled;
}

void AudioService::setDisabled(bool status)
{
    if (m_disabled != status) {
        m_disabled = status;
        emit disabledChanged();
    }
}

QString AudioService::cause()
{
    return m_cause;
}

void AudioService::setCause(const QString& cause)
{
    if (m_cause != cause) {
        m_cause = cause;
        emit causeChanged();
    }
}

QStringList AudioService::changed()
{
    return m_changed;
}

void AudioService::setChanged(const QStringList& changed)
{
    if (m_changed != changed) {
        m_changed = changed;
        emit changedChanged();
    }
}

void AudioService::volumeUp()
{
    requestService(AUDIOSERVICE_API_VOLUME_UP, "{}", AudioService::handleVolumeUpdate);
}

void AudioService::volumeDown()
{
    requestService(AUDIOSERVICE_API_VOLUME_DOWN, "{}", AudioService::handleVolumeUpdate);
}

void AudioService::volumeKeeping()
{
    requestService(AUDIOSERVICE_API_VOLUME_KEEPING, "{}", NULL);
}

void AudioService::toggleVolumeMute()
{
    changeVolumeMute(!m_muted);
}

void AudioService::changeVolumeMute(bool mute)
{
    if (mute) {
        requestService(AUDIOSERVICE_API_SETMUTED, "{\"muted\":true}", AudioService::handleMuteUpdate);
    } else {
        requestService(AUDIOSERVICE_API_SETMUTED, "{\"muted\":false}", AudioService::handleMuteUpdate);
    }
}

bool AudioService::requestService(const char *uri, const char *payload, LSFilterFunc callback)
{
    LSError lserror;
    LSErrorInit(&lserror);
    LSMessageToken sessionToken;

    bool ret;

    if (!m_handle || !m_mainLoop) {
        qDebug() << "LS bus not setup yet";
        return false;
    }

    if (!uri || !payload) {
        qDebug() << "Invalid argument";
        return false;
    }

    ret = LSCallFromApplication(m_handle, uri, payload, NULL, callback, this, &sessionToken, &lserror);

    if (!ret) {
        qDebug() << "Error on LSCallFromApplication";
        if (LSErrorIsSet(&lserror)) {
            LSErrorPrint(&lserror, stderr);
            LSErrorFree(&lserror);
        }
    }

    return ret;
}

bool AudioService::handleVolumeUpdate(LSHandle *handle, LSMessage *reply, void *ctx)
{
    LSError lserror;
    LSErrorInit(&lserror);
    bool result = false;

    const char *payload;
    payload = LSMessageGetPayload(reply);

    QJsonObject rootObject = QJsonDocument::fromJson(payload).object();
    result = rootObject.find("returnValue").value().toBool();
    if (result) {
        AudioService *svc = static_cast<AudioService *>(ctx);
        if (!svc->m_isSubscribed) {
            svc->requestService(AUDIOSERVICE_API_GET_VOLUME, "{\"subscribe\" : false}", AudioService::handleGetVolume);
        }
#if defined(DEBUG_AUDIO_SERVICE)
        qDebug("Response on volumeUp/volumeDown: '%s'", payload);
#endif
    }
    else {
        qDebug("Error response on volumeUp/volumeDown: '%s'", payload);
    }

    return result;
}

bool AudioService::handleMuteUpdate(LSHandle *handle, LSMessage *reply, void *ctx)
{
    LSError lserror;
    LSErrorInit(&lserror);
    bool result = false;

    const char *payload;
    payload = LSMessageGetPayload(reply);

    QJsonObject rootObject = QJsonDocument::fromJson(payload).object();
    result = rootObject.find("returnValue").value().toBool();
    if (result) {
        AudioService *svc = static_cast<AudioService *>(ctx);
        if (!svc->m_isSubscribed) {
            svc->requestService(AUDIOSERVICE_API_GET_VOLUME, "{\"subscribe\" : false}", AudioService::handleGetVolume);
        }
#if defined(DEBUG_AUDIO_SERVICE)
        qDebug("Response on setMuted: '%s'", payload);
#endif
    }
    else {
        qDebug("Error response on setMuted: '%s'", payload);
    }

    return result;
}

bool AudioService::handleGetVolume(LSHandle *handle, LSMessage *reply, void *ctx)
{
    LSError lserror;
    LSErrorInit(&lserror);
    bool result = false;
    bool subscribed = false;
    QString action;

    const char *payload;
    payload = LSMessageGetPayload(reply);

    QJsonObject rootObject = QJsonDocument::fromJson(payload).object();
    result = rootObject.find("returnValue").value().toBool();
    subscribed = rootObject.find("subscribed").value().toBool();
    action = rootObject.find("action").value().toString();
    if (result) {
        AudioService *svc = static_cast<AudioService *>(ctx);
        int volume = (int)rootObject.find("volume").value().toDouble();
        int volumeMax = (int)rootObject.find("volumeMax").value().toDouble();
        bool muted = rootObject.find("muted").value().toBool();
        QString scenario = rootObject.find("scenario").value().toString();
        QString cause = rootObject.find("cause").value().toString();

        // Set subscribed upon "subscribed":true
        if (!svc->m_isSubscribed && subscribed) {
            svc->m_isSubscribed = true;
        }

        if (volume > volumeMax) {
            volume = volumeMax;
        }
        if (volume < svc->volumeMin()) {
            volume = svc->volumeMin();
        }

        // Update values
        if (action == "changed" || action == "requested" || action == "enabled") {
            if (action == "changed") {
                QStringList changed = QVariant(rootObject.find("changed").value().toArray().toVariantList()).toStringList();
                svc->setChanged(changed);
            }
            svc->setVolume(volume);
            svc->setVolumeMax(volumeMax);
            svc->setMuted(muted);
            svc->setScenario(scenario);
            svc->setCause(cause);
            svc->setDisabled(false);
        }
        else if (action == "disabled") {
            svc->setDisabled(true);
        }

        if (!svc->m_isInitialized) {
            // Set initialized upon the first valid response
            svc->m_isInitialized = true;
        } else {
            emit svc->volumeUpdated();
        }

#if defined(DEBUG_AUDIO_SERVICE)
        qDebug("Response on getVolume: '%s'", payload);
#endif
    } else {
        qDebug("Error response on getVolume: '%s'", payload);
    }

    return result;
}

bool AudioService::handleServiceStatus(LSHandle *sh, const char *serviceName, bool connected, void *ctx)
{
    AudioService *svc = static_cast<AudioService *>(ctx);

    if (connected) {
        svc->initVolumeStatus();
    } else {
        svc->m_isInitialized = false;
        svc->m_isSubscribed = false;
    }

    return true;
}

void AudioService::setAppId(const QString& appId)
{
    if (appId != m_appId) {
        m_appId = appId;
        emit appIdChanged();
        detachLSBus();
        if (attachLSBus()) registerServerStatus();
    }
}
