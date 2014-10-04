#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <fftw3.h>
//#include <setjmp.h>


void print_array(double *array, int length)
{
    int i;
    for (i = 0; i < length; i++) { printf("%g, ", array[i]); }
    printf("\n");
}

int main(int argc, char *argv[]) {
    const char *inputFilename = argv[1];
    SNDFILE *input/*, *output*/;
    SF_INFO sfinfo;
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
    int repeat = 1;
    while (repeat){
        int first = (int)(offset);
        double interArray[(int)num_frames*sfinfo.channels];
        print_array(interArray, (int)num_frames*sfinfo.channels);
        offset = offset + sf_readf_double(input, interArray, num_frames); //check the array to see if done correctly? Should be pointer pointing at current position in array
       // printf("This is the return value: %d\n", (int)(sf_readf_double(input, interArray, num_frames)));
        int second = (int)(offset);
        if (second > first) {
            //probably want to do the FFT right here. Takes in a certain sample of audio and stores it away
            //fftw_complex *in, *out;
            //fftw_plan fftPlan;
            ////in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * num_frames);
            //out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * num_frames);
            //fftPlan = fftw_plan_dft_1d(num_frames, (fftw_complex)(interArray), out, FFTW_FORWARD, FFTW_MEASURE); //Change FFTW_MEASURE to FFTW_ESTIMATE if taking too long
            //fftw_execute(fftPlan); 
            //fftw_destroy_plan(fftPlan);
            //double outputArray[num_frames] = (double)out
            ////fftw_free(in);
            //fftw_free(out);
            //end fourier shit
        } else {
            repeat = 0;
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
