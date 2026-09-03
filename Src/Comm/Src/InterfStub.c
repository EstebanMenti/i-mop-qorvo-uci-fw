/**
 * @file      InterfStub.c
 *
 * @brief     Stub implementation when USB is disabled
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2025 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#include "InterfUsb.h"

void UsbSetState(enum usbState _state)
{
    (void)_state;
    /* Stub: do nothing when USB is disabled */
}

enum usbState UsbGetState(void)
{
    /* Stub: always return disconnected when USB is disabled */
    return USB_DISCONNECTED;
}
