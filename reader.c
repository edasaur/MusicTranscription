#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <fftw3.h>
#include <math.h>
//#include <setjmp.h>

double magnitude(double real, double imag) {
    return sqrt((real*real) + (imag*imag));
}

void print_darray(double *array, int length) {
    int i;
    for (i = 0; i < length; i++) { printf("%g, ", array[i]); }
    printf("I dern now\n");
}

void print_carray(fftw_complex *array, int length) {
    int i;
    for (i = 0; i < length; i++) { printf("%g + %gi, ", array[i][0], array[i][1]); }

    printf("harhar durrnn\n");
}

int main(int argc, char *argv[]) {
    const char *inputFilename = argv[1];
    SNDFILE *input/*, *output*/;
    SF_INFO sfinfo;
    sfinfo.format = 0;
    //Following checks whether there is an error in opening and reading the audio file
    if (! (input = sf_open(inputFilename, SFM_READ, &sfinfo))) {
        printf("Error: File could not be opened: %s\n", inputFilename);
        puts(sf_strerror(input)); //change back to NULL if 'input' does not work
        exit(1);
    }
    
    //set read pointer to beginning of audio data and returns position of read pointer, unless
    //before beginning of file or after end of file, in which case, -1 is returned
    sf_count_t offset = sf_seek(input, 0, SEEK_SET); 
    //at this point, offset == 0
    sf_count_t num_frames = 2096; // size of the array to be tossed into FFTW. Check if 1024 is valid
    int repeat = 2;
    while (repeat){
        int first = (int)(offset);
        double interArray[(int)num_frames*sfinfo.channels];
        offset = offset + sf_readf_double(input, interArray, num_frames); //check the array to see if done correctly? Should be pointer pointing at current position in array
        // printf("This is the return value: %d\n", (int)(sf_readf_double(input, interArray, num_frames)));
        int second = (int)(offset);
        fftw_complex* input = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(second-first));
        //Copies over from double array to complex array
        int j;
        for (j = 0; j < second - first; j++) {
            input[j][0] = interArray[j];
            input[j][1] = 0;
        }
        //Whenever there is more data to read
        if (second > first) {
            //printf("Original: \n");
            //print_darray(interArray, (second-first)*sfinfo.channels);
            //printf("Complex version of Original, should be the same");
            //print_carray(input, (second-first)*sfinfo.channels);
            //FFT time! Takes in a certain sample of audio and stores it away
            fftw_complex *out;
            fftw_plan fftPlan;
            //in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * num_frames);
            out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (second-first));
            fftPlan = fftw_plan_dft_1d(second-first, input, out, FFTW_FORWARD, FFTW_ESTIMATE); //Change FFTW_MEASURE to FFTW_ESTIMATE if taking too long
            fftw_execute(fftPlan); 
            fftw_destroy_plan(fftPlan);
            printf("After Fourier: \n");
            //could possibly be very wrong
            int k;
            double largest = 0.0;
            for (k = 0; k < second - first; k++) {
                double temp =  magnitude(out[k][0],out[k][1]);
                if (k==0) {
                    largest = temp; 
                }
                else {
                    if (temp > largest) {
                        largest = temp;
                    }
                }
            }
            printf("Largest number in Fourier is: %g", largest);
            //print_carray(out, (int)num_frames*sfinfo.channels);
            //double outputArray[num_frames] = (double)out
            //fftw_free(in);
            //fftw_free(out);
        } else {
            repeat--;
        }
    }
    if (sf_close(input) == 0) {
        printf("Successfully closed audio file!\n");
    } else {
        printf("Something went wrong when closing the file!\n");
        exit(1);
    }
    return 0;
}
