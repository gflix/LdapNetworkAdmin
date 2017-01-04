cmake_minimum_required(VERSION 3.5.0)
project(LdapNetworkAdmin)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
include_directories(src)

set(TRANSLATIONS i18n/ldap-network-admin_de.ts)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

qt5_add_translation(QM ${TRANSLATIONS})

# Find LDAP development files
find_path(LDAP_INCLUDE_DIR ldap.h)
find_library(LDAP_LIBRARIES NAMES ldap)
find_library(LBER_LIBRARIES NAMES lber)

if(LDAP_INCLUDE_DIR AND LDAP_LIBRARIES)
	set(LDAP_FOUND TRUE)
	if(LBER_LIBRARIES)
		set(LDAP_LIBRARIES ${LDAP_LIBRARIES} ${LBER_LIBRARIES})
	endif(LBER_LIBRARIES)
endif(LDAP_INCLUDE_DIR AND LDAP_LIBRARIES)

if(LDAP_FOUND)
	message(STATUS "Found ldap: ${LDAP_LIBRARIES}")
	include_directories(${LDAP_INCLUDE_PATH})
else(LDAP_FOUND)
	message(FATAL_ERROR "Could not find ldap")
endif(LDAP_FOUND)

# mark_as_advanced(LDAP_INCLUDE_DIR LDAP_LIBRARIES LBER_LIBRARIES)
# END LDAP

add_executable(
	ldap-network-admin WIN32
	src/LdapNetworkAdmin.cpp
	src/Common.cpp
	src/MainWindow.cpp
	src/GenericPanelItemEdit.cpp
	src/PanelDefault.cpp
	src/PanelOrganizationalUnit.cpp
	src/DialogConnections.cpp
	src/DialogPassword.cpp
	src/DialogTextInput.cpp
	src/ModelConnections.cpp
	src/ModelNetworkTree.cpp
	src/LdapConnection.cpp
	src/LdapObject.cpp
	${QM}
)
target_link_libraries(ldap-network-admin Qt5::Widgets Qt5::Xml ${LDAP_LIBRARIES})
