# Klepsydra Streaming  #
[![klepsydra logo](./images/klepsydra_logo.jpg)](http://www.klepsydra.org)

This repository containes the Streaming API for Klepsydra products.

# Installation Instructions

## System dependencies

* Ubuntu 16.04 or above
* Cmake 3.12 or above
* gcc for C++11 5.4.0 or above.
* Doxygen (optional)
* yaml-cpp (0.5 or above)

## Klepsydra dependencies

* kpsr-core with YAML support


## Installation

Given ```$KLEPSYDRA_HOME```, for example ```$HOME/klepsydra```. This assumes that the Klepsydra SDK (kpsr-core) is installed in ```$KLEPSYDRA_INSTALL```,

```
cd $KLEPSYDRA_HOME
git clone --recursive https://bitbucket.org/kpsr-devs/kpsr-streaming.git
cd kpsr-streaming
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=$KLEPSYDRA_INSTALL -DCMAKE_INSTALL_PREFIX=$KLEPSYDRA_INSTALL
make -j4
make test
make install
```

#  License

&copy; Copyright 2020-2022, Klepsydra Technologies, all rights reserved. Licensed under the terms in [LICENSE.md](./LICENSE.md)

This software and documentation are Copyright 2020-2022, Klepsydra Technologies
Limited and its licensees. All rights reserved. See [license file](./LICENSE.md) for full copyright notice and license terms.

#  Contact

https://www.klepsydra.com
support@klepsydra.com

