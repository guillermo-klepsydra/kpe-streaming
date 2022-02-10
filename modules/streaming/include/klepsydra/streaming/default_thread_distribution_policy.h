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
#ifndef DEAFAULT_THREAD_DISTRIBUTION_POLICY_H
#define DEAFAULT_THREAD_DISTRIBUTION_POLICY_H

#include <klepsydra/streaming/thread_distribution_policy.h>

namespace kpsr {
namespace streaming {

static const size_t CORE_RATIO = 1;
static const int NON_INTENSIVE_STREAM_RATIO = 1;

class DefaultThreadDistributionPolicy : public ThreadDistributionPolicy
{
public:
    DefaultThreadDistributionPolicy(size_t numberOfCores, size_t numberOfEventLoops);

    size_t addStepToEventLoop(const std::string &stepName) override;

    std::vector<int> streamEventLoopDistribution;

private:
    size_t _numberOfEventLoops;
};

} // namespace streaming
} // namespace kpsr

#endif