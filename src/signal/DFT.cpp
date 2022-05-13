#include <numbers>
#include <cmath>

// https://www.nayuki.io/page/how-to-implement-the-discrete-fourier-transform

void dft(
    double* inreal , double* inimag,
    double* outreal, double* outimag,
    size_t N)
{
    for (int k = 0; k < N; k++)
    {
        double sumreal = 0;
        double sumimag = 0;
        for (int t = 0; t < N; t++)
        {
            double angle = 2 * std::numbers::pi_v<double> * t * k / N;
            sumreal +=  inreal[t] * std::cos(angle) + inimag[t] * std::sin(angle);
            sumimag += -inreal[t] * std::sin(angle) + inimag[t] * std::cos(angle);
        }
        outreal[k] = sumreal;
        outimag[k] = sumimag;
    }
}
