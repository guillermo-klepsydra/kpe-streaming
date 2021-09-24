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

#include <klepsydra/streaming/data_multiplexer_factory_impl.h>

namespace kpsr {
namespace streaming {

    DataMultiplexerFactoryImpl::DataMultiplexerFactoryImpl(kpsr::Container * container)
        : _container(container)
    {}

    DataMultiplexerPtr<kpsr::streaming::F32AlignedVector> DataMultiplexerFactoryImpl::getDataMultiplexerF32A(const std::string& stepName, const size_t vectorSize) {
        auto multiplexer = _dataMultiplexerProviderFactory.getDataMultiplexer<DataBatchWithId<F32AlignedVector>, DATA_MULTIPLEXER_SIZE>(stepName);
        if (multiplexer) {
            return multiplexer;
        } else {
            std::string name = "step_" + std::to_string(_dataMultiplexerProviderFactory.size());
            return  _dataMultiplexerProviderFactory.insertMultiplexer<DataBatchWithId<F32AlignedVector>, DATA_MULTIPLEXER_SIZE>(
                stepName,
                std::make_shared<DataMultiplexer<kpsr::streaming::F32AlignedVector>>(
                    _container,
                    name,
                    [vectorSize] (DataBatchWithId<F32AlignedVector> & data) {
                        data.data->resize(vectorSize);
                    },
                    nullptr));
        }
    }

    DataMultiplexerPtr<std::vector<float>> DataMultiplexerFactoryImpl::getDataMultiplexerF32(const std::string& stepName,
                                                                                             const size_t vectorSize) {
        auto multiplexer = _dataMultiplexerProviderFactory.getDataMultiplexer<DataBatchWithId<std::vector<float>>, DATA_MULTIPLEXER_SIZE>(stepName);
        if (multiplexer) {
            return multiplexer;
        } else {
            std::string name = "step_" + std::to_string(_dataMultiplexerProviderFactory.size());
            return  _dataMultiplexerProviderFactory.insertMultiplexer<DataBatchWithId<std::vector<float>>, DATA_MULTIPLEXER_SIZE>(
                stepName,
                std::make_shared<kpsr::streaming::DataMultiplexer<std::vector<float>>>(
                    _container,
                    name,
                    [vectorSize] (kpsr::streaming::DataBatchWithId<std::vector<float>> & data) {
                        data.data->resize(vectorSize);
                    },
                    nullptr));
        }
    }

    DataMultiplexerPtr<std::vector<kpsr::streaming::F32AlignedVector>> DataMultiplexerFactoryImpl::getDataMultiplexerMF32A(const std::string& stepName,
                                                                                                 const size_t vectorSize,
                                                                                                 const size_t multiVectorSize) {
        auto multiplexer = _dataMultiplexerProviderFactory.getDataMultiplexer<DataBatchWithId<std::vector<F32AlignedVector>>, DATA_MULTIPLEXER_SIZE>(stepName);
        if (multiplexer) {
            return multiplexer;
        } else {
            std::string name = "step_" + std::to_string(_dataMultiplexerProviderFactory.size());
            return  _dataMultiplexerProviderFactory.insertMultiplexer<DataBatchWithId<std::vector<F32AlignedVector>>, DATA_MULTIPLEXER_SIZE>(
                stepName,
                std::make_shared<kpsr::streaming::DataMultiplexer<std::vector<F32AlignedVector>>>(
                    _container,
                    name,
                    [multiVectorSize, vectorSize] (DataBatchWithId<std::vector<F32AlignedVector>> & data) {
                        data.data->resize(multiVectorSize);
                        for (auto& d : *data.data) {
                            d.resize(vectorSize);
                        }
                    },
                    nullptr));
        }
    }

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>> * DataMultiplexerFactoryImpl::getPublisherF32Aligned(const std::string & stepName, const size_t vectorSize) {
        spdlog::debug("DataMultiplexerFactory::getPublisherF32Aligned: stepName: {}", stepName);
        return getDataMultiplexerF32A(stepName, vectorSize)->getPublisher();
    }

    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>> * DataMultiplexerFactoryImpl::getSubscriberF32Aligned(const std::string& stepName, const size_t vectorSize) {
        spdlog::debug("DataMultiplexerFactory::getSubscriberF32Aligned: stepName: {}", stepName);
        return getDataMultiplexerF32A(stepName, vectorSize)->getSubscriber();
    }


    kpsr::Publisher<kpsr::streaming::DataBatchWithId<std::vector<kpsr::streaming::F32AlignedVector>>> * DataMultiplexerFactoryImpl::getPublisherMultiF32Aligned(const std::string & stepName, const size_t vectorSize, const size_t multiVectorSize) {
        spdlog::debug("DataMultiplexerFactory::getPublisherMF32Aligned: stepName: {}", stepName);
        return getDataMultiplexerMF32A(stepName, vectorSize, multiVectorSize)->getPublisher();
    }

    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<std::vector<kpsr::streaming::F32AlignedVector>>> * DataMultiplexerFactoryImpl::getSubscriberMultiF32Aligned(const std::string & stepName, const size_t vectorSize, const size_t multiVectorSize) {
        spdlog::debug("DataMultiplexerFactory::getSubscriberMF32Aligned: stepName: {}", stepName);
        return getDataMultiplexerMF32A(stepName, vectorSize, multiVectorSize)->getSubscriber();
    }

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<std::vector<float>>> * DataMultiplexerFactoryImpl::getPublisherF32(const std::string & stepName, const size_t vectorSize) {
        spdlog::debug("DataMultiplexerFactory::getPublisherF32: stepName: {}", stepName);
        return getDataMultiplexerF32(stepName, vectorSize)->getPublisher();
    }

    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<std::vector<float>>> * DataMultiplexerFactoryImpl::getSubscriberF32(const std::string & stepName, const size_t vectorSize) {
        spdlog::debug("DataMultiplexerFactory::getSubscriberF32: stepName: {}", stepName);
        return getDataMultiplexerF32(stepName, vectorSize)->getSubscriber();
    }
}
}
