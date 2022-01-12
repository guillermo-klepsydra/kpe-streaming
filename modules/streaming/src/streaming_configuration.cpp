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
#include <klepsydra/streaming/streaming_configuration.h>

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

StreamingConfiguration::StreamingConfiguration() {}

StreamingConfiguration::StreamingConfiguration(int poolSize,
                                               size_t numberOfCores,
                                               size_t numberOfEventLoops,
                                               size_t nonCriticalThreadPoolSize,
                                               int numberOfParallelThreads,
                                               const std::vector<std::string> &parallelisedSteps)
    : poolSize(poolSize)
    , numberOfCores(numberOfCores)
    , numberOfEventLoops(numberOfEventLoops)
    , nonCriticalThreadPoolSize(nonCriticalThreadPoolSize)
    , numberOfParallelThreads(numberOfParallelThreads)
    , parallelisedSteps(parallelisedSteps)
{}

StreamingConfiguration::StreamingConfiguration(const std::string &jsonFileName)
{
    std::ifstream inputFileStream(jsonFileName, std::ios::binary);
    loadJsonStream(inputFileStream);
}

bool StreamingConfiguration::operator==(const kpsr::streaming::StreamingConfiguration &rhs) const
{
    if (this->poolSize != rhs.poolSize) {
        return false;
    }

    if (this->numberOfCores != rhs.numberOfCores) {
        return false;
    }

    if (this->numberOfEventLoops != rhs.numberOfEventLoops) {
        return false;
    }

    if (this->eventLoopCoreMap != rhs.eventLoopCoreMap) {
        return false;
    }

    if (this->stepIDEventLoopMap != rhs.stepIDEventLoopMap) {
        return false;
    }

    return true;
}

void StreamingConfiguration::loadJsonString(const std::string &jsonString)
{
    std::stringstream inputStream;
    inputStream << jsonString;
    loadJsonStream(inputStream);
}

void StreamingConfiguration::loadJsonStream(std::istream &jsonStream)
{
    ::cereal::JSONInputArchive iarchive(jsonStream);
    serialize(iarchive);
}

std::string StreamingConfiguration::exportJsonString()
{
    std::stringstream outputStream;
    {
        ::cereal::JSONOutputArchive oarchive(outputStream);
        serialize(oarchive);
    }
    return outputStream.str();
}

template<class Archive>
void StreamingConfiguration::serialize(Archive &archive)
{
    archive(cereal::make_nvp(POOL_SIZE, poolSize),
            cereal::make_nvp(NUMBER_OF_CORES, numberOfCores),
            cereal::make_nvp(NUMBER_OF_EVENT_LOOPS, numberOfEventLoops),
            cereal::make_nvp(NUMBER_OF_PARALLEL_THREADS, numberOfParallelThreads),
            cereal::make_nvp(EVENT_LOOP_CORE_MAP, eventLoopCoreMap),
            cereal::make_nvp(LAYER_EVENT_LOOP_MAP, stepIDEventLoopMap),
            cereal::make_nvp(PARALLISED_LAYERS, parallelisedSteps));
}
} // namespace streaming
} // namespace kpsr
