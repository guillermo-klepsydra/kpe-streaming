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
#include <klepsydra/streaming/default_thread_distribution_policy.h>
#include <spdlog/spdlog.h>

namespace kpsr {
namespace streaming {

DefaultThreadDistributionPolicy::DefaultThreadDistributionPolicy(size_t numberOfCores,
                                                                 size_t numberOfEventLoops)
    : streamEventLoopDistribution(numberOfEventLoops)
    , _numberOfEventLoops(numberOfEventLoops)
{
    for (size_t i = 0; i < _numberOfEventLoops; i++) {
        std::vector<int> cores{static_cast<int>(i % numberOfCores)};
        eventLoopCoreMap[i] = cores;
        spdlog::debug(
            "DefaultStreamingPolicy::DefaultStreamingPolicy. EventLoop {}, goes to core {}",
            i,
            cores[0]);
    }
}

size_t DefaultThreadDistributionPolicy::addStepToEventLoop(const std::string &stepName)
{
    auto index = stepIDEventLoopMap.find(stepName);
    if (index == stepIDEventLoopMap.end()) {
        size_t minIndex = 0;
        for (size_t i = 0; i < _numberOfEventLoops; i++) {
            if (streamEventLoopDistribution[i] < streamEventLoopDistribution[minIndex]) {
                minIndex = i;
            }
        }
        streamEventLoopDistribution[minIndex]++;
        stepIDEventLoopMap[stepName] = minIndex;
        return minIndex;
    } else {
        return index->second;
    }
}

} // namespace streaming
} // namespace kpsr