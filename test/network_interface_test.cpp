/*
* Unpublished Copyright (c) 2009-2017 AutonomouStuff, LLC, All Rights Reserved.
*
* This file is part of the network_interface ROS 1.0 driver which is released under the MIT license.
* See file LICENSE included with this software or go to https://opensource.org/licenses/MIT for full license details.
*/

#include <network_interface/network_utils.h>
#include <gtest/gtest.h>

#include <vector>

class NetworkUtilsTest
: public :: testing::Test
{
  protected:
    float test_float = 123.45670318603515625;
    double test_double = 123.4567;
    uint32_t test_int = 1234567891;
    std::vector<uint8_t> float_be_bytes = {0x42, 0xF6, 0xE9, 0xD5};
    std::vector<uint8_t> float_le_bytes = {0xD5, 0xE9, 0xF6, 0x42};
    std::vector<uint8_t> double_be_bytes = {0x40, 0x5E, 0xDD, 0x3A, 0x92, 0xA3, 0x05, 0x53};
    std::vector<uint8_t> double_le_bytes = {0x53, 0x05, 0xA3, 0x92, 0x3A, 0xDD, 0x5E, 0x40};
    std::vector<uint8_t> int_be_bytes = {0x49, 0x96, 0x02, 0xD3};
    std::vector<uint8_t> int_le_bytes = {0xD3, 0x02, 0x96, 0x49};
};

// Read tests

TEST_F(NetworkUtilsTest, testReadBEFloat)
{
  ASSERT_EQ(test_float, AS::Network::read_be<float>(float_be_bytes, 0));
}

TEST_F(NetworkUtilsTest, testReadBEDouble)
{
  ASSERT_EQ(test_double, AS::Network::read_be<double>(double_be_bytes, 0));
}

TEST_F(NetworkUtilsTest, testReadBEInt)
{
  ASSERT_EQ(test_int, AS::Network::read_be<uint32_t>(int_be_bytes, 0));
}

TEST_F(NetworkUtilsTest, testReadLEFloat)
{
  ASSERT_EQ(test_float, AS::Network::read_le<float>(float_le_bytes, 0));
}

TEST_F(NetworkUtilsTest, testReadLEDouble)
{
  ASSERT_EQ(test_double, AS::Network::read_le<double>(double_le_bytes, 0));
}

TEST_F(NetworkUtilsTest, testReadLEInt)
{
  ASSERT_EQ(test_int, AS::Network::read_le<uint32_t>(int_le_bytes, 0));
}

// Write tests

TEST_F(NetworkUtilsTest, testWriteBEFloat)
{
  ASSERT_EQ(float_be_bytes, AS::Network::write_be(&test_float));
}

TEST_F(NetworkUtilsTest, testWriteBEDouble)
{
  ASSERT_EQ(double_be_bytes, AS::Network::write_be(&test_double));
}

TEST_F(NetworkUtilsTest, testWriteBEInt)
{
  ASSERT_EQ(int_be_bytes, AS::Network::write_be(&test_int));
}

TEST_F(NetworkUtilsTest, testWriteLEFloat)
{
  ASSERT_EQ(float_le_bytes, AS::Network::write_le(&test_float));
}

TEST_F(NetworkUtilsTest, testWriteLEDouble)
{
  ASSERT_EQ(double_le_bytes, AS::Network::write_le(&test_double));
}

TEST_F(NetworkUtilsTest, testWriteLEInt)
{
  ASSERT_EQ(int_le_bytes, AS::Network::write_le(&test_int));
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
