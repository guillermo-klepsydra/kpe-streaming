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
#ifndef THREAD_DISTRIBUTION_POLICY_FACTORY_H
#define THREAD_DISTRIBUTION_POLICY_FACTORY_H

#include <klepsydra/core/environment.h>
#include <klepsydra/streaming/thread_distribution_policy.h>
#include <memory>

#include <spdlog/spdlog.h>

namespace kpsr {
namespace streaming {

class ThreadDistributionPolicyFactory
{
public:
    virtual ~ThreadDistributionPolicyFactory() {}

    virtual std::shared_ptr<ThreadDistributionPolicy> createThreadDistributionPolicy() = 0;
    virtual std::shared_ptr<ThreadDistributionPolicy> createThreadDistributionPolicy(
        kpsr::Environment *environment) = 0;
};

} // namespace streaming
} // namespace kpsr

#endif