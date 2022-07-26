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

#include <klepsydra/streaming/data_multiplexer_publish_subscribe_factory.h>
#include <klepsydra/streaming/default_thread_distribution_policy.h>
#include <klepsydra/streaming/event_loop_publish_subscribe_factory.h>

#include <numeric>

#include "gtest/gtest.h"

TEST(EventLoopDataMultiplexerPublishSubscribeFactoryTest, IntegrationTest)
{
    const int num_listeners = 2;
    int data_sent_ctr = 0;
    std::vector<int> data_received_ctr(num_listeners, 0);

    kpsr::streaming::DataMultiplexerPublishSubscribeFactory<kpsr::streaming::F32AlignedVector>
        dataMultiplexerInstance(nullptr, nullptr);

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>
        *dataMultiplexerPublisher = dataMultiplexerInstance.getPublisher("dataMultiplexer", 3);
    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>
        *dataMultiplexerSubscriber = dataMultiplexerInstance.getSubscriber("dataMultiplexer", 3);

    int poolSize = 2;
    size_t numberOfCores = std::thread::hardware_concurrency();
    size_t numberOfEventLoops = numberOfCores * 1;
    size_t nonCriticalThreadPoolSize = 1;
    int numberOfParallelThreads = 1;
    std::vector<std::string> parallisedSteps = {};
    auto defaultThreadDistributionPolicy =
        std::make_shared<kpsr::streaming::DefaultThreadDistributionPolicy>(numberOfCores,
                                                                           numberOfEventLoops);
    auto streamingConfigurationManager = std::make_unique<
        kpsr::streaming::StreamingConfigurationManager>(poolSize,
                                                        numberOfCores,
                                                        numberOfEventLoops,
                                                        nonCriticalThreadPoolSize,
                                                        numberOfParallelThreads,
                                                        parallisedSteps,
                                                        defaultThreadDistributionPolicy);

    auto eventLoopFactory =
        std::make_shared<kpsr::streaming::EventLoopFactory>(nullptr,
                                                            streamingConfigurationManager.get());
    kpsr::streaming::EventLoopPublishSubscribeFactory<kpsr::streaming::F32AlignedVector>
        eventloopInstance(eventLoopFactory);

    for (size_t i = 0; i < data_received_ctr.size(); i++) {
        dataMultiplexerSubscriber->registerListener(
            "dataMultiplexer_" + std::to_string(i),
            [i, &eventloopInstance](
                const kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector> &event) {
                eventloopInstance.getPublisher("eventloop_" + std::to_string(i), 3)->publish(event);
            });

        eventloopInstance.getSubscriber("eventloop_" + std::to_string(i), 3)
            ->registerListener("data_received_ctr_" + std::to_string(i),
                               [i, &data_received_ctr](const kpsr::streaming::DataBatchWithId<
                                                       kpsr::streaming::F32AlignedVector> &event) {
                                   data_received_ctr[i]++;
                               });
    }

    eventloopInstance.start();

    std::thread dataMultipexerPublisherThread([&dataMultiplexerPublisher, &data_sent_ctr]() {
        for (int i = 0; i < 5; i++) {
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
    eventloopInstance.stop();

    for (size_t i = 0; i < data_received_ctr.size(); i++) {
        dataMultiplexerSubscriber->removeListener("dataMultiplexer_" + std::to_string(i));
        eventloopInstance.getSubscriber("eventloop_" + std::to_string(i), 3)
            ->removeListener("data_received_ctr_" + std::to_string(i));
    }

    for (size_t i = 0; i < data_received_ctr.size(); i++) {
        spdlog::debug("Data received counter {} value = {}", i, data_received_ctr[i]);
    }
    spdlog::debug("Data sent counter {}", data_sent_ctr);
    EXPECT_EQ(std::accumulate(data_received_ctr.begin(), data_received_ctr.end(), 0),
              num_listeners * data_sent_ctr);
}