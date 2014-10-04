#include <stdio.h>
#include <sndfile.h>
#include <fftw3.h>





int main(const char *inputFilename) {
    SNDFILE *input, *output;
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
    sf_count_t num_frames = 1024; // size of the array to be tossed into FFTW. Check if 1024 is valid
    
    

    while ((int)(sf_seek(input, 0, SEEK_CUR)) >= 0){
        double interArray[num_frames];
        offset = offset + sf_readf_double(input, interArray, num_frames); //check the array to see if done correctly? Should be pointer pointing at current position in array
        //probably want to do the FFT right here. Takes in a certain sample of audio and stores it away
        fftw_complex *in, *out;
        fftw_plan p;
        in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
        out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
        p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
        fftw_execute(p); 
        fftw_destroy_plan(p);
        fftw_free(in); 
        fftw_free(out);
        //end fourier shit
        sf_seek(input, offset, SEEK_SET);
    }
    
    if (sf_close(input)) {
        printf("Successfully closed audio file!");
    } else {
        printf("Something went wrong when closing the file!");
        exit(1);
    }
}
