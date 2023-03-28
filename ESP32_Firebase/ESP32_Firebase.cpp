#include "ESP32_Firebase.h"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

bool taskCompleted = false;

// SD card mounted flag
uint64_t SDfreeSpace = -2;

// Global variables
int imageCounter; // The numbers image in the sd card

void setup()
{

    Serial.begin(115200);

    initWiFi();
    setupFlashPWM();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

    /* Sign up */
    // if (Firebase.signUp(&config, &auth, auth.user.email, auth.user.password))
    // {
    //     Serial.println("ok");
    // }
    // else
    // {
    //     Serial.printf("%s\n", config.signer.signupError.message.c_str());
    // }

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    /* Assign upload buffer size in byte */
    // Data to be uploaded will send as multiple chunks with this size, to compromise between speed and memory used for buffering.
    // The memory from external SRAM/PSRAM will not use in the TCP client internal tx buffer.
    config.fcs.upload_buffer_size = 512;

    // if use SD card, mount it.
    SD_Card_Mounting(); // See src/addons/SDHelper.h

    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Turn-off the 'brownout detector'


    if (esp_err_t err = initCamera() != ESP_OK)
        Serial.printf("Camera init failed with error 0x%x", err);

    SDfreeSpace = (uint64_t)(SD.totalBytes() - SD.usedBytes()) / (1024 * 1024);
    if (SDfreeSpace == -2)
        Serial.println("No SD Card detected");
    else if (SDfreeSpace == -1)
        Serial.println("SD Card's type detect failed");
    else
        Serial.printf("SD Card found, free space = %dmB \n", SDfreeSpace);

    imageCounter = fileCountInFolder("/img");
    if (imageCounter < 0)
        Serial.println("Unable to create IMG folder on sd card");
    else
        Serial.printf("File's number: %d \n", imageCounter);
}

void loop()
{

    // Firebase.ready() should be called repeatedly to handle authentication tasks.

    if (Firebase.ready() && !taskCompleted)
    {
        taskCompleted = true;
        if (takePictureAndSaveToSDCard(&imageCounter)) {
            Serial.println("\nUpload file...\n");

            // MIME type should be valid to avoid the download problem.
            // The file systems for flash and SD/SDMMC can be changed in FirebaseFS.h.
            if (!Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */, "/img/" + String(imageCounter) + ".jpg" /* path to local file */, mem_storage_type_sd /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, String(imageCounter) + ".jpg" /* path of remote file stored in the bucket */, "image/jpeg" /* mime type */, fcsUploadCallback /* callback function */))
                Serial.println(fbdo.errorReason());
        }
        if (takePictureAndSaveToSDCard(&imageCounter)) {
            Serial.println("\nUpload file...\n");

            // MIME type should be valid to avoid the download problem.
            // The file systems for flash and SD/SDMMC can be changed in FirebaseFS.h.
            if (!Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */, "/img/" + String(imageCounter) + ".jpg" /* path to local file */, mem_storage_type_sd /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, String(imageCounter) + ".jpg" /* path of remote file stored in the bucket */, "image/jpeg" /* mime type */, fcsUploadCallback /* callback function */))
                Serial.println(fbdo.errorReason());
        }
    }
}

void fcsUploadCallback(FCS_UploadStatusInfo info)
{
    if (info.status == fb_esp_fcs_upload_status_init)
    {
        Serial.printf("Uploading file %s (%d) to %s\n", info.localFileName.c_str(), info.fileSize, info.remoteFileName.c_str());
    }
    else if (info.status == fb_esp_fcs_upload_status_upload)
    {
        Serial.printf("Uploaded %d%s, Elapsed time %d ms\n", (int)info.progress, "%", info.elapsedTime);
    }
    else if (info.status == fb_esp_fcs_upload_status_complete)
    {
        Serial.println("Upload completed\n");
        FileMetaInfo meta = fbdo.metaData();
        Serial.printf("Name: %s\n", meta.name.c_str());
        Serial.printf("Bucket: %s\n", meta.bucket.c_str());
        Serial.printf("contentType: %s\n", meta.contentType.c_str());
        Serial.printf("Size: %d\n", meta.size);
        Serial.printf("Generation: %lu\n", meta.generation);
        Serial.printf("Metageneration: %lu\n", meta.metageneration);
        Serial.printf("ETag: %s\n", meta.etag.c_str());
        Serial.printf("CRC32: %s\n", meta.crc32.c_str());
        Serial.printf("Tokens: %s\n", meta.downloadTokens.c_str());
        Serial.printf("Download URL: %s\n\n", fbdo.downloadURL().c_str());
    }
    else if (info.status == fb_esp_fcs_upload_status_error)
    {
        Serial.printf("Upload failed, %s\n", info.errorMsg.c_str());
    }
}

bool takePictureAndSaveToSDCard(int *imageCounter)
{

    // brightLed(150);
    // delay(7000);
    camera_fb_t *fb = esp_camera_fb_get(); // capture image frame from camera
    // delay(1000);
    // brightLed(0);

    if (!fb)
    {
        Serial.println("Error: Camera capture failed");
        return false;
    }
    else if (SDfreeSpace)
    {
        if (createFile("/img/" + String(*imageCounter + 1) + ".jpg", fb->buf, fb->len))
        {
            Serial.println("Image saved to sd card");
            *imageCounter = *imageCounter + 1; // increment image counter
        }
        else
        {
            Serial.println("Error: failed to save image data file on sd card");
            return false;
        }
    }
    else
        return false;

    esp_camera_fb_return(fb); // return frame so memory can be released
    return true;
}

void initWiFi()
{
    // Wi-Fi setting
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
}