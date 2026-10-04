#ifndef VELO_STRING_H
#define VELO_STRING_H

#include <stddef.h>

/**
 * Copies bytes between blocks of memory that don't overlap.
 *
 * Use memmove if the blocks may overlap.
 *
 * @param destination Block to copy to.
 * @param source Block to copy from.
 * @param count Number of bytes to copy.
 * @return destination.
 */
void *memcpy(void *destination, const void *source, size_t count);
/**
 * Copies bytes between blocks of memory, which may overlap.
 *
 * @param destination Block to copy to.
 * @param source Block to copy from.
 * @param count Number of bytes to copy.
 * @return destination.
 */
void *memmove(void *destination, const void *source, size_t count);
/**
 * Fills a block of memory with a byte value.
 *
 * @param destination The block to fill.
 * @param value The byte value, converted to unsigned char.
 * @param count Number of bytes to fill.
 * @return destination.
 */
void *memset(void *destination, int value, size_t count);
/**
 * Compares two blocks of memory byte by byte.
 *
 * @param first First block.
 * @param second Second block.
 * @param count Number of bytes to compare.
 * @return Less than, equal to or greater than zero as first is less than,
 *         equal to or greater than second.
 */
int memcmp(const void *first, const void *second, size_t count);
/**
 * Finds the first occurrence of a byte in a block of memory.
 *
 * @param memory Block to search.
 * @param value Byte to find, converted to unsigned char.
 * @param count Number of bytes to search.
 * @return Pointer to the first match, or NULL if there is none.
 */
void *memchr(const void *memory, int value, size_t count);

#endif
