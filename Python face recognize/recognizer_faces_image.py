# Library for get file in folder
import os
# Custom library for some public variables
import constants
# Library for face recognize
import face_recognition
# Library for pickle's file
import pickle
# Need for image-related functions
import cv2

# Small need function
from collections import defaultdict

# Firebase's library
from firebase_admin import initialize_app, credentials, storage

# load the known faces's encodings
print("[INFO] loading encodings...")
data = pickle.loads(open(constants.PICKLE_PATH, "rb").read())      # loads - load từ file
if data["model"] != constants.DETECT_MODEL:
    print("Need to re-encode dataset")
    quit()

# Get images needs to recognize throught firebase
cred = credentials.Certificate(constants.FIREBASE_SERVICE_ACCOUN_PATH)
initialize_app(cred, {
    'storageBucket': '*.appspot.com'
})

bucket = storage.bucket()
blobs = bucket.list_blobs()

if not os.path.exists(constants.TEST_IMAGES_FOLDER): os.mkdir(constants.TEST_IMAGES_FOLDER)

for blob in blobs:
    blob.download_to_filename('/'.join([constants.TEST_IMAGES_FOLDER, blob.name]))
    print(blob.name)

image_paths = os.listdir(constants.TEST_IMAGES_FOLDER)

# The names's list will store the result of recognization
names = []

# Create folder for the implemented image and its result
if not os.path.exists(constants.TESTED_IMAGES_FOLDER): os.mkdir(constants.TESTED_IMAGES_FOLDER)
if not os.path.exists(constants.RESULT_IMAGES_FOLDER): os.mkdir(constants.RESULT_IMAGES_FOLDER)

# # Loop throught images needs to be recognize
for i, image_path in enumerate(image_paths):
    # get file's name
    print("[INFO] checking image: {}".format(image_path))
    file_name = image_path

    # load image by the "load_image_file" function with auto change image from BGR to RGB
    image = face_recognition.load_image_file('/'.join([constants.TEST_IMAGES_FOLDER, image_path]))

    print("[INFO] recognizing faces...")
    # we'll get all the faces in the image and encode them to recognize
    boxes = face_recognition.face_locations(image, model = data["model"])
    # print(len(boxes))
    encodings = face_recognition.face_encodings(image, boxes)
    for encoding, box in zip(encodings, boxes):
        matches = face_recognition.compare_faces(data["encodings"], encoding, 0.6)      # 0.6 is the function's default - recomment
        # faces_dis = face_recognition.face_distance(data["encodings"], encoding)
        # print(faces_dis)
        # print(matches)

        # To recognize the face in the pre-encoded data, we'll get maximum that the face is match with the face's identity
        name = "Unknown" 

        if True in matches:
            matchedIdxs = [index for (index, face_check) in enumerate(matches) if face_check]

            counts = defaultdict(lambda: 0)
            for index in matchedIdxs:
                name = data["names"][index]
                counts[name] = counts[name] + 1

            name = max(counts, key = counts.get)
        
        # crete face's place + the face's identity in the image
        top, right, bottom, left = box
        cv2.rectangle(image, (left, top), (right, bottom), (0, 255, 0), 2)
        y = top - 15 if top - 15 > 15 else top + 15

        cv2.putText(image, name, (left, y), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 0), 1)

        names.append((name, file_name))

    # change the image back to BGR and save it to the result's folder
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
    cv2.imwrite('/'.join([constants.RESULT_IMAGES_FOLDER, image_path]), image)
    os.replace('/'.join([constants.TEST_IMAGES_FOLDER, image_path]), '/'.join([constants.TESTED_IMAGES_FOLDER, image_path]))

# print(names)