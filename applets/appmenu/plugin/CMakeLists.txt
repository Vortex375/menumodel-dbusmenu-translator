set(appmenuapplet_qmenumodel_SRCS
    ../qmenumodel/actionstateparser.cpp
    ../qmenumodel/converter.cpp
    ../qmenumodel/dbus-enums.h
    ../qmenumodel/menunode.cpp
    ../qmenumodel/qmenumodel.cpp
    ../qmenumodel/qdbusobject.cpp
    ../qmenumodel/qdbusmenumodel.cpp
    ../qmenumodel/qdbusactiongroup.cpp
    ../qmenumodel/qmenumodelevents.cpp
    ../qmenumodel/qstateaction.cpp)
set(appmenuapplet_SRCS
    menuadaptor.cpp
    dbusmenuadaptor.cpp
    dbusmenuadaptor.h
    appmenumodel.cpp
    gtkmenuadaptor.cpp
    appmenuplugin.cpp
    ../../../appmenu/verticalmenu.cpp
    ../../../appmenu/verticalmenu.h
    ${appmenuapplet_qmenumodel_SRCS}
)
INCLUDE_DIRECTORIES(${GLIB_INCLUDE_DIRS} ../qmenumodel ../../../appmenu)
add_library(appmenuplugin SHARED ${appmenuapplet_SRCS})
target_link_libraries(appmenuplugin
                      Qt5::Core
                      Qt5::Widgets
                      Qt5::Quick
                      KF5::Plasma
                      KF5::WindowSystem
                      dbusmenuqt
                      ${GLIB_LIBRARIES}
                      ${GIO_LIBRARIES})

if(HAVE_X11)
    target_link_libraries(appmenuplugin Qt5::X11Extras XCB::XCB)
endif()

install(TARGETS appmenuplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/plasma/private/appmenu)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/plasma/private/appmenu)
