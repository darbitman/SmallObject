include_directories("../include")

add_library(Chunk "")
target_sources(Chunk PRIVATE
               Chunk.cpp
               "../include/Chunk.h")

add_executable(ChunkTest
               ChunkTest.cpp)

target_link_libraries(ChunkTest
                      Chunk)

add_library(FixedAllocator "")
target_sources(FixedAllocator PRIVATE
              FixedAllocator.cpp
              "../include/FixedAllocator.h")

add_executable(FixedAllocatorTest
               FixedAllocatorTest.cpp)

target_link_libraries(FixedAllocatorTest
                      FixedAllocator
                      Chunk)

add_library(SmallObjAllocator "")
target_sources(SmallObjAllocator PRIVATE
               SmallObjAllocator.cpp
               "../include/SmallObjAllocator.h")

add_executable(SmallObjAllocatorTest
               SmallObjAllocatorTest.cpp)

target_link_libraries(SmallObjAllocatorTest
                      SmallObjAllocator
                      FixedAllocator
                      Chunk)

add_executable(SingletonHolderTest
               SingletonHolderTest.cpp
               "../include/SingletonHolder.h")

target_link_libraries(SingletonHolderTest
                      SmallObjAllocator
                      FixedAllocator
                      Chunk)
                      
add_library(SmallObject "")
target_sources(SmallObject PRIVATE
               SmallObject.cpp
               "../include/SmallObject.h")

add_executable(SmallObjectTest
               SmallObjectTest.cpp)

target_link_libraries(SmallObjectTest
                      SmallObject
                      SmallObjAllocator
                      FixedAllocator
                      Chunk)