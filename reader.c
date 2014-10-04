#include <studio.h>
#include <sndfile.h>

main(const char *path) {

    /** store the .wav file into arr **/
    SNDFILE* arr = sf_open(cons char *path, int mode, SF_INFO *sfinfo);
    // *path is the path to the file. Prob take as input from main
    // mode is one of SFM_READ, SFM_WRITE, SFM_RDWR. We will use SFM_READ
    // sf_info->format = 0 must be done before call to sf_open
    // sndfile.h will fill in the rest of the fields in sf_info
    // The exception to this is if you are working with a RAW file
        // then you must specify sfinfo->samplerate, channels, format

    if (arr == NULL) { // the function failed
        printf(const char* sf_strerror(SNDFILE *arr);
    }

    sf_count_t start = sf_seek(arr, sf_count_t frames, int whence);
    // whence is one of SEEK_SET, SEEK_CUR, SEEK_END. We will use SEEK_SET
    // SEEK_SET is the start of the audio data and frames is an additional
    // offset.
    // Return value is not used

    sf_count_t read, num_frames;
    num_frames = N; // size of the array to be tossed into FFTW. Prob 1024?
    
    while (read == num_frames) {
        read = sf_readf_type(arr, type *ptr, sf_count_t num_frames);
        sf_seek(arr, num_frames, SEEK_CUR);
        // reads num_frames number of frames into *ptr
        // *ptr contains either short, int, float, or double
        // sf_seek(arr, frames, SEEK_CUR) moves the read pointer to the next
        // section of the audio file that needs to be read
        // READ is the actual number of frames that were read. Typically this
        // should be == num_frames. If you reach the end of the file with 
        // less than num_frames, READ will be the actual number read

    int good = sf_close(arr);
    // good == 0 if success else some error value
}
