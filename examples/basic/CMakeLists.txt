cmake_minimum_required(VERSION 3.1)

##########################

add_executable( Sub0Reflect_Example_Basic "" )

target_link_libraries(Sub0Reflect_Example_Basic
    PUBLIC
        Sub0Reflect)
        target_compile_features(Sub0Reflect_Example_Basic PRIVATE cxx_std_17)



target_sources( Sub0Reflect_Example_Basic
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/main.cpp"        
)