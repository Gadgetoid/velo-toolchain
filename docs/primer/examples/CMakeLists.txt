cmake_minimum_required(VERSION 3.20)
project(velo_primer C)

include(VeloCE)

velo_add_executable(hello ../../../examples/hello/hello.c)
velo_add_executable(window ../../../examples/window/window.c ICON ../../../examples/window/window.ico)

velo_add_executable(frame frame/frame.c RESOURCES frame/frame.rc frame/resource.h)
target_link_libraries(frame PRIVATE velo::commctrl)

velo_add_executable(appstore appstore/appstore.c ICON appstore/app.ico)
target_link_libraries(appstore PRIVATE velo::commctrl)

velo_add_executable(form form/form.c RESOURCES form/form.rc form/resource.h)

velo_add_executable(controls controls/controls.c)
target_link_libraries(controls PRIVATE velo::commctrl)

velo_add_executable(drawing drawing/drawing.c)
target_link_libraries(drawing PRIVATE velo::commctrl)

velo_add_executable(fetch fetch/fetch.c)
target_link_libraries(fetch PRIVATE velo::commctrl velo::winsock)
