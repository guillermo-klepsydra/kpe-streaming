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

#include <klepsydra/core/container.h>

#include <klepsydra/streaming/event_emitter_factory.h>

namespace kpsr {
namespace streaming {

class EventEmitterPublishSubscribeFactory
{
public:
    EventEmitterPublishSubscribeFactory(kpsr::Container *container, int poolSize);

    virtual ~EventEmitterPublishSubscribeFactory();

    kpsr::Container *getContainer();
    EventEmitterFactory &getEventEmitterFactory();
    int &getPoolSize();

    void start() {}
    void stop() {}

protected:
    kpsr::Container *_container;
    EventEmitterFactory _eventEmitterFactory;
    int _poolSize;
};
} // namespace streaming
} // namespace kpsr

#endif // EVENT_EMITTER_PUBLISH_SUBSCRIBE_FACTORY_H
