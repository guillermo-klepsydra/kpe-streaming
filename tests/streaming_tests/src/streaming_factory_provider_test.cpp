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

#include <klepsydra/streaming/streaming_factory_provider.h>

#include <klepsydra/mem_core/mem_env.h>
#include "config.h"

#include "gtest/gtest.h"

TEST(StreamingFactoryProvider, DefaultForTesting) {

    kpsr::streaming::StreamingFactoryProvider sut(true);

    std::shared_ptr<kpsr::streaming::PublishSubscribeFactory> streamingFactory = sut.getPublishSubcriberFactory();
    streamingFactory->getPublisherF32Aligned("conv", 10);
    streamingFactory->getPublisherMultiF32Aligned("relu", 10, 2);
    streamingFactory->getPublisherF32("relu", 10);
    streamingFactory->getSubscriberF32Aligned("conv", 10);
    streamingFactory->getSubscriberMultiF32Aligned("relu", 10, 2);
    streamingFactory->getSubscriberF32("relu", 10);

    streamingFactory->start();
    streamingFactory->stop();
}

TEST(StreamingFactoryProvider, DefaultForProduction) {

    kpsr::streaming::StreamingFactoryProvider sut(false);

    std::shared_ptr<kpsr::streaming::PublishSubscribeFactory> streamingFactory = sut.getPublishSubcriberFactory();
    streamingFactory->getPublisherF32Aligned("conv", 10);
    streamingFactory->getPublisherMultiF32Aligned("relu", 10, 2);
    streamingFactory->getPublisherF32("relu", 10);
    streamingFactory->getSubscriberF32Aligned("conv", 10);
    streamingFactory->getSubscriberMultiF32Aligned("relu", 10, 2);
    streamingFactory->getSubscriberF32("relu", 10);

    streamingFactory->start();
    streamingFactory->stop();
}

TEST(StreamingFactoryProvider, EnvironmentForProduction) {

    kpsr::mem::MemEnv environment;
    environment.setPropertyString("log_filename", "./logfile.log");
    environment.setPropertyInt("log_level", 1);
    environment.setPropertyBool("stat_socket_container_enable", true);
    environment.setPropertyInt("stat_admin_port", 9595);
    environment.setPropertyInt("stat_system_port", 9696);
    environment.setPropertyInt("pool_size", 32);
    environment.setPropertyBool("use_default_streaming_factory", true);
    environment.setPropertyInt("number_of_cores", 2);
    environment.setPropertyString("processor_intensive_layers", "max");
    environment.setPropertyString("streaming_conf_file", "");

    kpsr::streaming::StreamingFactoryProvider sut(&environment);

    std::shared_ptr<kpsr::streaming::PublishSubscribeFactory> streamingFactory = sut.getPublishSubcriberFactory();
    streamingFactory->getPublisherF32Aligned("conv", 10);
    streamingFactory->getPublisherMultiF32Aligned("relu", 10, 2);
    streamingFactory->getPublisherF32("relu", 10);
    streamingFactory->getSubscriberF32Aligned("conv", 10);
    streamingFactory->getSubscriberMultiF32Aligned("relu", 10, 2);
    streamingFactory->getSubscriberF32("relu", 10);

    streamingFactory->start();
    streamingFactory->stop();
}

TEST(StreamingFactoryProvider, EnvironmentForProductionFileContainer) {

    kpsr::mem::MemEnv environment;
    environment.setPropertyString("log_filename", "./logfile.log");
    environment.setPropertyInt("log_level", 1);
    environment.setPropertyBool("stat_file_container_enable", true);
    environment.setPropertyString("stat_filename", "./logfile.csv");
    environment.setPropertyInt("stat_log_interval_ms", 1000);
    environment.setPropertyInt("pool_size", 32);
    environment.setPropertyBool("use_default_streaming_factory", false);
    environment.setPropertyInt("number_of_cores", 2);
    environment.setPropertyString("processor_intensive_layers", "max");

    std::string yamlFile = std::string(TEST_DATA) + "/streaming_conf.json";
    environment.setPropertyString("streaming_conf_file", yamlFile);

    kpsr::streaming::StreamingFactoryProvider sut(&environment);

    std::shared_ptr<kpsr::streaming::PublishSubscribeFactory> streamingFactory = sut.getPublishSubcriberFactory();
    streamingFactory->getPublisherF32Aligned("conv", 10);
    streamingFactory->getPublisherMultiF32Aligned("relu", 10, 2);
    streamingFactory->getPublisherF32("relu", 10);
    streamingFactory->getSubscriberF32Aligned("conv", 10);
    streamingFactory->getSubscriberMultiF32Aligned("relu", 10, 2);
    streamingFactory->getSubscriberF32("relu", 10);

    streamingFactory->start();
    streamingFactory->stop();
}

TEST(StreamingFactoryProvider, InsufficientThreadsError) {
    {
        std::vector<std::shared_ptr<kpsr::streaming::StreamingFactoryProvider>> sut;
        for (size_t i = 0; i < std::thread::hardware_concurrency()*2; i++) {
            sut.emplace_back(std::make_shared<kpsr::streaming::StreamingFactoryProvider>(false)); // create new provider.
        }

        auto start = [&](){
                         for (auto& provider: sut) {
                             auto streamingFactory = provider->getPublishSubcriberFactory();
                             streamingFactory->start();
                         }
                     };
        if (!sut.empty()) {
            EXPECT_THROW(start(), std::runtime_error);
        }
        for (auto& provider: sut) {
            auto streamingFactory = provider->getPublishSubcriberFactory();
            streamingFactory->stop();
        }
    }
}

TEST(StreamingFactoryProvider, MultiStartStopTest) {
    {
        {
            kpsr::streaming::StreamingFactoryProvider sut(false);

            std::shared_ptr<kpsr::streaming::PublishSubscribeFactory> streamingFactory = sut.getPublishSubcriberFactory();
            EXPECT_NO_THROW(streamingFactory->start());
            streamingFactory->stop();
        }

        {
            kpsr::streaming::StreamingFactoryProvider sut(false);

            std::shared_ptr<kpsr::streaming::PublishSubscribeFactory> streamingFactory = sut.getPublishSubcriberFactory();
            EXPECT_NO_THROW(streamingFactory->start());
            streamingFactory->stop();
        }

            kpsr::streaming::StreamingFactoryProvider sut(false);

            std::shared_ptr<kpsr::streaming::PublishSubscribeFactory> streamingFactory = sut.getPublishSubcriberFactory();
            EXPECT_NO_THROW(streamingFactory->start());
            streamingFactory->stop();
    }
}
