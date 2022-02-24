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
#ifndef STREAMING_TEST_FIXTURE_H
#define STREAMING_TEST_FIXTURE_H
#include <klepsydra/streaming/default_thread_distribution_policy_factory_impl.h>
#include <klepsydra/streaming/streaming_factory_provider.h>

#include "gtest/gtest.h"

class StreamingFactoryProviderDefault : public testing::TestWithParam<bool>
{
protected:
    StreamingFactoryProviderDefault()
        : defaultThreadDistributionPolicyFactoryImpl()
        , sut(&defaultThreadDistributionPolicyFactoryImpl, GetParam())
    {}
    // @TODO: Add tests for the case of useChar = true.
    kpsr::streaming::DefaultThreadDistributionPolicyFactoryImpl
        defaultThreadDistributionPolicyFactoryImpl;
    kpsr::streaming::StreamingFactoryProvider sut;
};

#endif