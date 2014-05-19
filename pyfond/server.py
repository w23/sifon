import http.server
import urllib
import json

__all__ = ["HTTPServer"]

class HTTPHandler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        parts = self.path.split('/')

        print(str(parts))

        cmd = parts[1]
        args = [urllib.parse.unquote(s) for s in parts[2:]]
        ret = []

        if cmd == 'find':
            tracks = self.server._collection.find_track({'': args[0]})
            for track in tracks:
                ret.append({
                    'id': track.id,
                    'length': track.length,
                    'tags': track.tags
                    })

        elif cmd == 'get':
            profile = args[0]
            track_id = args[1]
            track = self.server._collection.get_track(track_id)
            status = self.server._converter.get_track_as(track, profile)
            ret = {'progress': status.progress, 'eta': status.eta}
            if status.done:
                ret['url'] = status.url
                ret['mime'] = status.mime

        else:
            self.send_response(404, message="wtf dude")
            self.end_headers()
            return

        response = json.dumps(ret)

        self.send_response(200)
        self.send_header("Content-type", "text/json")
        self.send_header("Content-length", len(response))
        self.end_headers()
        self.wfile.write(response.encode())

class HTTPServer:
    def __init__(self, host, port, collection, converter):
        self.__http = http.server.HTTPServer(tuple([host, port]), HTTPHandler)
        self.__http._collection = collection
        self.__http._converter = converter

    def serve(self):
        self.__http.serve_forever()

