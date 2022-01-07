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
#include <klepsydra/streaming/event_loop_publish_subscribe_factory_float32.h>


namespace kpsr {
namespace streaming {

    EventLoopPublishSubscribeFactoryFloat32::EventLoopPublishSubscribeFactoryFloat32(std::shared_ptr<EventLoopPublishSubscribeFactory> & eventLoopPublishSubscribeFactory)
        : kpsr::Service(nullptr, "EventLoopPubSubFactoryService")
        , _eventLoopPublishSubscribeFactory(eventLoopPublishSubscribeFactory)
    {
    }

    EventLoopPublishSubscribeFactoryFloat32::~EventLoopPublishSubscribeFactoryFloat32()
    {
    }

    kpsr::Publisher<DataBatchWithId<F32AlignedVector>> * EventLoopPublishSubscribeFactoryFloat32::getPublisherF32Aligned(
        const std::string & stepName, const size_t vectorSize) {

        spdlog::debug("EventLoopPublishSubscribeFactoryFloat32::getPublisher: stepName: {}", stepName);
        auto eventLoopName = _eventLoopPublishSubscribeFactory->processStepName(stepName);
        auto eventLoopPtr = _eventLoopPublishSubscribeFactory->getEventLoop(eventLoopName);

        return eventLoopPtr->template getPublisher<DataBatchWithId<F32AlignedVector>>(
            eventLoopName,
            _eventLoopPublishSubscribeFactory->getPoolSize(),
            [vectorSize] (DataBatchWithId<F32AlignedVector> & data) {
                data.data->resize(vectorSize);
            },
            nullptr);
    }

    kpsr::Publisher<DataBatchWithId<std::vector<float>>> * EventLoopPublishSubscribeFactoryFloat32::getPublisherF32(
        const std::string & stepName, const size_t vectorSize) {

        spdlog::debug("EventLoopPublishSubscribeFactory::getPublisher: stepName: {}", stepName);
        auto eventLoopName = _eventLoopPublishSubscribeFactory->processStepName(stepName);
        auto eventLoopPtr = _eventLoopPublishSubscribeFactory->getEventLoop(eventLoopName);

        return eventLoopPtr->template getPublisher<DataBatchWithId<std::vector<float>>>(
            eventLoopName,
            _eventLoopPublishSubscribeFactory->getPoolSize(),
            [vectorSize] (DataBatchWithId<std::vector<float>> & data) {
                data.data->resize(vectorSize);
            },
            nullptr);
    }

    kpsr::Publisher<DataBatchWithId<std::vector<F32AlignedVector>>> * EventLoopPublishSubscribeFactoryFloat32::getPublisherMultiF32Aligned(
            const std::string & stepName, const size_t vectorSize, const size_t multiVectorSize) {

            spdlog::debug("EventLoopPublishSubscribeFactory::getPublisher: stepName: {}", stepName);
            auto eventLoopName = _eventLoopPublishSubscribeFactory->processStepName(stepName);
            auto eventLoopPtr = _eventLoopPublishSubscribeFactory->getEventLoop(eventLoopName);

            return eventLoopPtr->template getPublisher<DataBatchWithId<std::vector<F32AlignedVector>>>(
                eventLoopName,
                _eventLoopPublishSubscribeFactory->getPoolSize(),
                [multiVectorSize, vectorSize] (DataBatchWithId<std::vector<F32AlignedVector>> & data) {
                    data.data->resize(multiVectorSize);
                    for (auto& d: *data.data) {
                        d.resize(vectorSize);
                    }
                },
                nullptr);
    }

    kpsr::Subscriber<DataBatchWithId<F32AlignedVector>> * EventLoopPublishSubscribeFactoryFloat32::getSubscriberF32Aligned(const std::string & stepName, const size_t vectorSize) {
        return _eventLoopPublishSubscribeFactory->getSubscriber<DataBatchWithId<F32AlignedVector>>(stepName);
    }

    kpsr::Subscriber<DataBatchWithId<std::vector<F32AlignedVector>>> * EventLoopPublishSubscribeFactoryFloat32::getSubscriberMultiF32Aligned(const std::string & stepName, const size_t vectorSize, const size_t multiVectorSize) {
        return _eventLoopPublishSubscribeFactory->getSubscriber<DataBatchWithId<std::vector<F32AlignedVector>>>(stepName);
    }

    kpsr::Subscriber<DataBatchWithId<std::vector<float>>> * EventLoopPublishSubscribeFactoryFloat32::getSubscriberF32(const std::string & stepName, const size_t vectorSize) {
        return _eventLoopPublishSubscribeFactory->getSubscriber<DataBatchWithId<std::vector<float>>>(stepName);
    }

    void EventLoopPublishSubscribeFactoryFloat32::start() {
        _eventLoopPublishSubscribeFactory->start();
    }
    
    void EventLoopPublishSubscribeFactoryFloat32::stop() {
        _eventLoopPublishSubscribeFactory->stop();
    }

}
}
