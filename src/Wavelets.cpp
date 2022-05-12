#pragma once

// https://gist.github.com/binaryfoundry/8ecf72399416bd0892ac5f7a18abb860

#include <stdint.h>
#include <string.h>

#define WAVELET_DIM 512

static inline uint8_t haar_newY(uint8_t y1, uint8_t y2)
{
    return 2 * y2 - y1;
}

static inline void predict(uint8_t *vec, size_t n, bool invert)
{
    size_t const half = n >> 1;
    uint32_t predict = 0;
    for (size_t i = 0; i < half; i++)
    {
        size_t j = i + half;
        if (i < half - 1)
        {
            predict = (vec[i] + vec[i + 1]) / 2;
        }
        else if (n == 2)
        {
            predict = vec[0];
        }
        else
        {
            predict = haar_newY(vec[i - 1], vec[i]);
        }
        if (!invert)
            vec[j] = vec[j] - predict;
        else
            vec[j] = vec[j] + predict;
    }
}

static inline void forward(uint8_t *mat_a, uint8_t *mat_b, size_t n)
{
    size_t half = n >> 1;
    for (size_t y = 0; y < n; y++)
    {
        size_t o = sizeof(uint8_t) * y * WAVELET_DIM;
        uint8_t *vec_a = mat_a + o;
        uint8_t *vec_b = mat_b + o;
        for (size_t i = 0, j = 0; i < n; i = i + 2, j++)
        {
            vec_b[j] = vec_a[i];
            vec_b[half + j] = vec_a[i + 1];
        }
        predict(vec_b, n, false);
    }
}

static inline void inverse(uint8_t *mat_a, uint8_t *mat_b, size_t n)
{
    size_t half = n >> 1;
    for (size_t y = 0; y < n; y++)
    {
        size_t o = sizeof(uint8_t) * y * WAVELET_DIM;
        uint8_t *vec_a = mat_a + o;
        uint8_t *vec_b = mat_b + o;
        predict(vec_a, n, true);
        for (size_t i = 0, j = 0; i < n; i = i + 2, j++)
        {
            vec_b[i] = vec_a[j];
            vec_b[i + 1] = vec_a[half + j];
        }
    }
}

void wavelet_forward(uint8_t *mat, size_t N)
{
    uint8_t mat_temp[WAVELET_DIM * WAVELET_DIM];
    memset(mat_temp, 0, WAVELET_DIM * WAVELET_DIM);

    uint8_t *mat_a = mat;
    uint8_t *mat_b = &mat_temp[0];

    for (size_t n = N; n > 1; n = n >> 1)
    {
        forward(mat_a, mat_b, n);
    }
}

void wavelet_inverse(uint8_t *mat, size_t N)
{
    uint8_t mat_temp[WAVELET_DIM * WAVELET_DIM];
    memset(mat_temp, 0, WAVELET_DIM * WAVELET_DIM);

    uint8_t *mat_a = mat;
    uint8_t *mat_b = &mat_temp[0];

    for (size_t n = 2; n <= N; n = n << 1)
    {
        inverse(mat_a, mat_b, n);
    }
}
