import collector.filesystem
import collector.database
import sys
import logging

logging.basicConfig(format='%(levelname)s:%(message)s', level=logging.INFO)

c = collector.filesystem.Collection(sys.argv[1])
c.start()
c.join()
while True:
    token = input('--> ')
    if len(token) == 0:
        break
    found = c.find_track({'':token})
    for track in found:
        print(c.retrieve(track.id))
