#pragma once

#include <complex>
#include <array>
#include <numbers>

using Complex = std::complex<double>;

// https://rosettacode.org/wiki/Fast_Fourier_transform#C.2B.2B

template <size_t S>
static void fft(std::array<Complex, S>& x)
{
    const size_t N = S;
    size_t k = N, n;
    double thetaT = std::numbers::pi / N;
    Complex phiT = Complex(cos(thetaT), -sin(thetaT));
    Complex t;
    while (k > 1)
    {
        n = k;
        k >>= 1;
        phiT = phiT * phiT;
        t = 1.0L;
        for (size_t l = 0; l < k; l++)
        {
            for (size_t a = l; a < N; a += n)
            {
                const size_t b = a + k;
                Complex t2 = x[a] - x[b];
                x[a] += x[b];
                x[b] = t2 * t;
            }
            t *= phiT;
        }
    }

    const size_t m = static_cast<size_t>(log2(N));
    for (unsigned int a = 0; a < N; a++)
    {
        size_t b = a;

        b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
        b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
        b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
        b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
        b = ((b >> 16) | (b << 16)) >> (32 - m);
        if (b > a)
        {
            Complex t2 = x[a];
            x[a] = x[b];
            x[b] = t2;
        }
    }
}

template <size_t S>
static void ifft(std::array<Complex, S>& x)
{
    for (size_t i = 0; i < S; i++)
    {
        x[i] = std::conj(x[i]);
    }

    fft(x);

    for (size_t i = 0; i < S; i++)
    {
        x[i] = std::conj(x[i]);
        x[i] /= S;
    }
}
