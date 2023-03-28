// Copyright (c) 2013-2022 LG Electronics, Inc.
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

#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER pmtrace_qmlplugin

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "./pmtrace_qmlplugin_provider.h"

#ifdef __cplusplus
extern "C"{
#endif /*__cplusplus */


#if !defined(_PMTRACE_QMLPLUGIN_PROVIDER_H) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _PMTRACE_QMLPLUGIN_PROVIDER_H

#include <lttng/tracepoint.h>

/* "message" tracepoint should be used for single event trace points */
TRACEPOINT_EVENT(
    pmtrace_qmlplugin,
    message,
    TP_ARGS(char*, text),
    TP_FIELDS(ctf_string(scope, text)))
/* "keyValue" tracepoint should be used for event with type and context data */
TRACEPOINT_EVENT(
    pmtrace_qmlplugin,
    keyValue,
    TP_ARGS(char*, eventType, char*, contextData),
    TP_FIELDS(ctf_string(key, eventType) ctf_string(value, contextData)))
/* "position" tracepoint records a message and two integer parameters */
TRACEPOINT_EVENT(
    pmtrace_qmlplugin,
    position,
    TP_ARGS(char*, text, int, x, int, y),
    TP_FIELDS(ctf_string(scope, text)
              ctf_integer(int, xPos, x)
              ctf_integer(int, yPos, y)))
/* "before"/"after" tracepoint should be used for measuring the
   duration of something that doesn't correspond with a function call or scope */
TRACEPOINT_EVENT(
    pmtrace_qmlplugin,
    before,
    TP_ARGS(char*, text),
    TP_FIELDS(ctf_string(scope, text)))
TRACEPOINT_EVENT(
    pmtrace_qmlplugin,
    after,
    TP_ARGS(char*, text),
    TP_FIELDS(ctf_string(scope, text)))

#endif /* _PMTRACE_QMLPLUGIN_PROVIDER_H */

#ifdef __cplusplus
}
#endif /*__cplusplus */
