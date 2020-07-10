#include "bbrot.h"

#include <complex>
#include <iostream>
#include <random>


using namespace std;
using d_complex = complex<double>;


const double REAL_LOWER_BOUND = -2;
const double REAL_UPPER_BOUND = 1;
const double IMAG_LOWER_BOUND = -1.5;
const double IMAG_UPPER_BOUND = 1.5;


// Random number engine and generators
default_random_engine re;
uniform_real_distribution<double> generate_real{-2, 1};
uniform_real_distribution<double> generate_imag{-1.5, 1.5};


/* Generates a random complex number c in the range (-2 - 1.5i) to (1 + 1.5i).
 */
d_complex generate_rand_c() {
    return d_complex{generate_real(re), generate_imag(re)};
}

/* Normalizes a value to a 0..1 range. */
double normalize(double min, double max, double value) {
    assert(min < max);

    return (value - min) / (max - min);
}

/* Updates the image with the contents of the info.points_in_path collection. */
void update_image(Image &image, const MandelbrotPointInfo &info) {
    for (d_complex point : info.points_in_path) {
        double real = point.real();
        double imag = point.imag();
        if (real > REAL_LOWER_BOUND && real < REAL_UPPER_BOUND
                && imag > IMAG_LOWER_BOUND && imag < IMAG_UPPER_BOUND) {
            int x = normalize(REAL_LOWER_BOUND, REAL_UPPER_BOUND, real)
                    * (image.getWidth() - 1);
            int y = normalize(IMAG_LOWER_BOUND, IMAG_UPPER_BOUND, imag)
                    * (image.getHeight() - 1);
            image.incValue(y, x);
            image.incValue(image.getHeight() - y - 1, x);
        }
    }
}

/* Encodes the image data in ASCII PGM format and writes it out to the output
 * stream os.
 */
void output_image_to_pgm(const Image &image, ostream &os) {
    int max_value = 255;

    cout << "P2 " << image.getWidth() << " " << image.getHeight() << " "
        << max_value << endl;

    // Find the maximum pixel in the image
    int max_pix = 0;

    for (int i = 0; i < image.getHeight(); i++) {
        for (int j = 0; j < image.getWidth(); j++) {
            max_pix = max(max_pix, image.getValue(i, j));
        }
    }

    // Scale each pixel value to range 0...max_value
    for (int i = 0; i < image.getHeight(); i++) {
        for (int j = 0; j < image.getWidth(); j++) {
            int val = round(normalize(0, max_pix, image.getValue(i, j))
                * max_value);
            cout << val << " ";
        }
        cout << endl;
    }
}


int main(int argc, char **argv) {
    if (argc != 4) {
        cerr << "Incorrect number of arguments!" << endl;
        return 1;
    }

    int img_size = atoi(argv[1]);
    int num_points = atoi(argv[2]);
    int max_iters = atoi(argv[3]);

    // Initialize image grid with values as 0
    Image grid{img_size, img_size};

    for (int i = 0; i < img_size; i++) {
        for (int j = 0; j < img_size; j++) {
            grid.setValue(i, j, 0);
        }
    }

    // Buddhabrot
    for (int i = 0; i < num_points; i++) {
        // Output progress
        if (i % 100000 == 0) {
            cerr << ".";
        }

        d_complex c = generate_rand_c();
        MandelbrotPointInfo info = compute_mandelbrot(c, max_iters, true);

        if (info.escaped) {
            update_image(grid, info);
        }
    }

    cerr << endl;

    output_image_to_pgm(grid, cout);

    return 0;
}
