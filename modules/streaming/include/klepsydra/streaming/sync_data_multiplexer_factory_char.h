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

#ifndef DATA_MULTIPLEXER_FACTORY_TEST_CHAR_H
#define DATA_MULTIPLEXER_FACTORY_TEST_CHAR_H

#include <klepsydra/core/container.h>
#include <klepsydra/core/event_emitter_middleware_provider.h>

#include <klepsydra/streaming/data_multiplexer_factory_char.h>
#include <klepsydra/streaming/event_emitter_publish_subscribe_factory_char.h>

namespace kpsr {
namespace streaming {

class SyncDataMultiplexerFactoryChar : public DataMultiplexerFactoryChar
{
public:
    SyncDataMultiplexerFactoryChar(kpsr::Container * container)
        : kpsr::Service(nullptr, "syncDataMuxFactoryChar")
        , _testFactory(container, 0)
        {}
    virtual kpsr::Publisher<DataBatchWithId<std::vector<char>>> * getPublisherChar(const std::string & stepName, const size_t vectorSize) override {
        return nullptr;
    }
    virtual kpsr::Subscriber<DataBatchWithId<std::vector<char>>> * getSubscriberChar(const std::string & stepName, const size_t vectorSize = 0) override {
        return nullptr;
    }
private:
    EventEmitterPublishSubscribeFactoryFloat32 _testFactory;
};
}
}

#endif // DATA_MULTIPLEXER_FACTORY_TEST_CHAR_H
