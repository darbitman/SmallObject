add_library(Chunk "")
include_directories("../include")
target_sources(Chunk PRIVATE
               Chunk.cpp
               "../include/Chunk.h")