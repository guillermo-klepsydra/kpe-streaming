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

#ifndef GENERIC_EVENT_EMITTER_PUBLISH_SUBSCRIBE_FACTORY_H
#define GENERIC_EVENT_EMITTER_PUBLISH_SUBSCRIBE_FACTORY_H

#include <klepsydra/streaming/event_emitter_factory.h>
#include <klepsydra/streaming/generic_publish_subscribe_factory.h>

namespace kpsr {
namespace streaming {
template<class TypeValue>

class GenericEventEmitterPublishSubscribeFactory : public GenericPublishSubscribeFactory<TypeValue>
{
public:
    GenericEventEmitterPublishSubscribeFactory(std::shared_ptr<EventEmitterFactory> eventEmitterFactory)
        : kpsr::Service(nullptr, "EmitterPubSubFactoryService")
        , _eventEmitterFactory(eventEmitterFactory)
    {}

    virtual ~GenericEventEmitterPublishSubscribeFactory() {}

    virtual kpsr::Publisher<TypeValue> *getPublisher(
        const std::string &stepName) override
    {
        spdlog::debug("GenericEventEmitterPublishSubscribeFactory::getPublisher: stepName: {}", stepName);
        return getEventEmitter(stepName)->getPublisher();
    }

    virtual kpsr::Subscriber<TypeValue> *getSubscriber(
        const std::string &stepName) override
    {
        spdlog::debug("GenericEventEmitterPublishSubscribeFactory::getSubscriber: stepName: {}", stepName);
        return getEventEmitter(stepName)->getSubscriber();
    }

    virtual void start() override {}

    virtual void stop() override {}

private:
    std::shared_ptr<kpsr::EventEmitterMiddlewareProvider<TypeValue>> getEventEmitter(
        const std::string &stepName)
    {
        return _eventEmitterFactory->getEventEmitter<TypeValue>(stepName, nullptr);
    }

    std::shared_ptr<EventEmitterFactory> _eventEmitterFactory;
};
} // namespace streaming
} // namespace kpsr

#endif // GENERIC_EVENT_EMITTER_PUBLISH_SUBSCRIBE_FACTORY_H