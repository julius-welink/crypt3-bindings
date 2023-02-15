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

    char password_str[128], salt_str[64];
    int32_t algo_num;
    napi_get_value_string_utf8(env, argv[0], password_str, sizeof(password_str), NULL);
    napi_get_value_string_utf8(env, argv[1], salt_str, sizeof(salt_str), NULL);
    napi_get_value_int32(env, argv[2], &algo_num);

    char unix_salt[128];
    int sprintf_result = snprintf(unix_salt, sizeof(unix_salt), "$%d$%s", algo_num, salt_str);
    if (sprintf_result < 0) {
        napi_throw_error(env, NULL, "Error constructing Unix salt string.");
        return NULL;
    }

    napi_value hashed_password;
    char *hashed_str = crypt(password_str, unix_salt);
    if (hashed_str == NULL) {
        napi_throw_error(env, NULL, "Error hashing password.");
        return NULL;
    }

    napi_create_string_utf8(env, hashed_str, NAPI_AUTO_LENGTH, &hashed_password);

    return hashed_password;
}

napi_value init_all(napi_env env, napi_value exports) {
    napi_value fn;
    napi_create_function(env, NULL, 0, hashPassword, NULL, &fn);
    napi_set_named_property(env, exports, "hashPassword", fn);
    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init_all)
