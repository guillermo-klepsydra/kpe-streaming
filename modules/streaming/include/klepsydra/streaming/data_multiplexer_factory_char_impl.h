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

#ifndef DATA_MULTIPLEXER_FACTORY_CHAR_IMPL_H
#define DATA_MULTIPLEXER_FACTORY_CHAR_IMPL_H

#include <klepsydra/core/container.h>
#include <klepsydra/high_performance/data_multiplexer_middleware_provider.h>

#include <klepsydra/streaming/data_multiplexer_factory_char.h>
#include <klepsydra/streaming/data_multiplexer_factory_provider.h>

namespace kpsr {
namespace streaming {

class DataMultiplexerFactoryCharImpl : public DataMultiplexerFactoryChar
{
public:
    DataMultiplexerFactoryCharImpl(kpsr::Container * container)
        : kpsr::Service(nullptr, "DataMultiplexerFactoryFloat32Service")
        , _container(container)
        {}

    virtual kpsr::Publisher<DataBatchWithId<std::vector<char>>> * getPublisherChar(const std::string & stepName, const size_t vectorSize) override {
        return nullptr;
    }
    virtual kpsr::Subscriber<DataBatchWithId<std::vector<char>>> * getSubscriberChar(const std::string & stepName, const size_t vectorSize = 0) override {
        return nullptr;
    }

private:

    kpsr::Container * _container;
    DataMultiplexerFactoryProvider _dataMultiplexerProviderFactory;
};
}
}

#endif // DATA_MULTIPLEXER_FACTORY_IMPL_H
