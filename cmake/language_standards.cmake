function(set_language_standards target)
    set_target_properties("${target}" PROPERTIES
            C_STANDARD            11
            C_STANDARD_REQUIRED   ON
            C_EXTENSIONS          OFF
            CXX_STANDARD          17
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS        OFF)
endfunction()
