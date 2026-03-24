# robot_ai_walls.py

import cv2
from ultralytics import YOLO
import serial
import time

# --- configure ---
PHONE_RTSP  = "rtsp://192.168.1.50:8080/h264_ulaw.sdp"   # your phone IP
SERIAL_PORT = "/dev/ttyACM0"
BAUD_RATE   = 9600

# classes (yolov8n default)
PERSON_CLASS  = 0      # people
OBSTACLE_CLASSES = [56, 57, 58]  # chairs, tables, couch, etc. (adjust as needed)

# command chars
CMD_FORWARD  = b'F'
CMD_LEFT     = b'L'
CMD_RIGHT    = b'R'
CMD_STOP     = b'S'

try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.1)
    time.sleep(2)
    print("Serial connected to Arduino:", SERIAL_PORT)
except Exception as e:
    print("Serial error:", e)
    ser = None

model = YOLO('yolov8n.pt')
cap   = cv2.VideoCapture(PHONE_RTSP)

if not cap.isOpened():
    print("Cannot open RTSP stream:", PHONE_RTSP)
    exit()

print("Press ESC to quit...")

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        print("Stream lost or closed.")
        break

    # detect people + obstacles
    results = model(frame, classes=PERSON_CLASS + OBSTACLE_CLASSES,
                    conf=0.5, imgsz=640)

    cmd = CMD_STOP   # default: stop

    # --- wall/obstacle logic ---
    has_obstacle_near_center = False
    if any(b.cls in OBSTACLE_CLASSES for b in results[0].boxes):
        for box in results[0].boxes:
            if box.cls not in OBSTACLE_CLASSES:
                continue
            x1, y1, x2, y2 = box.xyxy[0].tolist()
            cx = (x1 + x2) / 2
            cy = (y1 + y2) / 2
            h  = y2 - y1

            # if big enough and near bottom center → wall in front
            height_norm = h / frame.shape[0]
            if height_norm > 0.3 and abs(cx - frame.shape[1]/2) < 0.25*frame.shape[1]:
                has_obstacle_near_center = True

    # --- person following logic ---
    if len(results[0].boxes) > 0 and not has_obstacle_near_center:
        # pick first person to track
        person_box = None
        for box in results[0].boxes:
            if box.cls == PERSON_CLASS:
                person_box = box
                break

        if person_box:
            x1, y1, x2, y2 = person_box.xyxy[0].tolist()
            cx = (x1 + x2) / 2
            width = frame.shape[1]
            center_margin = width * 0.15

            if cx < width/2 - center_margin:
                cmd = CMD_LEFT
            elif cx > width/2 + center_margin:
                cmd = CMD_RIGHT
            else:
                cmd = CMD_FORWARD

    elif has_obstacle_near_center:
        # wall in front → stop; you can add 'R' or 'L' to turn randomly later
        cmd = CMD_STOP

    # --- send to Arduino ---
    if ser and ser.is_open:
        try:
            ser.write(cmd)
        except Exception as e:
            print("Serial write error:", e)

    # --- draw and display ---
    results[0].plot(boxes=True, labels=True)
    disp = results[0].plot()

    cv2.imshow("AI Robot Vision", disp)

    key = cv2.waitKey(1)
    if key == 27:  # ESC
        break

print("Shutting down...")
if ser and ser.is_open:
    ser.close()
cap.release()
cv2.destroyAllWindows()
