import logging
import sys
import bisect

class DummyDatabase:

    def __init__(self):
        self._tracks = []

    def insert(self, track):
        bisect.insort(self._tracks, track)

    def list_all(self):
        return self._tracks[:]

    def find(self, tags):

        class AnyTagIterator:
            def __init__(self, tracks, value):
                self._tracks = tracks[:]
                self._value = value.casefold()

            def __iter__(self):
                for track in self._tracks:
                    for tag, values in track.tags.items():
                        for value in values:
                            if self._value in value.casefold():
                                yield track
        return AnyTagIterator(self._tracks, tags[''])

