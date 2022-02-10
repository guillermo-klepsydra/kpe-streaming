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

#ifndef DATA_MULTIPLEXER_FACTORY_FLOAT32_H
#define DATA_MULTIPLEXER_FACTORY_FLOAT32_H

#include <klepsydra/core/container.h>
#include <klepsydra/high_performance/data_multiplexer_middleware_provider.h>

#include <klepsydra/streaming/data_multiplexer_factory_provider.h>

#include <klepsydra/streaming/publish_subscribe_factory_float32.h>
#include <klepsydra/streaming/stream_internal_types.h>
#include <klepsydra/streaming/streaming_configuration_manager.h>
#include <klepsydra/streaming/streaming_types.h>

namespace kpsr {
namespace streaming {

class DataMultiplexerFactoryFloat32 : public PublishSubscribeFactoryFloat32
{
public:
    DataMultiplexerFactoryFloat32(kpsr::Container *container,
                                  StreamingConfigurationManager *streamingConfigurationManager);

    ~DataMultiplexerFactoryFloat32();

    virtual kpsr::Publisher<DataBatchWithId<kpsr::streaming::F32AlignedVector>>
        *getPublisherF32Aligned(const std::string &stepName, const size_t vectorSize) override;
    virtual kpsr::Subscriber<DataBatchWithId<kpsr::streaming::F32AlignedVector>>
        *getSubscriberF32Aligned(const std::string &stepName, const size_t vectorSize) override;

    virtual kpsr::Publisher<DataBatchWithId<std::vector<kpsr::streaming::F32AlignedVector>>>
        *getPublisherMultiF32Aligned(const std::string &stepName,
                                     const size_t vectorSize,
                                     const size_t multiVectorSize) override;
    virtual kpsr::Subscriber<DataBatchWithId<std::vector<kpsr::streaming::F32AlignedVector>>>
        *getSubscriberMultiF32Aligned(const std::string &stepName,
                                      const size_t vectorSize,
                                      const size_t multiVectorSize) override;

    virtual kpsr::Publisher<DataBatchWithId<std::vector<float>>> *getPublisherF32(
        const std::string &stepName, const size_t vectorSize) override;
    virtual kpsr::Subscriber<DataBatchWithId<std::vector<float>>> *getSubscriberF32(
        const std::string &stepName, const size_t vectorSize) override;

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

    DataMultiplexerPtr<kpsr::streaming::F32AlignedVector> getDataMultiplexerF32A(
        const std::string &stepName, const size_t vectorSize);

    DataMultiplexerPtr<std::vector<float>> getDataMultiplexerF32(const std::string &stepName,
                                                                 const size_t vectorSize);

    DataMultiplexerPtr<std::vector<kpsr::streaming::F32AlignedVector>> getDataMultiplexerMF32A(
        const std::string &stepName, const size_t vectorSize, const size_t multiVectorSize);
    kpsr::Container *_container;
    std::function<std::vector<int>(const std::string &)> _affinityIdFunction;
    DataMultiplexerFactoryProvider _dataMultiplexerProviderFactory;
};
} // namespace streaming
} // namespace kpsr

#endif // DATA_MULTIPLEXER_FACTORY_FLOAT32_H
