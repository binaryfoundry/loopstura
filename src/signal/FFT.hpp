#pragma once

#include <complex>
#include <valarray>

using Complex = std::complex<double>;
using CArray = std::valarray<Complex>;

void fft(CArray& x);
void ifft(CArray& x);
