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
#include <fstream>
#include <map>

#include <klepsydra/core/property.h> // for cereal map specialization with std::string as key
#include <klepsydra/streaming/json_thread_distribution_policy.h>
#include <klepsydra/streaming/streaming_configuration_manager.h>

#include <cereal/archives/json.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <spdlog/spdlog.h>

namespace cereal {
//! Saving for std::map<size_t, KpsrClass> for text based archives
// Note that this shows off some internal cereal traits such as EnableIf,
// which will only allow this template to be instantiated if its predicates
// are true
template<class Archive,
         class C,
         class A,
         class KpsrClass,
         traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae>
inline void save(Archive &ar, std::map<size_t, KpsrClass, C, A> const &map)
{
    for (const auto &i : map)
        ar(cereal::make_nvp(std::to_string(i.first), i.second));
}

//! Loading for std::map<size_t,  KpsrClass> for text based archives
template<class Archive,
         class C,
         class A,
         class KpsrClass,
         traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae>
inline void load(Archive &ar, std::map<size_t, KpsrClass, C, A> &map)
{
    map.clear();

    auto hint = map.begin();
    while (true) {
        const auto namePtr = ar.getNodeName();

        if (!namePtr)
            break;

        std::stringstream key(namePtr);
        size_t keyInt;
        key >> keyInt;
        KpsrClass value;
        ar(value);
        hint = map.emplace_hint(hint, std::move(keyInt), std::move(value));
    }
}
} // namespace cereal

namespace kpsr {
namespace streaming {

StreamingConfigurationManager::StreamingConfigurationManager(
    int poolSize,
    size_t numberOfCores,
    size_t numberOfEventLoops,
    size_t nonCriticalThreadPoolSize,
    int numberOfParallelThreads,
    const std::vector<std::string> &parallelisedStreams,
    std::shared_ptr<ThreadDistributionPolicy> threadDistributionPolicy)
    : _streamingConfiguration(poolSize,
                              numberOfCores,
                              numberOfEventLoops,
                              nonCriticalThreadPoolSize,
                              numberOfParallelThreads,
                              parallelisedStreams)
    , _threadDistributionPolicy(threadDistributionPolicy)
{}

StreamingConfigurationManager::StreamingConfigurationManager(const std::string &jsonFileName)
    : _streamingConfiguration(StreamingConfiguration())
    , _threadDistributionPolicy(std::make_shared<JsonThreadDistributionPolicy>())
{
    std::ifstream inputFileStream(jsonFileName, std::ios::binary);
    loadJsonStream(inputFileStream);
}

size_t StreamingConfigurationManager::addStepToEventLoop(const std::string &stepName)
{
    return _threadDistributionPolicy->addStepToEventLoop(stepName);
}

StreamingConfiguration &StreamingConfigurationManager::getStreamingConfiguration()
{
    return _streamingConfiguration;
}

StreamingConfiguration StreamingConfigurationManager::getStreamingConfiguration() const
{
    return _streamingConfiguration;
}

std::shared_ptr<ThreadDistributionPolicy>
    &StreamingConfigurationManager::getThreadDistributionPolicy()
{
    return _threadDistributionPolicy;
}

std::shared_ptr<ThreadDistributionPolicy> StreamingConfigurationManager::getThreadDistributionPolicy()
    const
{
    return _threadDistributionPolicy;
}

bool StreamingConfigurationManager::operator==(const StreamingConfigurationManager &rhs) const
{
    if (this->_streamingConfiguration.poolSize != rhs.getStreamingConfiguration().poolSize) {
        return false;
    }

    if (this->_streamingConfiguration.numberOfCores !=
        rhs.getStreamingConfiguration().numberOfCores) {
        return false;
    }

    if (this->_streamingConfiguration.numberOfEventLoops !=
        rhs.getStreamingConfiguration().numberOfEventLoops) {
        return false;
    }

    if (this->_streamingConfiguration.nonCriticalThreadPoolSize !=
        rhs.getStreamingConfiguration().nonCriticalThreadPoolSize) {
        return false;
    }

    if (this->_threadDistributionPolicy->eventLoopCoreMap !=
        rhs.getThreadDistributionPolicy()->eventLoopCoreMap) {
        return false;
    }

    if (this->_threadDistributionPolicy->stepIDEventLoopMap !=
        rhs.getThreadDistributionPolicy()->stepIDEventLoopMap) {
        return false;
    }

    return true;
}

void StreamingConfigurationManager::loadJsonString(const std::string &jsonString)
{
    std::stringstream inputStream;
    inputStream << jsonString;
    loadJsonStream(inputStream);
}

void StreamingConfigurationManager::loadJsonStream(std::istream &jsonStream)
{
    ::cereal::JSONInputArchive iarchive(jsonStream);
    serialize(iarchive);
}

std::string StreamingConfigurationManager::exportJsonString()
{
    std::stringstream outputStream;
    {
        ::cereal::JSONOutputArchive oarchive(outputStream);
        serialize(oarchive);
    }
    return outputStream.str();
}

template<class Archive>
void StreamingConfigurationManager::serialize(Archive &archive)
{
    archive(cereal::make_nvp(POOL_SIZE, _streamingConfiguration.poolSize),
            cereal::make_nvp(NUMBER_OF_CORES, _streamingConfiguration.numberOfCores),
            cereal::make_nvp(NUMBER_OF_EVENT_LOOPS, _streamingConfiguration.numberOfEventLoops),
            cereal::make_nvp(NUMBER_OF_PARALLEL_THREADS,
                             _streamingConfiguration.numberOfParallelThreads),
            cereal::make_nvp(NON_CRITICAL_THREAD_POOL_SIZE,
                             _streamingConfiguration.nonCriticalThreadPoolSize),
            cereal::make_nvp(EVENT_LOOP_CORE_MAP, _threadDistributionPolicy->eventLoopCoreMap),
            cereal::make_nvp(LAYER_EVENT_LOOP_MAP, _threadDistributionPolicy->stepIDEventLoopMap),
            cereal::make_nvp(PARALLISED_LAYERS, _streamingConfiguration.parallelisedSteps));
}

} // namespace streaming
} // namespace kpsr