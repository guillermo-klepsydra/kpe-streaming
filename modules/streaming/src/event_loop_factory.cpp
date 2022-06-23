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

#include <klepsydra/admin/check_license.h>
#include <klepsydra/streaming/event_loop_factory.h>

namespace kpsr {
namespace streaming {

EventLoopFactory::EventLoopFactory(kpsr::Container *container,
                                   StreamingConfigurationManager *streamingConfigurationManager)
    : _container(container)
    , _streamingConfigurationManager(streamingConfigurationManager)
    , _poolSize(_streamingConfigurationManager->getStreamingConfiguration().poolSize)
    , _eventLoops(_streamingConfigurationManager->getStreamingConfiguration().numberOfEventLoops)
{
    check_license();
    kpsr::Threadpool::getCriticalThreadPool(
        _streamingConfigurationManager->getStreamingConfiguration().numberOfEventLoops + 2);
    kpsr::Threadpool::getNonCriticalThreadPool(
        _streamingConfigurationManager->getStreamingConfiguration().nonCriticalThreadPoolSize);

    for (size_t i = 0; i < _eventLoops.size(); i++) {
        spdlog::debug("EventLoopFactory::EventLoopFactory. New "
                      "eventloop with name: {}",
                      i);
        std::vector<int> cpuCores = _streamingConfigurationManager->getThreadDistributionPolicy()
                                        ->eventLoopCoreMap[i];
        _eventLoops[i] = std::make_shared<FactoryEventLoopType>(
            _container,
            std::to_string((i)),
            kpsr::high_performance::EVENT_LOOP_START_TIMEOUT_MICROSEC,
            cpuCores);
    }
}

EventLoopFactory::~EventLoopFactory()
{
    _eventLoops.clear();
}

EventLoopPtr EventLoopFactory::getEventLoop(const std::string &eventLoopName)
{
    return _eventLoops[_streamingConfigurationManager->addStepToEventLoop(eventLoopName)];
}

void EventLoopFactory::start()
{
    for (kpsr::streaming::EventLoopPtr const &eventloopPtr : _eventLoops) {
        eventloopPtr->start();
    }
    spdlog::debug("kpsr::streaming::EventLoopFactory::start. Started!");
}

void EventLoopFactory::stop()
{
    for (EventLoopPtr const &eventloopPtr : _eventLoops) {
        eventloopPtr->stop();
    }
}

} // namespace streaming
} // namespace kpsr
