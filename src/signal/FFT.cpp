#include "FFT.hpp"

// https://rosettacode.org/wiki/Fast_Fourier_transform#C.2B.2B

#include <numbers>

void fft(CArray& x)
{
    unsigned int N = x.size(), k = N, n;
    double thetaT = std::numbers::pi / N;
    Complex phiT = Complex(cos(thetaT), -sin(thetaT)), T;
    while (k > 1)
    {
        n = k;
        k >>= 1;
        phiT = phiT * phiT;
        T = 1.0L;
        for (unsigned int l = 0; l < k; l++)
        {
            for (unsigned int a = l; a < N; a += n)
            {
                unsigned int b = a + k;
                Complex t = x[a] - x[b];
                x[a] += x[b];
                x[b] = t * T;
            }
            T *= phiT;
        }
    }

    unsigned int m = static_cast<unsigned int>(log2(N));
    for (unsigned int a = 0; a < N; a++)
    {
        unsigned int b = a;

        b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
        b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
        b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
        b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
        b = ((b >> 16) | (b << 16)) >> (32 - m);
        if (b > a)
        {
            Complex t = x[a];
            x[a] = x[b];
            x[b] = t;
        }
    }
}

void ifft(CArray& x)
{
    x = x.apply(std::conj);

    fft( x );

    x = x.apply(std::conj);

    x /= x.size();
}
