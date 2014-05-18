import subprocess
import json
import sys

class ErrorInvalidMedia(Exception):
    def __init__(self, filename, description):
        self._filename = filename
        self._description = description
    def __repr__(self):
        return self.__class__.__name__ + ": "
        + self._filename + ": "
        + self._description

class Decoder_ffmpeg:
    def __init__(self, filename):
        self.__decoder = subprocess.Popen([
            'ffmpeg',
            '-v', 'quiet',
            '-i', filename,
            '-f', 's16le',
            '-ar', '44100',
            '-ac', '2',
            '-acodec', 'pcm_s16le',
            'pipe:'],
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL)

    def stream(self):
        return self.__decoder.stdout

    def done(self):
        return self.__decoder.poll() != None

    def wait(self):
        self.__decoder.wait()
        self.returncode = self.__decoder.returncode

class Track_ffmpeg:
    def __repr__(self):
        return self.__class__.__name__ + ": " + str({\
        'id:': self.id,\
        'codec': self.codec,\
        'bitrate': self.bitrate,\
        'channels': self.channels,\
        'sample_rate': self.sample_rate,\
        'sample_format': self.sample_format,\
        'length': self.length,\
        'tags': self.tags
        })

    def __gt__(self, right):
        assert isinstance(right, self.__class__)
        return self.id > right.id

    def decoder(self):
        return Decoder_ffmpeg(self._filename)

def read_media_file(filename, id):
    # determine whether teh file is mediafile
    probedata = subprocess.check_output(
        ["ffprobe",
        "-v", "quiet",
        "-print_format", "json",
        "-show_format",
        "-show_streams", filename],
        universal_newlines = True)

    data = json.loads(probedata)

    audio_streams = 0
    other_streams = 0
    audio_stream = None
    for stream in data['streams']:
        if stream['codec_type'] == 'audio':
            audio_stream = stream
            audio_streams = audio_streams + 1
        else:
            other_streams = other_streams + 1

    if audio_streams != 1:
        raise ErrorInvalidMedia(filename, "Invalid number of audio streams: " + str(audio_streams))

    if other_streams > 2:
        raise ErrorInvalidMedia(filename, "Too many non-audio streams: " + str(other_streams))

    track = Track_ffmpeg()
    track.id = id
    track._filename = filename
    track.format = data['format']['format_name']
    track.codec = audio_stream['codec_name']
    track.bitrate = int(data['format']['bit_rate'])
    track.channels = int(audio_stream['channels'])
    track.sample_rate = int(audio_stream['sample_rate'])
    track.sample_format = audio_stream['sample_fmt']
    track.length = float(audio_stream['duration'])
    track.tags = data['format']['tags']

    return track
