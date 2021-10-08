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

EventEmitterPublishSubscribeFactoryChar::EventEmitterPublishSubscribeFactoryChar(kpsr::Container * container, int poolSize)
    : kpsr::Service(nullptr, "EmitterPubSubFactoryService")
    , EventEmitterPublishSubscribeFactory(container, poolSize, "EmitterPubSubFactoryService")
{
}

EventEmitterPublishSubscribeFactoryChar::~EventEmitterPublishSubscribeFactoryChar()
{
}

std::shared_ptr<EventEmitterChar> EventEmitterPublishSubscribeFactoryChar::getEventEmitterChar(const std::string & stepName, const size_t vectorSize) {
    auto emitter = _eventEmitterFactory.getEventEmitter<DataBatchWithId<std::vector<char>>>(stepName);
    if (emitter) {
        return emitter;
    } else {
        return _eventEmitterFactory.insertEmitter(
            stepName,
            std::make_shared<EventEmitterChar>(
                _container,
                stepName,
                _poolSize,
                [vectorSize] (DataBatchWithId<std::vector<char>> & data) {
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

}
}
