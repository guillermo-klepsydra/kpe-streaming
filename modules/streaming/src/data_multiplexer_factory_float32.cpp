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

#include <klepsydra/streaming/data_multiplexer_factory_float32.h>

namespace kpsr {
namespace streaming {

DataMultiplexerFactoryFloat32::DataMultiplexerFactoryFloat32(kpsr::Container *container)
    : kpsr::Service(nullptr, "DataMultiplexerFactoryFloat32Service")
    , _container(container)
{}

DataMultiplexerFactoryFloat32::~DataMultiplexerFactoryFloat32() {}

DataMultiplexerFactoryFloat32::DataMultiplexerPtr<kpsr::streaming::F32AlignedVector>
DataMultiplexerFactoryFloat32::getDataMultiplexerF32A(const std::string &stepName,
                                                      const size_t vectorSize)
{
    auto multiplexer =
        _dataMultiplexerProviderFactory
            .getDataMultiplexer<DataBatchWithId<F32AlignedVector>, DATA_MULTIPLEXER_SIZE>(stepName);
    if (multiplexer) {
        return multiplexer;
    } else {
        return _dataMultiplexerProviderFactory.insertMultiplexer<DataBatchWithId<F32AlignedVector>,
                                                                 DATA_MULTIPLEXER_SIZE>(
            stepName,
            std::make_shared<
                DataMultiplexerFactoryFloat32::DataMultiplexer<kpsr::streaming::F32AlignedVector>>(
                _container,
                stepName,
                [vectorSize](DataBatchWithId<F32AlignedVector> &data) {
                    data.data->resize(vectorSize);
                },
                nullptr));
    }
}

DataMultiplexerFactoryFloat32::DataMultiplexerPtr<std::vector<float>>
DataMultiplexerFactoryFloat32::getDataMultiplexerF32(const std::string &stepName,
                                                     const size_t vectorSize)
{
    auto multiplexer =
        _dataMultiplexerProviderFactory
            .getDataMultiplexer<DataBatchWithId<std::vector<float>>, DATA_MULTIPLEXER_SIZE>(
                stepName);
    if (multiplexer) {
        return multiplexer;
    } else {
        return _dataMultiplexerProviderFactory
            .insertMultiplexer<DataBatchWithId<std::vector<float>>, DATA_MULTIPLEXER_SIZE>(
                stepName,
                std::make_shared<DataMultiplexerFactoryFloat32::DataMultiplexer<std::vector<float>>>(
                    _container,
                    stepName,
                    [vectorSize](kpsr::streaming::DataBatchWithId<std::vector<float>> &data) {
                        data.data->resize(vectorSize);
                    },
                    nullptr));
    }
}

DataMultiplexerFactoryFloat32::DataMultiplexerPtr<std::vector<kpsr::streaming::F32AlignedVector>>
DataMultiplexerFactoryFloat32::getDataMultiplexerMF32A(const std::string &stepName,
                                                       const size_t vectorSize,
                                                       const size_t multiVectorSize)
{
    auto multiplexer = _dataMultiplexerProviderFactory
                           .getDataMultiplexer<DataBatchWithId<std::vector<F32AlignedVector>>,
                                               DATA_MULTIPLEXER_SIZE>(stepName);
    if (multiplexer) {
        return multiplexer;
    } else {
        return _dataMultiplexerProviderFactory
            .insertMultiplexer<DataBatchWithId<std::vector<F32AlignedVector>>, DATA_MULTIPLEXER_SIZE>(
                stepName,
                std::make_shared<
                    DataMultiplexerFactoryFloat32::DataMultiplexer<std::vector<F32AlignedVector>>>(
                    _container,
                    stepName,
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

kpsr::Publisher<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>
    *DataMultiplexerFactoryFloat32::getPublisherF32Aligned(const std::string &stepName,
                                                           const size_t vectorSize)
{
    spdlog::debug("DataMultiplexerFactoryFloat32::getPublisherF32Aligned: stepName: {}", stepName);
    return getDataMultiplexerF32A(stepName, vectorSize)->getPublisher();
}

kpsr::Subscriber<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>
    *DataMultiplexerFactoryFloat32::getSubscriberF32Aligned(const std::string &stepName,
                                                            const size_t vectorSize)
{
    spdlog::debug("DataMultiplexerFactoryFloat32::getSubscriberF32Aligned: stepName: {}", stepName);
    return getDataMultiplexerF32A(stepName, vectorSize)->getSubscriber();
}

kpsr::Publisher<kpsr::streaming::DataBatchWithId<std::vector<kpsr::streaming::F32AlignedVector>>>
    *DataMultiplexerFactoryFloat32::getPublisherMultiF32Aligned(const std::string &stepName,
                                                                const size_t vectorSize,
                                                                const size_t multiVectorSize)
{
    spdlog::debug("DataMultiplexerFactoryFloat32::getPublisherMF32Aligned: stepName: {}", stepName);
    return getDataMultiplexerMF32A(stepName, vectorSize, multiVectorSize)->getPublisher();
}

kpsr::Subscriber<kpsr::streaming::DataBatchWithId<std::vector<kpsr::streaming::F32AlignedVector>>>
    *DataMultiplexerFactoryFloat32::getSubscriberMultiF32Aligned(const std::string &stepName,
                                                                 const size_t vectorSize,
                                                                 const size_t multiVectorSize)
{
    spdlog::debug("DataMultiplexerFactoryFloat32::getSubscriberMF32Aligned: stepName: {}", stepName);
    return getDataMultiplexerMF32A(stepName, vectorSize, multiVectorSize)->getSubscriber();
}

kpsr::Publisher<kpsr::streaming::DataBatchWithId<std::vector<float>>>
    *DataMultiplexerFactoryFloat32::getPublisherF32(const std::string &stepName,
                                                    const size_t vectorSize)
{
    spdlog::debug("DataMultiplexerFactoryFloat32::getPublisherF32: stepName: {}", stepName);
    return getDataMultiplexerF32(stepName, vectorSize)->getPublisher();
}

kpsr::Subscriber<kpsr::streaming::DataBatchWithId<std::vector<float>>>
    *DataMultiplexerFactoryFloat32::getSubscriberF32(const std::string &stepName,
                                                     const size_t vectorSize)
{
    spdlog::debug("DataMultiplexerFactoryFloat32::getSubscriberF32: stepName: {}", stepName);
    return getDataMultiplexerF32(stepName, vectorSize)->getSubscriber();
}

void DataMultiplexerFactoryFloat32::start() {}

void DataMultiplexerFactoryFloat32::stop() {}
} // namespace streaming
} // namespace kpsr
