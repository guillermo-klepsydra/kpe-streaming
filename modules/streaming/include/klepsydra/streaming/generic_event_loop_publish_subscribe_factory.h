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

#ifndef GENERIC_EVENTLOOP_PUBLISH_SUBSCRIBE_FACTORY_H
#define GENERIC_EVENTLOOP_PUBLISH_SUBSCRIBE_FACTORY_H

#include <klepsydra/high_performance/event_loop_middleware_provider.h>
#include <klepsydra/streaming/event_loop_factory.h>
#include <klepsydra/streaming/generic_publish_subscribe_factory.h>

namespace kpsr {
namespace streaming {
template<class TypeValue>

class GenericEventLoopPublishSubscribeFactory : public GenericPublishSubscribeFactory<TypeValue>
{
public:
    GenericEventLoopPublishSubscribeFactory(std::shared_ptr<EventLoopFactory> eventLoopFactory)
        : kpsr::Service(nullptr, "EventLoopPubSubFactoryService")
        , _eventLoopFactory(eventLoopFactory)
    {
        if (!_eventLoopFactory) {
            throw std::runtime_error("EventLoopFactory is a nulltpr");
        }
    }

    virtual ~GenericEventLoopPublishSubscribeFactory() {}

    virtual kpsr::Publisher<TypeValue> *getPublisher(const std::string &stepName) override
    {
        return _eventLoopFactory->getPublisher<TypeValue>(stepName, nullptr);
    }

    virtual kpsr::Subscriber<TypeValue> *getSubscriber(const std::string &stepName) override
    {
        return _eventLoopFactory->getSubscriber<TypeValue>(stepName);
    }

    virtual void start() override { _eventLoopFactory->start(); }

    virtual void stop() override { _eventLoopFactory->stop(); }

private:
    std::shared_ptr<EventLoopFactory> _eventLoopFactory;
};
} // namespace streaming
} // namespace kpsr

#endif // GENERIC_EVENTLOOP_PUBLISH_SUBSCRIBE_FACTORY_H