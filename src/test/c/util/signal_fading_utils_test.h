#ifndef SIGNAL_FADING_UTILS_H
#define SIGNAL_FADING_UTILS_H

/*
 * Checks the last element of discrete fade function
 * to be 1 + (10^(0.05 * (-6)) - 1) * (N - 1) / N, for N = 5.
 */
void SignalFadingUtils__fade_out__test_1();

/*
 * Checks the last element of discrete fade function
 * to be 10^(0.05 * (-6)) + (1 - 10^(0.05 * (-6))) * (N - 1) / N, for N = 5.
 */
void SignalFadingUtils__fade_in__test_1();

/*
 * Checks the faders' algorithms to be mathematically correct.
 */
void SignalFadingUtils__fade_expand__test_1();

void SignalFadingUtils__fade_expand__test_2();

#endif
