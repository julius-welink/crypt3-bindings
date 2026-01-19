#define NAPI_VERSION 3

#include <node_api.h>
#include <crypt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

napi_value hashPassword(napi_env env, napi_callback_info info) {
    size_t argc = 3;
    napi_value argv[3];
    napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

    if (argc != 3) {
        napi_throw_type_error(env, NULL, "Expecting 3 arguments.");
        return NULL;
    }

    // First, check the length of the input strings to prevent buffer overflow
    size_t password_len, salt_len;
    napi_get_value_string_utf8(env, argv[0], NULL, 0, &password_len);
    napi_get_value_string_utf8(env, argv[1], NULL, 0, &salt_len);

    // Validate input lengths (leave room for null terminator)
    if (password_len >= 128) {
        napi_throw_range_error(env, NULL, "Password too long (max 127 characters).");
        return NULL;
    }
    if (salt_len >= 64) {
        napi_throw_range_error(env, NULL, "Salt too long (max 63 characters).");
        return NULL;
    }

    // Validate that inputs are not empty
    if (password_len == 0) {
        napi_throw_type_error(env, NULL, "Password cannot be empty.");
        return NULL;
    }
    if (salt_len == 0) {
        napi_throw_type_error(env, NULL, "Salt cannot be empty.");
        return NULL;
    }

    // Now safe to copy into fixed-size buffers
    char password_str[128], salt_str[64];
    int32_t algo_num;
    napi_get_value_string_utf8(env, argv[0], password_str, sizeof(password_str), NULL);
    napi_get_value_string_utf8(env, argv[1], salt_str, sizeof(salt_str), NULL);
    napi_get_value_int32(env, argv[2], &algo_num);

    // Validate algorithm number (1=MD5, 5=SHA-256, 6=SHA-512)
    if (algo_num != 1 && algo_num != 5 && algo_num != 6) {
        napi_throw_range_error(env, NULL, "Invalid algorithm number (must be 1, 5, or 6).");
        return NULL;
    }

    char unix_salt[128];
    int sprintf_result = snprintf(unix_salt, sizeof(unix_salt), "$%d$%s", algo_num, salt_str);
    if (sprintf_result < 0 || sprintf_result >= (int)sizeof(unix_salt)) {
        napi_throw_error(env, NULL, "Error constructing Unix salt string.");
        return NULL;
    }

    // Use thread-safe crypt_r() instead of crypt()
    struct crypt_data data;
    memset(&data, 0, sizeof(data));

    napi_value hashed_password;
    char *hashed_str = crypt_r(password_str, unix_salt, &data);
    if (hashed_str == NULL) {
        napi_throw_error(env, NULL, "Error hashing password.");
        return NULL;
    }

    napi_create_string_utf8(env, hashed_str, NAPI_AUTO_LENGTH, &hashed_password);

    // Clear sensitive data from memory
    memset(password_str, 0, sizeof(password_str));
    memset(&data, 0, sizeof(data));

    return hashed_password;
}

napi_value init_all(napi_env env, napi_value exports) {
    napi_value fn;
    napi_create_function(env, NULL, 0, hashPassword, NULL, &fn);
    napi_set_named_property(env, exports, "hashPassword", fn);
    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init_all)
