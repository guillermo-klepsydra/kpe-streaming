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

#include <klepsydra/streaming/streaming_policy.h>
#include <spdlog/spdlog.h>

static const size_t CORE_RATIO = 1;
static const int NON_INTENSIVE_STREAM_RATIO = 1;

kpsr::streaming::DefaultStreamingPolicy::DefaultStreamingPolicy(size_t numberOfCores,
                                                                int poolSize,
                                                                size_t nonCriticalThreadPoolSize,
                                                                int numberOfParallelThreads,
                                                                const std::vector<std::string> & parallelisedSteps)
    : StreamingPolicy(poolSize, numberOfCores, numberOfCores * CORE_RATIO, nonCriticalThreadPoolSize, numberOfParallelThreads, parallelisedSteps)
    , streamEventLoopDistribution(_streamingConfiguration.numberOfEventLoops)
{
    for(size_t i = 0; i < _streamingConfiguration.numberOfEventLoops; i++) {
        std::vector<int> cores{static_cast<int>(i % numberOfCores)};
        _streamingConfiguration.eventLoopCoreMap[i] = cores;
        spdlog::debug("DefaultStreamingPolicy::DefaultStreamingPolicy. EventLoop {}, goes to core {}", i, cores[0]);
    }
}

size_t kpsr::streaming::DefaultStreamingPolicy::addStepToEventLoop(const std::string & stepName) {
    auto index = _streamingConfiguration.stepIDEventLoopMap.find(stepName);
    if (index == _streamingConfiguration.stepIDEventLoopMap.end()) {
        size_t minIndex = 0;
        for (size_t i = 0; i < _streamingConfiguration.numberOfEventLoops; i++) {
            if (streamEventLoopDistribution[i] < streamEventLoopDistribution[minIndex]) {
                minIndex = i;
            }
        }
        streamEventLoopDistribution[minIndex]++;
        _streamingConfiguration.stepIDEventLoopMap[stepName] = minIndex;
        return minIndex;
    } else {
        return index->second;
    }
}

kpsr::streaming::YamlStreamingPolicy::YamlStreamingPolicy(const std::string &yamlFileName)
    : StreamingPolicy(YAML::LoadFile(yamlFileName)[STREAMING_CONF_YAML_NODE].as<StreamingConfiguration>())
{}

size_t kpsr::streaming::YamlStreamingPolicy::addStepToEventLoop(const std::string & stepName) {
    return _streamingConfiguration.stepIDEventLoopMap[stepName];
}

void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr)
{
    // Get the first occurrence
    size_t pos = data.find(toSearch);
    // Repeat till end is reached
    while( pos != std::string::npos)
    {
        // Replace this occurrence of Sub String
        data.replace(pos, toSearch.size(), replaceStr);
        // Get the next occurrence from the current position
        pos =data.find(toSearch, pos + replaceStr.size());
    }
}
