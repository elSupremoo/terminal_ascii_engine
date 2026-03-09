#pragma once

// Cross-platform simple non-blocking input
void input_init();
void input_restore();
int input_poll(); // returns -1 if no key, otherwise ASCII or special codes
