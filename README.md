# ESP32Cam-Firebase-Face-recognize-dlib
This project receives images from esp32-cam and uses them for face recognition.

# Description
This project contents 2 mini-project:
+ Using ESP32-CAM and take pictures, save it to SD card and upload it to firebase.
+ Get some images from firebase and recognize faces in it by using "face_recognition"'s package.

# Getting Started

## Dependencies

+ Arduino and IDE
+ [Arduino core for ESP32-CAM](https://github.com/espressif/arduino-esp32) and [Firebase for ESP32-CAM](https://github.com/mobizt/Firebase-ESP-Client)
+ Python 3.3+ and pip3
+ ["face recognization"'s package](https://github.com/ageitgey/face_recognition) and dlib library
+ Firebase's project and its user

## Usage

+ Use the "ESP32_Firebase"'s folder to upload code to ESP32-CAM
    + Change some variables in "ESP32_Firebase.h":
        + "WIFI_SSID" and "WIFI_PASSWORD" is your network's information - if your network is not working then the picture only save it to SD card
        + "API_KEY", "STORAGE_BUCKET_ID" and "USER_EMAIL"  is your firebase's information
    + Need to have SD card or the image will not be saved and uploaded to firebase
    + After uploading's complete, esp32 will take the picture after 15s
+ The "Python face recognize"'s folder have 2 funtion
    + The "encode_faces.py"'s file is used for get the information of known face and will saved that to pickle's file
        + The code will get image from the "dataset" folder
        + The directory will be formatted as follows:

            ```bash
                Python face recognize/dataset
                ├── "the name of the person1 in the image"
                |   ├── image1.jpg
                |   └── image2.jpg
                └── "the name of the person2 in the image"
                    ├── image1.jpg
                    └── image2.jpg
            ```

    + The "recognizer_faces_image.py"'s file is used for get the image from firebase and recognize all faces in images
        + Need to get firebase project's admin SDK json's file and place it in its folder
        + In the code, need to replace the firebase's storage link
        ```python
        cred = credentials.Certificate(constants.FIREBASE_SERVICE_ACCOUN_PATH)
        initialize_app(cred, {
            'storageBucket': '*.appspot.com'
        })
        ```
# Help
+ To install dlib in python 3.7+, this [link](https://stackoverflow.com/questions/74476152/error-in-installing-dlib-library-in-python3-11) will have specific instructions