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
TARGET = pmtraceplugin
QT += qml

MOC_DIR = .moc
OBJECTS_DIR = .obj

QMAKE_CXXFLAGS += -fpermissive

# Input
HEADERS += pmtrace.h \
           pmtraceplugin.h

SOURCES += pmtrace.cpp \
           pmtraceplugin.cpp

OTHER_FILES += qmldir \
               pmtraceplugindescription.json

other_files.files = qmldir \
                    pmtraceplugindescription.json
other_files.path = $$WEBOS_INSTALL_QML/PmTrace

HEADERS += pmtrace_qmlplugin_provider.h \
           QmlPluginTracer.h
lttng {
    DEFINES += HAS_LTTNG
    SOURCES += pmtrace_qmlplugin_provider.c
    # -ldl : required for dynamic linking
    # -lurcu-bp : userspace synchronization primitives for lttng
    # -llttng-ust : lttng userspace tracing library
    # LTTng uses weak symbols, so if these libraries are not present
    # at runtime, it does not prevent dynamic linking.
    # LTTng libraries are only installed in the system image for
    # development, so in the production runtime, these libraries are
    # not loaded and have no memory impact.
    LIBS += -ldl -lurcu-bp -llttng-ust
}

target.path = $$WEBOS_INSTALL_QML/PmTrace
INSTALLS += target other_files
