cmake_minimum_required(VERSION 3.0.0)
project (dlep_radio_ipv6)
file(GLOB SOURCES "*.c")
add_executable(dlep_radio_ipv6 ${SOURCES})
install(
  TARGETS dlep_radio_ipv6
  RUNTIME DESTINATION bin)

