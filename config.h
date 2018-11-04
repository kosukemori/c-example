//
// Created by Kosuke Mori on 2018/11/04.
// 設定ファイル(config.txt)を読み込んでグローバル変数のconfigに放り込む
//

#include <stdio.h>

#ifndef C_NETWORKING_EXAMPLE_CONFIG_H
#define C_NETWORKING_EXAMPLE_CONFIG_H

struct Config {
    char ipv4_server_address[256];
    int ipv4_server_port;
    char ipv6_server_address[256];
    int ipv6_server_port;
} config;

void read_config() {
    FILE *fp;
    char line[512];
    char key[256], value[256];

    if ((fp = fopen("config.txt", "r")) == NULL) {
        perror("'fopen' failed");
        exit(1);
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        sscanf(line, "%s %s", key, value);

        if (strcmp(key, "IPV4_SERVER_ADDRESS") == 0) {
            strncpy(config.ipv4_server_address, value, strlen(value));
        } else if (strcmp(key, "IPV4_SERVER_PORT") == 0) {
            config.ipv4_server_port = (int) strtol(value, NULL, 10);
        } else if (strcmp(key, "IPV6_SERVER_ADDRESS") == 0) {
            strncpy(config.ipv6_server_address, value, strlen(value));
        } else if (strcmp(key, "IPV6_SERVER_PORT") == 0) {
            config.ipv6_server_port = (int)strtol(value, NULL, 10);
        }
    }

    fclose(fp);
}

#endif //C_NETWORKING_EXAMPLE_CONFIG_H
