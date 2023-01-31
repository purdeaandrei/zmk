/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zmk/matrix_transform.h>
#include <zmk/matrix.h>
#include <dt-bindings/zmk/matrix_transform.h>

#ifdef ZMK_KEYMAP_TRANSFORM_NODE

#if ZMK_KEYMAP_LEN - 1 > INT32_MAX
    #error "Transform is too large"
#elif ZMK_KEYMAP_LEN - 1 + 1 > (1 << 16) - 1
    #define TRANSFORM_TYPE uint32_t
#elif ZMK_KEYMAP_LEN - 1 + 1 > (1 << 8) - 1
    #define TRANSFORM_TYPE uint16_t
#else
    #define TRANSFORM_TYPE uint8_t
#endif

#define _TRANSFORM_ENTRY(i, _)                                                                     \
    [(KT_ROW(DT_PROP_BY_IDX(ZMK_KEYMAP_TRANSFORM_NODE, map, i)) * ZMK_MATRIX_COLS) +               \
        KT_COL(DT_PROP_BY_IDX(ZMK_KEYMAP_TRANSFORM_NODE, map, i))] = i + 1

static const TRANSFORM_TYPE transform[] = {LISTIFY(ZMK_KEYMAP_LEN, _TRANSFORM_ENTRY, (, ), 0)};
#endif

int32_t zmk_matrix_transform_row_column_to_position(uint32_t row, uint32_t column) {
    uint32_t matrix_index;
    uint32_t value;

#if DT_NODE_HAS_PROP(ZMK_KEYMAP_TRANSFORM_NODE, col_offset)
    column += DT_PROP(ZMK_KEYMAP_TRANSFORM_NODE, col_offset);
#endif

#if DT_NODE_HAS_PROP(ZMK_KEYMAP_TRANSFORM_NODE, row_offset)
    row += DT_PROP(ZMK_KEYMAP_TRANSFORM_NODE, row_offset);
#endif

    matrix_index = (row * ZMK_MATRIX_COLS) + column;

#ifdef ZMK_KEYMAP_TRANSFORM_NODE
    if (matrix_index >= ARRAY_SIZE(transform))
        return -1;
    value = transform[matrix_index];
    if (!value)
        return -1;
    return value - 1;
#else
    return matrix_index;
#endif /* ZMK_KEYMAP_TRANSFORM_NODE */
};
