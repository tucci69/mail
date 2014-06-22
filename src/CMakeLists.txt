
add_executable(mail main.cpp  Mail.cpp)


target_link_libraries(mail ${OPENSSL_LIB} ${CRYPTO_LIB})
