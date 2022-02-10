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

#include <klepsydra/admin/container_utils.h>
#include <klepsydra/streaming/data_multiplexer_factory_char.h>

namespace kpsr {
namespace streaming {

DataMultiplexerFactoryChar::DataMultiplexerFactoryChar(
    kpsr::Container *container, StreamingConfigurationManager *streamingConfigurationManager)
    : kpsr::Service(nullptr, "DataMultiplexerFactoryCharService")
    , _container(container)
{
    if (streamingConfigurationManager) {
        _affinityIdFunction = [streamingConfigurationManager](const std::string &name) {
            std::vector<int> cpuAffinity = {};
            cpuAffinity.push_back(streamingConfigurationManager->addStepToEventLoop(name));
            return cpuAffinity;
        };
    }
}

DataMultiplexerFactoryChar::~DataMultiplexerFactoryChar() {}

void DataMultiplexerFactoryChar::start() {}

void DataMultiplexerFactoryChar::stop() {}

kpsr::Publisher<kpsr::streaming::DataBatchWithId<std::vector<char>>>
    *DataMultiplexerFactoryChar::getPublisherChar(const std::string &stepName,
                                                  const size_t vectorSize)
{
    spdlog::debug("DataMultiplexerFactoryChar::getPublisherF32: stepName: {}", stepName);
    auto escapedStepName = kpsr::admin::ContainerUtils::escapedNameForOpenMct(stepName);
    return getDataMultiplexerChar(escapedStepName, vectorSize)->getPublisher();
}

kpsr::Subscriber<kpsr::streaming::DataBatchWithId<std::vector<char>>>
    *DataMultiplexerFactoryChar::getSubscriberChar(const std::string &stepName,
                                                   const size_t vectorSize)
{
    spdlog::debug("DataMultiplexerFactoryChar::getSubscriberF32: stepName: {}", stepName);
    auto escapedStepName = kpsr::admin::ContainerUtils::escapedNameForOpenMct(stepName);
    return getDataMultiplexerChar(escapedStepName, vectorSize)->getSubscriber();
}

kpsr::Publisher<kpsr::streaming::DataBatchWithId<std::vector<unsigned char>>>
    *DataMultiplexerFactoryChar::getPublisherUChar(const std::string &stepName,
                                                   const size_t vectorSize)
{
    spdlog::debug("DataMultiplexerFactoryChar::getPublisherF32: stepName: {}", stepName);
    auto escapedStepName = kpsr::admin::ContainerUtils::escapedNameForOpenMct(stepName);
    return getDataMultiplexerUChar(escapedStepName, vectorSize)->getPublisher();
}

kpsr::Subscriber<kpsr::streaming::DataBatchWithId<std::vector<unsigned char>>>
    *DataMultiplexerFactoryChar::getSubscriberUChar(const std::string &stepName,
                                                    const size_t vectorSize)
{
    spdlog::debug("DataMultiplexerFactoryChar::getSubscriberF32: stepName: {}", stepName);
    auto escapedStepName = kpsr::admin::ContainerUtils::escapedNameForOpenMct(stepName);
    return getDataMultiplexerUChar(escapedStepName, vectorSize)->getSubscriber();
}

DataMultiplexerFactoryChar::DataMultiplexerPtr<std::vector<char>>
DataMultiplexerFactoryChar::getDataMultiplexerChar(const std::string &stepName,
                                                   const size_t vectorSize)
{
    auto multiplexer =
        _dataMultiplexerProviderFactory
            .getDataMultiplexer<DataBatchWithId<std::vector<char>>, DATA_MULTIPLEXER_SIZE>(stepName);
    if (multiplexer) {
        return multiplexer;
    } else {
        return _dataMultiplexerProviderFactory
            .insertMultiplexer<DataBatchWithId<std::vector<char>>, DATA_MULTIPLEXER_SIZE>(
                stepName,
                std::make_shared<DataMultiplexerFactoryChar::DataMultiplexer<std::vector<char>>>(
                    _container,
                    stepName,
                    [vectorSize](kpsr::streaming::DataBatchWithId<std::vector<char>> &data) {
                        data.data->resize(vectorSize);
                    },
                    nullptr,
                    _affinityIdFunction));
    }
}

DataMultiplexerFactoryChar::DataMultiplexerPtr<std::vector<unsigned char>>
DataMultiplexerFactoryChar::getDataMultiplexerUChar(const std::string &stepName,
                                                    const size_t vectorSize)
{
    auto multiplexer =
        _dataMultiplexerProviderFactory
            .getDataMultiplexer<DataBatchWithId<std::vector<unsigned char>>, DATA_MULTIPLEXER_SIZE>(
                stepName);
    if (multiplexer) {
        return multiplexer;
    } else {
        return _dataMultiplexerProviderFactory
            .insertMultiplexer<DataBatchWithId<std::vector<unsigned char>>, DATA_MULTIPLEXER_SIZE>(
                stepName,
                std::make_shared<
                    DataMultiplexerFactoryChar::DataMultiplexer<std::vector<unsigned char>>>(
                    _container,
                    stepName,
                    [vectorSize](kpsr::streaming::DataBatchWithId<std::vector<unsigned char>> &data) {
                        data.data->resize(vectorSize);
                    },
                    nullptr,
                    _affinityIdFunction));
    }
}

} // namespace streaming
} // namespace kpsr
