#include "mbrot.h"

#include <cmath>

MandelbrotPointInfo compute_mandelbrot(d_complex c, int max_iters,
                                       bool collect_points) {
    MandelbrotPointInfo info;
    info.initial_point = c;
    info.max_iters = max_iters;

    vector<d_complex> points_in_path;
    d_complex z = 0;

    for (int i = 0; i < max_iters; i++) {
        z = pow(z, 2) + c;

        if (collect_points) {
            points_in_path.push_back(z);
        }

        if (abs(z) > 2) {
            info.escaped = true;
            info.num_iters = i + 1;
            info.points_in_path = points_in_path;

            return info;
        }
    }

    info.num_iters = max_iters;
    info.points_in_path = points_in_path;

    return info;
}
