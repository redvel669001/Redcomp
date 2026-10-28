#ifndef REDCOMP_H_
#define REDCOMP_H_

/* TODO: add support for mandatory flags. */
/* TODO: add support for more flag types. */

#ifndef RED_DEF
#define RED_DEF
#endif /* RED_DEF */

#ifndef REDCOMP_DEFAULT_RESERVE
#define REDCOMP_DEFAULT_RESERVE 64
#endif /* REDCOMO_DEFAULT_RESERVE */

#ifndef REDCOMP_ARENA_DEFAULT_RESERVE
#define REDCOMP_ARENA_DEFAULT_RESERVE 1024
#endif /* REDCOMP_ARENA_DEFAULT_RESERVE */

#ifdef __GNUC__
#define REDCOMP_PURE __attribute__ ((pure))
#define REDCOMP_CONST __attribute__ ((const))
#define REDCOMP_RETURNS_NONNULL __attribute__ ((returns_nonnull))
#define REDCOMP_NORETURN __attribute__ ((noreturn))

/*
  `string_index` - the index of the format string argument (indices
  start from 1, not 0)
  `first_to_check` - the index of the first argument that is to be
  checked against the format string (indices start from 1, not 0)
*/
#define REDCOMP_FORMAT(string_index, first_to_check) __attribute__ ((format (printf, (string_index), (first_to_check))))

#define REDCOMP_NONNULL_ONE(first) __attribute__ ((nonnull ((first))))
#define REDCOMP_NONNULL_TWO(first, second) __attribute__ ((nonnull ((first), (second))))
#define REDCOMP_NONNULL_THREE(first, second, third) __attribute__ ((nonnull ((first), (second), (third))))
#define REDCOMP_NONNULL_FOUR(first, second, third, fourth) __attribute__ ((nonnull ((first), (second), (third), (fourth))))
#define REDCOMP_NONNULL_FIVE(first, second, third, fourth, fifth) __attribute__ ((nonnull ((first), (second), (third), (fourth), (fifth))))

#define REDCOMP_BUILTIN_UNREACHABLE() __builtin_unreachable()
#define REDCOMP_BUILTIN_STATIC_ASSERT(condition, message) _Static_assert((condition), (message))
#endif /* __GNUC__ */

#ifndef REDCOMP_PURE
#define REDCOMP_PURE
#endif /* REDCOMP_PURE */

#ifndef REDCOMP_CONST
#define REDCOMP_CONST
#endif /* REDCOMP_CONST */

#ifndef REDCOMP_RETURNS_NONNULL
#define REDCOMP_RETURNS_NONNULL
#endif /* REDCOMP_RETURNS_NONNULL */

#ifndef REDCOMP_NORETURN
#define REDCOMP_NORETURN
#endif /* REDCOMP_NORETURN */

#ifndef REDCOMP_FORMAT
#define REDCOMP_FORMAT(string_index, first_to_check)
#endif /* REDCOMP_FORMAT */

#ifndef REDCOMP_NONNULL_ONE
#define REDCOMP_NONNULL_ONE(first)
#endif /* REDCOMP_NONNULL_ONE */

#ifndef REDCOMP_NONNULL_TWO
#define REDCOMP_NONNULL_TWO(first, second)
#endif /* REDCOMP_NONNULL_TWO */

#ifndef REDCOMP_NONNULL_THREE
#define REDCOMP_NONNULL_THREE(first, second, third)
#endif /* REDCOMP_NONNULL_THREE */

#ifndef REDCOMP_NONNULL_FOUR
#define REDCOMP_NONNULL_FOUR(first, second, third, fourth)
#endif /* REDCOMP_NONNULL_FOUR */

#ifndef REDCOMP_NONNULL_FIVE
#define REDCOMP_NONNULL_FIVE(first, second, third, fourth, fifth)
#endif /* REDCOMP_NONNULL_FIVE */

#ifndef REDCOMP_BUILTIN_UNREACHABLE
#define REDCOMP_BUILTIN_UNREACHABLE()
#endif /* REDCOMP_BUILTIN_UNREACHABLE */

#ifndef REDCOMP_BUILTIN_STATIC_ASSERT
#define REDCOMP_BUILTIN_STATIC_ASSERT(condition, message)
#endif /* REDCOMP_BUILTIN_STATIC_ASSERT */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>

#if defined(__linux__)

#include <sys/wait.h>

#endif /* __linux__ */

#include <unistd.h>
#include <time.h>
#include <errno.h>

#if defined(__WIN32)

typedef char i8;
typedef short int i16;
typedef int i32;
typedef long long int i64;

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long long int u64;

#define I64_FMT "%lld"
#define U64_FMT "%llu"

typedef long long int RedcompFileSize;

#define REDCOMP_FILE_SIZE_FMT "%lld"

#endif /* __WIN32 */

#if defined(__linux__)

typedef __int8_t i8;
typedef __int16_t i16;
typedef __int32_t i32;
typedef __int64_t i64;

typedef __uint8_t u8;
typedef __uint16_t u16;
typedef __uint32_t u32;
typedef __uint64_t u64;

#define I64_FMT "%ld"
#define U64_FMT "%lu"

typedef __int64_t RedcompFileSize;

#define REDCOMP_FILE_SIZE_FMT "%ld"

#endif /* __linux__ */


/*
  ############################################################
  ###################### MAKING ARRAYS #######################
  ############################################################
*/

#define redcomp_array_dynamic_make_inside(type) \
  type *first;                                  \
  size_t count;                                 \
  size_t reserved

#define redcomp_array_dynamic_iter_make_inside(type)    \
  type *first;                                          \
  size_t count;                                         \
  size_t reserved;                                      \
  type *iter

#define redcomp_array_view_make_inside(type)    \
  type *first;                                  \
  size_t count

#define redcomp_array_view_iter_make_inside(type)       \
  type *first;                                          \
  size_t count;                                         \
  type *iter

#define redcomp_array_dynamic_make(type, name)  \
  typedef struct {                              \
    redcomp_array_dynamic_make_inside(type);    \
  } name

#define redcomp_array_dynamic_iter_make(type, name)     \
  typedef struct {                                      \
    redcomp_array_dynamic_iter_make_inside(type);       \
  } name

#define redcomp_array_view_make(type, name)     \
  typedef struct {                              \
    redcomp_array_view_make_inside(type);       \
  } name

#define redcomp_array_view_iter_make(type, name)        \
  typedef struct {                                      \
    redcomp_array_view_iter_make_inside(type);          \
  } name

#define REDCOMP_ARRAY_LENGTH(array) sizeof (array) / sizeof *(array)

/*
  ############################################################
  ##################### ARRAY OPERATIONS #####################
  ############################################################
*/

#ifndef redcomp_array_reserve
#define redcomp_array_reserve(array, new_reserve)                       \
  do {                                                                  \
    if ((new_reserve) > (array)->reserved) {                            \
      if ((array)->reserved == 0) {                                     \
        (array)->reserved = REDCOMP_DEFAULT_RESERVE;                    \
      }                                                                 \
      while ((new_reserve) > (array)->reserved) {                       \
        (array)->reserved *= 2;                                         \
      }                                                                 \
      (array)->first = redcomp_realloc_wrapper((array)->first, (array)->reserved * sizeof *(array)->first, __FILE__, __LINE__); \
    }                                                                   \
  } while (0)
#endif /* redcomp_array_reserve */

#ifndef redcomp_array_insert
#define redcomp_array_insert(array, element)                            \
  do {                                                                  \
    redcomp_array_reserve((array), (array)->count + 1);                 \
    (array)->first[(array)->count++] = (element);                       \
  } while (0)
#endif /* redcomp_array_reserve */

#define redcomp_array_insert_many(array, elements, elements_count)      \
  do {                                                                  \
    array_reserve((array), (array)->count + (elements_count));          \
    memcpy((array)->first + (array)->count, (elements), (elements_count) * sizeof *(array)->first); \
    (array)->count += (elements_count);                                 \
  } while (0)

#define redcomp_iter_to_index(array, index)                             \
  (((array)->count > index ? ((array)->iter = (array)->first + index) : 0))

#define redcomp_iter_first(array) redcomp_iter_to_index((array), 0)

#define redcomp_iter_get_index(array) (size_t) ((array)->iter - (array)->first)

#define redcomp_iter_next(array) redcomp_iter_to_index((array), redcomp_iter_get_index(array) + 1)
#define redcomp_iter_forward(array, forward) redcomp_iter_to_index((array), redcomp_iter_get_index(array) + (forward))
#define redcomp_iter_prev(array) redcomp_iter_to_index((array), redcomp_iter_get_index(array) - 1)

#define redcomp_iter_peek_index(array, index)                           \
  ((array)->count > index ? (array)->first + index : (void *) 0)

#define redcomp_iter_peek_next(array) redcomp_iter_peek_index((array), redcomp_iter_get_index(array) + 1)
#define redcomp_iter_peek_forward(array, forward) redcomp_iter_peek_index((array), redcomp_iter_get_index(array) + (forward))
#define redcomp_iter_peek_prev(array) redcomp_iter_peek_index((array), redcomp_iter_get_index(array) - 1)

#define redcomp_array_cut_once(array)           \
  do {                                          \
    if ((array)->count > 0) {                   \
      (array)->first++;                         \
      (array)->count--;                         \
    }                                           \
  } while (0)

#define redcomp_array_cut(array, cuts)          \
  do {                                          \
    if ((array)->count > (cuts)) {              \
      (array)->first += (cuts);                 \
      (array)->count -= (cuts);                 \
    }                                           \
  } while (0)

/*
  ############################################################
  ####################### COMMAND AREA #######################
  ############################################################
*/

redcomp_array_dynamic_make(const char *, RedcompCommand);

/*
  ############################################################
  #################### COMMAND OPERATIONS ####################
  ############################################################
*/

#if defined(__linux__)

RED_DEF void redcomp_command_insert(RedcompCommand *command, ...);
RED_DEF void redcomp_command_display(const RedcompCommand *command);
RED_DEF bool redcomp_command_run_function(RedcompCommand *command, bool display, const char *file, i32 line);
RED_DEF bool redcomp_command_run_call_command_function(RedcompCommand *command, bool display, const char *file, i32 line);

#define redcomp_command_run(command, display)                            \
  redcomp_command_run_function((command), (display), __FILE__, __LINE__)

#endif /* __linux__ */

/*
  ############################################################
  ######################## FLAG AREA #########################
  ############################################################
*/

typedef enum {
  REDCOMP_FLAG_TYPE_BOOLEAN = 0,
  REDCOMP_FLAG_TYPE_NUMBER,
  REDCOMP_FLAG_TYPE_STRING,
  REDCOMP_FLAG_TYPE_COUNT
} RedcompFlagType;

typedef union {
  bool on;
  u64 number;
  const char *string;
} RedcompFlagValue;

typedef struct {
  const char *name_short;
  const char *name_long;
  const char *description;
  RedcompFlagType type;
  bool parsed, padding[3];
  RedcompFlagValue value;
  RedcompFlagValue default_value;
} RedcompFlag;

redcomp_array_view_make(RedcompFlag *, RedcompFlags);
redcomp_array_view_iter_make(const char *, RedcompArgs);

/*
  ############################################################
  ##################### FLAG OPERATIONS ######################
  ############################################################
*/

RED_DEF RedcompFlag redcomp_flag_make(const char *name_short, const char *name_long, const char *description, RedcompFlagType type, RedcompFlagValue default_value);

RED_DEF bool redcomp_parse_numeric_argument(const char *arg, u64 *result);
RED_DEF bool redcomp_flags_parse_function(RedcompArgs *args, RedcompFlags *flags, const char *file, i32 line);

RED_DEF size_t redcomp_max_pad_from_max_length(size_t max_length) REDCOMP_CONST;
RED_DEF void redcomp_print_usage_function(FILE *output_file, const char *program_name, RedcompFlags *flags, const char *file, i32 line);

#define redcomp_flags_parse(args, flags)                                \
  redcomp_flags_parse_function((args), (flags), __FILE__, __LINE__)

#define redcomp_print_usage(output_file, program_name, flags)           \
  redcomp_print_usage_function((output_file), (program_name), (flags), __FILE__, __LINE__)

/*
  ############################################################
  ######################## ARENA AREA ########################
  ############################################################
*/

redcomp_array_dynamic_make(char, RedcompRegion);
redcomp_array_dynamic_make(RedcompRegion, RedcompArena);

/*
  ############################################################
  ##################### ARENA OPERATIONS #####################
  ############################################################
*/

RED_DEF void redcomp_region_make(RedcompArena *arena, size_t reserve);
RED_DEF void *redcomp_region_alloc_function(RedcompRegion *region, size_t reserve, const char *file, i32 line) REDCOMP_RETURNS_NONNULL;
RED_DEF void *redcomp_arena_alloc_function(RedcompArena *arena, size_t reserve, const char *file, i32 line) REDCOMP_RETURNS_NONNULL;
RED_DEF void redcomp_arena_free(RedcompArena *arena);

#define redcomp_arena_alloc(arena, reserve)                             \
  redcomp_arena_alloc_function((arena), (reserveb), __FILE__, __LINE__)

/*
  ############################################################
  ################## TEMPORARY BUFFER AREA ###################
  ############################################################
*/

#ifndef REDCOMP_TEMPORARY_BUFFER_SIZE
#define REDCOMP_TEMPORARY_BUFFER_SIZE 1024*16
#endif /* REDCOMP_TEMPORARY_BUFFER_SIZE */

static char redcomp_temporary_buffer[REDCOMP_TEMPORARY_BUFFER_SIZE];
static size_t redcomp_temporary_buffer_cursor;

/*
  ############################################################
  ############### TEMPORARY BUFFER OPREATIONS ################
  ############################################################
*/

RED_DEF const char *redcomp_temporary_format(const char *format, ...) REDCOMP_FORMAT(1, 2) REDCOMP_RETURNS_NONNULL;
RED_DEF size_t redcomp_temporary_buffer_cursor_get(void) REDCOMP_PURE;
RED_DEF void redcomp_temporary_buffer_cursor_set(size_t cursor);

/*
  ############################################################
  ####################### STRING AREA ########################
  ############################################################
*/

#define REDCOMP_STRING_VIEW_ARG(string_view) (int) (string_view).count, (string_view).first
#define REDCOMP_STRING_ARG(string) (int) (string).count, (string).first
#define REDCOMP_STRING_VIEW_MAKE(string)        \
  {                                             \
    #string,              /* string */          \
    sizeof (#string) - 1  /* length */          \
  }

redcomp_array_view_make(const char, RedcompStringView);
redcomp_array_dynamic_make(char, RedcompString);

/*
  ############################################################
  #################### STRING OPERATIONS #####################
  ############################################################
*/

RED_DEF bool redcomp_string_read_from_file_function(const char *file_path, RedcompString *buffer, const char *file, i32 line);
RED_DEF bool redcomp_string_write_to_file_function(const char *file_path, const RedcompString *buffer, const char *file, i32 line);

RED_DEF void redcomp_string_insert_cstring_sized(RedcompString *redcomp_string, const char *cstring, size_t size);
RED_DEF void redcomp_string_insert_string_view(RedcompString *redcomp_string, RedcompStringView string_view);
RED_DEF void redcomp_string_insert_cstring_unsized(RedcompString *redcomp_string, const char *cstring);
RED_DEF void redcomp_string_insert_number_function(RedcompString *string, u64 number, const char *file, i32 line);
RED_DEF void redcomp_string_insert_format_function(RedcompString *string, const char *file, i32 line, const char *format, ...) REDCOMP_FORMAT(4, 5);
RED_DEF void redcomp_string_insert_format_va_list_function(RedcompString *string, const char *file, i32 line, const char *format, va_list args);

/*
  ############################################################
  ################## STRING VIEW OPERATIONS ##################
  ############################################################
*/

RED_DEF bool redcomp_string_view_starts_with_cstring_sized(RedcompStringView string_view, const char *start, size_t size) REDCOMP_PURE;
RED_DEF bool redcomp_string_view_starts_with_string_view(RedcompStringView string_view, RedcompStringView start) REDCOMP_PURE;
RED_DEF bool redcomp_string_view_starts_with_cstring_unsized(RedcompStringView string_view, const char *start) REDCOMP_PURE;

RED_DEF void redcomp_string_view_cut_once(RedcompStringView *string_view);
RED_DEF void redcomp_string_view_cut(RedcompStringView *string_view, size_t count);
RED_DEF size_t redcomp_string_view_contains_character(RedcompStringView string_view, char character) REDCOMP_PURE;
RED_DEF bool redcomp_string_view_equals(RedcompStringView string_view1, RedcompStringView string_view2) REDCOMP_PURE;
RED_DEF RedcompStringView redcomp_string_view_from_cstring_unsized(const char *cstring) REDCOMP_PURE;

#define redcomp_read_from_file(file_path, buffer)                       \
  redcomp_string_read_from_file_function((file_path), (buffer), __FILE__, __LINE__)

#define redcomp_write_to_file(file_path, buffer)                       \
  redcomp_string_write_to_file_function((file_path), (buffer), __FILE__, __LINE__)

#define redcomp_string_insert_number(string, number)                    \
  redcomp_string_insert_number_function((string), (number), __FILE__, __LINE__)

/*
  ############################################################
  ################# BUILD SYSTEM OPERATIONS ##################
  ############################################################
*/

RED_DEF bool redcomp_program_needs_rebuild_function(const char *program_path, const char *source_path, const char *file, i32 line);
RED_DEF bool redcomp_program_needs_rebuild_multiple_sources_function(const char *program_path, const char **sources_paths, size_t sources_count, const char *file, i32 line);
RED_DEF bool redcomp_file_time_get_function(const char *file_path, time_t *file_time, const char *file, i32 line);

#define redcomp_program_needs_rebuild(program_path, source_path)        \
  redcomp_program_needs_rebuild_function((program_path), (source_path), __FILE__, __LINE__)

#define redcomp_program_needs_rebuild_multiple_sources(program_path, sources_paths, sources_count) \
  redcomp_program_needs_rebuild_multiple_sources_function((program_path), (sources_paths), (sources_count), __FILE__, __LINE__)

#define redcomp_file_time_get(file_path, file_time)                     \
  redcomp_file_time_get((file_path), (file_time), __FILE__, __LINE__)

/*
  ############################################################
  ############### STANDARD LIBRARY REPLACEMENT ###############
  ############################################################
*/

RED_DEF void redcomp_assert_function(bool condition, const char *file, i32 line, const char *message);
RED_DEF void redcomp_assert_from_function(bool condition, const char *file, i32 line, const char *origin_file, i32 origin_line, const char *message);

#define redcomp_assert(condition, message)                               \
  redcomp_assert_function((condition), __FILE__, __LINE__, (message))

#define redcomp_assert_from(condition, origin_file, origin_line, message) \
  redcomp_assert_from_function((condition), __FILE__, __LINE__, (origin_file), (origin_line), (message))

RED_DEF void *redcomp_realloc_wrapper(void *original_pointer, size_t size, const char *file, i32 line) REDCOMP_RETURNS_NONNULL;

RED_DEF clock_t redcomp_current_time_get(void);
RED_DEF double redcomp_elapsed_time_get(clock_t start, clock_t end) REDCOMP_CONST;
RED_DEF void redcomp_elapsed_time_print(clock_t start, clock_t end, const char *subject);

typedef struct {
  u64 bytes[512];
} RedcompBytes4096;

typedef struct {
  u64 bytes[256];
} RedcompBytes2048;

typedef struct {
  u64 bytes[128];
} RedcompBytes1024;

typedef struct {
  u64 bytes[64];
} RedcompBytes512;

typedef struct {
  u64 bytes[32];
} RedcompBytes256;

typedef struct {
  u64 bytes[16];
} RedcompBytes128;

typedef struct {
  u64 bytes[8];
} RedcompBytes64;

typedef struct {
  u64 bytes[4];
} RedcompBytes32;

typedef struct {
  u64 bytes[2];
} RedcompBytes16;

RED_DEF void redcomp_string_copy(char *destination, const char *source, size_t size);
RED_DEF void redcomp_string_set(char *destination, u8 byte, size_t size);
RED_DEF void redcomp_memory_copy(void *destination, const void *source, size_t size);
RED_DEF void redcomp_memory_set(void *destination, u8 byte, size_t size);

#ifndef REDCOMP_KEEP_PREFIXES
/*
  ############################################################
  ##################### ARRAY OPERATIONS #####################
  ############################################################
*/

#define array_dynamic_make_inside redcomp_array_dynamic_make_inside
#define array_dynamic_iter_make_inside redcomp_array_dynamic_iter_make_inside
#define array_view_make_inside redcomp_array_view_make_inside
#define array_view_iter_make_inside redcomp_array_view_iter_make_inside
#define array_dynamic_make redcomp_array_dynamic_make
#define array_dynamic_iter_make redcomp_array_dynamic_iter_make
#define array_view_make redcomp_array_view_make
#define array_view_iter_make redcomp_array_view_iter_make

#define ARRAY_LENGTH REDCOMP_ARRAY_LENGTH

#define array_reserve redcomp_array_reserve
#define array_insert redcomp_array_insert
#define array_insert_many redcomp_array_insert_many

#define iter_to_index redcomp_iter_to_index

#define iter_first redcomp_iter_first
#define iter_next redcomp_iter_next
#define iter_forward redcomp_iter_forward
#define iter_prev redcomp_iter_prev

#define iter_peek_index redcomp_iter_peek_index

#define iter_peek_next redcomp_iter_peek_next
#define iter_peek_forward redcomp_iter_peek_forward
#define iter_peek_prev redcomp_iter_peek_prev

#define array_cut_once redcomp_array_cut_once
#define array_cut redcomp_array_cut

/*
  ############################################################
  ####################### COMMAND AREA #######################
  ############################################################
*/

#define Command RedcompCommand

/*
  ############################################################
  #################### COMMAND OPERATIONS ####################
  ############################################################
*/

#if defined(__linux__)

#define command_insert redcomp_command_insert
#define command_display redcomp_command_display
#define command_run_function redcomp_command_run_function
#define command_run_call_command_function redcomp_command_run_call_command_function

#define command_run redcomp_command_run

#endif /* __linux__ */

/*
  ############################################################
  ######################## FLAG AREA #########################
  ############################################################
*/

#define FlagType RedcompFlagType
#define FlagValue RedcompFlagValue
#define Flag RedcompFlag
#define Flags RedcompFlags
#define Args RedcompArgs

/*
  ############################################################
  ##################### FLAG OPERATIONS ######################
  ############################################################
*/

#define flag_make redcomp_flag_make

#define parse_numeric_argument redcomp_parse_numeric_argument
#define flags_parse_function redcomp_flags_parse_function
#define max_pad_from_max_length redcomp_max_pad_from_max_length
#define print_usage_function redcomp_print_usage_function

#define flags_parse redcomp_flags_parse
#define print_usage redcomp_print_usage

/*
  ############################################################
  ######################## ARENA AREA ########################
  ############################################################
*/

#define Region RedcompRegion
#define Arena RedcompArena

/*
  ############################################################
  ##################### ARENA OPERATIONS #####################
  ############################################################
*/

#define region_make redcomp_region_make
#define region_alloc_function redcomp_region_alloc_function
#define arena_alloc_function redcomp_arena_alloc_function
#define arena_free redcomp_arena_free

#define arena_alloc redcomp_arena_alloc

/*
  ############################################################
  ############### TEMPORARY BUFFER OPREATIONS ################
  ############################################################
*/

#define temporary_format redcomp_temporary_format
#define temporary_buffer_cursor_get redcomp_temporary_buffer_cursor_get
#define temporary_buffer_cursor_set redcomp_temporary_buffer_cursor_set

/*
  ############################################################
  ####################### STRING AREA ########################
  ############################################################
*/

#define STRING_VIEW_ARG REDCOMP_STRING_VIEW_ARG
#define STRING_ARG REDCOMP_STRING_ARG
#define STRING_VIEW_MAKE REDCOMP_STRING_VIEW_MAKE

#define StringView RedcompStringView
#define String RedcompString

/*
  ############################################################
  #################### STRING OPERATIONS #####################
  ############################################################
*/

#define string_read_from_file_function redcomp_string_read_from_file_function
#define string_write_to_file_function redcomp_string_write_to_file_function

#define string_insert_cstring_sized redcomp_string_insert_cstring_sized
#define string_insert_string_view redcomp_string_insert_string_view
#define string_insert_cstring_unsized redcomp_string_insert_cstring_unsized
#define string_insert_number_function redcomp_string_insert_number_function
#define string_insert_format_function redcomp_string_insert_format_function
#define string_insert_format_va_list_function redcomp_string_insert_format_va_list_function

/*
  ############################################################
  ################## STRING VIEW OPERATIONS ##################
  ############################################################
*/

#define string_view_starts_with_cstring_sized redcomp_string_view_starts_with_cstring_sized
#define string_view_starts_with_string_view redcomp_string_view_starts_with_string_view
#define string_view_starts_with_cstring_unsized redcomp_string_view_starts_with_cstring_unsized

#define string_view_cut_once redcomp_string_view_cut_once
#define string_view_cut redcomp_string_view_cut
#define string_view_contains_character redcomp_string_view_contains_character
#define string_view_equals redcomp_string_view_equals
#define string_view_from_cstring_unsized redcomp_string_view_from_cstring_unsized

#define read_from_file redcomp_read_from_file
#define write_to_file redcomp_write_to_file
#define string_insert_number redcomp_string_insert_number

/*
  ############################################################
  ################# BUILD SYSTEM OPERATIONS ##################
  ############################################################
*/

#define program_needs_rebuild redcomp_program_needs_rebuild
#define program_needs_rebuild_multiple_sources_function redcomp_program_needs_rebuild_multiple_sources_function
#define file_time_get_function redcomp_file_time_get_function
#define program_needs_rebuild redcomp_program_needs_rebuild
#define program_needs_rebuild_multiple_sources redcomp_program_needs_rebuild_multiple_sources
#define file_time_get redcomp_file_time_get

/*
  ############################################################
  ############### STANDARD LIBRARY REPLACEMENT ###############
  ############################################################
*/

#define assert_function redcomp_assert_function
#define assert_from_function redcomp_assert_from_function
#define assert redcomp_assert
#define assert_from redcomp_assert_from
#define realloc_wrapper redcomp_realloc_wrapper

#define current_time_get redcomp_current_time_get
#define elapsed_time_get redcomp_elapsed_time_get
#define elapsed_time_print redcomp_elapsed_time_print

#define string_copy redcomp_string_copy
#define string_set redcomp_string_set
#define memory_copy redcomp_memory_copy
#define memory_set redcomp_memory_set

#endif /* REDCOMP_KEEP_PREFIXES */

#endif /* REDCOMP_H_ */

#ifdef REDCOMP_IMPLEMENTATION

RED_DEF void redcomp_assert_function(bool condition, const char *file, i32 line, const char *message) {
  if (condition) return;
  fprintf(stderr, "%s:%d: %s\n", file, line, message);
  abort();
}

RED_DEF void redcomp_assert_from_function(bool condition, const char *file, i32 line, const char *origin_file, i32 origin_line, const char *message) {
  if (condition) return;
  fprintf(stderr, "%s:%d: %s\n", file, line, message);
  fprintf(stderr, "%s:%d: Called from here.\n", origin_file, origin_line);
  abort();
}

RED_DEF void *redcomp_realloc_wrapper(void *original_pointer, size_t new_size, const char *file, i32 line) {
  void *new_pointer;
  new_pointer = realloc(original_pointer, new_size);
  redcomp_assert_from(new_pointer != (void *) 0, file, line, "Allocation failed");
  return new_pointer;
}

/*
  ############################################################
  #################### COMMAND OPERATIONS ####################
  ############################################################
*/

#if defined(__linux__)

RED_DEF void redcomp_command_insert(RedcompCommand *command, ...) {
  const char *arg;
  va_list args;
  va_start(args, command);
  do {
    arg = va_arg(args, const char *);
    if (arg) redcomp_array_insert(command, arg);
  } while (arg);
  va_end(args);
}

RED_DEF void redcomp_command_display(const RedcompCommand *command) {
  size_t i;
  const char *arg;
  printf("[COMMAND] ");
  for (i = 0; i < command->count; i++) {
    arg = command->first[i];
    if (arg == (void *) 0) continue;
    printf("%s ", arg);
  }
  putchar(10);
}

RED_DEF bool redcomp_command_run_function(RedcompCommand *command, bool display, const char *file, i32 line) {
  bool success;
  redcomp_assert_from(command != (void *) 0, file, line, "Cannot run null pointer command.");
  redcomp_assert_from(command->first != (void *) 0, file, line, "Cannot run an empty command.");
  redcomp_array_insert(command, (void *) 0);
  success = redcomp_command_run_call_command_function(command, display, file, line);
  command->count = 0;
  return success;
}

RED_DEF bool redcomp_command_run_call_command_function(RedcompCommand *command, bool display, const char *file, i32 line) {
  int wstatus, exit_status;
  pid_t cpid;

  if (display) redcomp_command_display(command);

  cpid = fork();
  if (cpid < 0) {
    fprintf(stderr, "%s:%d: error: could not fork child process.\n", file, line);
    return false;
  }

  if (cpid == 0) {
    if (execvp(command->first[0], (char *const *) command->first) < 0) {
      fprintf(stderr, "%s:%d: error: could not exec child process for %s.\n", file, line, command->first[0]);
      return false;
    }
  }

  for (;;) {
    wstatus = 0;
    if (waitpid(cpid, &wstatus, 0) < 0) {
      fprintf(stderr, "%s:%d: error: could not wait on command (pid %d).\n", file, line, cpid);
      return false;
    }

    if (WIFEXITED(wstatus)) {
      exit_status = WEXITSTATUS(wstatus);
      if (exit_status != 0) {
        fprintf(stderr, "%s:%d: error: command exited with exit code %d.\n", file, line, exit_status);
        return false;
      }
      break;
    }

    if (WIFSIGNALED(wstatus)) {
      fprintf(stderr, "%s:%d: error: command process was terminated by signal %d.\n", file, line, WTERMSIG(wstatus));
      return false;
    }
  }

  return true;
}

#endif /* __linux__ */

/*
  ############################################################
  ##################### FLAG OPERATIONS ######################
  ############################################################
*/

RED_DEF RedcompFlag redcomp_flag_make(const char *name_short, const char *name_long, const char *description, RedcompFlagType type, RedcompFlagValue default_value) {
  RedcompFlag flag;
  memset(&flag, 0, sizeof flag);
  flag.name_short = name_short;
  flag.name_long = name_long;
  flag.description = description;
  flag.type = type;
  flag.default_value = default_value;
  return flag;
}

RED_DEF bool redcomp_parse_numeric_argument(const char *arg, u64 *result) {
  size_t i;
  char c;
  *result = 0;
  for (i = 0; i < strlen(arg); i++) {
    c = arg[i];
    if (c < '0' || c > '9') {
      fprintf(stderr, "Failed to parse number %s.\n", arg);
      return false;
    }
    *result = *result * 10 + (u64) (c - '0');
  }
  return true;
}

RED_DEF bool redcomp_flags_parse_function(RedcompArgs *args, RedcompFlags *flags, const char *file, i32 line) {
  size_t i;
  u64 value;
  RedcompFlag *flag;
  if (!redcomp_iter_first(args)) return false;
  do {
    for (i = 0; i < flags->count; i++) {
      flag = flags->first[i];
      if (strcmp(*args->iter, flag->name_short) != 0 &&
          strcmp(*args->iter, flag->name_long) != 0) continue;
      flag->parsed = true;
      switch (flag->type) {
      case REDCOMP_FLAG_TYPE_BOOLEAN:
        flag->value.on = true;
        break;
      case REDCOMP_FLAG_TYPE_NUMBER:
        if (!redcomp_iter_next(args)) return false;
        if (!redcomp_parse_numeric_argument(*args->iter, &value)) return false;
        flag->value.number = value;
        break;
      case REDCOMP_FLAG_TYPE_STRING:
        if (!redcomp_iter_next(args)) return false;
        flag->value.string = *args->iter;
        break;
      case REDCOMP_FLAG_TYPE_COUNT: default:
        redcomp_assert_from(false, file, line, "Unreachable.");
        break;
      }
      break;
    }
  } while (redcomp_iter_next(args));

  for (i = 0; i < flags->count; i++) {
    flag = flags->first[i];
    if (flag->parsed) continue;
    flag->value = flag->default_value;
  }

  return true;
}

RED_DEF size_t redcomp_max_pad_from_max_length(size_t max_length) {
  size_t max_pad, max_pad_rem;
  max_pad = max_length;
  max_pad_rem = max_pad % 4;
  if (max_pad_rem != 0) max_pad += (4 - max_pad_rem);
  if (max_pad <= max_length + 1) max_pad += 2;
  return max_pad;
}

RED_DEF void redcomp_print_usage_function(FILE *output_file, const char *program_name, RedcompFlags *flags, const char *file, i32 line) {
  RedcompFlag *flag;
  size_t i, length;
  size_t name_short_max_length, name_long_max_length;
  size_t name_short_max_pad, name_long_max_pad;
  size_t name_short_pad, name_long_pad;

  length = 0;
  name_short_max_length = 0;
  name_long_max_length = 0;

  fprintf(output_file, "Usage: %s [FLAGS]\n\n", program_name);
  fprintf(output_file, "FLAGS:\n");

  for (i = 0; i < flags->count; i++) {
    flag = flags->first[i];
    switch (flag->type) {
    case REDCOMP_FLAG_TYPE_BOOLEAN:
      length = strlen(flag->name_short);
      if (length > name_short_max_length) name_short_max_length = length;
      length = strlen(flag->name_long);
      if (length > name_long_max_length) name_long_max_length = length;
      break;
    case REDCOMP_FLAG_TYPE_NUMBER:
      length = strlen(flag->name_short) + strlen(" <number>");
      if (length > name_short_max_length) name_short_max_length = length;
      length = strlen(flag->name_long) + strlen(" <number>");
      if (length > name_long_max_length) name_long_max_length = length;
      break;
    case REDCOMP_FLAG_TYPE_STRING:
      length = strlen(flag->name_short) + strlen(" <string>");
      if (length > name_short_max_length) name_short_max_length = length;
      length = strlen(flag->name_long) + strlen(" <string>");
      if (length > name_long_max_length) name_long_max_length = length;
      break;
    case REDCOMP_FLAG_TYPE_COUNT: default:
      redcomp_assert_from(false, file, line, "Unreachable.");
      break;
    }
  }

  name_short_max_pad = redcomp_max_pad_from_max_length(name_short_max_length);
  name_long_max_pad = redcomp_max_pad_from_max_length(name_long_max_length);

  for (i = 0; i < flags->count; i++) {
    flag = flags->first[i];
    name_short_pad = name_short_max_pad - strlen(flag->name_short);
    name_long_pad = name_long_max_pad - strlen(flag->name_long);
    switch (flag->type) {
    case REDCOMP_FLAG_TYPE_BOOLEAN:
      fprintf(output_file, "%s%*s| %s%*s:\n",
              flag->name_short, (i32) name_short_pad, "",
              flag->name_long, (i32) name_long_pad, "");
      fprintf(output_file, "    %s\n", flag->description);
      fprintf(output_file, "    Default value: %s.\n", flag->default_value.on ? "true" : "false");
      break;
    case REDCOMP_FLAG_TYPE_NUMBER:
      name_short_pad -= strlen(" <number>");
      name_long_pad -= strlen(" <number>");
      fprintf(output_file, "%s <number>%*s| %s <number>%*s:\n",
              flag->name_short, (i32) name_short_pad, "",
              flag->name_long, (i32) name_long_pad, "");
      fprintf(output_file, "    %s\n", flag->description);
      fprintf(output_file, "    Default value: " U64_FMT ".\n", flag->default_value.number);
      break;
    case REDCOMP_FLAG_TYPE_STRING:
      name_short_pad -= strlen(" <string>");
      name_long_pad -= strlen(" <string>");
      fprintf(output_file, "%s <string>%*s| %s <string>%*s:\n",
              flag->name_short, (i32) name_short_pad, "",
              flag->name_long, (i32) name_long_pad, "");
      fprintf(output_file, "    %s\n", flag->description);
      fprintf(output_file, "    Default value: %s.\n", flag->default_value.string);
      break;
    case REDCOMP_FLAG_TYPE_COUNT: default:
      redcomp_assert_from(false, file, line, "Unreachable.");
      break;
    }
  }
  putchar(10);
}

/*
  ############################################################
  ##################### ARENA OPERATIONS #####################
  ############################################################
*/

RED_DEF void redcomp_region_make(RedcompArena *arena, size_t reserve) {
  RedcompRegion region;
  memset(&region, 0, sizeof region);
  region.reserved = REDCOMP_ARENA_DEFAULT_RESERVE;
  if (reserve > region.reserved) region.reserved = reserve;
  region.first = malloc(region.reserved);
  redcomp_array_insert(arena, region);
}

RED_DEF void *redcomp_region_alloc_function(RedcompRegion *region, size_t reserve, const char *file, i32 line) {
  void *result;
  redcomp_assert_from(region->count + reserve <= region->reserved, file, line, "Unreachable.");
  redcomp_assert_from(region->first != (void *) 0, file, line, "Cannot allocate in a region pointing to null.");
  result = region->first + region->count;
  region->count += reserve;
  return result;
}

RED_DEF void *redcomp_arena_alloc_function(RedcompArena *arena, size_t reserve, const char *file, i32 line) {
  size_t i;
  RedcompRegion *region;
  for (i = 0; i < arena->count; i++) {
    region = arena->first + i;
    if (region->count + reserve > region->reserved) continue;
    return redcomp_region_alloc_function(region, reserve, file, line);
  }
  redcomp_region_make(arena, reserve);
  region = arena->first + i;
  return redcomp_region_alloc_function(region, reserve, file, line);

}

RED_DEF void redcomp_arena_free(RedcompArena *arena) {
  size_t i;
  RedcompRegion region;
  memset(&region, 0, sizeof region);
  for (i = 0; i < arena->count; i++) {
    region = arena->first[i];
    if (region.first) free(region.first);
  }
  if (arena->first) free(arena->first);
  arena->count = 0;
}

/*
  ############################################################
  ############### TEMPORARY BUFFER OPREATIONS ################
  ############################################################
*/

RED_DEF const char *redcomp_temporary_format(const char *format, ...) {
  va_list args;
  size_t n;
  const char *result;

  result = redcomp_temporary_buffer + redcomp_temporary_buffer_cursor;

  va_start(args, format);
  n = (size_t) vsprintf(redcomp_temporary_buffer + redcomp_temporary_buffer_cursor, format, args);
  redcomp_assert(REDCOMP_TEMPORARY_BUFFER_SIZE > redcomp_temporary_buffer_cursor + n + 1, "Temporary buffer overrun.");
  va_end(args);

  redcomp_temporary_buffer_cursor += n;
  return result;
}

RED_DEF size_t redcomp_temporary_buffer_cursor_get(void) {
  return redcomp_temporary_buffer_cursor;
}

RED_DEF void redcomp_temporary_buffer_cursor_set(size_t cursor) {
  redcomp_temporary_buffer_cursor = cursor;
}

/*
  ############################################################
  #################### STRING OPERATIONS #####################
  ############################################################
*/

RED_DEF bool redcomp_string_read_from_file_function(const char *file_path, RedcompString *buffer, const char *file, i32 line) {
  RedcompFileSize file_size;
  FILE *input_file;
  input_file = fopen(file_path, "rb");
  if (input_file == (void *) 0) {
    fprintf(stderr, "%s:%d: error: could not open file `%s`.\n", file, line, file_path);
    return false;
  }

  if (fseek(input_file, 0, SEEK_END) != 0) {
    fprintf(stderr, "%s:%d: error: could not seek to the end of file `%s`.\n", file, line, file_path);
    fclose(input_file);
    return false;
  }

  file_size = ftell(input_file);
  if (file_size == -1) {
    fprintf(stderr, "%s:%d: error: could not measure size of file `%s`.\n", file, line, file_path);
    fclose(input_file);
    return false;
  }

  buffer->reserved = (size_t) file_size;
  buffer->first = malloc((size_t) file_size);
  if (buffer->first == (void *) 0) {
    fprintf(stderr, "%s:%d: error: malloc failed!\n", file, line);
    fclose(input_file);
    return false;
  }

  fseek(input_file, 0, SEEK_SET);
  if (fread(buffer->first, (size_t) file_size, 1, input_file) == 0) {
    fprintf(stderr, "%s:%d: error: could not read file `%s`. Either was an error, or the file was empty.\n", file, line, file_path);
  }
  buffer->count = buffer->reserved;

  fclose(input_file);
  return true;
}

RED_DEF bool redcomp_string_write_to_file_function(const char *file_path, const RedcompString *buffer, const char *file, i32 line) {
  FILE *output_file;
  output_file = fopen(file_path, "wb");
  if (output_file == (void *) 0) {
    fprintf(stderr, "%s:%d: error: could not open file `%s`.\n", file, line, file_path);
    return false;
  }

  if (buffer->count == 0 || buffer->first == (void *) 0) {
    fprintf(stderr, "%s:%d: error: invalid buffer.\n", file, line);
    fclose(output_file);
    return false;
  }

  fseek(output_file, 0, SEEK_SET);

  if (fwrite(buffer->first, 1, buffer->count, output_file) != buffer->count) {
    fprintf(stderr, "%s:%d: error: could not write to file `%s`.\n", file, line, file_path);
    perror("fwrite");
    return false;
  }

  fclose(output_file);
  return true;
}

RED_DEF void redcomp_string_insert_cstring_sized(RedcompString *redcomp_string, const char *cstring, size_t size) {
  redcomp_array_reserve(redcomp_string, redcomp_string->count + size);
  memcpy(redcomp_string->first + redcomp_string->count, cstring, size);
  redcomp_string->count += size;
}

RED_DEF void redcomp_string_insert_string_view(RedcompString *redcomp_string, RedcompStringView string_view) {
  redcomp_string_insert_cstring_sized(redcomp_string, string_view.first, string_view.count);
}

RED_DEF void redcomp_string_insert_cstring_unsized(RedcompString *redcomp_string, const char *cstring) {
  redcomp_string_insert_cstring_sized(redcomp_string, cstring, strlen(cstring));
}

RED_DEF void redcomp_string_insert_number_function(RedcompString *string, u64 number, const char *file, i32 line) {
#define NUMBER_BUFFER_LENGTH 20
  char buffer[NUMBER_BUFFER_LENGTH];
  size_t count;
  char digit;
  memset(buffer, 0, sizeof buffer);
  count = NUMBER_BUFFER_LENGTH;
  do {
    redcomp_assert_from(count > 0, file, line, "Invalid memory access.");
    digit = (char) (number % 10);
    buffer[--count] = (char) (digit + '0');
    number /= 10;
  } while (number);
  redcomp_string_insert_cstring_sized(string, buffer + count, NUMBER_BUFFER_LENGTH - count);
}

RED_DEF void redcomp_string_insert_format_function(RedcompString *string, const char *file, i32 line, const char *format, ...) {
  va_list args;
  va_start(args, format);
  redcomp_string_insert_format_va_list_function(string, file, line, format, args);
  va_end(args);
}

RED_DEF void redcomp_string_insert_format_va_list_function(RedcompString *string, const char *file, i32 line, const char *format, va_list args) {
  const char *string_arg;
  const char *current;
  size_t string_arg_size, current_length;
  u64 number_arg;
  i64 signed_number_arg;
  char cursor, char_arg;
  bool arg_is_signed_number, arg_is_number, arg_is_character;
  bool arg_is_cstring_unsized, arg_is_cstring_sized;
  bool pad_left, pad_right;
  const char *end;

  current = format;
  current_length = 0;
  cursor = *format;
  end = format + strlen(format);
  redcomp_array_reserve(string, string->count + strlen(format));

  while (format < end) {
    /* Handle formatting. */
    if (cursor == '%') {
      redcomp_string_insert_cstring_sized(string, current, current_length);
      if (format < end) cursor = *format;
      format++;
      arg_is_signed_number = false;
      arg_is_number = false;
      arg_is_character = false;
      arg_is_cstring_unsized = false;
      arg_is_cstring_sized = false;
      pad_left = false;
      pad_right = false;
      if (cursor == 'd') {
        /* Handle signed integer formatting. */
        signed_number_arg = (i64) va_arg(args, i32);
        arg_is_signed_number = true;
      } else if (cursor == 'u') {
        /* Handle unsigned integer formatting. */
        number_arg = (u64) va_arg(args, u32);
        arg_is_number = true;
      } else if (cursor == 'l') {
        if (format < end) cursor = *format;
        format++;
        if (cursor == 'd') {
          /* Handle long signed integer formatting. */
          signed_number_arg = va_arg(args, i64);
          arg_is_signed_number = true;
        }
        else if (cursor == 'u') {
          /* Handle long unsigned integer formatting. */
          number_arg = va_arg(args, u64);
          arg_is_number = true;
        } else {
          redcomp_assert_from(false, file, line, redcomp_temporary_format("Character sequence %%l%c isn't supported in this string formatting function.", cursor));
        }
      } else if (cursor == 'c') {
        /* Handle character formatting. */
        char_arg = (char) va_arg(args, i32); /* ‘char’ is promoted to ‘int’ when passed through ‘...’ */
        arg_is_character = true;
      } else if (cursor == 's') {
        /* Handle string formatting. */
        string_arg = va_arg(args, const char *);
        arg_is_cstring_unsized = true;
      } else if (cursor == '.') {
        /* Handle sized string formatting. */
        if (format < end) cursor = *format;
        format++;
        if (cursor == '*') {
          if (format < end) cursor = *format;
          format++;
          if (cursor == 's') {
            string_arg_size = (size_t) va_arg(args, i32);
            string_arg = va_arg(args, const char *);
            arg_is_cstring_sized = true;
          }
        }
      } else if (cursor == '*') {
        if (format < end) cursor = *format;
        format++;
        if (cursor == '.') {
          /* Handle left-padded string formatting. */
          if (format < end) cursor = *format;
          format++;
          if (cursor == 's') {
            /* Not entirely sure how else to handle this. */
            signed_number_arg = (i64) va_arg(args, i32);
            if (signed_number_arg < 0) {
              signed_number_arg *= -1;
            }
            string_arg_size = (size_t) signed_number_arg;
            string_arg = va_arg(args, const char *);
            arg_is_cstring_unsized = true;
            pad_left = true;
          } else {
            redcomp_assert_from(false, file, line, redcomp_temporary_format("Character sequence %%*.%c isn't supported in this string formatting function.", cursor));
          }
        } else if (cursor == 's') {
          /* Handle padded string formatting. */
          signed_number_arg = (i64) va_arg(args, i32);
          string_arg = va_arg(args, const char *);
          arg_is_cstring_unsized = true;
          if (signed_number_arg < 0) {
            signed_number_arg *= -1;
            pad_right = true;
          } else {
            pad_left = true;
          }
          string_arg_size = (size_t) signed_number_arg;
          if (string_arg_size >= strlen(string_arg)) {
            string_arg_size -= strlen(string_arg);
          } else {
            string_arg_size = 0;
          }
        } else {
          redcomp_assert_from(false, file, line, redcomp_temporary_format("Character sequence %%*%c isn't supported in this string formatting function.", cursor));
        }
      } else if (cursor == '%') {
        /* Handle `%` formatting. */
        redcomp_array_insert(string, cursor);
      } else {
        redcomp_assert_from(false, file, line, redcomp_temporary_format("Character sequence %%%c isn't supported in this string formatting function.", cursor));
      }

      if (arg_is_signed_number) {
        if (signed_number_arg < 0) {
          signed_number_arg *= -1;
          redcomp_array_insert(string, '-');
        }
        number_arg = (u64) signed_number_arg;
        arg_is_number = true;
      }

      if (arg_is_number) {
        redcomp_string_insert_number_function(string, number_arg, file, line);
      } else if (arg_is_character) {
        redcomp_array_insert(string, char_arg);
      } else if (arg_is_cstring_unsized) {
        if (pad_left) {
          redcomp_array_reserve(string, string->count + string_arg_size);
          memset(string->first + string->count, ' ', string_arg_size);
          string->count += string_arg_size;
        }
        redcomp_string_insert_cstring_unsized(string, string_arg);
        if (pad_right) {
          redcomp_array_reserve(string, string->count + string_arg_size);
          memset(string->first + string->count, ' ', string_arg_size);
          string->count += string_arg_size;
        }
      } else if (arg_is_cstring_sized) {
        redcomp_string_insert_cstring_sized(string, string_arg, string_arg_size);
      }

      current_length = 0;
      current = format;
    }

    if (format < end) cursor = *format;
    format++;
    if (cursor != '%') current_length++;
  }

  redcomp_string_insert_cstring_sized(string, current, current_length);
}

/*
  ############################################################
  ################## STRING VIEW OPERATIONS ##################
  ############################################################
*/

RED_DEF bool redcomp_string_view_starts_with_cstring_sized(RedcompStringView string_view, const char *start, size_t size) {
  const char *save;
  save = string_view.first;
  if (string_view.count < size) return false;
  while (string_view.first < save + size) {
    if (*string_view.first++ != *start++) return false;
  }
  return true;
}

RED_DEF bool redcomp_string_view_starts_with_string_view(RedcompStringView string_view, RedcompStringView start) {
  return redcomp_string_view_starts_with_cstring_sized(string_view, start.first, start.count);
}

RED_DEF bool redcomp_string_view_starts_with_cstring_unsized(RedcompStringView string_view, const char *start) {
  return redcomp_string_view_starts_with_cstring_sized(string_view, start, strlen(start));
}

RED_DEF void redcomp_string_view_cut_once(RedcompStringView *string_view) {
  redcomp_array_cut_once(string_view);
}

RED_DEF void redcomp_string_view_cut(RedcompStringView *string_view, size_t count) {
  redcomp_array_cut(string_view, count);
}

RED_DEF size_t redcomp_string_view_contains_character(RedcompStringView string_view, char character) {
  size_t i;
  for (i = 0; i < string_view.count; i++) {
    if (string_view.first[i] == character) break;
  }
  return i;
}

RED_DEF bool redcomp_string_view_equals(RedcompStringView string_view1, RedcompStringView string_view2) {
  if (string_view1.count != string_view2.count) return false;
  return strncmp(string_view1.first, string_view2.first, string_view1.count) == 0;
}

RED_DEF RedcompStringView redcomp_string_view_from_cstring_unsized(const char *cstring) {
  RedcompStringView string_view;
  memset(&string_view, 0, sizeof string_view);
  string_view.first = cstring;
  string_view.count = strlen(cstring);
  return string_view;
}

/*
  ############################################################
  ################# BUILD SYSTEM OPERATIONS ##################
  ############################################################
*/

RED_DEF bool redcomp_program_needs_rebuild_function(const char *program_path, const char *source_path, const char *file, i32 line) {
  return redcomp_program_needs_rebuild_multiple_sources_function(program_path, &source_path, 1, file, line);
}

RED_DEF bool redcomp_program_needs_rebuild_multiple_sources_function(const char *program_path, const char **sources_paths, size_t sources_count, const char *file, i32 line) {
  time_t source_modified;
  time_t last_modified;
  time_t program_modified;
  size_t i;

  source_modified = 0;
  last_modified = 0;
  program_modified = 0;

  for (i = 0; i < sources_count; i++) {
    redcomp_assert_from(redcomp_file_time_get_function(sources_paths[i], &last_modified, file, line), file, line, "Failed to get time for source.");
    if (last_modified > source_modified) source_modified = last_modified;
  }

  redcomp_assert_from(redcomp_file_time_get_function(program_path, &program_modified, file, line), file, line, "Failed to get time for program.");
  if (errno == ENOENT) return true; /* Refer to the error check in redcomp_file_time_get */
  return source_modified > program_modified;
}

RED_DEF bool redcomp_file_time_get_function(const char *file_path, time_t *file_time, const char *file, i32 line) {
  struct stat statbuf;
  int result;
  result = stat(file_path, &statbuf);
  if (result == -1) {
    fprintf(stderr, "%s:%d: couldn't get time for %s\n", file, line, file_path);
    /* If the file cannot be found, this could mean the binary */
    /* executable doesn't exist when its last modified time is grabbed */
    /* for comparison with any of the source code files, so this */
    /* should return true since a missing binary file output should */
    /* mean redcompilation is, in fact, necessary. */
    if (errno == ENOENT) return true;
    return false;
  }
  *file_time = statbuf.st_mtime;
  return true;
}

/*
  ############################################################
  #################### TIMING OPERATIONS #####################
  ############################################################
*/

RED_DEF clock_t redcomp_current_time_get(void) {
  return clock();
}

RED_DEF double redcomp_elapsed_time_get(clock_t start, clock_t end) {
  return (double) (end - start) / CLOCKS_PER_SEC;
}

RED_DEF void redcomp_elapsed_time_print(clock_t start, clock_t end, const char *subject) {
  printf("\n%s: %.10f\n", subject, redcomp_elapsed_time_get(start, end));
}

RED_DEF void redcomp_string_copy(char *destination, const char *source, size_t size) {
  while (size / 4096) {
    *(RedcompBytes4096 *) destination = *(const RedcompBytes4096 *) source;
    destination += 4096;
    source += 4096;
    size -= 4096;
  }

  if (size / 2048) {
    *(RedcompBytes2048 *) destination = *(const RedcompBytes2048 *) source;
    destination += 2048;
    source += 2048;
    size -= 2048;
  }

  if (size / 1024) {
    *(RedcompBytes1024 *) destination = *(const RedcompBytes1024 *) source;
    destination += 1024;
    source += 1024;
    size -= 1024;
  }

  if (size / 512) {
    *(RedcompBytes512 *) destination = *(const RedcompBytes512 *) source;
    destination += 512;
    source += 512;
    size -= 512;
  }

  if (size / 256) {
    *(RedcompBytes256 *) destination = *(const RedcompBytes256 *) source;
    destination += 256;
    source += 256;
    size -= 256;
  }

  if (size / 128) {
    *(RedcompBytes128 *) destination = *(const RedcompBytes128 *) source;
    destination += 128;
    source += 128;
    size -= 128;
  }

  if (size / 64) {
    *(RedcompBytes64 *) destination = *(const RedcompBytes64 *) source;
    destination += 64;
    source += 64;
    size -= 64;
  }

  if (size / 32) {
    *(RedcompBytes32 *) destination = *(const RedcompBytes32 *) source;
    destination += 32;
    source += 32;
    size -= 32;
  }

  if (size / 16) {
    *(RedcompBytes16 *) destination = *(const RedcompBytes16 *) source;
    destination += 16;
    source += 16;
    size -= 16;
  }

  if (size / 8) {
    *(u64 *) destination = *(const u64 *) source;
    destination += 8;
    source += 8;
    size -= 8;
  }

  if (size / 4) {
    *(u32 *) destination = *(const u32 *) source;
    destination += 4;
    source += 4;
    size -= 4;
  }

  if (size / 2) {
    *(u16 *) destination = *(const u16 *) source;
    destination += 2;
    source += 2;
    size -= 2;
  }

  if (size) {
    *destination = *source;
  }
}

RED_DEF void redcomp_string_set(char *destination, u8 byte, size_t size) {
  RedcompBytes64 bytes;
  u64 value;
  value = (u64) byte << 56 | (u64) byte << 48 | (u64) byte << 40 | (u64) byte << 32 | (u64) byte << 24 | (u64) byte << 16 | (u64) byte << 8 | byte;
  bytes.bytes[0] = value;
  bytes.bytes[1] = value;
  bytes.bytes[2] = value;
  bytes.bytes[3] = value;
  bytes.bytes[4] = value;
  bytes.bytes[5] = value;
  bytes.bytes[6] = value;
  bytes.bytes[7] = value;

  while (size / 64) {
    *(RedcompBytes64 *) destination = bytes;
    destination += 64;
    size -= 64;
  }

  if (size > 31) {
    *(RedcompBytes32 *) destination = *(RedcompBytes32 *) bytes.bytes;
    destination += 32;
    size -= 32;
  }

  if (size > 15) {
    *(RedcompBytes16 *) destination = *(RedcompBytes16 *) bytes.bytes;
    destination += 16;
    size -= 16;
  }

  if (size > 7) {
    *(u64 *) destination = value;
    destination += 8;
    size -= 8;
  }

  if (size > 3) {
    *(u32 *) destination = (u32) value;
    destination += 4;
    size -= 4;
  }

  if (size > 1) {
    *(u16 *) destination = (u16) value;
    destination += 2;
    size -= 2;
  }

  if (size) {
    *(u8 *) destination = (u8) value;
  }
}

RED_DEF void redcomp_memory_copy(void *destination, const void *source, size_t size) {
    redcomp_string_copy(destination, source, size);
}

RED_DEF void redcomp_memory_set(void *destination, u8 byte, size_t size) {
  redcomp_string_set(destination, byte, size);
}

#endif /* REDCOMP_IMPLEMENTATION */
