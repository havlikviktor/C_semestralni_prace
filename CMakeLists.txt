cmake_minimum_required(VERSION 3.24)
project(Semestralni_prace_C C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -pedantic -ansi")

add_executable(Semestralni_prace_C src/spamid.c src/table/hashTable.h src/table/hashTable.c src/reader/reader.h src/reader/reader.c "src/decider/decider.h" src/decider/decider.c)
