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

#ifndef EVENT_EMITTER_PUBLISH_SUBSCRIBE_FACTORY_CHAR_H
#define EVENT_EMITTER_PUBLISH_SUBSCRIBE_FACTORY_CHAR_H

#include <klepsydra/streaming/event_emitter_publish_subscribe_factory.h>
#include <klepsydra/streaming/publish_subscribe_factory_char.h>

namespace kpsr {
namespace streaming {

using EventEmitterChar = kpsr::EventEmitterMiddlewareProvider<DataBatchWithId<std::vector<char>>>;

class EventEmitterPublishSubscribeFactoryChar : public PublishSubscribeFactoryChar, EventEmitterPublishSubscribeFactory
{
public:
    EventEmitterPublishSubscribeFactoryChar(kpsr::Container * container, int poolSize);

    virtual ~EventEmitterPublishSubscribeFactoryChar();

    virtual kpsr::Publisher<DataBatchWithId<std::vector<char>>> * getPublisherChar(const std::string & stepName, const size_t vectorSize) override;
    virtual kpsr::Subscriber<DataBatchWithId<std::vector<char>>> * getSubscriberChar(const std::string & stepName, const size_t vectorSize = 0) override;

private:
    std::shared_ptr<EventEmitterChar> getEventEmitterChar(const std::string & stepName, const size_t vectorSize);

};
}
}

#endif // EVENT_EMITTER_PUBLISH_SUBSCRIBE_FACTORY_H
