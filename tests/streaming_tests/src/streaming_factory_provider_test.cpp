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

#include <klepsydra/streaming/default_thread_distribution_policy_factory_impl.h>
#include <klepsydra/streaming/streaming_factory_provider.h>

#include "config.h"
#include <klepsydra/mem_core/mem_env.h>

#include "test_fixture.h"
#include "gtest/gtest.h"

INSTANTIATE_TEST_SUITE_P(DefaultProviderTests,
                         StreamingFactoryProviderDefault,
                         testing::Values(true, false));

TEST_P(StreamingFactoryProviderDefault, DefaultGetFactory)
{
    std::shared_ptr<kpsr::streaming::PublishSubscribeFactory<kpsr::streaming::F32AlignedVector>>
        streamingFactoryFloat32 = sut.getEventLoopFactoryFloat32();
    ASSERT_NE(streamingFactoryFloat32.get(), nullptr);
    std::shared_ptr<kpsr::streaming::PublishSubscribeFactory<kpsr::streaming::I8AlignedVector>>
        streamingFactoryChar = sut.getEventLoopFactoryChar();
    ASSERT_NE(streamingFactoryChar.get(), nullptr);

    std::shared_ptr<kpsr::streaming::PublishSubscribeFactory<kpsr::streaming::UI8AlignedVector>>
        streamingFactoryUChar = sut.getEventLoopFactoryUChar();
    ASSERT_NE(streamingFactoryUChar.get(), nullptr);
}

TEST_P(StreamingFactoryProviderDefault, StartStopTest)
{
    ASSERT_NO_THROW(sut.start());
    ASSERT_NO_THROW(sut.stop());
}

TEST_P(StreamingFactoryProviderDefault, GetNewPubSubTest)
{
    std::shared_ptr<kpsr::streaming::PublishSubscribeFactory<kpsr::streaming::F32AlignedVector>>
        streamingFactoryFloat32 = sut.getEventLoopFactoryFloat32();
    ASSERT_NE(streamingFactoryFloat32.get(), nullptr);
    ASSERT_NE(streamingFactoryFloat32->getPublisher("conv", 10), nullptr);
    ASSERT_NE(streamingFactoryFloat32->getPublisherMulti("reluAlign", 10, 2), nullptr);
    ASSERT_NE(streamingFactoryFloat32->getPublisher("relu", 10), nullptr);
    ASSERT_NE(streamingFactoryFloat32->getSubscriber("conv", 10), nullptr);
    ASSERT_NE(streamingFactoryFloat32->getSubscriberMulti("reluAlign", 10, 2), nullptr);
    ASSERT_NE(streamingFactoryFloat32->getSubscriber("relu", 10), nullptr);

    std::shared_ptr<kpsr::streaming::PublishSubscribeFactory<kpsr::streaming::I8AlignedVector>>
        streamingFactoryChar = sut.getEventLoopFactoryChar();
    ASSERT_NE(streamingFactoryChar.get(), nullptr);

    ASSERT_NO_THROW(sut.start());
    ASSERT_NO_THROW(sut.stop());
}

TEST_P(StreamingFactoryProviderDefault, GetExistingPubSubTest)
{
    std::shared_ptr<kpsr::streaming::PublishSubscribeFactory<kpsr::streaming::F32AlignedVector>>
        streamingFactoryFloat32 = sut.getEventLoopFactoryFloat32();
    ASSERT_NE(streamingFactoryFloat32.get(), nullptr);
    auto F32APub = streamingFactoryFloat32->getPublisher("conv", 10);
    ASSERT_EQ(streamingFactoryFloat32->getPublisher("conv", 100), F32APub);
    auto MF32APub = streamingFactoryFloat32->getPublisherMulti("reluAlign", 10, 2);
    ASSERT_EQ(streamingFactoryFloat32->getPublisherMulti("reluAlign", 100, 12), MF32APub);
    auto F32Pub = streamingFactoryFloat32->getPublisher("relu", 10);
    ASSERT_EQ(streamingFactoryFloat32->getPublisher("relu", 1), F32Pub);

    auto F32ASub = streamingFactoryFloat32->getSubscriber("conv", 10);
    ASSERT_EQ(streamingFactoryFloat32->getSubscriber("conv", 100), F32ASub);
    auto MF32ASub = streamingFactoryFloat32->getSubscriberMulti("reluAlign", 10, 2);
    ASSERT_EQ(streamingFactoryFloat32->getSubscriberMulti("reluAlign", 100, 12), MF32ASub);
    auto F32Sub = streamingFactoryFloat32->getSubscriber("relu", 10);
    ASSERT_EQ(streamingFactoryFloat32->getSubscriber("relu", 1), F32Sub);

    std::shared_ptr<kpsr::streaming::PublishSubscribeFactory<kpsr::streaming::I8AlignedVector>>
        streamingFactoryChar = sut.getEventLoopFactoryChar();
    ASSERT_NE(streamingFactoryChar.get(), nullptr);

    ASSERT_NO_THROW(sut.start());
    ASSERT_NO_THROW(sut.stop());
}

TEST(StreamingFactoryProvider, EnvironmentForProductionFloat32)
{
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

    kpsr::streaming::DefaultThreadDistributionPolicyFactoryImpl
        defaultThreadDistributionPolicyFactoryImpl;
    kpsr::streaming::StreamingFactoryProvider sut(&defaultThreadDistributionPolicyFactoryImpl,
                                                  &environment);

    std::shared_ptr<kpsr::streaming::PublishSubscribeFactory<kpsr::streaming::F32AlignedVector>>
        streamingFactoryFloat32 = sut.getEventLoopFactoryFloat32();
    ASSERT_NE(streamingFactoryFloat32.get(), nullptr);
    streamingFactoryFloat32->getPublisher("conv", 10);
    streamingFactoryFloat32->getPublisherMulti("reluAlign", 10, 2);
    streamingFactoryFloat32->getPublisher("relu", 10);
    streamingFactoryFloat32->getSubscriber("conv", 10);
    streamingFactoryFloat32->getSubscriberMulti("reluAlign", 10, 2);
    streamingFactoryFloat32->getSubscriber("relu", 10);

    ASSERT_NO_THROW(sut.start());
    ASSERT_NO_THROW(sut.stop());
}

TEST(StreamingFactoryProvider, EnvironmentForProductionChar)
{
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

    kpsr::streaming::DefaultThreadDistributionPolicyFactoryImpl
        defaultThreadDistributionPolicyFactoryImpl;
    kpsr::streaming::StreamingFactoryProvider sut(&defaultThreadDistributionPolicyFactoryImpl,
                                                  &environment);

    std::shared_ptr<kpsr::streaming::PublishSubscribeFactory<kpsr::streaming::I8AlignedVector>>
        streamingFactoryChar = sut.getEventLoopFactoryChar();
    ASSERT_NE(streamingFactoryChar.get(), nullptr);
    auto CharPub = streamingFactoryChar->getPublisher("test", 10);
    ASSERT_EQ(streamingFactoryChar->getPublisher("test", 1), CharPub);

    auto CharSub = streamingFactoryChar->getSubscriber("test", 10);
    ASSERT_EQ(streamingFactoryChar->getSubscriber("test", 1), CharSub);

    std::shared_ptr<kpsr::streaming::PublishSubscribeFactory<kpsr::streaming::UI8AlignedVector>>
        streamingFactoryUChar = sut.getEventLoopFactoryUChar();
    ASSERT_NE(streamingFactoryUChar.get(), nullptr);
    auto UCharPub = streamingFactoryUChar->getPublisher("test_unsigned", 10);
    ASSERT_EQ(streamingFactoryUChar->getPublisher("test_unsigned", 1), UCharPub);

    auto UCharSub = streamingFactoryUChar->getSubscriber("test_unsigned", 10);
    ASSERT_EQ(streamingFactoryUChar->getSubscriber("test_unsigned", 1), UCharSub);

    ASSERT_NO_THROW(sut.start());
    ASSERT_NO_THROW(sut.stop());
}

TEST(StreamingFactoryProvider, EnvironmentForProductionFileContainer)
{
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

    std::string jsonFile = std::string(TEST_DATA) + "/streaming_conf.json";
    environment.setPropertyString("streaming_conf_file", jsonFile);

    kpsr::streaming::StreamingFactoryProvider sut(nullptr, &environment);

    std::shared_ptr<kpsr::streaming::PublishSubscribeFactory<kpsr::streaming::F32AlignedVector>>
        streamingFactoryFloat32 = sut.getEventLoopFactoryFloat32();
    streamingFactoryFloat32->getPublisher("conv", 10);
    streamingFactoryFloat32->getPublisherMulti("reluAlign", 10, 2);
    streamingFactoryFloat32->getPublisher("relu", 10);
    streamingFactoryFloat32->getSubscriber("conv", 10);
    streamingFactoryFloat32->getSubscriberMulti("reluAlign", 10, 2);
    streamingFactoryFloat32->getSubscriber("relu", 10);

    ASSERT_NO_THROW(sut.start());
    ASSERT_NO_THROW(sut.stop());
}

TEST(StreamingFactoryProvider, InsufficientThreadsError)
{
    {
        std::vector<std::shared_ptr<kpsr::streaming::StreamingFactoryProvider>> sut;
        kpsr::streaming::DefaultThreadDistributionPolicyFactoryImpl
            defaultThreadDistributionPolicyFactoryImpl;
        for (size_t i = 0; i < std::thread::hardware_concurrency() * 20; i++) {
            sut.emplace_back(std::make_shared<kpsr::streaming::StreamingFactoryProvider>(
                &defaultThreadDistributionPolicyFactoryImpl,
                false)); // create new provider.
        }

        auto start = [&]() {
            for (auto &provider : sut) {
                auto streamingFactoryFloat32 = provider->getEventLoopFactoryFloat32();
                provider->start();
            }
        };
        if (!sut.empty()) {
            EXPECT_THROW(start(), std::runtime_error);
        }
        for (auto &provider : sut) {
            auto streamingFactoryFloat32 = provider->getEventLoopFactoryFloat32();
            provider->stop();
        }
    }
}

TEST(StreamingFactoryProvider, MultiStartStopTest)
{
    {
        {
            kpsr::streaming::DefaultThreadDistributionPolicyFactoryImpl
                defaultThreadDistributionPolicyFactoryImpl;
            kpsr::streaming::StreamingFactoryProvider sut(&defaultThreadDistributionPolicyFactoryImpl,
                                                          false);

            std::shared_ptr<
                kpsr::streaming::PublishSubscribeFactory<kpsr::streaming::F32AlignedVector>>
                streamingFactoryFloat32 = sut.getEventLoopFactoryFloat32();
            EXPECT_NO_THROW(sut.start());
            sut.stop();
        }

        {
            kpsr::streaming::DefaultThreadDistributionPolicyFactoryImpl
                defaultThreadDistributionPolicyFactoryImpl;
            kpsr::streaming::StreamingFactoryProvider sut(&defaultThreadDistributionPolicyFactoryImpl,
                                                          false);

            std::shared_ptr<
                kpsr::streaming::PublishSubscribeFactory<kpsr::streaming::F32AlignedVector>>
                streamingFactoryFloat32 = sut.getEventLoopFactoryFloat32();
            EXPECT_NO_THROW(sut.start());
            sut.stop();
        }

        kpsr::streaming::DefaultThreadDistributionPolicyFactoryImpl
            defaultThreadDistributionPolicyFactoryImpl;
        kpsr::streaming::StreamingFactoryProvider sut(&defaultThreadDistributionPolicyFactoryImpl,
                                                      false);

        std::shared_ptr<kpsr::streaming::PublishSubscribeFactory<kpsr::streaming::F32AlignedVector>>
            streamingFactoryFloat32 = sut.getEventLoopFactoryFloat32();
        EXPECT_NO_THROW(sut.start());
        sut.stop();
    }
}