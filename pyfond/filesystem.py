import sys
import logging
import threading
import queue
import pathlib
import database
import mediafile

class Collection:
    """ Collect files from filesystem storage """

    class Collector(threading.Thread):
        def __init__(self, path, database):
            logging.info("Collecting from " + path)
            threading.Thread.__init__(self)
            self.__path = path
            self.__queue = queue.PriorityQueue()
            self.__database = database
            self.__queue.put((10, pathlib.Path(path)))

        def stop(self):
            self.__queue.put((0,None))
            threading.thread.join()

        def run(self):
            while not self.__queue.empty():
                event = self.__queue.get();
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
                    self.__queue.put((10, entry))
                elif entry.is_file():
                    self.process_file(entry)

        def process_file(self, entry):
            try:
                id = str(entry.relative_to(self.__path))
                self.__database.insert(mediafile.read_media_file(str(entry), id))
            except:
                logging.warning("couldn't read file " + str(entry) + ": " + str(sys.exc_info()))

    def __init__(self, path):
        self.__database = database.DummyDatabase()
        self.__collector = self.Collector(path, self.__database)

    def start(self):
        self.__collector.start()

    def stop(self):
        self.__collector.stop()

    def find_track(self, condition):
        return self.__database.find(condition)

    def get_track(self, track_id):
        return self.__database.get(track_id)
