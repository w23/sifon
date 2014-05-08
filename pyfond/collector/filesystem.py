import sys
import logging
import threading
import queue
import pathlib
import taglib
import collector.database

class Track:

    def __init__(self, id, tags, length):
        self.id = id
        self.tags = tags
        self.length = length

    def __gt__(self, right):
        assert isinstance(right, self.__class__)
        return self.id > right.id

    def __str__(self):
        return "\n\nTrack: " + self.id + "(" + str(self.length) + ")\nTags: " + str(self.tags)

    def __repr__(self):
        return str(self)

class Collection(threading.Thread):
    """ Collect files from filesystem storage """

    def __init__(self, path):
        logging.info("Collecting from " + path)
        threading.Thread.__init__(self)
        self._path = path
        self._queue = queue.PriorityQueue()
        self._database = collector.database.DummyDatabase()
        self._queue.put((10, pathlib.Path(path)))

    def stop(self):
        self._queue.put((0,None))
        threading.thread.join()

    def find_track(self, condition):
        return self._database.find(condition)

    def get_track(self, track_id):
        return self._database.get(track_id)

    def retrieve(self, track_id):
        fullpath = pathlib.Path(self._path, track_id)
        if fullpath.exists():
            return str(fullpath)
        return None

    def run(self):
        while not self._queue.empty():
            event = self._queue.get();
            logging.info("Collecting from path: " + str(event))
            if len(event) < 2 or not isinstance(event[1], pathlib.Path):
                break
            try:
                path = pathlib.Path(event[1])
                if path.is_dir():
                    self.process_dir(path)
            except:
                logging.warning("couldn't wat: " + str(sys.exc_info()))

    def process_dir(self, path):
        for entry in path.iterdir():
            if entry.is_dir():
                self._queue.put((10, entry))
            elif entry.is_file():
                self.process_file(entry)

    def process_file(self, entry):
        try:
            id = str(entry.relative_to(self._path))
            meta = taglib.File(str(entry))
            self._database.insert(Track(id, meta.tags, meta.length))
        except:
            logging.warning("couldn't read file " + str(entry) + ": " + str(sys.exc_info()))
