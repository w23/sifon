import sys
import logging
import filesystem
import converter
import server

logging.basicConfig(format='%(levelname)s:%(message)s', level=logging.INFO)

c = filesystem.Collection(sys.argv[1])
c.start()

conv = converter.Converter('/tmp/sifon', 'http://lol/', converter.DummyDatabase())
conv.register_profile(converter.Converter_ffmpeg("mp4_160", [
    '-c:a', 'libfdk_aac',
    '-b:a', '160k',
    '-cutoff', '20000',
    '-f', 'ipod'], 'audio/mp4'))

srv = server.HTTPServer('localhost', 8080, c, conv)
srv.serve()


while True:
    token = input('--> ')
    if len(token) == 0:
        break
    found = c.find_track({'':token})
    for track in found:
        print(str(track))
        instance = conv.get_track_as(track, 'mp4_160')
        print(str(instance))
