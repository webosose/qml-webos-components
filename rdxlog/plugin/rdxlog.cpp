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

#include "rdxlog.h"

#include <iostream>

using namespace std;

RdxLog::RdxLog(QObject *parent)
{
    Q_UNUSED(parent);
}

void RdxLog::makeReport(const QString &component, const QString &cause, const QString &detail, const QString &contents)
{
    RdxReportMetadata md = create_rdx_report_metadata();
    rdx_report_metadata_set_component(md, component.toStdString().c_str());
    rdx_report_metadata_set_cause(md, cause.toStdString().c_str());
    rdx_report_metadata_set_detail(md, detail.toStdString().c_str());
    rdx_make_report(md, contents.toStdString().c_str());
    destroy_rdx_report_metadata(md);
}
