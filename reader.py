# Read in a WAV and find the freq's
from ctypes import *
from cStringIO import StringIO
import pyaudio
import wave
import numpy as np
import sys

freq2note = {(1016.7103732476002, 1077.1671181080967): "c'''", (40.03046252816015, 42.41079769871837): 'e,,', (905.785968246273, 959.6468047454267): "ais''", (44.93267496413025, 47.604510855337864): 'fis,,', (179.73069985652097, 190.41804342135148): 'fis', (403.48177901005533, 427.4740541075866): "gis'", (47.604510855337864, 50.43522237625692): 'g,,', (427.4740541075866, 452.8929841231365): "a'", (1709.8962164303464, 1811.5719364925458): "a'''", (119.95585059317834, 127.08879665595002): 'b,', (33.661472440878015, 35.66308775290277): 'cis,,', (678.5727631794939, 718.922799426084): "f''", (190.41804342135148, 201.74088950502767): 'g', (75.56745061019491, 80.0609250563203): 'dis,', (1613.9271160402213, 1709.8962164303464): "gis'''", (302.2698024407796, 320.24370022528126): "dis'", (53.43425676344833, 56.611623015392055): 'a,,', (2561.9496018022496, 2714.2910527179756): "e''''", (2154.334236216193, 2282.437616185777): "cis''''", (169.64319079487348, 179.73069985652097): 'f', (854.9481082151732, 905.785968246273): "a''", (1209.0792097631183, 1280.974800901125): "dis'''", (106.86851352689666, 113.22324603078413): 'a,', (254.17759331190004, 269.2917795270241): "c'", (160.12185011264063, 169.64319079487348): 'e', (1077.1671181080967, 1141.2188080928886): "cis'''", (1919.2936094908534, 2033.4207464952008): "b'''", (142.65235101161107, 151.1349012203898): 'd', (213.7370270537933, 226.44649206156825): 'a', (4066.8414929904015, 4308.668472432386): "c'''''", (508.3551866238001, 538.5835590540484): "c''", (201.74088950502767, 213.7370270537933): 'gis', (3046.6886947416233, 3227.8542320804427): "g''''", (1811.5719364925458, 1919.2936094908534): "ais'''", (95.20902171067573, 100.87044475251383): 'g,', (2875.691197704336, 3046.6886947416233): "fis''''", (761.6721736854058, 806.9635580201107): "g''", (320.24370022528126, 339.28638158974695): "e'", (42.41079769871837, 44.93267496413025): 'f,,', (1523.3443473708119, 1613.9271160402213): "g'''", (151.1349012203898, 160.12185011264063): 'dis', (89.8653499282605, 95.20902171067573): 'fis,', (1437.8455988521678, 1523.3443473708119): "fis'''", (26.717128381724155, 28.305811507696028): 'a,,,', (359.46139971304194, 380.83608684270297): "fis'", (71.32617550580554, 75.56745061019491): 'd,', (380.83608684270297, 403.48177901005533): "g'", (640.4874004505624, 678.5727631794939): "e''", (285.30470202322215, 302.2698024407796): "d'", (1357.1455263589878, 1437.8455988521678): "f'''", (84.82159539743674, 89.8653499282605): 'f,', (959.6468047454267, 1016.7103732476002): "b''", (239.91170118635668, 254.17759331190004): 'b', (100.87044475251383, 106.86851352689666): 'gis,', (2714.2910527179756, 2875.691197704336): "f''''", (50.43522237625692, 53.43425676344833): 'gis,,', (479.82340237271336, 508.3551866238001): "b'", (1141.2188080928886, 1209.0792097631183): "d'''", (604.5396048815592, 640.4874004505624): "dis''", (269.2917795270241, 285.30470202322215): "cis'", (35.66308775290277, 37.783725305097455): 'd,,', (3227.8542320804427, 3419.792432860693): "gis''''", (37.783725305097455, 40.03046252816015): 'dis,,', (570.6094040464443, 604.5396048815592): "d''", (29.988962648294585, 31.772199163987512): 'b,,,', (134.6458897635121, 142.65235101161107): 'cis', (1280.974800901125, 1357.1455263589878): "e'''", (2033.4207464952008, 2154.334236216193): "c''''", (2282.437616185777, 2418.158419526237): "d''''", (63.544398327975024, 67.32294488175603): 'c,', (226.44649206156825, 239.91170118635668): 'ais', (339.28638158974695, 359.46139971304194): "f'", (3838.587218981707, 4066.8414929904015): "b''''", (28.305811507696028, 29.988962648294585): 'ais,,,', (31.772199163987512, 33.661472440878015): 'c,,', (80.0609250563203, 84.82159539743674): 'e,', (113.22324603078413, 119.95585059317834): 'ais,', (127.08879665595002, 134.6458897635121): 'c', (56.611623015392055, 59.97792529658917): 'ais,,', (2418.158419526237, 2561.9496018022496): "dis''''", (59.97792529658917, 63.544398327975024): 'b,,', (452.8929841231365, 479.82340237271336): "ais'", (3623.1438729850915, 3838.587218981707): "ais''''", (806.9635580201107, 854.9481082151732): "gis''", (3419.792432860693, 3623.1438729850915): "a''''", (718.922799426084, 761.6721736854058): "fis''", (67.32294488175603, 71.32617550580554): 'cis,', (538.5835590540484, 570.6094040464443): "cis''"}
def note(freq):
    key = filter(lambda x: freq >= x[0] and freq <= x[1], freq2note)
    if key:
        return freq2note[key[0]]
    return "r"


chunk = 2048

wf = wave.open(sys.argv[1], 'rb')
swidth = wf.getsampwidth()
RATE = wf.getframerate()
channels = wf.getnchannels()

window = np.blackman(chunk)

p = pyaudio.PyAudio()
stream = p.open(format =
                p.get_format_from_width(wf.getsampwidth()),
                channels = wf.getnchannels(),
                rate = RATE,
                output = True)

data = wf.readframes(chunk/channels)
print "%s frames, %s channels" % (wf.getnframes(), wf.getnchannels())


class Notes:
    def __init__(self):
        self.all_notes = []
        self.last_note = None
        self.last_dur = 0

    def heard(self, thenote):
        seconds = self.last_dur * chunk / float(RATE)
        if self.last_note is not None and self.last_dur > 1:
            print "%s  %.3fs" % (self.last_note.ljust(4), seconds)
            self.all_notes.append([self.last_note, self.last_dur, None])
        self.last_dur = 1
        self.last_note = thenote

    def cluster(self):
        MAX_GAP = 1.5
        x1 = lambda x: x[1]
        sorted_notes = sorted(self.all_notes, key=lambda x: x[1])
        groups = [[sorted_notes[0]]]
        for x in sorted_notes:
            if x[1] / groups[-1][-1][1] <= MAX_GAP:
                groups[-1].append(x)
                continue
            mult = 2
            while x[1] / groups[-1][-1][1] >= MAX_GAP * mult:
                mult *= 2
                groups.append([])
            groups.append([x])

        times = []
        for i, group in enumerate(groups):
            if group:
                times.append(sum(map(x1,group))/float(len(group)))
            else:
                times.append(times[-1] * 2)

        while times[0] > 10:
            times.insert(0, times[0]/2)

        cur = 0
        for note in sorted_notes:
            while note[1] > times[cur]*MAX_GAP:
                cur += 1
            note[2] = 8 / 2**cur

    def __str__(self):
        def print_note(note):
            return "%s%d" % (note[0], note[2])
        header = '\\version "2.16.2"\n{\n\\time 4/4 ' 
        return header + " ".join(map(print_note, self.all_notes)) + "\n}"


notes = Notes()

# play stream and find the frequency of each chunk
while len(data) == chunk*swidth:

    # write data out to the audio stream
    stream.write(data)

    # unpack the data and times by the hamming window
    indata = np.array(wave.struct.unpack("%dh"%(len(data)/swidth),\
                                         data))

    indata *= window
    # Take the fft and square each value
    fftData=abs(np.fft.rfft(indata))**2

    which = fftData[1:].argmax() + 1
    # use quadratic interpolation around the max
    if which != len(fftData)-1:
        y0,y1,y2 = np.log(fftData[which-1:which+2:])
        if y0 == float('-inf'):
            print "??"
            continue
        x1 = (y2 - y0) * .5 / (2 * y1 - y2 - y0)
        # find the frequency and output it
        thefreq = (which+x1)*RATE/chunk
    else:
        thefreq = which*RATE/chunk
    
    thenote = note(thefreq)
    # print "The freq is %f Hz. (%s)" % (thefreq, thenote)
    if thenote == notes.last_note:
        notes.last_dur += 1
    else:
        notes.heard(thenote)

    data = wf.readframes(chunk/channels)

notes.heard("the_end")
notes.cluster()
print notes

if data:
    stream.write(data)
stream.close()
p.terminate()
