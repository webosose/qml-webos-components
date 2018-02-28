# Copyright (c) 2013-2018 LG Electronics, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# SPDX-License-Identifier: Apache-2.0

TEMPLATE = lib
CONFIG += plugin
QT += quick
TARGET = audioserviceplugin

MOC_DIR = .moc
OBJECTS_DIR = .obj

DEFINES += DEBUG_AUDIO_SERVICE

HEADERS += audioserviceplugin.h \
           audioservice.h

SOURCES += audioserviceplugin.cpp \
           audioservice.cpp

CONFIG += link_pkgconfig
PKGCONFIG = glib-2.0 luna-service2

OTHER_FILES += audioserviceplugindescription.json

other_files.files = audioserviceplugindescription.json qmldir
other_files.path  = $$WEBOS_INSTALL_QML/AudioService

target.path = $$WEBOS_INSTALL_QML/AudioService

INSTALLS += target other_files
