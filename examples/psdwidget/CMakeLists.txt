# Copyright (C) 2024 Signal Slot Inc.
# SPDX-License-Identifier: BSD-3-Clause

# Only build the Qt Creator plugin if QtCreator_DIR is set
if(DEFINED QtCreator_DIR)
    qt_internal_add_example(qtcreator-psd-plugin)
else()
    message(STATUS "Skipping qtcreator-psd-plugin - QtCreator_DIR is not set")
endif()
