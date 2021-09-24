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

#include <klepsydra/admin/check_license.h>
#include <klepsydra/high_performance/event_loop_middleware_provider.h>
#include <klepsydra/streaming/streaming_policy.h>

#include <klepsydra/streaming/publish_subscribe_factory.h>

namespace kpsr {
namespace streaming {

const int EVENT_LOOP_SIZE = 32;

using FactoryEventLoopType = kpsr::high_performance::EventLoopMiddlewareProvider<EVENT_LOOP_SIZE>;
using EventLoopPtr = std::shared_ptr<FactoryEventLoopType>;

class EventLoopPublishSubscribeFactory : public PublishSubscribeFactory
{
public:
    EventLoopPublishSubscribeFactory(kpsr::Container * container,
                                     StreamingPolicy * streamingPolicy);

    virtual ~EventLoopPublishSubscribeFactory();

    virtual kpsr::Publisher<DataBatchWithId<F32AlignedVector>> * getPublisherF32Aligned(const std::string & stepName, const size_t vectorSize) override;
    virtual kpsr::Subscriber<DataBatchWithId<F32AlignedVector>> * getSubscriberF32Aligned(const std::string & stepName, const size_t vectorSize) override;

    virtual kpsr::Publisher<DataBatchWithId<std::vector<F32AlignedVector>>> * getPublisherMultiF32Aligned(const std::string & stepName, const size_t vectorSize, const size_t multiVectorSize) override;
    virtual kpsr::Subscriber<DataBatchWithId<std::vector<F32AlignedVector>>> * getSubscriberMultiF32Aligned(const std::string & stepName, const size_t vectorSize, const size_t multiVectorSize) override;

    virtual kpsr::Publisher<DataBatchWithId <std::vector<float>>> * getPublisherF32(const std::string & stepName, const size_t vectorSize) override;
    virtual kpsr::Subscriber<DataBatchWithId<std::vector<float>>> * getSubscriberF32(const std::string & stepName, const size_t vectorSize) override;

    void start() override;
    void stop() override;

    const std::vector<EventLoopPtr> getEventLoops();

    const StreamingPolicy * getStreamingPolicy();

private:
    template <class T>
    kpsr::Subscriber<T> * getSubscriber(const std::string& stepName) {
        spdlog::debug("EventLoopPublishSubscribeFactory::getSubscriber: stepName: {}", stepName);
        auto eventLoopName = processStepName(stepName);
        auto eventLoopPtr = getEventLoop(eventLoopName);
        return eventLoopPtr->template getSubscriber<T>(eventLoopName);
    }

    std::string processStepName(const std::string stepName) const;

    EventLoopPtr getEventLoop(const std::string & eventLoopName);


    kpsr::Container * _container;
    StreamingPolicy * _streamingPolicy;
    int _poolSize;
    std::vector<EventLoopPtr> _eventLoops;
};
}
}

#endif // PUBLISH_SUBCRIBE_EVENTLOOP_FACTORY_H
