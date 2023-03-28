import pickle
import os
import face_recognition
import constants

# The encoding's data is expressed as: encoding data / name of the image's face / file of the image
# Get pre-encoded face
print("[INFO] loading encodings...")
data = None
try:
    file = open(constants.PICKLE_PATH, "rb")
except:
    print("No pre-encoding pickle's file")
else:
    data = pickle.loads(file.read())
    if data["model"] != constants.DETECT_MODEL:
        data = None

print("[INFO] quantifying faces...")
image_paths = os.listdir(constants.DATASET_IMAGES_FOLDER)

# Initialize some list to store the encoded datas (so we not re-encode them) and the new-encode datas
if not data:
    names, encoded, files_path = [], [], []
else:
    names, encoded, files_path = data["names"], data["encodings"], data["file's path"]

# Loop throught folders inside
for i, folder_name in enumerate(image_paths):
    # The folder's name represend to the image's identity
    image_paths = os.listdir('/'.join([constants.DATASET_IMAGES_FOLDER, folder_name]))
    # Loop throught images inside
    for image_name in image_paths:
        # if the image is encoded then continue
        if '/'.join([folder_name, image_name]) not in files_path:
            print("[INFO] processing image {}".format(image_name))
            print(image_name.split(os.path.sep))
            name = folder_name

            # Load image by the "load_image_file" function with auto change image from BGR to RGB
            image = face_recognition.load_image_file('/'.join([constants.DATASET_IMAGES_FOLDER, folder_name, image_name]))

            # The dataset's image will contain only 1 face
            box = face_recognition.face_locations(image, model = constants.DETECT_MODEL)
            # print(len(box))

            # with diffrent detection's model, there will be some image that cant detect face so we have to be careful
            if len(box):
                encoding = face_recognition.face_encodings(image, box)[0]

                # Save the encoded data
                encoded.append(encoding)
                names.append(name)
                files_path.append('/'.join([folder_name, image_name]))

# Save the encoded data to pickle's file
print("[INFO] saved encodings...")
data = {"model": constants.DETECT_MODEL,"encodings": encoded, "names": names, "file's path": files_path}
# print(data)

with open(constants.PICKLE_PATH, "wb") as f:
    f.write(pickle.dumps(data))
