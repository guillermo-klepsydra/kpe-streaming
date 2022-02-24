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

#ifndef EVENTLOOP_PUBLISH_SUBSCRIBE_FACTORY_H
#define EVENTLOOP_PUBLISH_SUBSCRIBE_FACTORY_H

#include <klepsydra/high_performance/event_loop_middleware_provider.h>

#include <klepsydra/streaming/event_loop_factory.h>
#include <klepsydra/streaming/publish_subscribe_factory.h>

namespace kpsr {
namespace streaming {
template<class TypeValue>

class EventLoopPublishSubscribeFactory : public PublishSubscribeFactory<TypeValue>
{
public:
    EventLoopPublishSubscribeFactory(std::shared_ptr<EventLoopFactory> &eventLoopFactory)
        : kpsr::Service(nullptr, "EventLoopPubSubFactoryService")
        , _eventLoopFactory(eventLoopFactory)
    {}

    virtual ~EventLoopPublishSubscribeFactory() {}

    virtual kpsr::Publisher<DataBatchWithId<TypeValue>> *getPublisher(
        const std::string &stepName, const size_t vectorSize) override
    {
        return _eventLoopFactory
            ->getPublisher<DataBatchWithId<TypeValue>>(stepName,
                                                       [vectorSize](
                                                           DataBatchWithId<TypeValue> &data) {
                                                           data.data->resize(vectorSize);
                                                       });
    }

    virtual kpsr::Subscriber<DataBatchWithId<TypeValue>> *getSubscriber(
        const std::string &stepName, const size_t vectorSize) override
    {
        return _eventLoopFactory->getSubscriber<DataBatchWithId<TypeValue>>(stepName);
    }

    virtual kpsr::Publisher<DataBatchWithId<std::vector<TypeValue>>> *getPublisherMulti(
        const std::string &stepName, const size_t vectorSize, const size_t multiVectorSize) override
    {
        return _eventLoopFactory->getPublisher<DataBatchWithId<std::vector<TypeValue>>>(
            stepName, [multiVectorSize, vectorSize](DataBatchWithId<std::vector<TypeValue>> &data) {
                data.data->resize(multiVectorSize);
                for (auto &d : *data.data) {
                    d.resize(vectorSize);
                }
            });
    }

    virtual kpsr::Subscriber<DataBatchWithId<std::vector<TypeValue>>> *getSubscriberMulti(
        const std::string &stepName, const size_t vectorSize, const size_t multiVectorSize) override
    {
        return _eventLoopFactory->getSubscriber<DataBatchWithId<std::vector<TypeValue>>>(stepName);
    }

    virtual void start() override { _eventLoopFactory->start(); }

    virtual void stop() override { _eventLoopFactory->stop(); }

private:
    std::shared_ptr<EventLoopFactory> _eventLoopFactory;
};
} // namespace streaming
} // namespace kpsr

#endif // EVENTLOOP_PUBLISH_SUBSCRIBE_FACTORY_H