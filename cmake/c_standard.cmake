function(set_c11_standard target)
    set_target_properties("${target}" PROPERTIES
            C_STANDARD          11
            C_STANDARD_REQUIRED ON
            C_EXTENSIONS        OFF)
endfunction()
