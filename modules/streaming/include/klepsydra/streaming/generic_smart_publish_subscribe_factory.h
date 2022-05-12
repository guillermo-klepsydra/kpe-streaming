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

#ifndef GENERIC_SMART_PUBLISH_SUBSCRIBE_FACTORY_H
#define GENERIC_SMART_PUBLISH_SUBSCRIBE_FACTORY_H

#include <klepsydra/streaming/generic_event_loop_publish_subscribe_factory.h>
#include <klepsydra/streaming/generic_event_emitter_publish_subscribe_factory.h>
#include <klepsydra/streaming/generic_publish_subscribe_factory.h>
#include <klepsydra/streaming/streaming_configuration_manager.h>
#include <klepsydra/streaming/thread_distribution_policy.h>

namespace kpsr {
namespace streaming {
template<class TypeValue>

class GenericSmartPublishSubscribeFactory : public GenericPublishSubscribeFactory<TypeValue>
{
public:
    GenericSmartPublishSubscribeFactory(std::shared_ptr<EventEmitterFactory> eventEmitterFactory,
                                        std::shared_ptr<EventLoopFactory> eventLoopFactory,
                                        StreamingConfigurationManager *streamingConfigurationManager,
                                        bool useTestFactory = false)
        : kpsr::Service(nullptr, "SmartPublishSubscribeFactoryService")
        , _eventEmitterPublishSubscribeFactory(
              std::make_unique<GenericEventEmitterPublishSubscribeFactory<TypeValue>>(eventEmitterFactory))
        , _eventLoopPublishSubscribeFactory(
              std::make_unique<GenericEventLoopPublishSubscribeFactory<TypeValue>>(eventLoopFactory))
        , _streamingConfigurationManager(streamingConfigurationManager)
        , _useTestFactory(useTestFactory)
    {
        if(_streamingConfigurationManager == nullptr) {
            spdlog::error("StreamingConfigurationManger is a nullptr.");
        }
    }

    virtual ~GenericSmartPublishSubscribeFactory() {}

    virtual kpsr::Publisher<TypeValue> *getPublisher(const std::string &stepName) override
    {
        if (_useTestFactory) {
            return _eventEmitterPublishSubscribeFactory->getPublisher(stepName);
        } else {
            auto publisherFactoryType = _streamingConfigurationManager
                                            ->getThreadDistributionPolicy()
                                            ->getFactoryType(stepName);
            switch (publisherFactoryType) {
            case kpsr::streaming::FactoryType::EventEmitter: {
                return _eventEmitterPublishSubscribeFactory->getPublisher(stepName);
            }

            case FactoryType::EventLoop: {
                return _eventLoopPublishSubscribeFactory->getPublisher(stepName);
            }

            case FactoryType::DataMultiplexer: {
                spdlog::warn("The Publisher Factory Type DataMultiplexer, for the stepName {}, "
                             "does not exist by now.",
                             stepName);
                return nullptr;
            }

            default: {
                spdlog::warn("The Publisher Factory Type {}, for the stepName {}, does not exist.",
                             publisherFactoryType,
                             stepName);
                return nullptr;
            }
            }
        }
    }

    virtual kpsr::Subscriber<TypeValue> *getSubscriber(const std::string &stepName) override
    {
        if (_useTestFactory) {
            return _eventEmitterPublishSubscribeFactory->getSubscriber(stepName);
        } else {
            auto subscriberFactoryType = _streamingConfigurationManager
                                             ->getThreadDistributionPolicy()
                                             ->getFactoryType(stepName);
            switch (subscriberFactoryType) {
            case FactoryType::EventEmitter: {
                return _eventEmitterPublishSubscribeFactory->getSubscriber(stepName);
            }

            case FactoryType::EventLoop: {
                return _eventLoopPublishSubscribeFactory->getSubscriber(stepName);
            }

            case FactoryType::DataMultiplexer: {
                spdlog::warn("The Subscriber Factory Type DataMultiplexer, for the stepName {}, "
                             "does not exist by now.",
                             stepName);
                return nullptr;
            }

            default: {
                spdlog::warn("The Subscriber Factory Type {}, for the stepName {}, does not exist.",
                             subscriberFactoryType,
                             stepName);
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
    std::unique_ptr<GenericPublishSubscribeFactory<TypeValue>> _eventEmitterPublishSubscribeFactory;
    std::unique_ptr<GenericPublishSubscribeFactory<TypeValue>> _eventLoopPublishSubscribeFactory;
    StreamingConfigurationManager *_streamingConfigurationManager;
    bool _useTestFactory;
};
} // namespace streaming
} // namespace kpsr

#endif // GENERIC_SMART_PUBLISH_SUBSCRIBE_FACTORY_H