#pragma once



bool tls_init();

void tls_deinit();


void* tls_get_data();

bool tls_set_data(void* data);


