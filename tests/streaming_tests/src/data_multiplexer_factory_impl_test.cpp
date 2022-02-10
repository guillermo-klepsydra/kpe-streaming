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
#include <klepsydra/streaming/data_multiplexer_factory_float32.h>
#include <klepsydra/streaming/default_thread_distribution_policy.h>
#include <klepsydra/streaming/streaming_configuration_manager.h>

#include <numeric>

#include "gtest/gtest.h"
#include <klepsydra/mem_core/mem_env.h>

using TestTuple = std::tuple<kpsr::Container *, bool>;

class DataMultiplexerFactoryTest : public ::testing::TestWithParam<TestTuple>
{
protected:
    DataMultiplexerFactoryTest()
        : container(nullptr)
        , poolSize(2)
        , numberOfCores(std::thread::hardware_concurrency())
        , numberOfEventLoops(std::thread::hardware_concurrency() * 1)
        , nonCriticalThreadPoolSize(1)
        , numberOfParallelThreads(1)
        , parallisedStreams()
        , defaultThreadDistributionPolicy(
              std::make_shared<kpsr::streaming::DefaultThreadDistributionPolicy>(numberOfCores,
                                                                                 numberOfEventLoops))
    {}
    virtual void SetUp()
    {
        bool isNullStreaming;
        std::tie(container, isNullStreaming) = GetParam();
        if (isNullStreaming) {
            streamingConfigurationManager = std::make_unique<
                kpsr::streaming::StreamingConfigurationManager>(poolSize,
                                                                numberOfCores,
                                                                numberOfEventLoops,
                                                                nonCriticalThreadPoolSize,
                                                                numberOfParallelThreads,
                                                                parallisedStreams,
                                                                defaultThreadDistributionPolicy);
        }
    }

    virtual void TearDown() { container = nullptr; }
    kpsr::Container *container;
    int poolSize;
    size_t numberOfCores;
    size_t numberOfEventLoops;
    size_t nonCriticalThreadPoolSize;
    int numberOfParallelThreads;
    std::vector<std::string> parallisedStreams;
    std::shared_ptr<kpsr::streaming::DefaultThreadDistributionPolicy> defaultThreadDistributionPolicy;
    std::unique_ptr<kpsr::streaming::StreamingConfigurationManager> streamingConfigurationManager;
};

kpsr::mem::MemEnv testEnvironment;
kpsr::Container testContainer(&testEnvironment, "TestEnvironment");

INSTANTIATE_TEST_SUITE_P(DataMultiplexerFactoryTests,
                         DataMultiplexerFactoryTest,
                         ::testing::Combine(::testing::Values(nullptr, &testContainer),
                                            ::testing::Bool()));

TEST_P(DataMultiplexerFactoryTest, ConstructorTest)
{
    ASSERT_NO_THROW(
        kpsr::streaming::DataMultiplexerFactoryFloat32
            dataMultiplexerFloat32Instance(container, streamingConfigurationManager.get()));
    ASSERT_NO_THROW(kpsr::streaming::DataMultiplexerFactoryChar
                        dataMultiplexerCharInstance(container, streamingConfigurationManager.get()));
}

TEST_P(DataMultiplexerFactoryTest, getPubSubFloat32Test)
{
    kpsr::streaming::DataMultiplexerFactoryFloat32
        dataMultiplexerInstance(container, streamingConfigurationManager.get());

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>
        *dataMultiplexerPublisher = nullptr;
    ASSERT_NO_THROW(
        dataMultiplexerPublisher = dataMultiplexerInstance.getPublisherF32Aligned("dataMultiplexer",
                                                                                  3));
    ASSERT_NE(dataMultiplexerPublisher, nullptr);
    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>
        *dataMultiplexerSubscriber = nullptr;
    ASSERT_NO_THROW(dataMultiplexerSubscriber = dataMultiplexerInstance
                                                    .getSubscriberF32Aligned("dataMultiplexer", 3));
    ASSERT_NE(dataMultiplexerSubscriber, nullptr);
    const std::string registerListenerName = "dataMultiplexerListener";
    ASSERT_NO_THROW(dataMultiplexerSubscriber->registerListener(
        registerListenerName,
        [](const kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector> &event) {
            spdlog::info("Register Listener Float32");
        }));
    ASSERT_NO_THROW(dataMultiplexerSubscriber->removeListener(registerListenerName));
}

TEST_P(DataMultiplexerFactoryTest, getPubSubCharTest)
{
    kpsr::streaming::DataMultiplexerFactoryChar
        dataMultiplexerInstance(container, streamingConfigurationManager.get());

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<std::vector<char>>> *dataMultiplexerPublisher =
        nullptr;
    ASSERT_NO_THROW(
        dataMultiplexerPublisher = dataMultiplexerInstance.getPublisherChar("dataMultiplexer", 3));
    ASSERT_NE(dataMultiplexerPublisher, nullptr);
    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<std::vector<char>>>
        *dataMultiplexerSubscriber = nullptr;
    ASSERT_NO_THROW(
        dataMultiplexerSubscriber = dataMultiplexerInstance.getSubscriberChar("dataMultiplexer", 3));
    ASSERT_NE(dataMultiplexerSubscriber, nullptr);
    const std::string registerListenerName = "dataMultiplexerListener";
    ASSERT_NO_THROW(dataMultiplexerSubscriber->registerListener(
        registerListenerName, [](const kpsr::streaming::DataBatchWithId<std::vector<char>> &event) {
            spdlog::info("Register Listener Char");
        }));
    ASSERT_NO_THROW(dataMultiplexerSubscriber->removeListener(registerListenerName));
}

TEST_P(DataMultiplexerFactoryTest, SimpleTestFloat)
{
    const int num_listeners = 2;
    int data_sent_ctr = 0;
    std::vector<int> data_received_ctr(num_listeners, 0);

    kpsr::streaming::DataMultiplexerFactoryFloat32
        dataMultiplexerInstance(container, streamingConfigurationManager.get());

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>> *
        dataMultiplexerPublisher = dataMultiplexerInstance.getPublisherF32Aligned("dataMultiplexer",
                                                                                  3);
    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>
        *dataMultiplexerSubscriber =
            dataMultiplexerInstance.getSubscriberF32Aligned("dataMultiplexer", 3);

    for (size_t i = 0; i < data_received_ctr.size(); i++) {
        dataMultiplexerSubscriber->registerListener(
            "data_received_ctr_" + std::to_string(i),
            [i, &data_received_ctr](
                const kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector> &event) {
                data_received_ctr[i]++;
            });
    }

    std::thread dataMultipexerPublisherThread([&dataMultiplexerPublisher, &data_sent_ctr]() {
        for (size_t i = 0; i < 5; i++) {
            std::shared_ptr<kpsr::streaming::F32AlignedVector> inputPtr =
                std::make_shared<kpsr::streaming::F32AlignedVector>(
                    std::initializer_list<float>{float(i + 1), float(i + 2), float(i + 3)});
            kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>
                inputDataBatchWithId(0, inputPtr);
            dataMultiplexerPublisher->publish(inputDataBatchWithId);
            data_sent_ctr++;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    });

    dataMultipexerPublisherThread.join();

    std::vector<size_t> discardedMessages(data_received_ctr.size(), 0);
    for (size_t i = 0; i < data_received_ctr.size(); i++) {
        auto listenerName = "data_received_ctr_" + std::to_string(i);
        discardedMessages[i] = dataMultiplexerSubscriber->getSubscriptionStats(listenerName)
                                   ->_totalDiscardedEvents;
        dataMultiplexerSubscriber->removeListener(listenerName);
    }

    for (size_t i = 0; i < data_received_ctr.size(); i++) {
        spdlog::debug("Data received counter {} value = {}", i, data_received_ctr[i]);
    }
    spdlog::debug("Data sent counter {}", data_sent_ctr);
    EXPECT_EQ(std::accumulate(data_received_ctr.begin(), data_received_ctr.end(), 0) +
                  std::accumulate(discardedMessages.begin(), discardedMessages.end(), 0),
              num_listeners * data_sent_ctr);
}

TEST_P(DataMultiplexerFactoryTest, SimpleTestChar)
{
    const int num_listeners = 2;
    int data_sent_ctr = 0;
    std::vector<int> data_received_ctr(num_listeners, 0);

    kpsr::streaming::DataMultiplexerFactoryChar
        dataMultiplexerInstance(container, streamingConfigurationManager.get());

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<std::vector<char>>> *dataMultiplexerPublisher =
        dataMultiplexerInstance.getPublisherChar("dataMultiplexer", 3);
    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<std::vector<char>>> *
        dataMultiplexerSubscriber = dataMultiplexerInstance.getSubscriberChar("dataMultiplexer", 3);

    for (size_t i = 0; i < data_received_ctr.size(); i++) {
        dataMultiplexerSubscriber
            ->registerListener("data_received_ctr_" + std::to_string(i),
                               [i, &data_received_ctr](
                                   const kpsr::streaming::DataBatchWithId<std::vector<char>> &event) {
                                   data_received_ctr[i]++;
                               });
    }

    std::thread dataMultipexerPublisherThread([&dataMultiplexerPublisher, &data_sent_ctr]() {
        for (size_t i = 0; i < 5; i++) {
            std::shared_ptr<std::vector<char>> inputPtr = std::make_shared<std::vector<char>>(
                std::initializer_list<char>{char(i + 1), char(i + 2), char(i + 3)});
            kpsr::streaming::DataBatchWithId<std::vector<char>> inputDataBatchWithId(0, inputPtr);
            dataMultiplexerPublisher->publish(inputDataBatchWithId);
            data_sent_ctr++;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    });

    dataMultipexerPublisherThread.join();

    std::vector<size_t> discardedMessages(data_received_ctr.size(), 0);
    for (size_t i = 0; i < data_received_ctr.size(); i++) {
        auto listenerName = "data_received_ctr_" + std::to_string(i);
        discardedMessages[i] = dataMultiplexerSubscriber->getSubscriptionStats(listenerName)
                                   ->_totalDiscardedEvents;
        dataMultiplexerSubscriber->removeListener(listenerName);
    }

    for (size_t i = 0; i < data_received_ctr.size(); i++) {
        spdlog::debug("Data received counter {} value = {}", i, data_received_ctr[i]);
    }
    spdlog::debug("Data sent counter {}", data_sent_ctr);
    EXPECT_EQ(std::accumulate(data_received_ctr.begin(), data_received_ctr.end(), 0) +
                  std::accumulate(discardedMessages.begin(), discardedMessages.end(), 0),
              num_listeners * data_sent_ctr);
}

TEST_P(DataMultiplexerFactoryTest, SimpleTestUChar)
{
    const int num_listeners = 2;
    int data_sent_ctr = 0;
    std::vector<int> data_received_ctr(num_listeners, 0);

    kpsr::streaming::DataMultiplexerFactoryChar
        dataMultiplexerInstance(container, streamingConfigurationManager.get());

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<std::vector<unsigned char>>>
        *dataMultiplexerPublisher = dataMultiplexerInstance.getPublisherUChar("dataMultiplexer", 3);
    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<std::vector<unsigned char>>>
        *dataMultiplexerSubscriber = dataMultiplexerInstance.getSubscriberUChar("dataMultiplexer",
                                                                                3);

    for (size_t i = 0; i < data_received_ctr.size(); i++) {
        dataMultiplexerSubscriber->registerListener(
            "data_received_ctr_" + std::to_string(i),
            [i, &data_received_ctr](
                const kpsr::streaming::DataBatchWithId<std::vector<unsigned char>> &event) {
                data_received_ctr[i]++;
            });
    }

    std::thread dataMultipexerPublisherThread([&dataMultiplexerPublisher, &data_sent_ctr]() {
        for (size_t i = 0; i < 5; i++) {
            std::shared_ptr<std::vector<unsigned char>> inputPtr =
                std::make_shared<std::vector<unsigned char>>(
                    std::initializer_list<unsigned char>{(unsigned char) (i + 1),
                                                         (unsigned char) (i + 2),
                                                         (unsigned char) (i + 3)});
            kpsr::streaming::DataBatchWithId<std::vector<unsigned char>>
                inputDataBatchWithId(0, inputPtr);
            dataMultiplexerPublisher->publish(inputDataBatchWithId);
            data_sent_ctr++;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    });

    dataMultipexerPublisherThread.join();

    std::vector<size_t> discardedMessages(data_received_ctr.size(), 0);
    for (size_t i = 0; i < data_received_ctr.size(); i++) {
        auto listenerName = "data_received_ctr_" + std::to_string(i);
        discardedMessages[i] = dataMultiplexerSubscriber->getSubscriptionStats(listenerName)
                                   ->_totalDiscardedEvents;
        dataMultiplexerSubscriber->removeListener(listenerName);
    }

    for (size_t i = 0; i < data_received_ctr.size(); i++) {
        spdlog::debug("Data received counter {} value = {}", i, data_received_ctr[i]);
    }
    spdlog::debug("Data sent counter {}", data_sent_ctr);
    EXPECT_EQ(std::accumulate(data_received_ctr.begin(), data_received_ctr.end(), 0) +
                  std::accumulate(discardedMessages.begin(), discardedMessages.end(), 0),
              num_listeners * data_sent_ctr);
}