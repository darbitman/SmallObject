include_directories("../include")

add_library(Chunk "")
target_sources(Chunk PRIVATE
               Chunk.cpp
               "../include/Chunk.h")

add_library(FixedAllocator "")
target_sources(FixedAllocator PRIVATE
              FixedAllocator.cpp
              "../include/FixedAllocator.h")
