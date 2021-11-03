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

#include <klepsydra/streaming/event_emitter_publish_subscribe_factory.h>

namespace kpsr {
namespace streaming {

EventEmitterPublishSubscribeFactory::EventEmitterPublishSubscribeFactory(kpsr::Container * container, int poolSize, const std::string& name)
    : kpsr::Service(nullptr, name)
    , _container(container)
    , _eventEmitterFactory()
    , _poolSize(poolSize)
{
}

EventEmitterPublishSubscribeFactory::~EventEmitterPublishSubscribeFactory()
{
    _container = nullptr;
}

}
}
