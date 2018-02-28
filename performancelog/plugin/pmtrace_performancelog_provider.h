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

#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER pmtrace_performancelog

#undef TRACEPOINT_INCLUDE_FILE
#define TRACEPOINT_INCLUDE_FILE ./pmtrace_performancelog_provider.h

#ifdef __cplusplus
extern "C"{
#endif /*__cplusplus */


#if !defined(_PMTRACE_PERFORMANCELOG_PROVIDER_H) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _PMTRACE_PERFORMANCELOG_PROVIDER_H

#include <lttng/tracepoint.h>

/* "message" tracepoint should be used for single event trace points */
TRACEPOINT_EVENT(
    pmtrace_performancelog,
    message,
    TP_ARGS(char*, text),
    TP_FIELDS(ctf_string(scope, text)))
/* "before"/"after" tracepoint should be used for measuring the
   duration of something that doesn't correspond with a function call or scope */
TRACEPOINT_EVENT(
    pmtrace_performancelog,
    before,
    TP_ARGS(char*, text),
    TP_FIELDS(ctf_string(scope, text)))
TRACEPOINT_EVENT(
    pmtrace_performancelog,
    after,
    TP_ARGS(char*, text),
    TP_FIELDS(ctf_string(scope, text)))

#endif /* _PMTRACE_PERFORMANCELOG_PROVIDER_H */

#ifdef __cplusplus
}
#endif /*__cplusplus */
