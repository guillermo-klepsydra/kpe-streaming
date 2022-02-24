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

#ifndef EVENTLOOP_FACTORY_H
#define EVENTLOOP_FACTORY_H

#include <klepsydra/admin/container_utils.h>
#include <klepsydra/high_performance/event_loop_middleware_provider.h>
#include <klepsydra/streaming/streaming_configuration_manager.h>

namespace kpsr {
namespace streaming {

const int EVENT_LOOP_SIZE = 32;

using FactoryEventLoopType = kpsr::high_performance::EventLoopMiddlewareProvider<EVENT_LOOP_SIZE>;
using EventLoopPtr = std::shared_ptr<FactoryEventLoopType>;

class EventLoopFactory
{
public:
    EventLoopFactory(kpsr::Container *container,
                     StreamingConfigurationManager *streamingConfigurationManager);

    virtual ~EventLoopFactory();

    void start();
    void stop();

    template<class T>
    kpsr::Subscriber<T> *getSubscriber(const std::string &stepName)
    {
        spdlog::debug("EventLoopFactory::getSubscriber: stepName: {}", stepName);
        auto escapedStepName = kpsr::admin::ContainerUtils::escapedNameForOpenMct(stepName);
        auto eventLoopPtr = getEventLoop(escapedStepName);
        return eventLoopPtr->template getSubscriber<T>(escapedStepName);
    }

    template<class T>
    kpsr::Publisher<T> *getPublisher(const std::string &stepName,
                                     const std::function<void(T &)> initializerFunction)
    {
        spdlog::debug("EventLoopFactory::getPublisher: stepName: {}", stepName);
        auto escapedStepName = kpsr::admin::ContainerUtils::escapedNameForOpenMct(stepName);
        auto eventLoopPtr = getEventLoop(escapedStepName);
        return eventLoopPtr->template getPublisher<T>(escapedStepName,
                                                      _poolSize,
                                                      initializerFunction,
                                                      nullptr);
    }

private:
    kpsr::Container *_container;
    StreamingConfigurationManager *_streamingConfigurationManager;
    int _poolSize;
    std::vector<EventLoopPtr> _eventLoops;

    EventLoopPtr getEventLoop(const std::string &eventLoopName);
};
} // namespace streaming
} // namespace kpsr

#endif // EVENTLOOP_FACTORY_H