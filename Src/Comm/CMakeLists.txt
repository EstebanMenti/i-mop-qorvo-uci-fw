# SPDX-FileCopyrightText: Copyright (c) 2025 Qorvo US, Inc.
# SPDX-License-Identifier: LicenseRef-QORVO-2

if(USB_ENABLE)
  add_library(Interface Src/InterfUsb.c)
else()
  # Use stub implementation when USB is disabled
  add_library(Interface Src/InterfStub.c)
endif()

target_include_directories(Interface PUBLIC Inc)

install(TARGETS Interface EXPORT Interface-config)
install(EXPORT Interface-config DESTINATION lib/${CMAKE_LIBRARY_ARCHITECTURE}/Interface/cmake)
