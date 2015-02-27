#! /bin/bash

/usr/local/bin/anubisVideo/mjpg_streamer -i "/usr/local/bin/anubisVideo/input_uvc.so -r 640x480 -f 15" -o "/usr/local/bin/anubisVideo/output_http.so -w /usr/local/bin/anubisVideo/www --port 1235"
