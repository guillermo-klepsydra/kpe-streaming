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

#ifndef SMART_PUBLISH_SUBSCRIBE_FACTORY_H
#define SMART_PUBLISH_SUBSCRIBE_FACTORY_H

#include <klepsydra/streaming/data_multiplexer_publish_subscribe_factory.h>
#include <klepsydra/streaming/event_emitter_publish_subscribe_factory.h>
#include <klepsydra/streaming/event_loop_publish_subscribe_factory.h>
#include <klepsydra/streaming/publish_subscribe_factory.h>

#include <klepsydra/core/container.h>
#include <klepsydra/streaming/streaming_configuration_manager.h>
#include <klepsydra/streaming/thread_distribution_policy.h>

namespace kpsr {
namespace streaming {
template<class TypeValue>

class SmartPublishSubscribeFactory : public PublishSubscribeFactory<TypeValue>
{
public:
    SmartPublishSubscribeFactory(std::shared_ptr<EventEmitterFactory> &eventEmitterFactory,
                                 std::shared_ptr<EventLoopFactory> &eventLoopFactory,
                                 kpsr::Container *container,
                                 StreamingConfigurationManager *streamingConfigurationManager,
                                 bool useTestFactory = false)
        : kpsr::Service(nullptr, "SmartPublishSubscribeFactoryService")
        , _useTestFactory(useTestFactory)
        , _streamingConfigurationManager(streamingConfigurationManager)
        , _eventEmitterPublishSubscribeFactory(
              std::make_unique<EventEmitterPublishSubscribeFactory<TypeValue>>(eventEmitterFactory))
        , _eventLoopPublishSubscribeFactory(
              std::make_unique<EventLoopPublishSubscribeFactory<TypeValue>>(eventLoopFactory))
        , _dataMultiplexerPublishSubscribeFactory(
              std::make_unique<DataMultiplexerPublishSubscribeFactory<TypeValue>>(
                  container, _streamingConfigurationManager))
    {}

    virtual ~SmartPublishSubscribeFactory() {}

    virtual kpsr::Publisher<DataBatchWithId<TypeValue>> *getPublisher(
        const std::string &stepName, const size_t vectorSize) override
    {
        if (_useTestFactory) {
            return _eventEmitterPublishSubscribeFactory->getPublisher(stepName, vectorSize);
        } else {
            auto publisherFactoryType = _streamingConfigurationManager
                                            ->getThreadDistributionPolicy()
                                            ->getFactoryType(stepName);
            switch (publisherFactoryType) {
            case kpsr::streaming::FactoryType::EventEmitter: {
                return _eventEmitterPublishSubscribeFactory->getPublisher(stepName, vectorSize);
            }

            case FactoryType::EventLoop: {
                return _eventLoopPublishSubscribeFactory->getPublisher(stepName, vectorSize);
            }

            case FactoryType::DataMultiplexer: {
                return _dataMultiplexerPublishSubscribeFactory->getPublisher(stepName, vectorSize);
            }

            default: {
                spdlog::warn("The Factory Type does not exist.");
                return nullptr;
            }
            }
        }
    }

    virtual kpsr::Subscriber<DataBatchWithId<TypeValue>> *getSubscriber(
        const std::string &stepName, const size_t vectorSize) override
    {
        if (_useTestFactory) {
            return _eventEmitterPublishSubscribeFactory->getSubscriber(stepName, vectorSize);
        } else {
            auto subscriberFactoryType = _streamingConfigurationManager
                                             ->getThreadDistributionPolicy()
                                             ->getFactoryType(stepName);
            switch (subscriberFactoryType) {
            case FactoryType::EventEmitter: {
                return _eventEmitterPublishSubscribeFactory->getSubscriber(stepName, vectorSize);
            }

            case FactoryType::EventLoop: {
                return _eventLoopPublishSubscribeFactory->getSubscriber(stepName, vectorSize);
            }

            case FactoryType::DataMultiplexer: {
                return _dataMultiplexerPublishSubscribeFactory->getSubscriber(stepName, vectorSize);
            }

            default: {
                spdlog::warn("The Factory Type does not exist.");
                return nullptr;
            }
            }
        }
    }

    virtual kpsr::Publisher<DataBatchWithId<std::vector<TypeValue>>> *getPublisherMulti(
        const std::string &stepName, const size_t vectorSize, const size_t multiVectorSize) override
    {
        if (_useTestFactory) {
            return _eventEmitterPublishSubscribeFactory->getPublisherMulti(stepName,
                                                                           vectorSize,
                                                                           multiVectorSize);
        } else {
            auto publisherFactoryType = _streamingConfigurationManager
                                            ->getThreadDistributionPolicy()
                                            ->getFactoryType(stepName);
            switch (publisherFactoryType) {
            case FactoryType::EventEmitter: {
                return _eventEmitterPublishSubscribeFactory->getPublisherMulti(stepName,
                                                                               vectorSize,
                                                                               multiVectorSize);
            }

            case FactoryType::EventLoop: {
                return _eventLoopPublishSubscribeFactory->getPublisherMulti(stepName,
                                                                            vectorSize,
                                                                            multiVectorSize);
            }

            case FactoryType::DataMultiplexer: {
                return _dataMultiplexerPublishSubscribeFactory->getPublisherMulti(stepName,
                                                                                  vectorSize,
                                                                                  multiVectorSize);
            }

            default: {
                spdlog::warn("The Factory Type does not exist.");
                return nullptr;
            }
            }
        }
    }

    virtual kpsr::Subscriber<DataBatchWithId<std::vector<TypeValue>>> *getSubscriberMulti(
        const std::string &stepName, const size_t vectorSize, const size_t multiVectorSize) override
    {
        if (_useTestFactory) {
            return _eventEmitterPublishSubscribeFactory->getSubscriberMulti(stepName,
                                                                            vectorSize,
                                                                            multiVectorSize);
        } else {
            auto subscriberFactoryType = _streamingConfigurationManager
                                             ->getThreadDistributionPolicy()
                                             ->getFactoryType(stepName);
            switch (subscriberFactoryType) {
            case FactoryType::EventEmitter: {
                return _eventEmitterPublishSubscribeFactory->getSubscriberMulti(stepName,
                                                                                vectorSize,
                                                                                multiVectorSize);
            }

            case FactoryType::EventLoop: {
                return _eventLoopPublishSubscribeFactory->getSubscriberMulti(stepName,
                                                                             vectorSize,
                                                                             multiVectorSize);
            }

            case FactoryType::DataMultiplexer: {
                return _dataMultiplexerPublishSubscribeFactory->getSubscriberMulti(stepName,
                                                                                   vectorSize,
                                                                                   multiVectorSize);
            }

            default: {
                spdlog::warn("The Factory Type does not exist.");
                return nullptr;
            }
            }
        }
    }

    virtual void start() override
    {
        if (_useTestFactory) {
            _eventEmitterPublishSubscribeFactory->startup();
        } else {
            _eventEmitterPublishSubscribeFactory->startup();
            _eventLoopPublishSubscribeFactory->startup();
        }
    }

    virtual void stop() override
    {
        if (_useTestFactory) {
            _eventEmitterPublishSubscribeFactory->shutdown();
        } else {
            _eventEmitterPublishSubscribeFactory->shutdown();
            _eventLoopPublishSubscribeFactory->shutdown();
        }
    }

private:
    bool _useTestFactory;
    StreamingConfigurationManager *_streamingConfigurationManager;
    std::unique_ptr<PublishSubscribeFactory<TypeValue>> _eventEmitterPublishSubscribeFactory;
    std::unique_ptr<PublishSubscribeFactory<TypeValue>> _eventLoopPublishSubscribeFactory;
    std::unique_ptr<PublishSubscribeFactory<TypeValue>> _dataMultiplexerPublishSubscribeFactory;
};
} // namespace streaming
} // namespace kpsr

#endif // SMART_PUBLISH_SUBSCRIBE_FACTORY_H