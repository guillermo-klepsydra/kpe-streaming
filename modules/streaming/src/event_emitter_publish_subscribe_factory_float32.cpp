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

#include <klepsydra/streaming/event_emitter_publish_subscribe_factory_float32.h>

namespace kpsr {
namespace streaming {

EventEmitterPublishSubscribeFactoryFloat32::EventEmitterPublishSubscribeFactoryFloat32(
    std::shared_ptr<EventEmitterPublishSubscribeFactory> &eventEmitterPublishSubscribeFactory)
    : kpsr::Service(nullptr, "EmitterPubSubFactoryService")
    , _eventEmitterPublishSubscribeFactory(eventEmitterPublishSubscribeFactory)
{}

EventEmitterPublishSubscribeFactoryFloat32::~EventEmitterPublishSubscribeFactoryFloat32() {}

std::shared_ptr<EventEmitterF32A> EventEmitterPublishSubscribeFactoryFloat32::getEventEmitterF32A(
    const std::string &stepName, const size_t vectorSize)
{
    auto &eventEmitterFactory = _eventEmitterPublishSubscribeFactory->getEventEmitterFactory();
    auto emitter = eventEmitterFactory.getEventEmitter<DataBatchWithId<F32AlignedVector>>(stepName);
    if (emitter) {
        return emitter;
    } else {
        return eventEmitterFactory
            .insertEmitter(stepName,
                           std::make_shared<EventEmitterF32A>(
                               _eventEmitterPublishSubscribeFactory->getContainer(),
                               stepName,
                               _eventEmitterPublishSubscribeFactory->getPoolSize(),
                               [vectorSize](DataBatchWithId<F32AlignedVector> &data) {
                                   data.data->resize(vectorSize);
                               },
                               nullptr));
    }
}

std::shared_ptr<EventEmitterMF32A> EventEmitterPublishSubscribeFactoryFloat32::getEventEmitterMF32A(
    const std::string &stepName, const size_t vectorSize, const size_t multiVectorSize)
{
    auto &eventEmitterFactory = _eventEmitterPublishSubscribeFactory->getEventEmitterFactory();
    auto emitter = eventEmitterFactory
                       .getEventEmitter<DataBatchWithId<std::vector<F32AlignedVector>>>(stepName);
    if (emitter) {
        return emitter;
    } else {
        return eventEmitterFactory
            .insertEmitter(stepName,
                           std::make_shared<EventEmitterMF32A>(
                               _eventEmitterPublishSubscribeFactory->getContainer(),
                               stepName,
                               _eventEmitterPublishSubscribeFactory->getPoolSize(),
                               [multiVectorSize,
                                vectorSize](DataBatchWithId<std::vector<F32AlignedVector>> &data) {
                                   data.data->resize(multiVectorSize);
                                   for (auto &d : *data.data) {
                                       d.resize(vectorSize);
                                   }
                               },
                               nullptr));
    }
}

std::shared_ptr<EventEmitterF32> EventEmitterPublishSubscribeFactoryFloat32::getEventEmitterF32(
    const std::string &stepName, const size_t vectorSize)
{
    auto &eventEmitterFactory = _eventEmitterPublishSubscribeFactory->getEventEmitterFactory();
    auto emitter = eventEmitterFactory.getEventEmitter<DataBatchWithId<std::vector<float>>>(
        stepName);
    if (emitter) {
        return emitter;
    } else {
        return eventEmitterFactory
            .insertEmitter(stepName,
                           std::make_shared<EventEmitterF32>(
                               _eventEmitterPublishSubscribeFactory->getContainer(),
                               stepName,
                               _eventEmitterPublishSubscribeFactory->getPoolSize(),
                               [vectorSize](DataBatchWithId<std::vector<float>> &data) {
                                   data.data->resize(vectorSize);
                               },
                               nullptr));
    }
}

kpsr::Publisher<DataBatchWithId<F32AlignedVector>>
    *EventEmitterPublishSubscribeFactoryFloat32::getPublisherF32Aligned(const std::string &stepName,
                                                                        const size_t vectorSize)
{
    spdlog::debug(
        "EventEmitterPublishSubscribeFactoryFloat32::getPublisherF32Aligned: stepName: {}",
        stepName);
    return getEventEmitterF32A(stepName, vectorSize)->getPublisher();
}

kpsr::Subscriber<DataBatchWithId<F32AlignedVector>>
    *EventEmitterPublishSubscribeFactoryFloat32::getSubscriberF32Aligned(const std::string &stepName,
                                                                         const size_t vectorSize)
{
    spdlog::debug(
        "EventEmitterPublishSubscribeFactoryFloat32::getSubscriberF32Aligned: stepName: {}",
        stepName);
    return getEventEmitterF32A(stepName, vectorSize)->getSubscriber();
}

kpsr::Publisher<DataBatchWithId<std::vector<F32AlignedVector>>>
    *EventEmitterPublishSubscribeFactoryFloat32::getPublisherMultiF32Aligned(
        const std::string &stepName, const size_t vectorSize, const size_t multiVectorSize)
{
    spdlog::debug(
        "EventEmitterPublishSubscribeFactoryFloat32::getPublisherMF32Aligned: stepName: {}",
        stepName);
    return getEventEmitterMF32A(stepName, vectorSize, multiVectorSize)->getPublisher();
}

kpsr::Subscriber<DataBatchWithId<std::vector<F32AlignedVector>>>
    *EventEmitterPublishSubscribeFactoryFloat32::getSubscriberMultiF32Aligned(
        const std::string &stepName, const size_t vectorSize, const size_t multiVectorSize)
{
    spdlog::debug(
        "EventEmitterPublishSubscribeFactoryFloat32::getSubscriberMF32Aligned: stepName: {}",
        stepName);
    return getEventEmitterMF32A(stepName, vectorSize, multiVectorSize)->getSubscriber();
}

kpsr::Publisher<DataBatchWithId<std::vector<float>>>
    *EventEmitterPublishSubscribeFactoryFloat32::getPublisherF32(const std::string &stepName,
                                                                 const size_t vectorSize)
{
    spdlog::debug("EventEmitterPublishSubscribeFactoryFloat32::getPublisherF32: stepName: {}",
                  stepName);
    return getEventEmitterF32(stepName, vectorSize)->getPublisher();
}

kpsr::Subscriber<DataBatchWithId<std::vector<float>>>
    *EventEmitterPublishSubscribeFactoryFloat32::getSubscriberF32(const std::string &stepName,
                                                                  const size_t vectorSize)
{
    spdlog::debug("EventEmitterPublishSubscribeFactoryFloat32::getSubscriberF32: stepName: {}",
                  stepName);
    return getEventEmitterF32(stepName, vectorSize)->getSubscriber();
}

void EventEmitterPublishSubscribeFactoryFloat32::start()
{
    _eventEmitterPublishSubscribeFactory->start();
}

void EventEmitterPublishSubscribeFactoryFloat32::stop()
{
    _eventEmitterPublishSubscribeFactory->stop();
}

} // namespace streaming
} // namespace kpsr