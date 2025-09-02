cmake_minimum_required(VERSION 3.8)
project(misora2_qr)

if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # add_compile_options(-Wall -Wextra -Wpedantic)
  # -Wpedantic を有効にしているため、C++17での std::make_pair の仕様変更に伴う警告が出ています。
  # これはC++14とC++17でのパラメータ渡し方法の違いによるもので、ビルドには致命的ではありません。
  # 警告を抑制するには -Wpedantic を外すか無効化してください。
  add_compile_options(-Wall -Wextra)
endif()


# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(OpenCV 4.8 REQUIRED COMPONENTS core highgui imgproc)
find_package(misora2_custom_msg REQUIRED)

# zbar を pkg-config で探す                                                    
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZBAR REQUIRED zbar)

link_directories(${ZBAR_LIBRARY_DIRS})

include_directories(
  include
)

set(dependencies
    rclcpp
    rclcpp_components
    cv_bridge
    OpenCV
    sensor_msgs
    std_msgs
    misora2_custom_msg
)

add_library(qr_library SHARED src/qr_component.cpp src/qr_reader.cpp)
ament_target_dependencies(qr_library ${dependencies})
target_link_libraries(qr_library ${ZBAR_LIBRARIES})
rclcpp_components_register_nodes(qr_library "component_qr::DetectQR")

add_executable(qr_node src/qr_component.cpp src/qr_node.cpp src/qr_reader.cpp)
ament_target_dependencies(qr_node ${dependencies})
target_link_libraries(qr_node ${ZBAR_LIBRARIES})

# QR読み取り器 main関数から実行
add_executable(qr_reader 
    src/qr_reader_main.cpp 
    src/qr_reader.cpp 
    )
ament_target_dependencies(qr_reader ${dependencies})
target_link_libraries(qr_reader ${ZBAR_LIBRARIES})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(TARGETS
  qr_library
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(TARGETS
  qr_node
  qr_reader
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
