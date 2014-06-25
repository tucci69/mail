
add_executable(mail main.cpp  Mail.cpp)


target_link_libraries(mail ${OPENSSL_LIB} ${CRYPTO_LIB})

set_target_properties(mail
    PROPERTIES
    COMPILE_FLAGS ${COMPILE_FLAGS}
    LINK_FLAGS ${LINK_FLAGS}
)

add_executable(dns dns.cpp)

