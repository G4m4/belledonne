# @brief Belledonne plugin root cmake file

set(BELLEDONNE_PLUGIN_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
)

include_external_msproject(BelledonneVSIX ${BELLEDONNE_PLUGIN_DIR}/BelledonneVSIX/BelledonneVSIX.csproj)

add_dependencies(BelledonneVSIX belledonne_lib)
