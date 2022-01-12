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

#ifndef DATA_MULTIPLEXER_FACTORY_CHAR_H
#define DATA_MULTIPLEXER_FACTORY_CHAR_H

#include <klepsydra/core/container.h>
#include <klepsydra/high_performance/data_multiplexer_middleware_provider.h>

#include <klepsydra/streaming/data_multiplexer_factory_provider.h>

#include <klepsydra/streaming/publish_subscribe_factory_char.h>
#include <klepsydra/streaming/stream_internal_types.h>
#include <klepsydra/streaming/streaming_types.h>

namespace kpsr {
namespace streaming {

class DataMultiplexerFactoryChar : public PublishSubscribeFactoryChar
{
public:
    DataMultiplexerFactoryChar(kpsr::Container *container);

    ~DataMultiplexerFactoryChar();

    virtual kpsr::Publisher<DataBatchWithId<std::vector<char>>> *getPublisherChar(
        const std::string &stepName, const size_t vectorSize) override;
    virtual kpsr::Subscriber<DataBatchWithId<std::vector<char>>> *getSubscriberChar(
        const std::string &stepName, const size_t vectorSize = 0) override;

    virtual kpsr::Publisher<DataBatchWithId<std::vector<unsigned char>>> *getPublisherUChar(
        const std::string &stepName, const size_t vectorSize) override;
    virtual kpsr::Subscriber<DataBatchWithId<std::vector<unsigned char>>> *getSubscriberUChar(
        const std::string &stepName, const size_t vectorSize = 0) override;

protected:
    virtual void start() override;

    virtual void stop() override;

private:
    static const int DATA_MULTIPLEXER_SIZE = 8;

    template<class T>
    using DataMultiplexer =
        kpsr::high_performance::DataMultiplexerMiddlewareProvider<kpsr::streaming::DataBatchWithId<T>,
                                                                  DATA_MULTIPLEXER_SIZE>;

    template<class T>
    using DataMultiplexerPtr = std::shared_ptr<DataMultiplexer<T>>;

    DataMultiplexerPtr<std::vector<char>> getDataMultiplexerChar(const std::string &stepName,
                                                                 const size_t vectorSize);
    DataMultiplexerPtr<std::vector<unsigned char>> getDataMultiplexerUChar(
        const std::string &stepName, const size_t vectorSize);

    kpsr::Container *_container;
    DataMultiplexerFactoryProvider _dataMultiplexerProviderFactory;
};
} // namespace streaming
} // namespace kpsr

#endif // DATA_MULTIPLEXER_FACTORY_CHAR_H
