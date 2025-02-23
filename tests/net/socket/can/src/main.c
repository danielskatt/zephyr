/*
 * Copyright (c) 2022 Vestas Wind Systems A/S
 * Copyright (c) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/logging/log.h>
#include <zephyr/net/socketcan.h>
#include <zephyr/net/socketcan_utils.h>
#include <zephyr/ztest.h>

LOG_MODULE_REGISTER(socket_can, LOG_LEVEL_ERR);

/**
 * @brief Test of @a socketcan_to_can_frame()
 */
ZTEST(socket_can, test_socketcan_frame_to_can_frame)
{
	struct socketcan_frame sframe = { 0 };
	struct can_frame expected = { 0 };
	struct can_frame zframe;
	const uint8_t data[SOCKETCAN_MAX_DLEN] = { 0x01, 0x02, 0x03, 0x04,
						   0x05, 0x06, 0x07, 0x08 };

	sframe.can_id = BIT(31) | BIT(30) | 1234;
	sframe.can_dlc = sizeof(data);
	memcpy(sframe.data, data, sizeof(sframe.data));

	expected.rtr = CAN_REMOTEREQUEST;
	expected.id_type = CAN_EXTENDED_IDENTIFIER;
	expected.id = 1234U;
	expected.dlc = sizeof(data);

	socketcan_to_can_frame(&sframe, &zframe);

	LOG_HEXDUMP_DBG((const uint8_t *)&sframe, sizeof(sframe), "sframe");
	LOG_HEXDUMP_DBG((const uint8_t *)&zframe, sizeof(zframe), "zframe");
	LOG_HEXDUMP_DBG((const uint8_t *)&expected, sizeof(expected), "expected");

	zassert_equal(zframe.rtr, expected.rtr, "RTR bit not set");
	zassert_equal(zframe.id_type, expected.id_type, "Id-type bit not set");
	zassert_equal(zframe.id, expected.id, "CAN id invalid");
	zassert_equal(zframe.dlc, expected.dlc, "Msg length invalid");
}

/**
 * @brief Test of @a socketcan_from_can_frame()
 */
ZTEST(socket_can, test_can_frame_to_socketcan_frame)
{
	struct socketcan_frame sframe = { 0 };
	struct socketcan_frame expected = { 0 };
	struct can_frame zframe = { 0 };
	const uint8_t data[SOCKETCAN_MAX_DLEN] = { 0x01, 0x02, 0x03, 0x04,
						   0x05, 0x06, 0x07, 0x08 };

	expected.can_id = BIT(31) | BIT(30) | 1234;
	expected.can_dlc = sizeof(data);
	memcpy(expected.data, data, sizeof(expected.data));

	zframe.rtr = CAN_REMOTEREQUEST;
	zframe.id_type = CAN_EXTENDED_IDENTIFIER;
	zframe.id = 1234U;
	zframe.dlc = sizeof(data);
	memcpy(zframe.data, data, sizeof(data));

	socketcan_from_can_frame(&zframe, &sframe);

	LOG_HEXDUMP_DBG((const uint8_t *)&sframe, sizeof(sframe), "sframe");
	LOG_HEXDUMP_DBG((const uint8_t *)&zframe, sizeof(zframe), "zframe");
	LOG_HEXDUMP_DBG((const uint8_t *)&expected, sizeof(expected), "expected");

	zassert_equal(sframe.can_id, expected.can_id, "CAN ID not same");
	zassert_mem_equal(&sframe.data, &expected.data, sizeof(sframe.data),
			  "CAN data not same");
	zassert_equal(sframe.can_dlc, expected.can_dlc,
		      "CAN msg length not same");
}

/**
 * @brief Test of @a socketcan_to_can_filter()
 */
ZTEST(socket_can, test_socketcan_filter_to_can_filter)
{
	struct socketcan_filter sfilter = { 0 };
	struct can_filter expected = { 0 };
	struct can_filter zfilter = { 0 };

	sfilter.can_id = BIT(31) | BIT(30) | 1234;
	sfilter.can_mask = BIT(31) | BIT(30) | 1234;

	expected.rtr = CAN_REMOTEREQUEST;
	expected.id_type = CAN_EXTENDED_IDENTIFIER;
	expected.id = 1234U;
	expected.rtr_mask = 1U;
	expected.id_mask = 1234U;

	socketcan_to_can_filter(&sfilter, &zfilter);

	LOG_HEXDUMP_DBG((const uint8_t *)&zfilter, sizeof(zfilter), "zfilter");
	LOG_HEXDUMP_DBG((const uint8_t *)&sfilter, sizeof(sfilter), "sfilter");
	LOG_HEXDUMP_DBG((const uint8_t *)&expected, sizeof(expected), "expected");

	zassert_equal(zfilter.rtr, expected.rtr, "RTR bit not set");
	zassert_equal(zfilter.id_type, expected.id_type, "Id-type bit not set");
	zassert_equal(zfilter.id, expected.id, "CAN id invalid");
	zassert_equal(zfilter.rtr_mask, expected.rtr_mask, "RTR mask bit not set");
	zassert_equal(zfilter.id_mask, expected.id_mask, "id mask not set");
}

/**
 * @brief Test of @a socketcan_from_can_filter()
 */
ZTEST(socket_can, test_can_filter_to_socketcan_filter)
{
	struct socketcan_filter sfilter = { 0 };
	struct socketcan_filter expected = { 0 };
	struct can_filter zfilter = { 0 };

	expected.can_id = BIT(31) | BIT(30) | 1234;
	expected.can_mask = BIT(31) | BIT(30) | 1234;

	zfilter.rtr = CAN_REMOTEREQUEST;
	zfilter.id_type = CAN_EXTENDED_IDENTIFIER;
	zfilter.id = 1234U;
	zfilter.rtr_mask = 1U;
	zfilter.id_mask = 1234U;

	socketcan_from_can_filter(&zfilter, &sfilter);

	LOG_HEXDUMP_DBG((const uint8_t *)&zfilter, sizeof(zfilter), "zfilter");
	LOG_HEXDUMP_DBG((const uint8_t *)&sfilter, sizeof(sfilter), "sfilter");
	LOG_HEXDUMP_DBG((const uint8_t *)&expected, sizeof(expected), "expected");

	zassert_equal(sfilter.can_id, expected.can_id, "CAN ID not same");
	zassert_equal(sfilter.can_mask, expected.can_mask, "CAN mask not same");
}

ZTEST_SUITE(socket_can, NULL, NULL, NULL, NULL, NULL);
