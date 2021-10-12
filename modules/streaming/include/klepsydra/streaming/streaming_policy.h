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

#ifndef STREAMING_POLICY_H
#define STREAMING_POLICY_H

#include <klepsydra/streaming/visibility.h>

#include <map>
#include <vector>
#include <functional>

#include <yaml-cpp/yaml.h>
#include <klepsydra/streaming/streaming_types.h>
#include <klepsydra/streaming/streaming_configuration.h>

namespace kpsr {
namespace streaming {

class StreamingPolicy {
public:
    StreamingPolicy(int poolSize,
                    size_t numberOfCores,
                    size_t numberOfEventLoops,
                    size_t nonCriticalThreadPoolSize,
                    int numberOfParallelThreads,
                    const std::vector<std::string> & parallelisedStreams)
        : _streamingConfiguration(poolSize, numberOfCores, numberOfEventLoops, nonCriticalThreadPoolSize, numberOfParallelThreads, parallelisedStreams)
    {}

    StreamingPolicy(const StreamingConfiguration & streamingConfiguration)
        : _streamingConfiguration(streamingConfiguration)
    {}

    virtual size_t addStepToEventLoop(const std::string & stepName) = 0;

    StreamingConfiguration & getStreamingConfiguration() {
        return _streamingConfiguration;
    }

    virtual ~StreamingPolicy() {}

protected:
    StreamingConfiguration _streamingConfiguration;
};

class DefaultStreamingPolicy : public StreamingPolicy {
public:
    DefaultStreamingPolicy(size_t numberOfCores, int poolSize, size_t nonCriticalThreadPoolSize,
                           int numberOfParallelThreads, const std::vector<std::string> & parallelisedStreams);

    virtual size_t addStepToEventLoop(const std::string & stepName) override;

    std::vector<int> streamEventLoopDistribution;
};

class JsonStreamingPolicy : public StreamingPolicy {
public:
    JsonStreamingPolicy(const std::string& jsonFileName);
    virtual size_t addStepToEventLoop(const std::string & stepName) override;

};

}
}

const static std::string STREAMING_CONF_YAML_NODE("streaming_configuration");

void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr);

#endif
