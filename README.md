Arduino Robot Car with Raspberry Pi 5
This project uses a Raspberry Pi 5 to run YOLO object detection and an Arduino UNO with an Elegoo Smart Car Shield V1.1 to drive the motors. The robot can follow people and avoid walls using a video stream from an Android phone (IP Webcam). Everything you need is in this repo.

1. What you need
Raspberry Pi 5 (or 4/3)

Arduino UNO

Elegoo Smart Car Shield V1.1

2 DC motors (front wheels)

2 DC motors (rear wheels)

Plastic/metal chassis and wheels

microSD card (16–32 GB)

HDMI cable + monitor / TV

USB keyboard + mouse

USB‑A to USB‑B cable (to connect Arduino ↔ Pi 5)

USB‑A cable (for Pi 5 power)

Android phone with IP Webcam app

Wi‑Fi network (all devices on same network)


2. Build the physical robot car
Assemble the chassis

Put the plastic or metal chassis together using the screws and nuts that come with the Elegoo kit.

Screw the motors and wheels into the chassis so the car sits flat.

Attach the electronics

Plug the Arduino UNO into the Elegoo Smart Car Shield V1.1; the shield sits on top of the UNO.

Place the shield + Arduino on the chassis and screw them down.

Connect the motors

Left front motor → M1

Left rear motor → M4

Right front motor → M2

Right rear motor → M3
Make sure the wires are tight and the labels match on the shield.

Power the car

Insert batteries into the battery holder.

Plug the battery holder wires into the bat+/bat‑ terminals on the shield.

Turn on the shield’s power switch; the Arduino will power on and the motors are now ready.

3. Install Raspberry Pi OS on the Pi 5 (no OS yet)
3.1. Prepare the microSD card
Download Raspberry Pi Imager:
https://www.raspberrypi.com/software/

Install it on your Windows/Mac/Linux computer.

Open Raspberry Pi Imager.

Click “Choose OS” → select “Raspberry Pi OS (64‑bit)”.

Click “Choose Storage” → select your microSD card.

(Optional but recommended):

Click “Settings” (gear icon).

Set:

Username: pi

Password: raspberry

Wi‑Fi network and password.

Hostname: robotcar.
This lets the Pi connect to Wi‑Fi automatically.

Click “Write” and wait until it finishes, then “Continue”.
​
​

3.2. Boot the Pi 5 for the first time
Insert the microSD card into the Pi 5’s card slot.

Plug the Pi 5 into a monitor with HDMI/micro‑HDMI.

Plug in a USB keyboard and USB mouse.

Plug in the USB‑C power cable.

The Pi will boot; you’ll see the Raspberry Pi Desktop after a short delay.

If Wi‑Fi was configured in the Imager, the Pi connects automatically.

If not, click the network icon in the top‑right, choose your Wi‑Fi, and type the password.

4. Open the Terminal on the Pi 5
Click the Raspberry menu (Pi logo in top‑left).

Go to Accessories → Terminal.

A black window with a prompt like pi@raspberrypi:~ $ opens; this is the Terminal (command line).
You can also open it with Ctrl + Alt + T.

5. Update the Pi and install Python packages
In the Terminal, copy‑paste one line at a time and press Enter after each:

bash
sudo apt update
sudo apt upgrade
After that finishes, install the robot code dependencies:

bash
sudo apt install python3-opencv
pip install ultralytics pyserial
Now your Pi 5 is ready to run the robot AI.

6. Upload the Arduino code (Motor Controller)
Plug the Arduino UNO into your computer (USB‑B cable).

Download and install the Arduino IDE from https://www.arduino.cc if you don’t have it.

In your browser, go to your GitHub repo:
https://github.com/edafawd/Arduino-Robot-car-with-rassberry-pi-5

Click on Arduino_uno_motor_controller.ino.

Click “Raw”, then right‑click → “Save as” → save the file.

In the Arduino IDE:

Click File → New → paste the code into the editor.

Save the sketch somewhere (e.g., Car_Motor_Controller).

In Arduino IDE:

Tools → Board → choose Arduino Uno.

Tools → Port → choose the USB port shown when the Arduino is plugged in.

Click the Upload button (right‑arrow icon).
The Arduino now knows how to drive the motors using F/L/R/S commands.

7. Connect the Arduino to the Raspberry Pi 5
Plug one end of a USB‑A to USB‑B cable into the Arduino UNO (the UNO’s USB port).

Plug the other end into a USB‑A port on the Raspberry Pi 5.

The Pi and Arduino are now connected by USB and can talk via serial.

8. Set up IP Webcam on the Android phone
Install IP Webcam from the Google Play Store.

Open the app and make sure the phone is on the same Wi‑Fi network as your Raspberry Pi.

Turn on the server; the app will show a local IP and port (e.g., rtsp://192.168.1.50:8080/h264_ulaw.sdp).

Note this full RTSP URL; you will paste it into the Pi code later.

9. Put the Raspberry Pi robot code on the Pi 5
In your browser, go back to your GitHub repo:
https://github.com/edafawd/Arduino-Robot-car-with-rassberry-pi-5

Click on Raspberry_Pi_Code/robot_ai.py (or whichever name you use).

Click “Raw”, then right‑click → “Save as” → save the file to your computer.

Copy robot_ai.py to the Pi 5, for example:

Using a USB stick:

Copy the file to the USB stick.

Insert the stick into the Pi and drag the file into ~/robot_car in the Pi’s file manager.

Or in the Terminal, create a folder:

bash
mkdir ~/robot_car
Then move the file into it.

In the Terminal, run:

bash
cd ~/robot_car
ls
You should see robot_ai.py listed.
​

10. Configure the IP Webcam address
In the Terminal, open the file:

bash
nano robot_ai.py
Find the line:

python
PHONE_RTSP = "rtsp://192.168.1.50:8080/h264_ulaw.sdp"
Replace the IP and port with what your IP Webcam app shows.

Press Ctrl + X, then Y, then Enter to save.

11. Run the robot
In the Terminal, run:

bash
cd ~/robot_car
python3 robot_ai.py
A window will open on the Pi’s screen showing the camera view; the robot will:

Move forward when a person is centered.

Turn left/right if the person is off‑center.

Stop when an obstacle (chair, wall, etc.) is directly in front.

Now your Raspberry Pi 5 and Arduino‑driven car are fully working with AI.
