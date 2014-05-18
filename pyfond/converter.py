import threading
import subprocess
import pathlib
import logging
import sys

class Converter:
    def __init__(self, path, url_prefix, database):
        self.__path = pathlib.Path(path)
        self.__url_prefix = url_prefix
        self.__database = database
        self.__profiles = {}
        # todo: remove stale jobs

    def register_profile(self, converter):
        logging.info('Register profile: %s', converter.name)
        self.__profiles[converter.name] = converter

    def get_track_as(self, track, profile):
        logging.info('Get "%s" as %s', track.id, profile)
        class Status:
            def __init__(self, track):
                self.track = track

        status = Status(track)

        instance = self.__database.instance_get(track, profile)

        if instance.new:
            logging.info('Creating "%s" isntance %s', track.id, profile)
            output_absolute = str(self.__path / instance.name)
            output_relative = instance.name
            job = self.__profiles[profile].create_job(
                track, self.__database, output_relative, output_absolute)
            status.done = False
            status.progress = 0
            status.eta = job.eta
        elif not instance.complete:
            logging.info('"%s" isntance %s is at %d%%', track.id, profile, instance.progress)
            status.done = False
            status.progress = instance.progress
            status.eta = intsance.eta
        elif instance.aborted:
            logging.info('"%s" isntance %s was aborted', track.id, profile)
            status.done = False
            status.progress = instance.progress
            status.eta = 0x7fffffff
            status.aborted = True
        else:
            logging.info('"%s" isntance %s return %s', track.id, profile, instance.output)
            status.done = True
            status.progress = 100
            status.eta = 0
            status.url = self._url_prefix + instance.output
            status.mime = instance.mime
        return status

class Converter_ffmpeg:
    def __init__(self, name, options, mime):
        self.name = name
        self.__options = options
        self.__mime = mime

    def create_job(self, track, database, out_relative, out_absolute):
        class ConverterThread(threading.Thread):
            def __init__(self, options):
                threading.Thread.__init__(self)
                self.__options = options
            def start(self, profile, mime, database, track, out_relative, out_absolute):
                self.__profile = profile
                self.__mime = mime
                self.__database = database.threaded_copy()
                self.__track = track
                self.__output_relative = out_relative
                self.__output_absolute = out_absolute
                threading.Thread.start(self)
            def run(self):
                try:
                    logging.info('getting "%s" decoded stream', self.__track.id)

                    decoder = self.__track.decoder()

                    self.__database.instance_update(
                        self.__track, self.__profile, 1, 10)

                    logging.info('starting encode "%s" to "%s"', self.__track.id, self.__profile)
                    encoder = subprocess.Popen([
                        'ffmpeg',
                        '-v', 'quiet',
                        '-f', 's16le',
                        '-ar', '44100',
                        '-ac', '2',
                        '-i', '-'] + self.__options +
                        ['-vn', self.__output_absolute],
                        stdin=decoder.stream(),
                        stdout=subprocess.PIPE,
                        stderr=subprocess.DEVNULL)
                    decoder.stream().close()
                    encoder.communicate()

                    logging.info('encoding "%s" complete', self.__track.id)

                    decoder.wait()

                    if decoder.returncode != 0:
                        raise Exception("decoder.returncode != 0")

                    if encoder.returncode != 0:
                        raise Exception("encoder.returncode != 0")

                    self.__database.instance_complete(
                        self.__track, self.__profile, self.__mime,
                        self.__output_relative)
                except:
                    logging.error('error ecoding "%s"', self.__track.id, exc_info=sys.exc_info())
                    self.__database.instance_abort(self.__track, self.__profile)
        worker = ConverterThread(self.__options)
        worker.start(self.name, self.__mime, database, track,
            out_relative, out_absolute)

        class Status:
            pass
        status = Status()
        status.eta = 10
        return status


class DummyDatabase:
    class Instance:
        def __init__(self, track, profile):
            self.__track = track
            self.__profile = profile
            self.new = True
            self.complete = False
            self.aborted = False
            self.progress = 0
            self.eta = 10
        def __eq__(self, other):
            return self.__track == other.__track and self.__profile == other.__profile
        def __repr__(self):
            print(str({
                '__track': self.__track,
                '__profile': self.__profile,
                'new': self.new,
                'complete': self.complete,
                'aborted': self.aborted,
                'progress': self.progress,
                'eta': self.eta
                }))

    def __init__(self):
        self.__lock = threading.Lock()
        self.__instances = []
        self.__counter = 0

    def threaded_copy(self):
        return self

    def instance_get(self, track, profile):
        instance = self.Instance(track, profile)
        with self.__lock:
            try:
                index = self.__instances.index(instance)
                instance = self.__instances[index]
                return instance
            except:
                instance.name = str(self.__counter)
                self.__counter = self.__counter + 1
                self.__instances.append(instance)
        return instance

    def instance_update(self, track, profile, progress, eta):
        instance = self.Instance(track, profile)
        with self.__lock:
            index = self.__instances.index(instance)
            instance = self.__instances[index]
            instance.progress = progress
            instance.eta = eta

    def instance_abort(self, track, profile):
        instance = self.Instance(track, profile)
        with self.__lock:
            index = self.__instances.index(instance)
            instance = self.__instances[index]
            instance.aborted = True

    def instance_complete(self, track, profile, mime, output):
        instance = self.Instance(track, profile)
        with self.__lock:
            index = self.__instances.index(instance)
            instance = self.__instances[index]
            instance.complete = True
            instance.progress = 100
            instance.eta = 0
            instance.mime = mime
            instance.output = output


