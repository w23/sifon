import subprocess
import sys

opts_decoder = [
    'ffmpeg',
    '-v', 'quiet',
    '-i', sys.argv[1],
    '-f', 's16le',
    '-ar', '44100',
    '-ac', '2',
    '-acodec', 'pcm_s16le',
    'pipe:']

opts_encoder = [
    'ffmpeg',
    '-v', 'quiet',
    '-f', 's16le',
    '-ar', '44100',
    '-ac', '2',
    '-i', '-',
    '-c:a', 'libfdk_aac',
    '-b:a', '160k',
    '-cutoff', '20000',
#    '-o',
    '/tmp/test.m4a']

print(" ".join(opts_decoder))
print(' '.join(opts_encoder))

try:
    decoder = subprocess.Popen(opts_decoder, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
    encoder = subprocess.Popen(opts_encoder, stdin=decoder.stdout, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
    decoder.stdout.close()
    #output = encoder.communicate()[0]
    encoder.communicate()

    decoder.wait()

    print("D", decoder.returncode, " E", encoder.returncode)
except:
    print(str(sys.exc_info()))
