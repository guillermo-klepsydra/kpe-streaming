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

#include <klepsydra/streaming/data_multiplexer_factory_char.h>

namespace kpsr {
namespace streaming {

    DataMultiplexerFactoryChar::DataMultiplexerFactoryChar(kpsr::Container * container)
        : kpsr::Service(nullptr, "DataMultiplexerFactoryCharService")
        , _container(container)
    {}

    DataMultiplexerFactoryChar::~DataMultiplexerFactoryChar() {
    }

    void DataMultiplexerFactoryChar::start() {
    }

    void DataMultiplexerFactoryChar::stop() {
    }

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<std::vector<char>>> * DataMultiplexerFactoryChar::getPublisherChar(const std::string & stepName, const size_t vectorSize) {
        spdlog::debug("DataMultiplexerFactoryChar::getPublisherF32: stepName: {}", stepName);
        return getDataMultiplexerChar(stepName, vectorSize)->getPublisher();
    }

    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<std::vector<char>>> * DataMultiplexerFactoryChar::getSubscriberChar(const std::string & stepName, const size_t vectorSize) {
        spdlog::debug("DataMultiplexerFactoryChar::getSubscriberF32: stepName: {}", stepName);
        return getDataMultiplexerChar(stepName, vectorSize)->getSubscriber();
    }

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<std::vector<unsigned char>>> * DataMultiplexerFactoryChar::getPublisherUChar(const std::string & stepName, const size_t vectorSize) {
        spdlog::debug("DataMultiplexerFactoryChar::getPublisherF32: stepName: {}", stepName);
        return getDataMultiplexerUChar(stepName, vectorSize)->getPublisher();
    }

    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<std::vector<unsigned char>>> * DataMultiplexerFactoryChar::getSubscriberUChar(const std::string & stepName, const size_t vectorSize) {
        spdlog::debug("DataMultiplexerFactoryChar::getSubscriberF32: stepName: {}", stepName);
        return getDataMultiplexerUChar(stepName, vectorSize)->getSubscriber();
    }

    DataMultiplexerFactoryChar::DataMultiplexerPtr<std::vector<char>> DataMultiplexerFactoryChar::getDataMultiplexerChar(const std::string& stepName,
                                                                                                                         const size_t vectorSize) {
        auto multiplexer = _dataMultiplexerProviderFactory.getDataMultiplexer<DataBatchWithId<std::vector<char>>, DATA_MULTIPLEXER_SIZE>(stepName);
        if (multiplexer) {
            return multiplexer;
        } else {
            return  _dataMultiplexerProviderFactory.insertMultiplexer<DataBatchWithId<std::vector<char>>, DATA_MULTIPLEXER_SIZE>(
                stepName,
                std::make_shared<DataMultiplexerFactoryChar::DataMultiplexer<std::vector<char>>>(
                    _container,
                    stepName,
                    [vectorSize] (kpsr::streaming::DataBatchWithId<std::vector<char>> & data) {
                        data.data->resize(vectorSize);
                    },
                    nullptr));
        }
    }

    DataMultiplexerFactoryChar::DataMultiplexerPtr<std::vector<unsigned char>> DataMultiplexerFactoryChar::getDataMultiplexerUChar(const std::string& stepName,
                                                                                                                                   const size_t vectorSize) {
        auto multiplexer = _dataMultiplexerProviderFactory.getDataMultiplexer<DataBatchWithId<std::vector<unsigned char>>, DATA_MULTIPLEXER_SIZE>(stepName);
        if (multiplexer) {
            return multiplexer;
        } else {
            return  _dataMultiplexerProviderFactory.insertMultiplexer<DataBatchWithId<std::vector<unsigned char>>, DATA_MULTIPLEXER_SIZE>(
                stepName,
                std::make_shared<DataMultiplexerFactoryChar::DataMultiplexer<std::vector<unsigned char>>>(
                    _container,
                    stepName,
                    [vectorSize] (kpsr::streaming::DataBatchWithId<std::vector<unsigned char>> & data) {
                        data.data->resize(vectorSize);
                    },
                    nullptr));
        }
    }

}
}
