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

#include <klepsydra/streaming/event_emitter_publish_subscribe_factory_char.h>

namespace kpsr {
namespace streaming {

EventEmitterPublishSubscribeFactoryChar::EventEmitterPublishSubscribeFactoryChar(
    std::shared_ptr<EventEmitterPublishSubscribeFactory> & eventEmitterPublishSubscribeFactory)
    : kpsr::Service(nullptr, "EmitterPubSubFactoryService")
    , _eventEmitterPublishSubscribeFactory(eventEmitterPublishSubscribeFactory)
{
}

EventEmitterPublishSubscribeFactoryChar::~EventEmitterPublishSubscribeFactoryChar()
{
}

std::shared_ptr<EventEmitterPublishSubscribeFactoryChar::EventEmitterChar> EventEmitterPublishSubscribeFactoryChar::getEventEmitterChar(const std::string & stepName, const size_t vectorSize) {
    auto eventEmitterFactory = _eventEmitterPublishSubscribeFactory->getEventEmitterFactory();
    auto emitter = eventEmitterFactory.getEventEmitter<DataBatchWithId<std::vector<char>>>(stepName);
    if (emitter) {
        return emitter;
    } else {
        return eventEmitterFactory.insertEmitter(
            stepName,
            std::make_shared<EventEmitterChar>(
                _eventEmitterPublishSubscribeFactory->getContainer(),
                stepName,
                _eventEmitterPublishSubscribeFactory->getPoolSize(),
                [vectorSize] (DataBatchWithId<std::vector<char>> & data) {
                    data.data->resize(vectorSize);
                },
                nullptr));
    }
}

std::shared_ptr<EventEmitterPublishSubscribeFactoryChar::EventEmitterUChar> EventEmitterPublishSubscribeFactoryChar::getEventEmitterUChar(const std::string & stepName, const size_t vectorSize) {
    auto eventEmitterFactory = _eventEmitterPublishSubscribeFactory->getEventEmitterFactory();
    auto emitter = eventEmitterFactory.getEventEmitter<DataBatchWithId<std::vector<unsigned char>>>(stepName);
    if (emitter) {
        return emitter;
    } else {
        return eventEmitterFactory.insertEmitter(
            stepName,
            std::make_shared<EventEmitterUChar>(
                _eventEmitterPublishSubscribeFactory->getContainer(),
                stepName,
                _eventEmitterPublishSubscribeFactory->getPoolSize(),
                [vectorSize] (DataBatchWithId<std::vector<unsigned char>> & data) {
                    data.data->resize(vectorSize);
                },
                nullptr));
    }
}

kpsr::Publisher<DataBatchWithId<std::vector<char>>> * EventEmitterPublishSubscribeFactoryChar::getPublisherChar(const std::string & stepName, const size_t vectorSize) {
    spdlog::debug("EventEmitterPublishSubscribeFactoryChar::getPublisherChar: stepName: {}", stepName);
    return getEventEmitterChar(stepName, vectorSize)->getPublisher();
}

kpsr::Subscriber<DataBatchWithId<std::vector<char>>> * EventEmitterPublishSubscribeFactoryChar::getSubscriberChar(const std::string & stepName, const size_t vectorSize) {
    spdlog::debug("EventEmitterPublishSubscribeFactoryChar::getSubscriberChar: stepName: {}", stepName);
    return getEventEmitterChar(stepName, vectorSize)->getSubscriber();
}

kpsr::Publisher<DataBatchWithId<std::vector<unsigned char>>> * EventEmitterPublishSubscribeFactoryChar::getPublisherUChar(const std::string & stepName, const size_t vectorSize) {
    spdlog::debug("EventEmitterPublishSubscribeFactoryChar::getPublisherChar: stepName: {}", stepName);
    return getEventEmitterUChar(stepName, vectorSize)->getPublisher();
}

kpsr::Subscriber<DataBatchWithId<std::vector<unsigned char>>> * EventEmitterPublishSubscribeFactoryChar::getSubscriberUChar(const std::string & stepName, const size_t vectorSize) {
    spdlog::debug("EventEmitterPublishSubscribeFactoryChar::getSubscriberChar: stepName: {}", stepName);
    return getEventEmitterUChar(stepName, vectorSize)->getSubscriber();
}

void EventEmitterPublishSubscribeFactoryChar::start() {
    _eventEmitterPublishSubscribeFactory->start();
}

void EventEmitterPublishSubscribeFactoryChar::stop() {
    _eventEmitterPublishSubscribeFactory->stop();
}

}
}