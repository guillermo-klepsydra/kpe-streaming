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

#ifndef DATA_MULTIPLEXER_PUBLISH_SUBSCRIBE_FACTORY_H
#define DATA_MULTIPLEXER_PUBLISH_SUBSCRIBE_FACTORY_H

#include <klepsydra/core/container.h>
#include <klepsydra/high_performance/data_multiplexer_middleware_provider.h>

#include <klepsydra/admin/container_utils.h>
#include <klepsydra/streaming/data_multiplexer_factory_provider.h>
#include <klepsydra/streaming/publish_subscribe_factory.h>
#include <klepsydra/streaming/stream_internal_types.h>
#include <klepsydra/streaming/streaming_configuration_manager.h>
#include <klepsydra/streaming/streaming_types.h>

namespace kpsr {
namespace streaming {
template<class TypeValue>

class DataMultiplexerPublishSubscribeFactory : public PublishSubscribeFactory<TypeValue>
{
public:
    DataMultiplexerPublishSubscribeFactory(
        kpsr::Container *container, StreamingConfigurationManager *streamingConfigurationManager)
        : kpsr::Service(nullptr, "DataMultiplexerPublishSubscribeFactory")
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

    ~DataMultiplexerPublishSubscribeFactory() {}

    virtual kpsr::Publisher<DataBatchWithId<TypeValue>> *getPublisher(
        const std::string &stepName, const size_t vectorSize) override
    {
        spdlog::debug("DataMultiplexerPublishSubscribeFactory::getPublisher: stepName: {}",
                      stepName);
        auto escapedStepName = kpsr::admin::ContainerUtils::escapedNameForOpenMct(stepName);
        return getDataMultiplexer(escapedStepName, vectorSize)->getPublisher();
    }

    virtual kpsr::Subscriber<DataBatchWithId<TypeValue>> *getSubscriber(
        const std::string &stepName, const size_t vectorSize) override
    {
        spdlog::debug("DataMultiplexerPublishSubscribeFactory::getSubscriber: stepName: {}",
                      stepName);
        auto escapedStepName = kpsr::admin::ContainerUtils::escapedNameForOpenMct(stepName);
        return getDataMultiplexer(escapedStepName, vectorSize)->getSubscriber();
    }

    virtual kpsr::Publisher<DataBatchWithId<std::vector<TypeValue>>> *getPublisherMulti(
        const std::string &stepName, const size_t vectorSize, const size_t multiVectorSize) override
    {
        spdlog::debug("DataMultiplexerPublishSubscribeFactory::getPublisherMulti: stepName: {}",
                      stepName);
        auto escapedStepName = kpsr::admin::ContainerUtils::escapedNameForOpenMct(stepName);
        return getDataMultiplexerMulti(escapedStepName, vectorSize, multiVectorSize)->getPublisher();
    }

    virtual kpsr::Subscriber<DataBatchWithId<std::vector<TypeValue>>> *getSubscriberMulti(
        const std::string &stepName, const size_t vectorSize, const size_t multiVectorSize) override
    {
        spdlog::debug("DataMultiplexerPublishSubscribeFactory::getSubscriberMulti: stepName: {}",
                      stepName);
        auto escapedStepName = kpsr::admin::ContainerUtils::escapedNameForOpenMct(stepName);
        return getDataMultiplexerMulti(escapedStepName, vectorSize, multiVectorSize)->getSubscriber();
    }

protected:
    virtual void start() override {}

    virtual void stop() override {}

private:
    static const int DATA_MULTIPLEXER_SIZE = 8;
    template<class T>
    using DataMultiplexer =
        kpsr::high_performance::DataMultiplexerMiddlewareProvider<kpsr::streaming::DataBatchWithId<T>,
                                                                  DATA_MULTIPLEXER_SIZE>;

    template<class T>
    using DataMultiplexerPtr = std::shared_ptr<DataMultiplexer<T>>;

    DataMultiplexerPtr<TypeValue> getDataMultiplexer(const std::string &stepName,
                                                     const size_t vectorSize)
    {
        auto multiplexer =
            _dataMultiplexerProviderFactory
                .getDataMultiplexer<DataBatchWithId<TypeValue>, DATA_MULTIPLEXER_SIZE>(stepName);
        if (multiplexer) {
            return multiplexer;
        } else {
            return _dataMultiplexerProviderFactory
                .insertMultiplexer<DataBatchWithId<TypeValue>, DATA_MULTIPLEXER_SIZE>(
                    stepName,
                    std::make_shared<DataMultiplexer<TypeValue>>(
                        _container,
                        stepName,
                        [vectorSize](DataBatchWithId<TypeValue> &data) {
                            data.data->resize(vectorSize);
                        },
                        nullptr,
                        _affinityIdFunction));
        }
    }

    DataMultiplexerPtr<std::vector<TypeValue>> getDataMultiplexerMulti(const std::string &stepName,
                                                                       const size_t vectorSize,
                                                                       const size_t multiVectorSize)
    {
        auto multiplexer =
            _dataMultiplexerProviderFactory
                .getDataMultiplexer<DataBatchWithId<std::vector<TypeValue>>, DATA_MULTIPLEXER_SIZE>(
                    stepName);
        if (multiplexer) {
            return multiplexer;
        } else {
            return _dataMultiplexerProviderFactory
                .insertMultiplexer<DataBatchWithId<std::vector<TypeValue>>, DATA_MULTIPLEXER_SIZE>(
                    stepName,
                    std::make_shared<DataMultiplexer<std::vector<TypeValue>>>(
                        _container,
                        stepName,
                        [multiVectorSize,
                         vectorSize](DataBatchWithId<std::vector<TypeValue>> &data) {
                            data.data->resize(multiVectorSize);
                            for (auto &d : *data.data) {
                                d.resize(vectorSize);
                            }
                        },
                        nullptr,
                        _affinityIdFunction));
        }
    }

    kpsr::Container *_container;
    std::function<std::vector<int>(const std::string &)> _affinityIdFunction;
    DataMultiplexerFactoryProvider _dataMultiplexerProviderFactory;
};
} // namespace streaming
} // namespace kpsr

#endif // DATA_MULTIPLEXER_PUBLISH_SUBSCRIBE_FACTORY_H
