/*****************************************************************************
*                           Klepsydra Streaming Modules
*              Copyright (C) 2020-2022  Klepsydra Technologies GmbH
*                            All Rights Reserved.
*
*  This file is subject to the terms and conditions defined in
*  file 'LICENSE.md', which is part of this source code package.
*
*  NOTICE:  All information contained herein is, and remains the property of Klepsydra
*  Technologies GmbH and its suppliers, if any. The intellectual and technical concepts
*  contained herein are proprietary to Klepsydra Technologies GmbH and its suppliers and
*  may be covered by Swiss and Foreign Patents, patents in process, and are protected by
*  trade secret or copyright law. Dissemination of this information or reproduction of
*  this material is strictly forbidden unless prior written permission is obtained from
*  Klepsydra Technologies GmbH.
*
*****************************************************************************/
#ifndef THREAD_DISTRIBUTION_POLICY_H
#define THREAD_DISTRIBUTION_POLICY_H

#include <map>
#include <vector>

namespace kpsr {
namespace streaming {

class ThreadDistributionPolicy
{
public:
    ThreadDistributionPolicy() {}

    virtual size_t addStepToEventLoop(const std::string &stepName) = 0;

    virtual ~ThreadDistributionPolicy() {}

    std::map<size_t, std::vector<int>> eventLoopCoreMap;
    std::map<std::string, size_t> stepIDEventLoopMap;
};

} // namespace streaming
} // namespace kpsr

#endif