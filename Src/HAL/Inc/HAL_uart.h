/**
 * @file      HAL_uart.h
 *
 * @brief     Interface for HAL_uart
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#pragma once

#include "comm_helpers.h"

#include <stdbool.h>
#include <stdint.h>

#define UART_OFF_TIMEOUT 30000

bool IsUartDown(void);
void SetUartDown(bool val);
void deca_discard_next_symbol(void);
void deca_uart_close(void);
void deca_uart_init(CommRxCallback callback);
void deca_uart_receive(void);
/**
 * @brief Queue up to @p sz bytes into the UART TX FIFO, stopping at the
 * first byte that could not be queued (FIFO full).
 * @return Number of bytes actually queued (<= sz). If less than @p sz,
 * the caller must retry the remaining bytes later instead of dropping them.
 */
uint16_t deca_uart_transmit(uint8_t *ptr, uint16_t sz);
void deca_uart_flush(void);
