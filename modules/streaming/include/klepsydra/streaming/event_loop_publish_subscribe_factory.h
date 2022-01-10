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

#ifndef PUBLISH_SUBCRIBE_EVENTLOOP_FACTORY_H
#define PUBLISH_SUBCRIBE_EVENTLOOP_FACTORY_H

#include <klepsydra/high_performance/event_loop_middleware_provider.h>
#include <klepsydra/streaming/streaming_policy.h>
#include <klepsydra/admin/container_utils.h>

namespace kpsr {
namespace streaming {

const int EVENT_LOOP_SIZE = 32;

using FactoryEventLoopType = kpsr::high_performance::EventLoopMiddlewareProvider<EVENT_LOOP_SIZE>;
using EventLoopPtr = std::shared_ptr<FactoryEventLoopType>;

class EventLoopPublishSubscribeFactory
{
public:
    EventLoopPublishSubscribeFactory(kpsr::Container * container,
                                     StreamingPolicy * streamingPolicy);

    virtual ~EventLoopPublishSubscribeFactory();

    const std::vector<EventLoopPtr> getEventLoops();

    const StreamingPolicy * getStreamingPolicy();

    void start();
    void stop();

    template <class T>
    kpsr::Subscriber<T> * getSubscriber(const std::string& stepName) {
        spdlog::debug("EventLoopPublishSubscribeFactory::getSubscriber: stepName: {}", stepName);
        auto eventLoopName = kpsr::admin::ContainerUtils::escapedNameForOpenMct(stepName);
        auto eventLoopPtr = getEventLoop(eventLoopName);
        return eventLoopPtr->template getSubscriber<T>(eventLoopName);
    }

    EventLoopPtr getEventLoop(const std::string & eventLoopName);

    int getPoolSize() const;

private:
    kpsr::Container * _container;
    StreamingPolicy * _streamingPolicy;
    int _poolSize;
    std::vector<EventLoopPtr> _eventLoops;
};
}
}

#endif // PUBLISH_SUBCRIBE_EVENTLOOP_FACTORY_H