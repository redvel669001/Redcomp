#define REDCOMP_IMPLEMENTATION
#include "redcomp.h"

#include <time.h>
#include <sys/times.h>

#define CFLAGS "-std=c89", "-Wall", "-Wextra", "-pedantic", "-Wduplicated-cond", "-Wduplicated-branches", "-Wlogical-op", "-Wnull-dereference", "-Wjump-misses-init", "-Wdouble-promotion", "-Wshadow", "-Wformat=2", "-Wformat-overflow=2", "-Wformat-truncation=2", "-Wformat-signedness", "-Winit-self", "-Wmissing-include-dirs", "-Wsync-nand", "-Wtrivial-auto-var-init", "-Wunused-const-variable=2", "-Wuse-after-free=3", "-Wstrict-flex-arrays", "-Walloc-zero", "-Wtrampolines", "-Wundef", "-Wunused-macros", "-Wbad-function-cast", "-Wcast-align", "-Wstrict-prototypes", "-Wold-style-definition", "-Wpacked", "-Wnested-externs", "-fstrict-flex-arrays", "-Wstrict-overflow=2", "-Wstringop-overflow=4", "-Warray-bounds=2", "-Warith-conversion", "-Wwrite-strings", "-Wdate-time", "-Wredundant-decls", "-Wrestrict", "-Wswitch-enum", "-Wpadded", "-Wconversion", "-Wmissing-declarations", "-Wmissing-prototypes", "-Wdeclaration-after-statement"

array_dynamic_iter_make(u64, Numbers);

void setup_flags(Flags *flags);
RED_DEF bool compile(const char *source_path, const char *output_path, Command *command, bool display);
RED_DEF void compare_formatting(void);

enum {
  FLAGS_BOOLEAN,
  FLAGS_NUMBER,
  FLAGS_STRING,
  FLAGS_USE_STRING,
  FLAGS_USE_NUMBER,
  FLAGS_FAST,
  FLAGS_DEBUG,
  FLAGS_ANALYZE,
  FLAGS_REBUILD,
  FLAGS_NO_REBUILD,
  FLAGS_SANITIZE_ADDRESS,
  FLAGS_SANITIZE_UNDEFINED,
  FLAGS_HELP,
  FLAGS_COUNT
};

static Flag flag_boolean, flag_number, flag_string, flag_use_string, flag_use_number, flag_fast, flag_debug, flag_analyze, flag_rebuild, flag_no_rebuild, flag_sanitize_address, flag_sanitize_undefined, flag_help;
static Flag *flags_array[FLAGS_COUNT];

int main(int argc, char **argv) {
  Command command;
  String string;
  String test_string;
  Numbers numbers;
  Flags flags;
  Flag flag;
  Args args;
  u64 i, max, number;
  char ch;
  clock_t insertions_start, insertions_end;
  clock_t printing_start, printing_end;
  const char *sources[2];

  memset(&command, 0, sizeof command);
  memset(&string, 0, sizeof string);
  memset(&test_string, 0, sizeof test_string);
  memset(&numbers, 0, sizeof numbers);
  memset(&flags, 0, sizeof flags);
  memset(&flag, 0, sizeof flag);
  memset(&args, 0, sizeof args);
  memset(sources, 0, sizeof sources);

  setup_flags(&flags);

  args.first = (const char **) argv;
  args.count = (u64) argc;

  if (!flags_parse(&args, &flags)) return 1;

  if (flag_help.value.on) {
    print_usage(stdout, *argv, &flags);
    return 0;
  }

  sources[0] = __FILE__;
  sources[1] = "./redcomp.h";

  temporary_buffer_cursor_set(0);
  if ((program_needs_rebuild_multiple_sources(*argv, sources, ARRAY_LENGTH(sources)) || flag_rebuild.value.on) && !flag_no_rebuild.value.on) {
    if (!compile(__FILE__, *argv, &command, true)) {
      if (command.first) {
        free(command.first);
        command.first = (void *) 0;
      }
      return 1;
    }

    array_insert(&command, *argv);
    for (i = 0; i < flags.count; i++) {
      flag = *flags.first[i];
      if (!flag.parsed) continue;
      array_insert(&command, flag.name_short);
      switch (flag.type) {
      case REDCOMP_FLAG_TYPE_BOOLEAN:
        break;
      case REDCOMP_FLAG_TYPE_NUMBER:
        array_insert(&command, temporary_format(U64_FMT, flag.value.number));
        break;
      case REDCOMP_FLAG_TYPE_STRING:
        array_insert(&command, flag.value.string);
        break;
      case REDCOMP_FLAG_TYPE_COUNT:
        redcomp_assert(false, "Unreachable.");
        break;
      }
    }

    array_insert(&command, flag_no_rebuild.name_short);
    if (!command_run(&command, true)) {
      if (command.first) {
        free(command.first);
        command.first = (void *) 0;
      }
      return 1;
    }
    if (command.first) free(command.first);
    return 0;
  }

  insertions_start = current_time_get();
  max = flag_number.value.number;
  for (i = 0; i < max; i++) {
    if (flag_use_string.value.on) {
      ch = (char) i;
      array_insert(&string, ch);
    } else if (flag_use_number.value.on) {
      array_insert(&numbers, i*i);
    }
  }

  insertions_end = current_time_get();
  iter_first(&numbers);
  printing_start = current_time_get();
  for (i = 0; i < max; i++) {
    if (flag_use_string.value.on) {
      ch = string.first[i];
      /* printf("string.first[" U64_FMT "] = %d\n", i, ch); */
    } else if (flag_use_number.value.on) {
      number = numbers.first[i];
      printf("numbers.first[" U64_FMT "] = " U64_FMT "\n", i, number);
    }
  }

  string.count = 0;

  printing_end = current_time_get();
  redcomp_elapsed_time_print(insertions_start, insertions_end, "Insertions");
  redcomp_elapsed_time_print(printing_start, printing_end, "Printing");
  if (numbers.first) free(numbers.first);
  if (string.first) free(string.first);
  if (test_string.first) free(test_string.first);
  if (command.first) free(command.first);
  return 0;
}

void setup_flags(Flags *flags) {
  FlagValue value;
  memset(&value, 0, sizeof value);
  value.on = true;
  flag_boolean = flag_make("-b", "--boolean", "Boolean flag.", REDCOMP_FLAG_TYPE_BOOLEAN, value);
  memset(&value, 0, sizeof value);
  value.number = 123;
  flag_number = flag_make("-n", "--number", "Number flag.", REDCOMP_FLAG_TYPE_NUMBER, value);
  memset(&value, 0, sizeof value);
  value.string = "default string";
  flag_string = flag_make("-s", "--string", "String flag.", REDCOMP_FLAG_TYPE_STRING, value);
  memset(&value, 0, sizeof value);
  flag_use_string = flag_make("-us", "--use-string", "Use a string.", REDCOMP_FLAG_TYPE_BOOLEAN, value);
  flag_use_number = flag_make("-un", "--use-number", "Use a number.", REDCOMP_FLAG_TYPE_BOOLEAN, value);
  flag_fast = flag_make("-f", "--fast", "Use optimization when compiling [-O3 flag].", REDCOMP_FLAG_TYPE_BOOLEAN, value);
  flag_debug = flag_make("-g", "--debug", "Compile with debug information [-g or --ggdb flag].", REDCOMP_FLAG_TYPE_BOOLEAN, value);
  flag_analyze = flag_make("-a", "--analyze", "Enable static analysis during compilation [-fanalyzer].", REDCOMP_FLAG_TYPE_BOOLEAN, value);
  flag_rebuild = flag_make("-rb", "--rebuild", "Force the build system to rebuild itself.", REDCOMP_FLAG_TYPE_BOOLEAN, value);
  flag_no_rebuild = flag_make("-nrb", "--no-rebuild", "Force the build system to NOT rebuild itself.", REDCOMP_FLAG_TYPE_BOOLEAN, value);
  flag_sanitize_address = flag_make("-sa", "--sanitize-address", "Compile with an address sanitizer [-fsanitize=address flag] to catch more bugs.", REDCOMP_FLAG_TYPE_BOOLEAN, value);
  flag_sanitize_undefined = flag_make("-su", "--sanitize-undefined", "Compile with an undefined behavior sanitizer [-fsanitize=undefined flag] to catch more bugs.", REDCOMP_FLAG_TYPE_BOOLEAN, value);
  flag_help = flag_make("-h", "--help", "Present this information.", REDCOMP_FLAG_TYPE_BOOLEAN, value);
  flags_array[FLAGS_BOOLEAN] = &flag_boolean;
  flags_array[FLAGS_NUMBER] = &flag_number;
  flags_array[FLAGS_STRING] = &flag_string;
  flags_array[FLAGS_USE_STRING] = &flag_use_string;
  flags_array[FLAGS_USE_NUMBER] = &flag_use_number;
  flags_array[FLAGS_FAST] = &flag_fast;
  flags_array[FLAGS_DEBUG] = &flag_debug;
  flags_array[FLAGS_ANALYZE] = &flag_analyze;
  flags_array[FLAGS_REBUILD] = &flag_rebuild;
  flags_array[FLAGS_NO_REBUILD] = &flag_no_rebuild;
  flags_array[FLAGS_SANITIZE_ADDRESS] = &flag_sanitize_address;
  flags_array[FLAGS_SANITIZE_UNDEFINED] = &flag_sanitize_undefined;
  flags_array[FLAGS_HELP] = &flag_help;
  flags->first = flags_array;
  flags->count = FLAGS_COUNT;
}

RED_DEF bool compile(const char *source_path, const char *output_path, Command *command, bool display) {
  command_insert(command, "gcc", CFLAGS, (void *) 0);
  command_insert(command, source_path, "-o", output_path, (void *) 0);
  if (1) {
    command_insert(command, "-mavx512f", (void *) 0);
  }
  if (flag_fast.value.on) command_insert(command, "-O3", (void *) 0);
  if (flag_debug.value.on) command_insert(command, "-ggdb", (void *) 0);
  if (flag_analyze.value.on) command_insert(command, "-fanalyzer", (void *) 0);
  if (flag_sanitize_address.value.on) command_insert(command, "-fsanitize=address", (void *) 0);
  if (flag_sanitize_undefined.value.on) command_insert(command, "-fsanitize=undefined", (void *) 0);
  return command_run(command, display);
}

typedef struct {
  double total;
  double best;
  double worst;
} ThoroughTimer;

RED_DEF void thoroughly_time(ThoroughTimer *timer, clock_t start, clock_t end);

RED_DEF void thoroughly_time(ThoroughTimer *timer, clock_t start, clock_t end) {
  double current;
  current = redcomp_elapsed_time_get(start, end);
  if (current < timer->best) timer->best = current;
  else if (current > timer->worst) timer->worst = current;
  timer->total += current;
}

RED_DEF void thorough_timer_print(ThoroughTimer timer, u64 count, const char *subject);

RED_DEF void thorough_timer_print(ThoroughTimer timer, u64 count, const char *subject) {
  double average;
  average = timer.total / (double) count;
  printf("%s:\n", subject);
  printf("    Average: %.10f.\n", average);
  printf("    Best: %.10f.\n", timer.best);
  printf("    Worst: %.10f.\n", timer.worst);
  printf("    Total: %.10f.\n", timer.total);
}

RED_DEF void compare_formatting(void) {
  String string;
  ThoroughTimer redcomp_string_insertions, libc_string_insertions;
  ThoroughTimer redcomp_number_insertions, libc_number_insertions;
  ThoroughTimer redcomp_left_pad, libc_left_pad;
  ThoroughTimer redcomp_right_pad, libc_right_pad;
  ThoroughTimer redcomp_sized_string_insertions, libc_sized_string_insertions;
  ThoroughTimer redcomp_memory_copy_timer, libc_memory_copy_timer;
  ThoroughTimer redcomp_memory_set_timer, libc_memory_set_timer;
  clock_t start, end;
  char *memory;
  u64 size;
  u64 i, j, max, j_max;
  memset(&string, 0, sizeof string);
  memset(&redcomp_string_insertions, 0, sizeof redcomp_string_insertions);
  memset(&libc_string_insertions, 0, sizeof libc_string_insertions);
  memset(&redcomp_number_insertions, 0, sizeof redcomp_number_insertions);
  memset(&libc_number_insertions, 0, sizeof libc_number_insertions);
  memset(&redcomp_left_pad, 0, sizeof redcomp_left_pad);
  memset(&libc_left_pad, 0, sizeof libc_left_pad);
  memset(&redcomp_right_pad, 0, sizeof redcomp_right_pad);
  memset(&libc_right_pad, 0, sizeof libc_right_pad);
  memset(&redcomp_sized_string_insertions, 0, sizeof redcomp_sized_string_insertions);
  memset(&libc_sized_string_insertions, 0, sizeof libc_sized_string_insertions);
  memset(&redcomp_memory_copy_timer, 0, sizeof redcomp_memory_copy_timer);
  memset(&libc_memory_copy_timer, 0, sizeof libc_memory_copy_timer);
  memset(&redcomp_memory_set_timer, 0, sizeof redcomp_memory_set_timer);
  memset(&libc_memory_set_timer, 0, sizeof libc_memory_set_timer);

  redcomp_string_insertions.best = 1;
  libc_string_insertions.best = 1;
  redcomp_number_insertions.best = 1;
  libc_number_insertions.best = 1;
  redcomp_left_pad.best = 1;
  libc_left_pad.best = 1;
  redcomp_right_pad.best = 1;
  libc_right_pad.best = 1;
  redcomp_sized_string_insertions.best = 1;
  libc_sized_string_insertions.best = 1;
  redcomp_memory_copy_timer.best = 1;
  libc_memory_copy_timer.best = 1;
  redcomp_memory_set_timer.best = 1;
  libc_memory_set_timer.best = 1;
  max = 10000;
  j_max = 10000;

  for (i = 0; i < max; i++) {
    start = redcomp_current_time_get();
    for (j = 0; j < j_max; j++) {
      string_insert_format_function(&string, __FILE__, __LINE__, "Testing string insertions: %s\n", "Testing string insertions");
    }
    string.count = 0;
    end = redcomp_current_time_get();
    thoroughly_time(&redcomp_string_insertions, start, end);
  }

  for (i = 0; i < max; i++) {
    start = redcomp_current_time_get();
    for (j = 0; j < j_max; j++) {
      sprintf(string.first, "Testing string insertions: %s\n", "Testing string insertions");
    }
    string.count = 0;
    end = redcomp_current_time_get();
    thoroughly_time(&libc_string_insertions, start, end);
  }

  for (i = 0; i < max; i++) {
    start = redcomp_current_time_get();
    for (j = 0; j < j_max; j++) {
      string_insert_format_function(&string, __FILE__, __LINE__, "Testing number insertions: %d %u %ld " U64_FMT "\n", (i32) i, (u32) i, (i64) i, (u64) i);
    }
    string.count = 0;
    end = redcomp_current_time_get();
    thoroughly_time(&redcomp_number_insertions, start, end);
  }

  for (i = 0; i < max; i++) {
    start = redcomp_current_time_get();
    for (j = 0; j < j_max; j++) {
      sprintf(string.first, "Testing number insertions: %d %u %ld " U64_FMT "\n", (i32) i, (u32) i, (i64) i, (u64) i);
    }
    string.count = 0;
    end = redcomp_current_time_get();
    thoroughly_time(&libc_number_insertions, start, end);
  }

  for (i = 0; i < max; i++) {
    start = redcomp_current_time_get();
    for (j = 0; j < j_max; j++) {
      string_insert_format_function(&string, __FILE__, __LINE__, "Testing left pad insertions: %*s|%*s|%*s|%*s|\n", (i32) (i % 4000), "", (i32) (i % 4000), "", (i32) (i % 4000), "", (i32) (i % 4000), "");
    }
    string.count = 0;
    end = redcomp_current_time_get();
    thoroughly_time(&redcomp_left_pad, start, end);
  }

  for (i = 0; i < max; i++) {
    start = redcomp_current_time_get();
    for (j = 0; j < j_max; j++) {
      sprintf(string.first, "Testing left pad insertions: %*s|%*s|%*s|%*s|\n", (i32) (i % 4000), "", (i32) (i % 4000), "", (i32) (i % 4000), "", (i32) (i % 4000), "");
    }
    string.count = 0;
    end = redcomp_current_time_get();
    thoroughly_time(&libc_left_pad, start, end);
  }

  for (i = 0; i < max; i++) {
    start = redcomp_current_time_get();
    for (j = 0; j < j_max; j++) {
      string_insert_format_function(&string, __FILE__, __LINE__, "Testing right pad insertions: %*s|%*s|%*s|%*s|\n", (i32) -(i % 4000), "", (i32) -(i % 4000), "", (i32) -(i % 4000), "", (i32) -(i % 4000), "");
    }
    string.count = 0;
    end = redcomp_current_time_get();
    thoroughly_time(&redcomp_right_pad, start, end);
  }

  for (i = 0; i < max; i++) {
    start = redcomp_current_time_get();
    for (j = 0; j < j_max; j++) {
      sprintf(string.first, "Testing right pad insertions: %*s|%*s|%*s|%*s|\n", (i32) -(i % 4000), "", (i32) -(i % 4000), "", (i32) -(i % 4000), "", (i32) -(i % 4000), "");
    }
    string.count = 0;
    end = redcomp_current_time_get();
    thoroughly_time(&libc_right_pad, start, end);
  }

  for (i = 0; i < max; i++) {
    start = redcomp_current_time_get();
    for (j = 0; j < j_max; j++) {
      string_insert_format_function(&string, __FILE__, __LINE__, "Testing sized strings insertions: %.*s|%.*s|\n", (i32) strlen("first sized string"), "first sized string", (i32) strlen("second sized string"), "second sized string");
    }
    string.count = 0;
    end = redcomp_current_time_get();
    thoroughly_time(&redcomp_sized_string_insertions, start, end);
  }

  for (i = 0; i < max; i++) {
    start = redcomp_current_time_get();
    for (j = 0; j < j_max; j++) {
      sprintf(string.first, "Testing sized strings insertions: %.*s|%.*s|\n", (i32) strlen("first sized string"), "first sized string", (i32) strlen("second sized string"), "second sized string");
    }
    string.count = 0;
    end = redcomp_current_time_get();
    thoroughly_time(&libc_sized_string_insertions, start, end);
  }

  string_insert_format_function(&string, __FILE__, __LINE__, "Testing sized strings insertions: %.*s|%.*s|\nAnd then some more testing, because this needs to be a chunky memory string to copy around! Will it get better if I make this even bigger? Maybe! Here are some numbers: %d, %d, %d, %d! Wow!\nHow about some fibonacci numbers? %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", (i32) strlen("first sized string"), "first sized string", (i32) strlen("second sized string"), "second sized string", 1, 2, 3, 4, 0, 1, 1, 2, 3, 5, 8, 13, 21, 34);
  printf("String = %.*s\n", STRING_ARG(string));
  memory = malloc(string.count);
  memcpy(memory, string.first, string.count);
  size = string.count;

  printf("Before redcomp_memory_copy: \n%.*s\n", STRING_ARG(string));
  string.count = 0;
  string.reserved = 0;
  free(string.first);
  string.first = (void *) 0;
  array_reserve(&string, size);

  for (i = 0; i < max; i++) {
    start = redcomp_current_time_get();
    for (j = 0; j < j_max; j++) {
      redcomp_memory_copy(string.first, memory, size);
    }
    end = redcomp_current_time_get();
    thoroughly_time(&redcomp_memory_copy_timer, start, end);
  }
  string.count = size;
  printf("After redcomp_memory_copy: \n%.*s\n", STRING_ARG(string));
  string.count = 0;

  printf("Before memcpy: \n%.*s\n", STRING_ARG(string));
  string.count = 0;
  string.reserved = 0;
  free(string.first);
  string.first = (void *) 0;
  array_reserve(&string, size);

  for (i = 0; i < max; i++) {
    start = redcomp_current_time_get();
    for (j = 0; j < j_max; j++) {
      memcpy(string.first, memory, size);
    }
    end = redcomp_current_time_get();
    thoroughly_time(&libc_memory_copy_timer, start, end);
  }
  string.count = size;
  printf("After memcpy: \n%.*s\n", STRING_ARG(string));

  printf("Before redcomp_memory_set: \n%.*s\n", STRING_ARG(string));
  string.count = 0;
  string.reserved = 0;
  free(string.first);
  string.first = (void *) 0;
  array_reserve(&string, size);

  for (i = 0; i < max; i++) {
    start = redcomp_current_time_get();
    for (j = 0; j < j_max; j++) {
      redcomp_memory_set(string.first, (u8) memory[i % size], size);
    }
    end = redcomp_current_time_get();
    thoroughly_time(&redcomp_memory_set_timer, start, end);
  }
  string.count = size;
  printf("After redcomp_memory_set: \n%.*s\n", STRING_ARG(string));
  string.count = 0;

  printf("Before memset: \n%.*s\n", STRING_ARG(string));
  string.count = 0;
  string.reserved = 0;
  free(string.first);
  string.first = (void *) 0;
  array_reserve(&string, size);

  for (i = 0; i < max; i++) {
    start = redcomp_current_time_get();
    for (j = 0; j < j_max; j++) {
      memset(string.first, (u8) memory[i % size], size);
    }
    end = redcomp_current_time_get();
    thoroughly_time(&libc_memory_set_timer, start, end);
  }
  string.count = size;
  printf("After memset: \n%.*s\n", STRING_ARG(string));

  thorough_timer_print(redcomp_string_insertions, max, "Redcomp string insertions");
  thorough_timer_print(libc_string_insertions, max, "Libc string insertions");
  thorough_timer_print(redcomp_number_insertions, max, "Redcomp number insertions");
  thorough_timer_print(libc_number_insertions, max, "Libc number insertions");
  thorough_timer_print(redcomp_left_pad, max, "Redcomp left pad");
  thorough_timer_print(libc_left_pad, max, "Libc left pad");
  thorough_timer_print(redcomp_right_pad, max, "Redcomp right pad");
  thorough_timer_print(libc_right_pad, max, "Libc right pad");
  thorough_timer_print(redcomp_sized_string_insertions, max, "Redcomp sized string insertions");
  thorough_timer_print(libc_sized_string_insertions, max, "Libc sized string insertions");
  thorough_timer_print(redcomp_memory_copy_timer, max, "Redcomp memmory copy");
  thorough_timer_print(libc_memory_copy_timer, max, "Libc memmory copy");
  thorough_timer_print(redcomp_memory_set_timer, max, "Redcomp memmory set");
  thorough_timer_print(libc_memory_set_timer, max, "Libc memmory set");

  if (string.first) free(string.first);
  if (memory) free(memory);
}
