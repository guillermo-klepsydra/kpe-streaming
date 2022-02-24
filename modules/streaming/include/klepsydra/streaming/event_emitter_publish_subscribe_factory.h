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

#ifndef EVENT_EMITTER_PUBLISH_SUBSCRIBE_FACTORY_H
#define EVENT_EMITTER_PUBLISH_SUBSCRIBE_FACTORY_H

#include <klepsydra/streaming/event_emitter_factory.h>
#include <klepsydra/streaming/publish_subscribe_factory.h>

namespace kpsr {
namespace streaming {
template<class TypeValue>

class EventEmitterPublishSubscribeFactory : public PublishSubscribeFactory<TypeValue>
{
public:
    EventEmitterPublishSubscribeFactory(std::shared_ptr<EventEmitterFactory> &eventEmitterFactory)
        : kpsr::Service(nullptr, "EmitterPubSubFactoryService")
        , _eventEmitterFactory(eventEmitterFactory)
    {}

    virtual ~EventEmitterPublishSubscribeFactory() {}

    virtual kpsr::Publisher<DataBatchWithId<TypeValue>> *getPublisher(
        const std::string &stepName, const size_t vectorSize) override
    {
        spdlog::debug("EventEmitterPublishSubscribeFactory::getPublisher: stepName: {}", stepName);
        return getEventEmitter(stepName, vectorSize)->getPublisher();
    }

    virtual kpsr::Subscriber<DataBatchWithId<TypeValue>> *getSubscriber(
        const std::string &stepName, const size_t vectorSize) override
    {
        spdlog::debug("EventEmitterPublishSubscribeFactory::getSubscriber: stepName: {}", stepName);
        return getEventEmitter(stepName, vectorSize)->getSubscriber();
    }

    virtual kpsr::Publisher<DataBatchWithId<std::vector<TypeValue>>> *getPublisherMulti(
        const std::string &stepName, const size_t vectorSize, const size_t multiVectorSize) override
    {
        spdlog::debug("EventEmitterPublishSubscribeFactory::getPublisherMulti: stepName: {}",
                      stepName);
        return getEventEmitterMulti(stepName, vectorSize, multiVectorSize)->getPublisher();
    }

    virtual kpsr::Subscriber<DataBatchWithId<std::vector<TypeValue>>> *getSubscriberMulti(
        const std::string &stepName, const size_t vectorSize, const size_t multiVectorSize) override
    {
        spdlog::debug("EventEmitterPublishSubscribeFactory::getSubscriberMulti: stepName: {}",
                      stepName);
        return getEventEmitterMulti(stepName, vectorSize, multiVectorSize)->getSubscriber();
    }

    virtual void start() override {}

    virtual void stop() override {}

private:
    std::shared_ptr<kpsr::EventEmitterMiddlewareProvider<DataBatchWithId<TypeValue>>> getEventEmitter(
        const std::string &stepName, const size_t vectorSize)
    {
        return _eventEmitterFactory
            ->getEventEmitter<DataBatchWithId<TypeValue>>(stepName,
                                                          [vectorSize](
                                                              DataBatchWithId<TypeValue> &data) {
                                                              data.data->resize(vectorSize);
                                                          });
    }

    std::shared_ptr<kpsr::EventEmitterMiddlewareProvider<DataBatchWithId<std::vector<TypeValue>>>>
    getEventEmitterMulti(const std::string &stepName,
                         const size_t vectorSize,
                         const size_t multiVectorSize)
    {
        return _eventEmitterFactory->getEventEmitter<DataBatchWithId<std::vector<TypeValue>>>(
            stepName, [multiVectorSize, vectorSize](DataBatchWithId<std::vector<TypeValue>> &data) {
                data.data->resize(multiVectorSize);
                for (auto &d : *data.data) {
                    d.resize(vectorSize);
                }
            });
    }

    std::shared_ptr<EventEmitterFactory> _eventEmitterFactory;
};
} // namespace streaming
} // namespace kpsr

#endif // EVENT_EMITTER_PUBLISH_SUBSCRIBE_FACTORY_H