#! /bin/bash

/root/mjpgstream/mjpg_streamer -i "/root/mjpgstream/input_uvc.so -r 640x480 -f 15" -o "/root/mjpgstream/output_http.so -w /root/mjpgstream/www --port 1235"
