set(appmenuapplet_SRCS
    appmenumodel.cpp
    appmenugtkmenumodel.cpp
    appmenugtkmenuimporter.cpp
    appmenuplugin.cpp
)
INCLUDE_DIRECTORIES(${GLIB_INCLUDE_DIRS})
add_library(appmenuplugin SHARED ${appmenuapplet_SRCS})
target_link_libraries(appmenuplugin
                      Qt5::Core
                      Qt5::Widgets
                      Qt5::Quick
                      KF5::Plasma
                      KF5::WindowSystem
                      dbusmenuqt)

if(HAVE_X11)
    target_link_libraries(appmenuplugin Qt5::X11Extras XCB::XCB)
endif()

install(TARGETS appmenuplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/plasma/private/appmenu)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/plasma/private/appmenu)
