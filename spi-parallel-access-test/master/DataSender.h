/**
 * Copyright (C) 2016  Jonas Sticha (jonas.sticha@tum.de)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef SPI_PARALLEL_ACCESS_TEST_DATASENDER_H
#define SPI_PARALLEL_ACCESS_TEST_DATASENDER_H

#include <string>

using namespace std;


class DataSender {
private:
    DataSender();
    int allocateSendDataMemory(uint32_t numBytes);
    void freeSendDataMemory();
    int csChange;
    string spiDeviceName;
    uint8_t* txBuffer;
    struct spi_ioc_transfer* xfer;
    static const constexpr int maxBytesPerTransfer = 64;
public:
    DataSender(string spiDevName, int csChange);
    void sendData(int numBytes, int repetitions = 1);
    ~DataSender();
};


#endif //SPI_PARALLEL_ACCESS_TEST_DATASENDER_H
