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

#ifndef PerformanceLogTracer_h
#define PerformanceLogTracer_h

#ifdef HAS_LTTNG

#include "pmtrace_performancelog_provider.h"

/* PMTRACE_LOG is for free form tracing. Provide a string
   which uniquely identifies your trace point. */
#define PMTRACE_LOG(label) \
    tracepoint(pmtrace_performancelog, message, label)

/* PMTRACE_BEFORE / AFTER is for tracing a time duration
 * which is not contained within a scope (curly braces) or function,
 * or in C code where there is no mechanism to automatically detect
 * exiting a scope or function.
 */
#define PMTRACE_BEFORE(label) \
    tracepoint(pmtrace_performancelog, before, label)
#define PMTRACE_AFTER(label) \
    tracepoint(pmtrace_performancelog, after, label)

#else // HAS_LTNG

#define PMTRACE_LOG(label)
#define PMTRACE_BEFORE(label)
#define PMTRACE_AFTER(label)

#endif // HAS_LTTNG

#endif // PerformanceLogTracer_h
