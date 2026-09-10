#ifndef BLACK_FREESTANDING_H
#define BLACK_FREESTANDING_H
/* Optional target-side helpers. Black itself does not require this header. */
#include <stdint.h>
#include <stddef.h>
#define BLACK_MMIO8(addr) (*(volatile uint8_t *)(uintptr_t)(addr))
#define BLACK_MMIO16(addr) (*(volatile uint16_t *)(uintptr_t)(addr))
#define BLACK_MMIO32(addr) (*(volatile uint32_t *)(uintptr_t)(addr))
#define BLACK_MMIO64(addr) (*(volatile uint64_t *)(uintptr_t)(addr))
#endif
